#!/usr/bin/env python3
"""Generate Daslang bindings for the Flecs C API from flecs.h.

Usage:
    python3 gen_flecs_bindings.py <flecs.h> <out_dir> <flecs.das>

Outputs:
    <out_dir>/module_flecs_annotations.inc  -- C++ annotation class definitions
    <out_dir>/module_flecs_register.inc     -- C++ constructor registration calls
    Updates <flecs.das> with typedefs and enum constants
"""

import re
import sys
import os

# ─── Skip lists (already hand-written in module_flecs.cpp) ──────────────────
SKIP_STRUCTS = {"ecs_world_t", "ecs_type_t", "ecs_entities_t"}
SKIP_FUNCS   = {"ecs_init", "ecs_fini", "ecs_get_entities"}

# Already declared in the hand-written flecs.das
EXISTING_DAS_TYPEDEFS = {"ecs_id_t", "ecs_entity_t"}

# ─── Known scalar typedefs (from private headers not parsed here) ────────────
# These ARE safe to bind as struct fields and to emit as Daslang typedefs.
EXTRA_SCALAR_TYPEDEFS = {
    "ecs_flags8_t":   "uint8",
    "ecs_flags16_t":  "uint16",
    "ecs_flags32_t":  "uint",
    "ecs_flags64_t":  "uint64",
    "ecs_size_t":     "int",
    "ecs_float_t":    "float",
    "ecs_ftime_t":    "float",
    "ecs_termset_t":  "uint",
}

# C primitives that are always safe as field base types
PRIMITIVE_BASE_TYPES = {
    "void", "bool", "char", "int", "short", "long", "float", "double",
    "int8_t", "int16_t", "int32_t", "int64_t",
    "uint8_t", "uint16_t", "uint32_t", "uint64_t",
    "size_t", "ptrdiff_t",
}

# Daslang reserved keywords that need field name mapping
# C name : Daslang name
DAS_RESERVED_FIELD_RENAMES = {
    "type": "vtype",
}

# Some FLECS_API extern const declarations are present in headers but are not
# exported by every Flecs build configuration that consumers link against.
# Keep these out of the generated binding surface unless the binding gains a
# build-config-aware export check.
SKIP_EXTERN_CONSTS = {
    "ecs_id(EcsComponent)",
    "ecs_id(EcsIdentifier)",
    "ecs_id(EcsPoly)",
    "ecs_id(EcsDefaultChildComponent)",
    "ecs_id(EcsTickSource)",
    "ecs_id(EcsPipelineQuery)",
    "ecs_id(EcsTimer)",
    "ecs_id(EcsRateFilter)",
}


# ─── Comment stripping ────────────────────────────────────────────────────────
def strip_comments(text: str) -> str:
    """Remove C/C++ comments, preserving newlines for line-count fidelity."""
    result = []
    i = 0
    n = len(text)
    while i < n:
        if text[i:i+2] == "//":
            while i < n and text[i] != "\n":
                result.append(" ")
                i += 1
        elif text[i:i+2] == "/*":
            result.append(" ")
            i += 2
            while i < n and text[i:i+2] != "*/":
                result.append("\n" if text[i] == "\n" else " ")
                i += 1
            result.append(" ")
            i += 2
        else:
            result.append(text[i])
            i += 1
    return "".join(result)


# ─── Function pointer typedef names ──────────────────────────────────────────
def parse_fn_ptr_typedefs(text: str) -> set:
    """Return names of all typedef'd function pointer types."""
    pat = re.compile(r"typedef\s+[\w\s\*]+\(\s*\*\s*(\w+)\s*\)\s*\(")
    return {m.group(1) for m in pat.finditer(text)}


# ─── Scalar typedef discovery ─────────────────────────────────────────────────
_C_TO_DAS = {
    "uint64_t": "uint64", "int64_t": "int64",
    "uint32_t": "uint32", "int32_t": "int",
    "uint16_t": "uint16", "int16_t": "int16",
    "uint8_t":  "uint8",  "int8_t":  "int8",
    "float":    "float",  "double":  "double",
    "bool":     "bool",
}


def parse_scalar_typedefs(text: str) -> list:
    """Return [(c_name, das_type)] for simple primitive typedefs not already declared."""
    results = []
    pat = re.compile(
        r"typedef\s+(uint64_t|int64_t|uint32_t|int32_t|uint16_t|int16_t"
        r"|uint8_t|int8_t|float|double|bool)\s+(\w+)\s*;"
    )
    seen = set(EXISTING_DAS_TYPEDEFS)
    for m in pat.finditer(text):
        c_type, name = m.group(1), m.group(2)
        if name not in seen and not name.startswith("__"):
            results.append((name, _C_TO_DAS[c_type]))
            seen.add(name)
    return results


def parse_void_typedefs(text: str) -> set:
    """Return names of all 'typedef void X;' declarations (not pointer typedefs)."""
    # Match 'typedef void name;' but NOT 'typedef void (*name)(' (fn ptrs)
    pat = re.compile(r"typedef\s+void\s+(\w+)\s*;")
    return {m.group(1) for m in pat.finditer(text)
            if not m.group(1).startswith("__")}


# ─── Opaque struct detection ──────────────────────────────────────────────────
def parse_opaque_structs(text: str) -> list:
    """Return names of all typedef struct X X; forward declarations."""
    pat = re.compile(r"typedef\s+struct\s+(\w+)\s+\1\s*;")
    return [m.group(1) for m in pat.finditer(text)]


# ─── Struct body parsing ──────────────────────────────────────────────────────
def extract_struct_body(text: str, brace_start: int):
    """Starting at the '{' at brace_start, return (body_incl_braces, end_pos)."""
    depth = 0
    i = brace_start
    n = len(text)
    while i < n:
        c = text[i]
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                return text[brace_start:i + 1], i + 1
        i += 1
    return None, n


def parse_fields(body: str, fn_ptr_types: set, safe_base_types: set) -> list:
    """
    Extract safe, bindable field names from a struct body (including outer braces).
    Skips: function pointer fields, anonymous nested structs, private/compiler fields,
    and fields whose base type is not in safe_base_types.
    """
    fields = []
    inner = body[1:-1]  # strip outer { }

    # Walk char-by-char, collecting statements at brace depth 0
    depth = 0
    token_start = 0
    i = 0
    n = len(inner)

    while i < n:
        c = inner[i]
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth < 0:
                depth = 0
        elif c == ";" and depth == 0:
            stmt = inner[token_start:i].strip()
            token_start = i + 1
            field = _try_parse_field(stmt, fn_ptr_types, safe_base_types)
            if field:
                fields.append(field)
        i += 1

    return fields


def _try_parse_field(stmt: str, fn_ptr_types: set, safe_base_types: set):
    """Return field name if the statement is a safe, bindable field declaration."""
    stmt = re.sub(r"\s+", " ", stmt).strip()
    if not stmt:
        return None

    # Skip preprocessor, inline comments, keyword-only lines
    if stmt.startswith(("#", "//", "/*")):
        return None

    # Skip if braces/parens remain (nested struct, fn-like macro, etc.)
    if "{" in stmt or "}" in stmt or "(" in stmt:
        return None

    # Strip array dimensions to isolate the base declaration
    stmt_no_arr = re.sub(r"\s*\[.*?\]\s*", "", stmt).strip()
    if not stmt_no_arr:
        return None

    # Extract tokens (treating * as whitespace)
    tokens = stmt_no_arr.replace("*", " ").split()
    if len(tokens) < 2:
        return None

    field_name = tokens[-1]
    type_tokens = tokens[:-1]

    # Skip compiler/internal names
    if not re.match(r"^[A-Za-z_]\w*$", field_name):
        return None
    if field_name.startswith("__"):
        return None
    # Skip private-by-convention (trailing underscore, e.g. priv_)
    if field_name.endswith("_") and len(field_name) > 1:
        return None

    # Base type: last type token stripped of *
    base_type = type_tokens[-1].strip("*") if type_tokens else ""

    # Skip function pointer fields
    if base_type in fn_ptr_types:
        return None

    # Only include fields whose base type is known-safe
    if base_type not in safe_base_types:
        return None

    return field_name


def parse_defined_structs(text: str, fn_ptr_types: set, safe_base_types: set) -> list:
    """
    Return [(struct_name, [field_names])] for all structs with visible bodies.
    Handles both 'typedef struct X { } X;' and 'struct X { };' patterns.
    """
    results = []
    seen_names = set()

    pat = re.compile(r"\b(typedef\s+)?struct\s+(\w+)\s*\{")
    for m in pat.finditer(text):
        is_typedef = bool(m.group(1))
        struct_name = m.group(2)

        # Skip anonymous or compiler-internal structs
        if not struct_name or struct_name.startswith("__"):
            continue

        # Find the body
        body, end = extract_struct_body(text, m.end() - 1)
        if not body:
            continue

        # Determine the canonical type name
        if is_typedef:
            rest = text[end:].lstrip()
            alias_m = re.match(r"(\w+)\s*;", rest)
            type_name = alias_m.group(1) if alias_m else struct_name
        else:
            type_name = struct_name

        if not type_name or type_name in seen_names:
            continue
        seen_names.add(type_name)

        fields = parse_fields(body, fn_ptr_types, safe_base_types)
        results.append((type_name, fields))

    return results


# ─── Enum parsing ──────────────────────────────────────────────────────────────
def parse_enums(text: str) -> list:
    """Return [(enum_name, [member_name, ...])] for all typedef enum blocks."""
    results = []
    pat = re.compile(
        r"typedef\s+enum\s+(\w+)\s*\{([^}]*)\}\s*\1\s*;", re.DOTALL
    )
    for m in pat.finditer(text):
        enum_name = m.group(1)
        body = m.group(2)
        members = []
        for line in body.splitlines():
            line = line.strip().rstrip(",").strip()
            if not line:
                continue
            # Take only the name (before any = sign)
            name = re.split(r"[=,\s]", line)[0].strip()
            if name and re.match(r"^[A-Za-z_]\w*$", name):
                members.append(name)
        results.append((enum_name, members))
    return results


# ─── FLECS_API extern const parsing ──────────────────────────────────────────
def parse_extern_consts(text: str) -> list:
    """Return [(c_type, symbol_expr)] for FLECS_API extern const declarations."""
    pat = re.compile(
        r"FLECS_API\s+extern\s+const\s+([A-Za-z_]\w*)\s+([^;]+?)\s*;"
    )
    results = []
    for m in pat.finditer(text):
        c_type = m.group(1).strip()
        symbol_expr = m.group(2).strip()
        if not symbol_expr:
            continue
        results.append((c_type, symbol_expr))
    return results


def sanitize_const_symbol(symbol_expr: str) -> str:
    """Convert C constant expression to a stable, function-safe identifier suffix."""
    cleaned = symbol_expr.strip()
    cleaned = cleaned.replace("::", "_")
    cleaned = cleaned.replace("(", "_")
    cleaned = cleaned.replace(")", "")
    cleaned = cleaned.replace(",", "_")
    cleaned = cleaned.replace(" ", "")
    cleaned = re.sub(r"[^A-Za-z0-9_]", "_", cleaned)
    cleaned = re.sub(r"_+", "_", cleaned).strip("_")
    if not cleaned:
        cleaned = "Const"
    if cleaned[0].isdigit():
        cleaned = f"Const_{cleaned}"
    return cleaned


def collect_supported_extern_consts(extern_consts: list) -> list:
    """
    Keep externally useful Flecs constants and attach generated getter names.

    Returns list of dicts:
      {
        'c_type': 'ecs_entity_t'|'ecs_id_t',
        'expr': 'EcsTarget'|'ecs_id(EcsComponent)'|...,
        'das_name': 'EcsTarget'
      }
    """
    supported_types = {"ecs_entity_t", "ecs_id_t"}
    out = []
    seen_names = set()

    for c_type, expr in extern_consts:
        if expr in SKIP_EXTERN_CONSTS:
            continue

        if c_type not in supported_types:
            continue

        keep = (
            expr.startswith("Ecs")
            or expr.startswith("ECS_")
        )
        if not keep:
            continue

        suffix = sanitize_const_symbol(expr)
        if suffix in seen_names:
            continue
        seen_names.add(suffix)

        out.append({
            "c_type": c_type,
            "expr": expr,
            "das_name": suffix,
        })

    return out


# ─── FLECS_API function parsing ───────────────────────────────────────────────
def collect_flecs_api_decls(text: str) -> list:
    """Collect raw text of each FLECS_API function declaration."""
    decls = []
    pos = 0
    n = len(text)

    while True:
        idx = text.find("FLECS_API", pos)
        if idx == -1:
            break
        pos = idx + 9

        # Skip extern (variable exports)
        rest = text[idx + 9:].lstrip()
        if rest.startswith("extern"):
            continue

        # Collect up to the closing `;` at paren depth 0
        end = idx + 9
        depth = 0
        while end < n:
            c = text[end]
            if c == "(":
                depth += 1
            elif c == ")":
                depth -= 1
            elif c == ";" and depth == 0:
                decls.append(text[idx:end + 1])
                break
            elif c == "{":
                break  # hit a body — not a forward decl
            end += 1

    return decls


def parse_func_decl(decl: str):
    """
    Parse a FLECS_API function declaration.
    Returns (ret_type, func_name, params_str) or None.
    """
    text = re.sub(r"^FLECS_API\s*", "", decl.strip())
    text = re.sub(r"\s+", " ", text).strip()

    # Last word before '(' is the function name; everything before is return type
    # Find the opening paren of the parameter list
    paren_start = text.find("(")
    if paren_start == -1:
        return None

    before_paren = text[:paren_start].strip()
    # The function name is the last word
    name_m = re.search(r"\b(\w+)\s*$", before_paren)
    if not name_m:
        return None

    func_name = name_m.group(1)
    ret_type = before_paren[:name_m.start()].strip()

    # Find the matching close paren
    paren_end = text.rfind(")")
    if paren_end == -1:
        return None
    params = text[paren_start + 1:paren_end].strip()

    # Only bind ecs_* and flecs_* functions
    if not func_name.startswith(("ecs_", "flecs_")):
        return None

    return ret_type, func_name, params


# Macros that can appear between FLECS_API and the real return type
_QUALIFIER_MACROS_RE = re.compile(
    r"\b(FLECS_ALWAYS_INLINE|FLECS_DEPRECATED|FLECS_API|__attribute__\s*\(\([^)]*\)\)|"
    r"__declspec\s*\([^)]*\)|__inline|__forceinline|static|inline|extern)\b"
)


def _clean_ret_type(ret_type: str) -> str:
    """Strip qualifier macros and normalize whitespace from a return type string."""
    cleaned = _QUALIFIER_MACROS_RE.sub("", ret_type)
    return re.sub(r"\s+", " ", cleaned).strip()


def classify_return(ret_type: str, defined_struct_names: set, opaque_names: set,
                    known_scalar_types: set) -> str:
    """Return 'plain', 'copy_or_move', or 'skip'."""
    ret_type = _clean_ret_type(ret_type)
    is_pointer = "*" in ret_type
    base = re.sub(r"[*\s]|const", "", ret_type).strip()

    if is_pointer:
        return "plain"
    if base in ("void", ""):
        return "plain"
    if base in defined_struct_names:
        return "copy_or_move"
    if base in opaque_names:
        # Opaque struct by value — size unknown, unsafe to bind
        return "skip"
    if base in known_scalar_types:
        return "plain"
    # Unknown type (defined in a private/included header) — could be a struct.
    # Binding as plain would cause das::cast<> compile errors; skip instead.
    return "skip"


# ─── Naming helpers ──────────────────────────────────────────────────────────
def to_annotation_class(struct_name: str) -> str:
    """'ecs_query_count_t' → 'EcsQueryCountAnnotation'"""
    name = struct_name
    for prefix in ("ecs_", "flecs_"):
        if name.startswith(prefix):
            name = name[len(prefix):]
            break
    else:
        # No ecs_/flecs_ prefix — use struct name as class base (e.g. EcsComponent)
        if name[0].isupper():
            return f"{name}Annotation"
        return f"{name.capitalize()}Annotation"

    if name.endswith("_t"):
        name = name[:-2]
    parts = [p.capitalize() for p in name.split("_") if p]
    return f"Ecs{''.join(parts)}Annotation"


# ─── Unknown param-type discovery ─────────────────────────────────────────────

def collect_unknown_types(
    functions: list,
    known_types: set,
    skip_funcs: set,
    fn_ptr_types: set,
) -> list:
    """
    Walk every bound function's parameter types AND return types, and collect
    base struct type names not in known_types.  These come from private headers
    not parsed here and must be registered at least as opaque types so that
    das::makeType<T>() compiles.
    Excludes function-pointer typedef names.
    Returns a sorted, deduplicated list of unknown type names.
    """
    _STRIP_RE = re.compile(r"[*\s]|const\b|restrict\b")
    unknown = set()

    for ret_type, func_name, params, classification in functions:
        if func_name in skip_funcs or classification == "skip":
            continue

        # --- Return type ---
        cleaned_ret = _clean_ret_type(ret_type)
        ret_base = re.sub(r"[*\s]|const\b", "", cleaned_ret).strip()
        if (ret_base.startswith("ecs_") or ret_base.startswith("flecs_")):
            if ret_base not in fn_ptr_types and ret_base not in known_types:
                unknown.add(ret_base)

        # --- Parameter types ---
        if not params or params.strip() in ("void", ""):
            continue
        for param in params.split(","):
            param = param.strip()
            if not param or param == "...":
                continue
            no_arr = re.sub(r"\s*\[.*?\]", "", param)
            tokens = _STRIP_RE.sub(" ", no_arr).split()
            if len(tokens) < 2:
                continue
            base = tokens[-2]  # second-to-last = type name, last = param name
            if not (base.startswith("ecs_") or base.startswith("flecs_")):
                continue
            if base in fn_ptr_types:
                continue
            if base not in known_types:
                unknown.add(base)

    return sorted(unknown)


# ─── Code generation ──────────────────────────────────────────────────────────
_HEADER = """\
// GENERATED - do not edit manually.
// Regenerate with: bash manage.sh gen-flecs-bind
"""


def gen_annotations(
    opaque_names: list,
    defined_structs: list,
    skip_structs: set,
    extra_opaque: list = (),
) -> str:
    lines = [_HEADER]

    # Opaque types: declared-opaque + extra (from private headers) - anything with a body
    defined_names = {n for n, _ in defined_structs}
    true_opaque = [n for n in opaque_names
                   if n not in skip_structs and n not in defined_names]
    all_opaque = list(dict.fromkeys(true_opaque + [n for n in extra_opaque
                                                    if n not in skip_structs
                                                    and n not in defined_names
                                                    and n not in true_opaque]))
    if all_opaque:
        lines.append("// --- Opaque types (DummyTypeAnnotation) ---\n")
    for name in all_opaque:
        cls = to_annotation_class(name)
        lines.append(f"MAKE_TYPE_FACTORY({name}, {name});")
        lines.append(f"struct {cls} : das::DummyTypeAnnotation {{")
        lines.append(f"    {cls}()")
        lines.append(
            f'        : DummyTypeAnnotation("{name}", "{name}",'
            " sizeof(void*), alignof(void*)) {}"
        )
        lines.append("};")
        lines.append("")

    # Defined structs
    if any(n not in skip_structs for n, _ in defined_structs):
        lines.append("// --- Defined structs (ManagedStructureAnnotation) ---\n")
    for name, fields in defined_structs:
        if name in skip_structs:
            continue
        cls = to_annotation_class(name)
        lines.append(f"MAKE_TYPE_FACTORY({name}, {name});")
        lines.append(
            f"struct {cls} : das::ManagedStructureAnnotation<{name}, false> {{"
        )
        lines.append(
            f"    {cls}(das::ModuleLibrary &ml)"
            f' : ManagedStructureAnnotation("{name}", ml) {{'
        )
        for field in fields:
            das_name = DAS_RESERVED_FIELD_RENAMES.get(field, field)
            lines.append(
                f'        addField<DAS_BIND_MANAGED_FIELD({field})>("{das_name}", "{field}");'
            )
        lines.append("    }")
        lines.append("};")
        lines.append("")

    return "\n".join(lines)


def gen_register(
    opaque_names: list,
    defined_structs: list,
    functions: list,
    skip_structs: set,
    skip_funcs: set,
    extra_opaque: list = (),
    extern_consts: list = (),
) -> str:
    lines = [_HEADER]

    # Defined struct names (those with bodies)
    defined_names = {n for n, _ in defined_structs}
    true_opaque = [n for n in opaque_names
                   if n not in skip_structs and n not in defined_names]
    all_opaque = list(dict.fromkeys(true_opaque + [n for n in extra_opaque
                                                    if n not in skip_structs
                                                    and n not in defined_names
                                                    and n not in true_opaque]))

    lines.append("// --- Annotation registrations ---")
    lines.append("")
    for name in all_opaque:
        cls = to_annotation_class(name)
        lines.append(f"addAnnotation(new {cls}());")

    for name, _ in defined_structs:
        if name in skip_structs:
            continue
        cls = to_annotation_class(name)
        lines.append(f"addAnnotation(new {cls}(lib));")

    lines.append("")
    lines.append("// --- Function registrations ---")
    lines.append("")

    for item in extern_consts:
        name = item["das_name"]
        expr = item["expr"]
        c_type = item["c_type"]
        lines.append(
            f"addConstant(*this, \"{name}\", ({c_type}){expr});"
        )

    if extern_consts:
        lines.append("")

    for ret_type, func_name, params, classification in functions:
        if func_name in skip_funcs:
            continue
        if classification == "skip":
            lines.append(
                f"// SKIPPED: {func_name} (struct-by-value return of opaque type)"
            )
            continue
        side_effects = "das::SideEffects::modifyExternal"
        if classification == "copy_or_move":
            lines.append(
                f"das::addExtern<DAS_BIND_FUN({func_name}),"
                f" das::SimNode_ExtFuncCallAndCopyOrMove>"
                f'(*this, lib, "{func_name}", {side_effects}, "{func_name}");'
            )
        else:
            lines.append(
                f"das::addExtern<DAS_BIND_FUN({func_name})>"
                f'(*this, lib, "{func_name}", {side_effects}, "{func_name}");'
            )

    return "\n".join(lines)


def gen_flecs_das_additions(
    scalar_typedefs: list,
    enums: list,
) -> str:
    lines = ["", "// BEGIN GENERATED (gen_flecs_bindings.py) - do not edit below this line"]

    # Scalar typedefs not already in the file
    if scalar_typedefs:
        lines.append("")
        lines.append("// Type aliases")
        for name, das_type in scalar_typedefs:
            lines.append(f"typedef {name} = {das_type};")

    # Extra hardcoded aliases from private headers
    for name, das_type in EXTRA_SCALAR_TYPEDEFS.items():
        if name not in EXISTING_DAS_TYPEDEFS:
            lines.append(f"typedef {name} = {das_type};")

    # Enum constants
    if enums:
        lines.append("")
        lines.append("// Enum constants")
        for enum_name, members in enums:
            lines.append(f"// {enum_name}")
            for i, member in enumerate(members):
                lines.append(f"let {member} : int = {i}")

    return "\n".join(lines) + "\n"


# ─── Main ──────────────────────────────────────────────────────────────────────
def main():
    if len(sys.argv) != 4:
        print(f"Usage: {sys.argv[0]} <flecs.h> <out_dir> <flecs.das>")
        sys.exit(1)

    header_path, out_dir, flecs_das_path = sys.argv[1], sys.argv[2], sys.argv[3]

    print(f"Reading {header_path} ...")
    with open(header_path, encoding="utf-8", errors="replace") as f:
        raw = f.read()
    text = strip_comments(raw)

    # ── Parse ────────────────────────────────────────────────────────────────
    fn_ptr_types    = parse_fn_ptr_typedefs(text)
    opaque_names    = parse_opaque_structs(text)
    scalar_typedefs = parse_scalar_typedefs(text)
    void_typedefs   = parse_void_typedefs(text)

    # Build the set of "safe" base types for field inclusion
    safe_base_types = (
        PRIMITIVE_BASE_TYPES
        | set(EXTRA_SCALAR_TYPEDEFS.keys())
        | set(EXISTING_DAS_TYPEDEFS)
        | {name for name, _ in scalar_typedefs}
        | set(opaque_names)        # pointers to opaque types are safe
    )

    defined_structs = parse_defined_structs(text, fn_ptr_types, safe_base_types)
    defined_struct_names = {n for n, _ in defined_structs}

    # Extend safe_base_types with the defined struct names (for nested refs)
    safe_base_types |= defined_struct_names

    # Known scalar types for return-type classification (no structs, just scalars + void)
    known_scalar_types = (
        PRIMITIVE_BASE_TYPES
        | set(EXTRA_SCALAR_TYPEDEFS.keys())
        | set(EXISTING_DAS_TYPEDEFS)
        | {name for name, _ in scalar_typedefs}
        | void_typedefs      # typedef void X — treated as void, not struct
        | fn_ptr_types       # fn-ptr typedefs — handled separately, not struct annotations
    )

    enums = parse_enums(text)
    extern_consts = collect_supported_extern_consts(parse_extern_consts(text))

    # ── Functions ────────────────────────────────────────────────────────────
    raw_decls = collect_flecs_api_decls(text)
    functions = []
    skipped_params = []

    for decl in raw_decls:
        parsed = parse_func_decl(decl)
        if not parsed:
            continue
        ret_type, func_name, params = parsed

        # Skip functions with array-style params (e.g. char *argv[])
        if "[" in params:
            skipped_params.append(func_name)
            continue
        # Skip variadic
        if "..." in params:
            skipped_params.append(func_name)
            continue
        # Skip functions with function-pointer parameters (need lambda trampolines)
        _strip = re.compile(r"[*\s]|const\b|restrict\b")
        has_fn_ptr_param = any(
            _strip.sub(" ", re.sub(r"\s*\[.*?\]", "", p)).split()[-2:-1]
            and _strip.sub(" ", re.sub(r"\s*\[.*?\]", "", p)).split()[-2] in fn_ptr_types
            for p in params.split(",") if p.strip()
        )
        if has_fn_ptr_param:
            skipped_params.append(func_name)
            continue

        classification = classify_return(ret_type, defined_struct_names,
                                         set(opaque_names), known_scalar_types)
        functions.append((ret_type, func_name, params, classification))

    # ── Collect extra opaque types from function parameters ──────────────────
    all_known = (
        known_scalar_types | set(opaque_names) | defined_struct_names | SKIP_STRUCTS
    )
    extra_opaque = collect_unknown_types(functions, all_known, SKIP_FUNCS, fn_ptr_types)

    # ── Report ───────────────────────────────────────────────────────────────
    print(f"  Opaque structs:   {len(opaque_names)}")
    print(f"  Defined structs:  {len(defined_structs)}")
    print(f"  Enums:            {len(enums)}")
    print(f"  Extern consts:    {len(extern_consts)}")
    print(f"  Functions:        {len(functions)}")
    print(f"  Skipped (params): {skipped_params}")
    skipped_copy = [f for _, f, _, c in functions if c == "skip"]
    if skipped_copy:
        print(f"  Skipped (opaque struct-by-value): {skipped_copy}")
    if extra_opaque:
        print(f"  Extra opaque (from private headers): {extra_opaque}")

    # ── Write outputs ────────────────────────────────────────────────────────
    os.makedirs(out_dir, exist_ok=True)

    ann_path = os.path.join(out_dir, "module_flecs_annotations.inc")
    reg_path = os.path.join(out_dir, "module_flecs_register.inc")

    with open(ann_path, "w", newline="\n") as f:
        f.write(gen_annotations(opaque_names, defined_structs, SKIP_STRUCTS, extra_opaque))
    print(f"  Wrote {ann_path}")

    with open(reg_path, "w", newline="\n") as f:
        f.write(gen_register(opaque_names, defined_structs, functions,
                             SKIP_STRUCTS, SKIP_FUNCS, extra_opaque,
                             extern_consts))
    print(f"  Wrote {reg_path}")

    # ── Update flecs.das ──────────────────────────────────────────────────────
    with open(flecs_das_path, encoding="utf-8") as f:
        das_content = f.read()

    # Strip any previously generated section
    marker = "\n// BEGIN GENERATED"
    cut = das_content.find(marker)
    if cut != -1:
        das_content = das_content[:cut]

    das_content = das_content.rstrip() + "\n"
    das_content += gen_flecs_das_additions(scalar_typedefs, enums)


    with open(flecs_das_path, "w", newline="\n", encoding="utf-8") as f:
        f.write(das_content)
    print(f"  Updated {flecs_das_path}")

    print("Done.")


if __name__ == "__main__":
    main()

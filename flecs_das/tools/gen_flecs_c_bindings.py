#!/usr/bin/env python3
"""
gen_flecs_c_bindings.py
Generate flecs_c.das — Daslang Layer 2 convenience wrappers for flecs_c.h macros.

Usage: python gen_flecs_c_bindings.py <output_das>
"""

import sys
import textwrap

# ---------------------------------------------------------------------------
# Category A — Simple _id wrappers  (no sizeof, no cast)
# def ecs_foo(world; entity; id) -> raw_fn(world, entity, id)
# ---------------------------------------------------------------------------
#   (das_name, raw_fn, extra_params, extra_args, return_type)
#   extra_params / extra_args: additional parameters beyond (world, entity, id)
CAT_A = [
    # name                   raw function              extra params          extra args  return
    ("ecs_add",              "ecs_add_id",             [],                   [],         "void"),
    ("ecs_remove",           "ecs_remove_id",          [],                   [],         "void"),
    ("ecs_auto_override",    "ecs_auto_override_id",   [],                   [],         "void"),
    ("ecs_modified",         "ecs_modified_id",        [],                   [],         "void"),
    ("ecs_has",              "ecs_has_id",             [],                   [],         "bool"),
    ("ecs_owns",             "ecs_owns_id",            [],                   [],         "bool"),
    ("ecs_enable_component", "ecs_enable_id",          ["enable: bool"],     ["enable"], "void"),
    ("ecs_is_enabled",       "ecs_is_enabled_id",      [],                   [],         "bool"),
]

# ecs_count uses only (world, id) — no entity param
CAT_A_COUNT = True

# ecs_get_target_for uses (world, entity, rel, id) — rel comes before id
CAT_A_TARGET_FOR = True

# new_w uses (world, id) — no entity param
CAT_A_NEW_W = [
    ("ecs_new_w",   "ecs_new_w_id",   "ecs_entity_t"),
]

# bulk_new uses (world, id, count)
CAT_A_BULK = [
    ("ecs_bulk_new", "ecs_bulk_new_w_id"),
]

# each uses (world, id) — returns ecs_iter_t
CAT_A_EACH = [
    ("ecs_each", "ecs_each_id"),
]

# ---------------------------------------------------------------------------
# Category B — Pair wrappers
# def ecs_foo_pair(world; entity; first; second [; extra]) -> raw_fn(world, entity, ecs_make_pair(first,second) [, extra])
# ---------------------------------------------------------------------------
#   (das_name, raw_fn, extra_params, extra_args, return_type)
CAT_B = [
    ("ecs_add_pair",           "ecs_add_id",           [],                   [],         "void"),
    ("ecs_remove_pair",        "ecs_remove_id",        [],                   [],         "void"),
    ("ecs_auto_override_pair", "ecs_auto_override_id", [],                   [],         "void"),
    ("ecs_modified_pair",      "ecs_modified_id",      [],                   [],         "void"),
    ("ecs_has_pair",           "ecs_has_id",           [],                   [],         "bool"),
    ("ecs_owns_pair",          "ecs_owns_id",          [],                   [],         "bool"),
    ("ecs_enable_pair",        "ecs_enable_id",        ["enable: bool"],     ["enable"], "void"),
    ("ecs_is_enabled_pair",    "ecs_is_enabled_id",    [],                   [],         "bool"),
]

# new_w_pair and each_pair use (world, first, second) — no entity
CAT_B_NO_ENTITY = [
    ("ecs_new_w_pair",  "ecs_new_w_id",  "ecs_entity_t"),
    ("ecs_each_pair",   "ecs_each_id",   "ecs_iter_t"),
]

# ---------------------------------------------------------------------------
# Category C — Pointer-returning generics with type<auto(T)> phantom parameter
# ---------------------------------------------------------------------------
#   (das_name, raw_fn, extra_params, extra_args, return_type_template, needs_sizeof)
#   return_type_template uses {cv} for const qualifier
CAT_C = [
    # name              raw_fn              extra_params  extra_args  return_cv  needs_sizeof
    ("ecs_get",         "ecs_get_id",       [],           [],         "const",   False),
    ("ecs_get_mut",     "ecs_get_mut_id",   [],           [],         "",        False),
    ("ecs_ensure",      "ecs_ensure_id",    [],           [],         "",        True),
    ("ecs_ref_get",     "ecs_ref_get_id",   [],           [],         "",        False),
    ("ecs_table_get",   "ecs_table_get_id", ["offset: int"], ["offset"], "",     False),
]

# emplace needs sizeof + is_new bool?
CAT_C_EMPLACE = True

# ---------------------------------------------------------------------------
# Category E — Iterator field access
# ---------------------------------------------------------------------------
# ecs_field      → flecs_field_w_size      (existing C++ helper)
# ecs_field_self → flecs_field_self_w_size (new C++ helper)
# ecs_field_at   → flecs_field_at_w_size   (new C++ helper)

# ---------------------------------------------------------------------------
# Category H — Path shorthands (hardcode "." separator)
# ---------------------------------------------------------------------------


# ===========================================================================
# Helpers
# ===========================================================================

def ret_stmt(ret_type: str, expr: str) -> str:
    if ret_type == "void":
        return f"{expr}"
    else:
        return f"return {expr}"


def gen_cat_a(entries) -> str:
    lines = ["// Category A — Simple _id wrappers", ""]
    for (name, raw, extra_params, extra_args, ret) in entries:
        params = ["world: ecs_world_t?", "entity: ecs_entity_t", "id: ecs_entity_t"] + extra_params
        args   = ["world", "entity", "id"] + extra_args
        sig    = "; ".join(params)
        call   = f"{raw}({', '.join(args)})"
        body   = ret_stmt(ret, call)
        ret_ann = f" : {ret}" if ret != "void" else ""
        lines.append(f"def {name}({sig}){ret_ann} {{")
        lines.append(f"    {body}")
        lines.append("}")
        lines.append("")
    return "\n".join(lines)


def gen_cat_a_new_w(entries) -> str:
    lines = []
    for (name, raw, ret) in entries:
        lines.append(f"def {name}(world: ecs_world_t?; id: ecs_entity_t) : {ret} {{")
        lines.append(f"    return {raw}(world, id)")
        lines.append("}")
        lines.append("")
    return "\n".join(lines)


def gen_cat_a_bulk() -> str:
    return textwrap.dedent("""\
        def ecs_bulk_new(world: ecs_world_t?; id: ecs_entity_t; count: int) : ecs_entity_t const? {
            return ecs_bulk_new_w_id(world, id, count)
        }
        """)


def gen_cat_a_each() -> str:
    return textwrap.dedent("""\
        def ecs_each(world: ecs_world_t?; id: ecs_entity_t) : ecs_iter_t {
            return ecs_each_id(world, id)
        }
        """)


def gen_cat_a_count() -> str:
    return textwrap.dedent("""\
        def ecs_count(world: ecs_world_t?; id: ecs_entity_t) : int {
            return ecs_count_id(world, id)
        }
        """)


def gen_cat_a_target_for() -> str:
    return textwrap.dedent("""\
        def ecs_get_target_for(world: ecs_world_t?; entity: ecs_entity_t; rel: ecs_entity_t; id: ecs_entity_t) : ecs_entity_t {
            return ecs_get_target_for_id(world, entity, rel, id)
        }
        """)


def gen_cat_b(entries) -> str:
    lines = ["// Category B — Pair wrappers", ""]
    for (name, raw, extra_params, extra_args, ret) in entries:
        params = ["world: ecs_world_t?", "entity: ecs_entity_t",
                  "first: ecs_entity_t", "second: ecs_entity_t"] + extra_params
        args   = ["world", "entity", "ecs_make_pair(first, second)"] + extra_args
        sig    = "; ".join(params)
        call   = f"{raw}({', '.join(args)})"
        body   = ret_stmt(ret, call)
        ret_ann = f" : {ret}" if ret != "void" else ""
        lines.append(f"def {name}({sig}){ret_ann} {{")
        lines.append(f"    {body}")
        lines.append("}")
        lines.append("")
    return "\n".join(lines)


def gen_cat_b_no_entity(entries) -> str:
    lines = []
    for (name, raw, ret) in entries:
        lines.append(f"def {name}(world: ecs_world_t?; first: ecs_entity_t; second: ecs_entity_t) : {ret} {{")
        lines.append(f"    return {raw}(world, ecs_make_pair(first, second))")
        lines.append("}")
        lines.append("")
    return "\n".join(lines)


def gen_cat_c(entries) -> str:
    lines = ["// Category C — Pointer-returning generics", ""]
    for (name, raw, extra_params, extra_args, cv, needs_sizeof) in entries:
        # ecs_get / ecs_get_mut: first param is world, second entity, third id, last type<auto(T)>
        # ecs_ref_get: first world, second ref: ecs_ref_t?, third id, last type<auto(T)>
        # ecs_table_get: first world, second table: ecs_table_t?, third id, extra offset, last type<auto(T)>
        if name == "ecs_ref_get":
            base_params = ["world: ecs_world_t?", "ref: ecs_ref_t?", "id: ecs_entity_t"]
            base_args   = ["world", "ref", "id"]
        elif name == "ecs_table_get":
            base_params = ["world: ecs_world_t?", "tbl: ecs_table_t?", "id: ecs_entity_t"]
            base_args   = ["world", "tbl", "id"]
        else:
            base_params = ["world: ecs_world_t?", "entity: ecs_entity_t", "id: ecs_entity_t"]
            base_args   = ["world", "entity", "id"]

        params = base_params + extra_params + ["_t: type<auto(T)>"]
        args   = base_args + extra_args
        if needs_sizeof:
            args.append("typeinfo sizeof(type<T>)")

        sig  = "; ".join(params)
        call = f"{raw}({', '.join(args)})"
        cv_suffix = " const" if cv == "const" else ""
        ret_type = f"T?{cv_suffix}"
        reint    = f"reinterpret<{ret_type}>({call})"

        lines.append(f"def {name}({sig}) : {ret_type} {{")
        lines.append(f"    unsafe {{ return {reint}; }}")
        lines.append("}")
        lines.append("")
    return "\n".join(lines)


def gen_cat_c_emplace() -> str:
    return textwrap.dedent("""\
        def ecs_emplace(world: ecs_world_t?; entity: ecs_entity_t; id: ecs_entity_t; is_new: bool?; _t: type<auto(T)>) : T? {
            unsafe { return reinterpret<T?>(ecs_emplace_id(world, entity, id, typeinfo sizeof(type<T>), is_new)); }
        }
        """)


def gen_cat_d() -> str:
    return textwrap.dedent("""\
        // Category D — Value-setting

        def ecs_set(world: ecs_world_t?; entity: ecs_entity_t; id: ecs_entity_t; var value: auto) {
            unsafe { flecs_set_id(world, entity, id, typeinfo sizeof(value), addr(value)); }
        }

        def ecs_set_ptr(world: ecs_world_t?; entity: ecs_entity_t; id: ecs_entity_t; size: int; ptr: void?) {
            unsafe { flecs_set_id(world, entity, id, size, ptr); }
        }
        """)


def gen_cat_e() -> str:
    return textwrap.dedent("""\
        // Category E — Iterator field access

        // Read-only access (T? const result when indexed).
        def ecs_field(var it: ecs_iter_t; _t: type<auto(T)>; index: int8) : T? {
            unsafe { return reinterpret<T?>(flecs_field_w_size(addr(it), typeinfo sizeof(type<T>), index)); }
        }

        def ecs_field_self(var it: ecs_iter_t; _t: type<auto(T)>; index: int8) : T? {
            unsafe { return reinterpret<T?>(flecs_field_self_w_size(addr(it), typeinfo sizeof(type<T>), index)); }
        }

        def ecs_field_at(var it: ecs_iter_t; _t: type<auto(T)>; index: int8; row: int) : T? {
            unsafe { return reinterpret<T?>(flecs_field_at_w_size(addr(it), typeinfo sizeof(type<T>), index, row)); }
        }

        // Mutable access — must be called from within an unsafe block.
        // Returns void? so the caller reinterprets in their own unsafe scope,
        // which is what enables p[i].field += ... to compile without "left side can't be constant".
        // Usage:
        //   unsafe {
        //       var p = reinterpret<Position?>(ecs_field_mut(addr(it), typeinfo sizeof(type<Position>), int8(0)))
        //       p[i].x += v[i].x
        //   }
        def ecs_field_mut(it: ecs_iter_t?; size: int; index: int8) : void? {
            unsafe { return flecs_field_w_size(it, size, index); }
        }

        def ecs_field_self_mut(it: ecs_iter_t?; size: int; index: int8) : void? {
            unsafe { return flecs_field_self_w_size(it, size, index); }
        }

        def ecs_field_at_mut(it: ecs_iter_t?; size: int; index: int8; row: int) : void? {
            unsafe { return flecs_field_at_w_size(it, size, index, row); }
        }
        """)


def gen_cat_f() -> str:
    return textwrap.dedent("""\
        // Category F — Component registration  (replaces ECS_COMPONENT boilerplate)

        def ecs_component_register(world: ecs_world_t?; _t: type<auto(T)>) : ecs_entity_t {
            let tname = typeinfo stripped_typename(type<T>)
            let edesc = ecs_entity_desc_t(name=tname, symbol=tname, use_low_id=true)
            var entity: ecs_entity_t
            unsafe { entity = ecs_entity_init(world, addr(edesc)); }
            var cdesc = ecs_component_desc_t(entity=entity, vtype=ecs_type_info_t(
                size=typeinfo sizeof(type<T>), alignment=typeinfo alignof(type<T>)))
            var id: ecs_entity_t
            unsafe { id = ecs_component_init(world, addr(cdesc)); }
            return id
        }
        """)


def gen_cat_g() -> str:
    return textwrap.dedent("""\
        // Category G — Init shorthands

        def ecs_entity_new(world: ecs_world_t?; name: string) : ecs_entity_t {
            let edesc = ecs_entity_desc_t(name=name)
            var entity: ecs_entity_t
            unsafe { entity = ecs_entity_init(world, addr(edesc)); }
            return entity
        }

        def ecs_query_str(world: ecs_world_t?; expr: string) : ecs_query_t? {
            var qdesc: ecs_query_desc_t
            ecs_query_desc_set_expr(qdesc, expr)
            var q: ecs_query_t?
            unsafe { q = ecs_query_init(world, addr(qdesc)); }
            return q
        }
        """)


def gen_cat_h() -> str:
    return textwrap.dedent("""\
        // Category H — Path shorthands  (hardcoded "::" separator, NULL prefix via C wrappers)

        def ecs_lookup_from(world: ecs_world_t?; parent: ecs_entity_t; path: string) : ecs_entity_t {
            return flecs_lookup_path(world, parent, path, true)
        }

        def ecs_get_path_from(world: ecs_world_t?; parent: ecs_entity_t; child: ecs_entity_t) : string const {
            return flecs_get_path_from(world, parent, child)
        }

        def ecs_get_path(world: ecs_world_t?; child: ecs_entity_t) : string const {
            return flecs_get_path_from(world, 0ul, child)
        }

        def ecs_new_from_path(world: ecs_world_t?; parent: ecs_entity_t; path: string) : ecs_entity_t {
            return flecs_new_from_path(world, parent, path)
        }

        def ecs_add_path(world: ecs_world_t?; entity: ecs_entity_t; parent: ecs_entity_t; path: string) : ecs_entity_t {
            return flecs_add_path(world, entity, parent, path)
        }

        def ecs_add_fullpath(world: ecs_world_t?; entity: ecs_entity_t; path: string) : ecs_entity_t {
            return flecs_add_path(world, entity, 0ul, path)
        }
        """)


def generate(out_path: str) -> None:
    header = textwrap.dedent("""\
        options gen2
        // GENERATED — do not edit manually.
        // Regenerate with: bash manage.sh gen-flecs-c-bind

        require flecs_core

        """)

    sep = "// " + "=" * 62 + "\n"

    sections = [
        header,
        sep,
        gen_cat_a(CAT_A),
        gen_cat_a_count(),
        gen_cat_a_target_for(),
        gen_cat_a_new_w(CAT_A_NEW_W),
        gen_cat_a_bulk(),
        gen_cat_a_each(),
        sep,
        gen_cat_b(CAT_B),
        gen_cat_b_no_entity(CAT_B_NO_ENTITY),
        sep,
        gen_cat_c(CAT_C),
        gen_cat_c_emplace(),
        sep,
        gen_cat_d(),
        sep,
        gen_cat_e(),
        sep,
        gen_cat_f(),
        sep,
        gen_cat_g(),
        sep,
        gen_cat_h(),
    ]

    content = "\n".join(sections)

    with open(out_path, "w", encoding="utf-8") as f:
        f.write(content)

    print(f"Generated: {out_path}")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <output.das>")
        sys.exit(1)
    generate(sys.argv[1])

#!/usr/bin/env python3

from __future__ import annotations

import re
import sys
from pathlib import Path


TYPE_MAP = {
    "bool": "bool",
    "int": "int",
    "int8": "signed char",
    "int16": "short",
    "int32": "int",
    "int64": "long long",
    "uint": "unsigned int",
    "uint8": "unsigned char",
    "uint16": "unsigned short",
    "uint32": "unsigned int",
    "uint64": "unsigned long long",
    "float": "float",
    "string": "const char *",
    "void": "void",
}


def map_type(type_text: str) -> str:
    type_text = type_text.strip()
    return TYPE_MAP.get(type_text, type_text)


def transform_typedef(line: str) -> str | None:
    match = re.match(r"^(\s*)typedef\s+([A-Za-z_][\w]*)\s*=\s*([^;]+);?\s*$", line)
    if not match:
        return None
    indent, name, type_text = match.groups()
    return f"{indent}typedef {map_type(type_text)} {name};"


def transform_let(line: str) -> str | None:
    match = re.match(r"^(\s*)let\s+([A-Za-z_][\w]*)\s*(?::\s*([^=]+?))?\s*=\s*(.+?)\s*$", line)
    if not match:
        return None
    indent, name, type_text, value = match.groups()
    mapped_type = map_type(type_text.strip()) if type_text else "int"
    return f"{indent}{mapped_type} {name} = {value};"


def extract_param_name(param_text: str) -> str:
    param_text = param_text.strip()
    if not param_text:
        return ""
    param_text = param_text.split("=", 1)[0].strip()
    if ":" in param_text:
        param_text = param_text.split(":", 1)[0].strip()
    tokens = [token for token in re.split(r"\s+", param_text) if token and token != "var"]
    if not tokens:
        return ""
    return tokens[-1].replace("&", "")


def transform_def(signature: str) -> list[str]:
    exact_signature = signature.strip()
    cleaned = exact_signature.removeprefix("def ").strip()
    header, _, _ = cleaned.partition("{")
    header = header.strip()
    name_match = re.match(r"^([A-Za-z_][\w]*)\s*\((.*)\)(?:\s*:\s*([^\{]+))?$", header)
    if not name_match:
        return ["/// @code", f"/// {exact_signature}", "/// @endcode", f"void {header};"]
    name, params_text, _return_text = name_match.groups()
    params = []
    for raw_param in params_text.split(";"):
        param_name = extract_param_name(raw_param)
        if param_name:
            params.append(param_name)
    return ["/// @code", f"/// {exact_signature}", "/// @endcode", f"void {name}({', '.join(params)});"]


def main() -> int:
    if len(sys.argv) < 2:
        print("Expected an input file path", file=sys.stderr)
        return 1

    input_path = Path(sys.argv[-1])
    lines = input_path.read_text(encoding="utf-8").splitlines()

    inside_function = False
    brace_depth = 0
    signature_parts: list[str] = []

    for raw_line in lines:
        line = raw_line.rstrip("\n")
        stripped = line.strip()

        if inside_function:
            brace_depth += line.count("{")
            brace_depth -= line.count("}")
            if brace_depth <= 0:
                inside_function = False
                brace_depth = 0
            continue

        if signature_parts:
            signature_parts.append(line)
            if "{" in line:
                for output_line in transform_def(" ".join(signature_parts)):
                    print(output_line)
                inside_function = True
                brace_depth = sum(part.count("{") - part.count("}") for part in signature_parts)
                signature_parts.clear()
            continue

        if not stripped:
            print()
            continue

        if stripped.startswith("//") or stripped.startswith("/*") or stripped.startswith("*") or stripped.startswith("@"):
            print(line)
            continue

        if stripped.startswith("options ") or stripped.startswith("require "):
            print(f"// {stripped}")
            continue

        typedef_line = transform_typedef(line)
        if typedef_line is not None:
            print(typedef_line)
            continue

        let_line = transform_let(line)
        if let_line is not None:
            print(let_line)
            continue

        if stripped.startswith("def "):
            if "{" in line:
                for output_line in transform_def(line):
                    print(output_line)
                inside_function = True
                brace_depth = line.count("{") - line.count("}")
                if brace_depth <= 0:
                    inside_function = False
                    brace_depth = 0
            else:
                signature_parts.append(line)
            continue

        print(line)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

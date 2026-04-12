#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
GENERATE_XXD="$SCRIPT_DIR/flecs_das/tools/generate_xxd.py"
DASLANG="$SCRIPT_DIR/flecs_das/vendor/daslang/build/Debug/daslang.exe"
GEN_ADAPTER_DAS="$SCRIPT_DIR/flecs_das/src/gen_bind.das"
GEN_BIND_DAS="$SCRIPT_DIR/flecs_das/src/gen_bind.das"
SCRIPT_INC="$SCRIPT_DIR/flecs_das/src"
DAS_FORMATTER="$SCRIPT_DIR/flecs_das/vendor/daslang/utils/dasCodeFormatter/main.das"
DASROOT="$SCRIPT_DIR/flecs_das/vendor/daslang"

GEN_FLECS_BIND_PY="$SCRIPT_DIR/flecs_das/tools/gen_flecs_bindings.py"
GENERATED_DIR="$SCRIPT_DIR/flecs_das/src/generated"
FLECS_DAS="$SCRIPT_DIR/flecs_das/src/scripts/flecs.das"

GEN_FLECS_C_BIND_PY="$SCRIPT_DIR/flecs_das/tools/gen_flecs_c_bindings.py"
FLECS_C_DAS="$SCRIPT_DIR/flecs_das/src/scripts/flecs_c.das"
DOXYGEN_DIR="$SCRIPT_DIR/docs"

# Resolve Flecs headers from either:
# 1) local checkout under flecs-daslang/flecs
# 2) sibling Duin vendor checkout under ../flecs
if [[ -f "$SCRIPT_DIR/flecs/include/flecs.h" ]]; then
    FLECS_H="$SCRIPT_DIR/flecs/include/flecs.h"
elif [[ -f "$SCRIPT_DIR/../flecs/include/flecs.h" ]]; then
    FLECS_H="$SCRIPT_DIR/../flecs/include/flecs.h"
else
    FLECS_H="$SCRIPT_DIR/flecs/include/flecs.h"
fi

if [[ -f "$SCRIPT_DIR/flecs/include/flecs/addons/flecs_c.h" ]]; then
    FLECS_C_H="$SCRIPT_DIR/flecs/include/flecs/addons/flecs_c.h"
elif [[ -f "$SCRIPT_DIR/../flecs/include/flecs/addons/flecs_c.h" ]]; then
    FLECS_C_H="$SCRIPT_DIR/../flecs/include/flecs/addons/flecs_c.h"
else
    FLECS_C_H="$SCRIPT_DIR/flecs/include/flecs/addons/flecs_c.h"
fi

usage() {
    echo "Usage: $0 <command> [args]"
    echo ""
    echo "Commands:"
    echo "  gen-inc           Regenerate all .das.inc files from their .das sources"
    echo "  gen-adapter       Regenerate flecs_das_bind_gen.inc via cpp_bind"
    echo "  gen-flecs-bind    Parse flecs.h and generate C++ binding fragments + update flecs.das"
    echo "  fmt <file.das>    Format a .das file in-place (backs up, formats, verifies)"
    echo "  fmt-all           Format all .das files under flecs_das/src"
    echo "  gen-flecs-c-bind  Generate flecs_c.das Layer 2 wrappers from gen_flecs_c_bindings.py"
    echo "  docs              Build the Doxygen HTML documentation"
  echo "  codegen           Run the full pipeline: gen-flecs-bind, gen-flecs-c-bind, gen-inc, gen-adapter"
    echo "  help              Show this help message"
}

cmd_gen_inc() {
    local script_dir="$SCRIPT_DIR/flecs_das/src"
    local count=0

    while IFS= read -r -d '' das_file; do
        local inc_file="${das_file}.inc"
        echo "  ${das_file##*/} -> ${inc_file##*/}"
        python "$GENERATE_XXD" "$das_file" "$inc_file"
        (( count++ )) || true
    done < <(find "$script_dir" -name "*.das" -print0)

    echo "Generated $count file(s)."
}

cmd_gen_adapter() {
    local out="$SCRIPT_DIR/flecs_das/src/flecs_das_bind_gen.inc"
    local dasroot="$SCRIPT_DIR/flecs_das/vendor/daslang"
    local project="$SCRIPT_INC/gen_bind.das_project"
    echo "Running gen_bind.das via daslang..."
    "$DASLANG" -dasroot "$dasroot" -no-dynamic-modules -project "$project" "$GEN_BIND_DAS" -- "$out"
}

cmd_fmt() {
    local das_file="${1:-}"
    if [[ -z "$das_file" ]]; then
        echo "Usage: $0 fmt <file.das>"
        exit 1
    fi
    local bak="${das_file}.bak"
    echo "  Formatting ${das_file##*/}..."
    cp "$das_file" "$bak"
    if "$DASLANG" -dasroot "$DASROOT" "$DAS_FORMATTER" -- "$das_file"; then
        rm -f "$bak"
        echo "  Done."
    else
        echo "  Formatter failed — restoring backup."
        cp "$bak" "$das_file"
        rm -f "$bak"
        exit 1
    fi
}

cmd_fmt_all() {
    local script_dir="$SCRIPT_DIR/flecs_das/src"
    local count=0

    while IFS= read -r -d '' das_file; do
        cmd_fmt "$das_file"
        (( count++ )) || true
    done < <(find "$script_dir" -name "*.das" -print0)

    echo "Formatted $count file(s)."
}

cmd_gen_flecs_bind() {
    echo "Parsing flecs.h and generating C++ binding fragments..."
    python3 "$GEN_FLECS_BIND_PY" "$FLECS_H" "$GENERATED_DIR" "$FLECS_DAS"
}

cmd_gen_flecs_c_bind() {
    echo "Generating flecs_c.das Layer 2 wrappers..."
    python3 "$GEN_FLECS_C_BIND_PY" "$FLECS_C_DAS"
}

cmd_codegen() {
    echo "==> gen-flecs-bind"
    cmd_gen_flecs_bind
    echo "==> gen-flecs-c-bind"
    cmd_gen_flecs_c_bind
    echo "==> gen-inc"
    cmd_gen_inc
    echo "==> gen-adapter"
    cmd_gen_adapter
}

cmd_docs() {
    echo "Building Doxygen documentation..."
    mkdir -p "$SCRIPT_DIR/generated_docs"
    (cd "$DOXYGEN_DIR" && doxygen Doxyfile)
}

case "${1:-}" in
    gen-inc)         cmd_gen_inc ;;
    gen-adapter)     cmd_gen_adapter ;;
    gen-flecs-bind)   cmd_gen_flecs_bind ;;
    gen-flecs-c-bind) cmd_gen_flecs_c_bind ;;
    fmt)             cmd_fmt "${2:-}" ;;
    fmt-all)         cmd_fmt_all ;;
    docs)            cmd_docs ;;
    codegen)         cmd_codegen ;;
    help|"")         usage ;;
    *)               echo "Unknown command: $1"; usage; exit 1 ;;
esac

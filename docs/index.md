# Flecs Daslang Documentation

This documentation covers the C++ bridge and the Daslang bindings for the `flecs_das` project.

## What lives where

- `flecs_das/src/flecs_das.h`: public consumer header for loading the module.
- `flecs_das/src/module_flecs.h`: module declaration and Flecs C API bridge.
- `flecs_das/src/module_flecs.cpp`: handwritten helpers, type annotations, and module registration.
- `flecs_das/src/scripts/flecs.das`: base Daslang aliases and generated metadata.
- `flecs_das/src/scripts/flecs_helpers.das`: handwritten Daslang helpers for queries, terms, and field access.
- `flecs_das/src/scripts/flecs_c.das`: generated convenience wrappers over the raw Flecs API.
- `flecs_das_tests/scripts/*.das`: example and integration test scripts that show how the bindings are used.

## Build the docs

From the repository root:

```bash
bash manage.sh docs
```

That command runs Doxygen with the repository-local configuration in this folder and writes HTML output to `generated_docs/doxygen`.

## Notes

- The generated `.inc` files are part of the build, but they are not the source of truth for the documentation.
- The `.das` files are filtered into a Doxygen-friendly form so the handwritten API remains documented without rewriting the binding sources.

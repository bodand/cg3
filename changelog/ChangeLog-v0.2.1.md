# Cg3 v0.2.1 changelog

## Breaking changes

- cg3-db now generates absolute paths to files in the compilation database; this now changes error output using clang
  diagnostics to generate full paths instead of relative to the project directory

## Compatible changes

- Add documentation for driver exectuable cg3 (#5)
- Add dependency for subprocess handling (related to #5): reproc.

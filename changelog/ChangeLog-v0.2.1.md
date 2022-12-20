# Cg3 v0.2.1 changelog

## Breaking changes

- cg3-db now generates absolute paths to files in the compilation database; this now changes error output using clang
  diagnostics to generate full paths instead of relative to the project directory
- cg3-db command line parser changed; this probably introduces incompatibilities compared to previous argument parsing
  behavior
- cg3-db now handles multiple paths to create db of
- cg3-db now handles files as well as directories

## Dependency changes

- Removed Lyra dependency
- Added InfoCLI. This is the author's own argument parser, resurrected from the dead. It is
  quite liberal on argument parsing, so we will now probably accept command lines we did not before. However, it does
  not support subcommands as of now, so we will have to implement that manually.

## Compatible changes

- Add documentation for driver exectuable cg3 (#5)
- Add new exectuable cg3 (#5)
- Add dependency for subprocess handling (related to #5): reproc.
- cg3-db now includes default include paths for GCC and MSVC (#11)
- update documentations to reflect changes
- internal: path_finder files now handle files being passed to them

## Bugfixes

- Substring filters returned inverse of what they were supposed to be doing... now they work as they should


# Cg3 v0.2.1 changelog

## Breaking changes

- cg3-db now generates absolute paths to files in the compilation database; this now changes error output using clang
  diagnostics to generate full paths instead of relative to the project directory
- cg3-db command line parser changed; this probably introduces incompatibilities between argument parsing behaviors. 

## Dependency changes

- Changed Lyra argument parser to InfoCLI. This is the authors own argument parser, resurrected from the dead. It is quite liberal on argument parsing, so we will now probably accept command lines we did not before. However, it does not support subcommands as of now, so we will have to implement that manually. Or backport it.

## Compatible changes

- Add documentation for driver exectuable cg3 (#5)
- Add dependency for subprocess handling (related to #5): reproc.
- cg3-db now includes default include paths for GCC and MSVC (#11)


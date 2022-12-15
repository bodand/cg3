# Cg3 v0.2.0 changelog

## Breaking changes

- Rename project's packages to cg3-checker
- Rename cg3 executable to cg3-check
- Changed bugmalloc check to use clang diagnostics instead of homebrew error formatting.
- Changed chonktion check to use clang diagnostics instead of homebrew error formatting.
- Changed fleak check to use clang diagnostics instead of homebrew error formatting; minimal change in internal data
  format.

## Compatible changes

- Bump minor version, for a lot of internal changes occurred, and incompatible changes in the output format. (While
  MAJOR is 0, anything can be changed freely.)

## Package changes

- DEB: Package now contains a--hopefully--correctly formatted copyright file.

## Infrastructure changes

- Added changelog directory for storing changelogs for the releases. These will also be used in the GitHub Releases so
  they are in Markdown to allow us to skip the translation step.
- Cirrus CI deploy task should now be able to upload artifacts if the release already exists with that name.

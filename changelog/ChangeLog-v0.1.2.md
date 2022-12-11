# Cg3 v0.1.2 changelog

- Changed bugmalloc check to use clang diagnostics instead of homebrew error formatting.
- Changed chonktion check to use clang diagnostics instead of homebrew error formatting.
- Added changelog directory for storing changelogs for the releases. These will also be used in the GitHub Releases so they are in Markdown to allow us to skip the translation step.
- Cirrus CI deploy task should now be able to upload artifacts if the release already exists with that name.
- Changed fleak check to use clang diagnostics instead of homebrew error formatting; minimal change in internal data format.

# Cg3 v0.2.2 changelog

## Internal changes

- WIP releases should now be created only as Drafts
- Automate upgrading completed draft releases to complete releases using Starlark
- Automate deleting incomplete releases using Starlark
- Extract all logic from .cirrus.yml into external shell scripts
- Write Windows specific batch/powershell scripts for CI/CD
- Windows CI
- Windows CD
- Releases now contain Windows MSI installers

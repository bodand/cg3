echo:Uploading to release id %GH_REL_ID%...
curl -X POST ^
  -H "Accept: application/vnd.github+json" ^
  -H "Authorization: Bearer %GITHUB_TOKEN%" ^
  -H "X-GitHub-Api-Version: 2022-11-28" ^
  -H "Content-Type: %~2" ^
  "https://uploads.github.com/repos/bodand/cg3/releases/%GH_REL_ID%/assets?name=%~1" ^
  --data-binary "@%~1"

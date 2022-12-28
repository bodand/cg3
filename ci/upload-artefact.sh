#!/usr/bin/env sh

filename=$0
type=$1
if [ -z "$type" ]; then
  type="application/octet-stream"
fi

curl -X POST \
  -H "Accept: application/vnd.github+json" \
  -H "Authorization: Bearer ${GITHUB_TOKEN}" \
  -H "X-GitHub-Api-Version: 2022-11-28" \
  -H "Content-Type: $type" \
  "https://uploads.github.com/repos/bodand/cg3/releases/${GH_REL_ID}/assets?name=$filename" \
  --data-binary "@$filename"

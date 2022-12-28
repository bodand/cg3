#!/usr/bin/env sh

jq --null-input \
  --arg tag_name "${CIRRUS_TAG}" \
  --arg name "cg3 release ${CIRRUS_TAG}" \
  --arg body "$(cat ${CIRRUS_WORKING_DIR}/changelog/ChangeLog-${CIRRUS_TAG}.md)" \
  '{"tag_name": $tag_name, "name": $name, "body": $body, "generate_release_notes": false}' \
  >release-payload.json

curl -X POST \
  -H "Accept: application/vnd.github+json" \
  -H "Authorization: Bearer ${GITHUB_TOKEN}" \
  -H "X-GitHub-Api-Version: 2022-11-28" \
  "https://api.github.com/repos/bodand/cg3/releases" \
  -d "@release-payload.json"

curl \
  -H "Accept: application/vnd.github+json" \
  -H "Authorization: Bearer ${GITHUB_TOKEN}" \
  -H "X-GitHub-Api-Version: 2022-11-28" \
  "https://api.github.com/repos/bodand/cg3/releases/tags/${CIRRUS_TAG}" | tee response.json
echo "GH_REL_ID=$(jq .id response.json)" >${CIRRUS_ENV}

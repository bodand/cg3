#!/usr/bin/env sh

cd "${BUILD_PATH}" || exit 1
llvm-profdata merge "$(find . -type f -name "*.profraw")" -o collected.profdata

test_exes=$(find . -type f -name "cg3-*-test" | sed -e 's/^/--object /' | tr "\n" ' ' | cut -f'2-' -d' ')
llvm-cov export --format=lcov --instr-profile collected.profdata "$test_exes" > lcov.info

echo "Uploading to Coveralls ${CIRRUS_BRANCH}/${CIRRUS_BUILD_ID}"
coveralls-lcov -t "${COVERALLS_TOKEN}" -b "${CIRRUS_BRANCH}" -s "cirrus-ci" --service-job-id="${CIRRUS_BUILD_ID%}" lcov.info

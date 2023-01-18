setlocal ENABLEDELAYEDEXPANSION
setlocal ENABLEEXTENSIONS
cd %BUILD_PATH%

set "files="
for /f "usebackq delims=" %%r IN (`dir . /s /b ^| findstr /i /e ".profraw"`) DO set "files=!files! %%r"
llvm-profdata merge !files! -o collected.profdata

set "test_exes="
for /f "usebackq delims=" %%t IN (`dir . /s /b ^| findstr /i /r "cg3-[-a-z]*-test.exe$"`) DO set "test_exes=!test_exes! %%t"
llvm-cov export --format=lcov --instr-profile collected.profdata !test_exes! > lcov.info

@echo:Uploading to Coveralls %CIRRUS_BRANCH%/%CIRRUS_BUILD_ID%
@coveralls-lcov -t %COVERALLS_TOKEN% -b %CIRRUS_BRANCH% -s "cirrus-ci" --service-job-id=%CIRRUS_BUILD_ID% lcov.info

endlocal

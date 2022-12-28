cd %BUILD_PATH%
for /f "usebackq" %%v in (`type versioninfo.txt`) do echo "CG_VER=%%v" > %CIRRUS_ENV%

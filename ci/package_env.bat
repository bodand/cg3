@REM Windows is fix, we don't need to emulate the `uname` part
cd %BUILD_PATH%
for /f "usebackq" %%a in (`winarch`) do echo "CG_PKG_NAME=cg3-checker-Windows-%%a" > %CIRRUS_ENV%

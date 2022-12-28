for /f "usebackq tokens=*" %%f in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath`) do set "vsdir=%%f"
"%vsdir%\Common7\Tools\vsdevcmd.bat" -arch=x64 -host_arch=x64

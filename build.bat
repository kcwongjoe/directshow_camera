@echo off



:: Delete all files in build except vcpkg_installed
echo Delete all files in build......
pushd ".\build\" || exit /B 1
for /D %%D in ("*") do (
    if /I not "%%~nxD"=="vcpkg_installed" rd /S /Q "%%~D"
)
for %%F in ("*") do (
    del "%%~F"
)
popd

:: Get complier version
set "SYSVERSION=X64"
if "%~1" == "x86" (
    set "SYSVERSION=X86"
)
if "%~1" == "x64" (
    set "SYSVERSION=X64"
)

:: Build (cmake)
echo Start to build by cmake.....
if "%SYSVERSION%" == "X86" (
    echo Building x86 MakeFile
    cmake -B ./build -G "Visual Studio 17 2022" -A win32 --preset=default
) else (
    echo Building x64 MakeFile
    cmake -B ./build -G "Visual Studio 17 2022" -A x64 --preset=default
)

:: Copy dll from vcpkg
:: if "%SYSVERSION%" == "X86" (
::     call copy_dll.bat "x86"
:: ) else (
::     call copy_dll.bat "x64"
::)


echo Finished
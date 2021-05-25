@echo off

:: Delete all files in build
echo Delete all files in build......
del /q ".\build\*"
rmdir ".\build\.vs" /s /q
for /D %%p in (".\build\*.*") do rmdir "%%p" /s /q

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
    cmake -B ./build -G "Visual Studio 16 2019" -A win32
) else (
    echo Building x64 MakeFile
    cmake -B ./build -G "Visual Studio 16 2019" -A x64
)

:: Copy dll from vcpkg
if "%SYSVERSION%" == "X86" (
    call copy_dll.bat "x86"
) else (
    call copy_dll.bat "x64"
)


echo Finished
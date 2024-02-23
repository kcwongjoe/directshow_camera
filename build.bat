@echo off

:: Create dependencies
if not exist ".\dependencies" mkdir dependencies

:: Delete all files in build
if exist ".\build" (
    echo Delete all files in build......
    pushd ".\build\" || exit /B 1
    for /D %%D in ("*") do (
        rd /S /Q "%%~D"
    )
    for %%F in ("*") do (
        del "%%~F"
    )
    popd
) else (
    echo Create build directory......
    mkdir build
)

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
    cmake -B ./build -G "Visual Studio 17 2022" -A win32
) else (
    echo Building x64 MakeFile
    cmake -B ./build -G "Visual Studio 17 2022" -A x64
)

echo Finished
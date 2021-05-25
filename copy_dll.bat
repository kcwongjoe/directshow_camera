@echo off
setlocal enabledelayedexpansion

:: Get current path
SET CURRENT_PATH=%~dp0

:: Load vcpkg path from config folder. If vcpkg_path.txt is not found, current folder will be used.
set "VCPKG_PATH_TXT_FILE=.\config\vcpkg_path.txt"

if exist %VCPKG_PATH_TXT_FILE% (
    set /p VCPKG_PATH=<%VCPKG_PATH_TXT_FILE%
) else (
    set "VCPKG_PATH=.\vcpkg"
)

:: Window version
if "%~1" == "x86" (
    set "VCPKG_PATH=%VCPKG_PATH%\installed\x86-windows"
)
if "%~1" == "x64" (
    set "VCPKG_PATH=%VCPKG_PATH%\installed\x64-windows"
)

:: Get vcpkg dll location
set "VCPKG_PATH_DEBUG=%VCPKG_PATH%\debug\bin"
set "VCPKG_PATH_RELEASE=%VCPKG_PATH%\bin"

:: Set Copy target information
set "TARGET_DIR[0]=build/examples"

mkdir build
for /l %%i in (0,1) do ( 
    call set "CURRENT_TARGET_DIR=%%TARGET_DIR[%%i]%%" 

    :: Make folder
    mkdir "!CURRENT_TARGET_DIR!"
    mkdir "!CURRENT_TARGET_DIR!/Debug"
    mkdir "!CURRENT_TARGET_DIR!/Release"

    :: Go to Debug
    cd !CURRENT_TARGET_DIR!/Debug

    echo "Start to copy dll from %VCPKG_PATH_DEBUG% to !CURRENT_TARGET_DIR!/Debug"

    :: Copy dll
    xcopy "%VCPKG_PATH_DEBUG%\jpeg62.dll" "."
    xcopy "%VCPKG_PATH_DEBUG%\libpng16d.dll" "."
    xcopy "%VCPKG_PATH_DEBUG%\lzmad.dll" "."
    xcopy "%VCPKG_PATH_DEBUG%\opencv_calib3dd.dll" "."
    xcopy "%VCPKG_PATH_DEBUG%\opencv_cored.dll" "."
    xcopy "%VCPKG_PATH_DEBUG%\opencv_features2dd.dll" "."
    xcopy "%VCPKG_PATH_DEBUG%\opencv_flannd.dll" "."
    xcopy "%VCPKG_PATH_DEBUG%\opencv_imgcodecsd.dll" "."
    xcopy "%VCPKG_PATH_DEBUG%\opencv_imgprocd.dll" "."
    xcopy "%VCPKG_PATH_DEBUG%\opencv_photod.dll" "."
    xcopy "%VCPKG_PATH_DEBUG%\opencv_stitchingd.dll" "."
    xcopy "%VCPKG_PATH_DEBUG%\tiffd.dll" "."
    xcopy "%VCPKG_PATH_DEBUG%\webpd.dll" "."
    xcopy "%VCPKG_PATH_DEBUG%\zlibd1.dll" "."

    :: Exit Debug
    cd %CURRENT_PATH%

    echo "Start to copy dll from %VCPKG_PATH_RELEASE% to !CURRENT_TARGET_DIR!/Release"

    :: Go to Release
    cd !CURRENT_TARGET_DIR!/Release

    :: Copy dll
    xcopy "%VCPKG_PATH_RELEASE%\jpeg62.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\libpng16.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\lzma.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\opencv_calib3d.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\opencv_core.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\opencv_features2d.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\opencv_flann.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\opencv_imgcodecs.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\opencv_imgproc.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\opencv_photo.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\opencv_stitching.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\tiff.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\webp.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\zlib1.dll" "."

    :: Exit Debug
    cd %CURRENT_PATH%

)
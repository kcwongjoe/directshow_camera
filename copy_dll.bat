@echo off

:: Set vcpkg path. If vcpkg is not installed in current folder, change this path.
:; For example: set "VCPKG_PATH=C:/vcpkg"
set "VCPKG_PATH=.\vcpkg"

:: Window version
if "%~1" == "x86" (
    set "VCPKG_PATH=%VCPKG_PATH%\installed\x86-windows"
)
if "%~1" == "x64" (
    set "VCPKG_PATH=%VCPKG_PATH%\installed\x64-windows"
)

set "VCPKG_PATH_DEBUG=%VCPKG_PATH%\debug\bin"
set "VCPKG_PATH_RELEASE=%VCPKG_PATH%\bin"

:: Build Dll in Debug Folder
if exist build/Debug (

    :: Go to Debug
    cd ./build/Debug

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

    :: Exit Dbug
    cd ../../
)

:: Build Dll in Release Folder
if exist build/Release (

    :: Go to Release
    cd ./build/Release

    :: Copy dll
    xcopy "%VCPKG_PATH_RELEASE%\jpeg62.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\libpng16d.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\lzmad.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\opencv_calib3dd.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\opencv_cored.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\opencv_features2dd.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\opencv_flannd.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\opencv_imgcodecsd.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\opencv_imgprocd.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\opencv_photod.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\opencv_stitchingd.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\opengl32sw.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\tiffd.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\webpd.dll" "."
    xcopy "%VCPKG_PATH_RELEASE%\zlibd1.dll" "."

    :: Exit Dbug
    cd ../../
)
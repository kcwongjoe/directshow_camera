# A Macro to install OpenCV from VCPKG
macro(install_vcpkg_opencv)
    # OpenCV
    if (CMAKE_SIZEOF_VOID_P EQUAL 4)
        # 32bit
        set( OpenCV_INSTALLED_DIR "${VCPKG_PATH}/installed/x86-windows" )
        set( OpenCV_DIR "${OpenCV_INSTALLED_DIR}/share/opencv" )  # Opencv path

        if(NOT DEFINED OpenCV_LIB_DIR)
            set( OpenCV_LIB_DIR "${VCPKG_PATH}/installed/x86-windows/lib" "${VCPKG_PATH}/installed/x86-windows/debug/lib")  # Opencv lib path
        endif()
    elseif ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
        # 64bit
        set( OpenCV_INSTALLED_DIR "${VCPKG_PATH}/installed/x64-windows" )
        set( OpenCV_DIR "${OpenCV_INSTALLED_DIR}/share/opencv" )  # Opencv path

        if(NOT DEFINED OpenCV_LIB_DIR)
            set( OpenCV_LIB_DIR "${VCPKG_PATH}/installed/x64-windows/lib" "${VCPKG_PATH}/installed/x64-windows/debug/lib")  # Opencv lib path
        endif()
    endif()

    # Install
    find_package(OpenCV REQUIRED
        opencv_core
        opencv_imgcodecs
        opencv_imgproc
        opencv_photo
    )


endmacro()

# Pre-build process: Copy opencv dll from VCPKG
macro(prebuild_copy_vcpkg_opencv_dll PostBuildTarget CopyLocation)

    add_custom_command(TARGET ${PostBuildTarget} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_INSTALLED_DIR}/debug/bin/jpeg62.dll" "${CopyLocation}/Debug/jpeg62.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_INSTALLED_DIR}/debug/bin/libpng16d.dll" "${CopyLocation}/Debug/libpng16d.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_INSTALLED_DIR}/debug/bin/lzmad.dll" "${CopyLocation}/Debug/lzmad.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_INSTALLED_DIR}/debug/bin/opencv_cored.dll" "${CopyLocation}/Debug/opencv_cored.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_INSTALLED_DIR}/debug/bin/opencv_imgcodecsd.dll" "${CopyLocation}/Debug/opencv_imgcodecsd.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_INSTALLED_DIR}/debug/bin/opencv_imgprocd.dll" "${CopyLocation}/Debug/opencv_imgprocd.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_INSTALLED_DIR}/debug/bin/opencv_photod.dll" "${CopyLocation}/Debug/opencv_photod.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_INSTALLED_DIR}/debug/bin/tiffd.dll" "${CopyLocation}/Debug/tiffd.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_INSTALLED_DIR}/debug/bin/webpd.dll" "${CopyLocation}/Debug/webpd.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_INSTALLED_DIR}/debug/bin/zlibd1.dll" "${CopyLocation}/Debug/zlibd1.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_INSTALLED_DIR}/bin/jpeg62.dll" "${CopyLocation}/Release/jpeg62.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_INSTALLED_DIR}/bin/libpng16.dll" "${CopyLocation}/Release/libpng16.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_INSTALLED_DIR}/bin/lzma.dll" "${CopyLocation}/Release/lzma.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_INSTALLED_DIR}/bin/opencv_core.dll" "${CopyLocation}/Release/opencv_core.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_INSTALLED_DIR}/bin/opencv_imgcodecs.dll" "${CopyLocation}/Release/opencv_imgcodecs.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_INSTALLED_DIR}/bin/opencv_imgproc.dll" "${CopyLocation}/Release/opencv_imgproc.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_INSTALLED_DIR}/bin/opencv_photo.dll" "${CopyLocation}/Release/opencv_photo.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_INSTALLED_DIR}/bin/tiff.dll" "${CopyLocation}/Release/tiff.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_INSTALLED_DIR}/bin/webp.dll" "${CopyLocation}/Release/webp.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OpenCV_INSTALLED_DIR}/bin/zlib1.dll" "${CopyLocation}/Release/zlib1.dll"
    )
endmacro()

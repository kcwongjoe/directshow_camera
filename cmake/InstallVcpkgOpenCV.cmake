# A Macro to install OpenCV from VCPKG
macro(install_vcpkg_opencv)

    # Platform folder name
    if (CMAKE_SIZEOF_VOID_P EQUAL 4)
        # 32bit
        set(PLATFORM_FOLDER "x86-windows")
    elseif ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
        # 64bit
        set(PLATFORM_FOLDER "x64-windows")
    endif()

    # Search vcpkg dependency in vcpkg manifest mode
    if (EXISTS "${CMAKE_BINARY_DIR}/vcpkg_installed/${PLATFORM_FOLDER}/share/opencv")
        set(MY_VCPKG_DEPENDENCIES_DIR "${CMAKE_BINARY_DIR}/vcpkg_installed/${PLATFORM_FOLDER}")
    endif()

    # Search vcpkg dependency in vcpkg root
    if (NOT DEFINED MY_VCPKG_DEPENDENCIES_DIR AND EXISTS "${VCPKG_PATH}/installed/${PLATFORM_FOLDER}/share/opencv")
        set(MY_VCPKG_DEPENDENCIES_DIR "${VCPKG_PATH}/installed/${PLATFORM_FOLDER}")
    endif()
    
    # OpenCV
    if (DEFINED MY_VCPKG_DEPENDENCIES_DIR)

        # OpenCV Path
        set( OpenCV_DIR "${MY_VCPKG_DEPENDENCIES_DIR}/share/opencv" )  # Opencv path

        if(NOT DEFINED OpenCV_LIB_DIR)
            set( OpenCV_LIB_DIR "${MY_VCPKG_DEPENDENCIES_DIR}/lib" "${MY_VCPKG_DEPENDENCIES_DIR}/debug/lib")  # Opencv lib path
        endif()

        # Install
        find_package(OpenCV REQUIRED
            opencv_core
            opencv_imgcodecs
            opencv_imgproc
            opencv_photo
        )
    else()
        message("Fail to install OpenCV. Library is not found in ${CMAKE_BINARY_DIR}/vcpkg_installed and ${VCPKG_PATH}/installed.")
    endif()

endmacro()

# Pre-build process: Copy opencv dll from VCPKG
macro(prebuild_copy_vcpkg_opencv_dll PostBuildTarget CopyLocation)

    add_custom_command(TARGET ${PostBuildTarget} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MY_VCPKG_DEPENDENCIES_DIR}/debug/bin/jpeg62.dll" "${CopyLocation}/Debug/jpeg62.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MY_VCPKG_DEPENDENCIES_DIR}/debug/bin/libpng16d.dll" "${CopyLocation}/Debug/libpng16d.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MY_VCPKG_DEPENDENCIES_DIR}/debug/bin/lzmad.dll" "${CopyLocation}/Debug/lzmad.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MY_VCPKG_DEPENDENCIES_DIR}/debug/bin/opencv_cored.dll" "${CopyLocation}/Debug/opencv_cored.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MY_VCPKG_DEPENDENCIES_DIR}/debug/bin/opencv_imgcodecsd.dll" "${CopyLocation}/Debug/opencv_imgcodecsd.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MY_VCPKG_DEPENDENCIES_DIR}/debug/bin/opencv_imgprocd.dll" "${CopyLocation}/Debug/opencv_imgprocd.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MY_VCPKG_DEPENDENCIES_DIR}/debug/bin/opencv_photod.dll" "${CopyLocation}/Debug/opencv_photod.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MY_VCPKG_DEPENDENCIES_DIR}/debug/bin/tiffd.dll" "${CopyLocation}/Debug/tiffd.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MY_VCPKG_DEPENDENCIES_DIR}/debug/bin/webpd.dll" "${CopyLocation}/Debug/webpd.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MY_VCPKG_DEPENDENCIES_DIR}/debug/bin/zlibd1.dll" "${CopyLocation}/Debug/zlibd1.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MY_VCPKG_DEPENDENCIES_DIR}/bin/jpeg62.dll" "${CopyLocation}/Release/jpeg62.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MY_VCPKG_DEPENDENCIES_DIR}/bin/libpng16.dll" "${CopyLocation}/Release/libpng16.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MY_VCPKG_DEPENDENCIES_DIR}/bin/lzma.dll" "${CopyLocation}/Release/lzma.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MY_VCPKG_DEPENDENCIES_DIR}/bin/opencv_core.dll" "${CopyLocation}/Release/opencv_core.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MY_VCPKG_DEPENDENCIES_DIR}/bin/opencv_imgcodecs.dll" "${CopyLocation}/Release/opencv_imgcodecs.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MY_VCPKG_DEPENDENCIES_DIR}/bin/opencv_imgproc.dll" "${CopyLocation}/Release/opencv_imgproc.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MY_VCPKG_DEPENDENCIES_DIR}/bin/opencv_photo.dll" "${CopyLocation}/Release/opencv_photo.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MY_VCPKG_DEPENDENCIES_DIR}/bin/tiff.dll" "${CopyLocation}/Release/tiff.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MY_VCPKG_DEPENDENCIES_DIR}/bin/webp.dll" "${CopyLocation}/Release/webp.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MY_VCPKG_DEPENDENCIES_DIR}/bin/zlib1.dll" "${CopyLocation}/Release/zlib1.dll"
    )
endmacro()

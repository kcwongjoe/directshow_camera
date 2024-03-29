# A Macro to install OpenCV
macro(install_opencv)

    set(OpenCV_DIR "${DIRECTSHOWCAMERA_ROOT_PATH}/dependencies/opencv/build")  # Opencv path

    if (EXISTS ${OpenCV_DIR})
        # Install
        find_package(OpenCV REQUIRED
            opencv_core
            opencv_imgcodecs
            opencv_imgproc
            opencv_photo
        )

        # Add define HAS_OPENCV
        add_compile_definitions(HAS_OPENCV)
    else()
        message("Fail to install OpenCV. Library is not found in ${OpenCV_DIR}.")
    endif()

endmacro()

# Pre-build process: Copy opencv dll
macro(prebuild_copy_opencv_dll PreBuildTarget CopyLocation)

    # Platform folder name
    if (CMAKE_SIZEOF_VOID_P EQUAL 4)
        # 32bit
        set(PLATFORM_FOLDER "x86")
    elseif ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
        # 64bit
        set(PLATFORM_FOLDER "x64")
    endif()

        # Copy opencv dll
        add_custom_command(TARGET ${PreBuildTarget} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${OpenCV_DIR}/${PLATFORM_FOLDER}/vc16/bin/opencv_world490d.dll "${CopyLocation}/Debug/opencv_world490d.dll"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${OpenCV_DIR}/${PLATFORM_FOLDER}/vc16/bin/opencv_world490.dll "${CopyLocation}/Release/opencv_world490.dll"
        )
endmacro()
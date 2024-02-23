# A Macro to install OpenCV from VCPKG
macro(install_opencv)

    # Platform folder name
    if (CMAKE_SIZEOF_VOID_P EQUAL 4)
        # 32bit
        set(PLATFORM_FOLDER "x86")
    elseif ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
        # 64bit
        set(PLATFORM_FOLDER "x64")
    endif()

    set( OpenCV_DIR "${DIRECTSHOWCAMERA_ROOT_PATH}/dependencies/opencv/build/${PLATFORM_FOLDER}/vc16" )  # Opencv path

    if (EXISTS ${OpenCV_DIR})
        # Install
        find_package(OpenCV REQUIRED
            opencv_core
            opencv_imgcodecs
            opencv_imgproc
            opencv_photo
        )
    else()
        message("Fail to install OpenCV. Library is not found in ${OpenCV_DIR}.")
    endif()

endmacro()

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
    if (EXISTS "${DIRECTSHOWCAMERA_ROOT_PATH}/vcpkg_installed/${PLATFORM_FOLDER}/share/opencv")
        set(MY_VCPKG_DEPENDENCIES_DIR "${DIRECTSHOWCAMERA_ROOT_PATH}/vcpkg_installed/${PLATFORM_FOLDER}")
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

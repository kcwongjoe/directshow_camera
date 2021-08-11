# A Macro to install OpenCV from VCPKG
macro(add_vcpkg)

    # Get vcpkg path
    if (NOT DEFINED VCPKG_PATH)
        # If the vcpkg folder is not located in current folder, change the path in the vcpkg_path.txt

        #    vcpkg folder path txt file
        set(VCPKG_PATH_FILE "${DIRECTSHOWCAMERA_ROOT_PATH}/config/vcpkg_path.txt")

        #    try to load from that file
        if (EXISTS ${VCPKG_PATH_FILE})
            # Read path from file
            file(READ ${VCPKG_PATH_FILE} VCPKG_PATH)
        endif()

        #    fail to load the path, use current folder as vcpkg path.
        if (NOT DEFINED VCPKG_PATH)
            set(VCPKG_PATH ${DIRECTSHOWCAMERA_ROOT_PATH}/vcpkg)
        endif()

    endif()

    # Set vcpkg
    if (DEFINED VCPKG_PATH)
        set(CMAKE_TOOLCHAIN_FILE
            ${VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake
            CACHE STRING "Vcpkg toolchain file")
    else()
        message("Vcpkg not found. Please set the absolute path in the ${DIRECTSHOWCAMERA_ROOT_PATH}/config/vcpkg_path.txt or add a argument: cmake -DVCPKG_PATH=Your/Vcpkg/Absolute/Path")
    endif()

endmacro()
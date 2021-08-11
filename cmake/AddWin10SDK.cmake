macro(add_window10_sdk)

    if (NOT DEFINED ${CMAKE_SYSTEM_VERSION} AND NOT DEFINED ${CMAKE_WINDOWS_KITS_10_DIR})
        #    Window SDK path txt file
        set(WINDOW_SDK_PATH_FILE "${DIRECTSHOWCAMERA_ROOT_PATH}/config/window_sdk_path.txt")

        #    try to load path from file
        if (EXISTS ${WINDOW_SDK_PATH_FILE})
            # Read path from file
            file(READ ${WINDOW_SDK_PATH_FILE} CMAKE_WINDOWS_KITS_10_DIR)
        endif()

        #    fail to load the path, find in Program files.
        if (NOT DEFINED CMAKE_WINDOWS_KITS_10_DIR)
            if (EXISTS "C:/Program Files (x86)/Windows Kits/10")
                set(CMAKE_WINDOWS_KITS_10_DIR "C:/Program Files (x86)/Windows Kits/10")
            endif()
        endif()
    endif()

    # Show error message if window sdk not found.
    if (NOT DEFINED CMAKE_WINDOWS_KITS_10_DIR)
        message("Window SDK Path not found. Please set the absolute path in the ${DIRECTSHOWCAMERA_ROOT_PATH}/config/window_sdk_path.txt or add a argument: cmake -DCMAKE_WINDOWS_KITS_10_DIR=Your/Window10SDK/Absolute/Path")
    endif()

endmacro()
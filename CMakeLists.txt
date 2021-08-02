cmake_minimum_required(VERSION 3.5)

#******Project******
project(directshow_camera LANGUAGES CXX)

#******Setting******
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#  C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")

# Check static library or executable
if(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    set(DIRECTSHOWCAMERA_AS_STATIC_LIBRARY TRUE)
endif()

# Add cmake module
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(InstallVcpkgOpenCV)

#******Source******
if (${DIRECTSHOWCAMERA_AS_STATIC_LIBRARY}) 
    # ---Static library---

    #******Source******
    add_subdirectory("./src")

else()
    # ---Build example and test---

    # ******OpenCV and Window SDK******

    #******Set vcpkg as submodule******

    if (NOT DEFINED VCPKG_PATH)
        # If the vcpkg folder is not located in current folder, change the path in the vcpkg_path.txt

        #    vcpkg folder path txt file
        set(VCPKG_PATH_FILE "config/vcpkg_path.txt")

        #    try to load from that file
        if (EXISTS ${VCPKG_PATH_FILE})
            # Read path from file
            file(READ ${VCPKG_PATH_FILE} VCPKG_PATH)
        endif()

        #    fail to load the path, use current folder as vcpkg path.
        if (NOT DEFINED VCPKG_PATH)
            set(VCPKG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg)
        endif()

        set(CMAKE_TOOLCHAIN_FILE
            ${VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake
            CACHE STRING "Vcpkg toolchain file")
    endif()

    #******Set Window SDK path******

    if (NOT DEFINED ${CMAKE_SYSTEM_VERSION})
        #    Window SDK path txt file
        set(WINDOW_SDK_PATH_FILE "config/window_sdk_path.txt")

        #    try to load path from file
        if (EXISTS ${WINDOW_SDK_PATH_FILE})
            # Read path from file
            file(READ ${WINDOW_SDK_PATH_FILE} CMAKE_WINDOWS_KITS_10_DIR)
        endif()

        #    fail to load the path, find in Program files.
        if (NOT DEFINED ${CMAKE_WINDOWS_KITS_10_DIR})
            if (EXISTS "C:/Program Files (x86)/Windows Kits/10")
                set(CMAKE_WINDOWS_KITS_10_DIR "C:/Program Files (x86)/Windows Kits/10")
            endif()
        endif()
    endif()

    #******Packages******

    # OpenCV

    #   Install OpenCV in vcpkg. See cmake/InstallVcpkgOpenCV.cmake
    install_vcpkg_opencv()

    #   Link OpenCV
    include_directories( ${OpenCV_INCLUDE_DIRS} )
    link_directories( ${OpenCV_LIB_DIR} )

    #******Source******
    add_subdirectory("./src")

    #******Example******
    add_subdirectory("./examples")

    #******Test******

    include(CTest)
    if(BUILD_TESTING)
        enable_testing()
        add_subdirectory("./test")
    endif()

endif()
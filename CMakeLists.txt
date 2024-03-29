cmake_minimum_required(VERSION 3.22)

#******Project******
project(directshow_camera LANGUAGES CXX VERSION 1.1.0)

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

# Get this CMakeLists location
set(DIRECTSHOWCAMERA_ROOT_PATH ${CMAKE_SOURCE_DIR})

# Add cmake module
list(APPEND CMAKE_MODULE_PATH "${DIRECTSHOWCAMERA_ROOT_PATH}/cmake")
include(InstallOpenCV)
include(AddWin10SDK)

# Path
Set(DIRECTSHOWCAMERA_SOURCES_PATH "${DIRECTSHOWCAMERA_ROOT_PATH}/src/directshow_camera")

#******Source******
if (${DIRECTSHOWCAMERA_AS_STATIC_LIBRARY}) 
    # ---Static library---

    #******Source******
    add_subdirectory("${DIRECTSHOWCAMERA_SOURCES_PATH}/src")

else()
    # ---Build example and test---

    # ******OpenCV and Window SDK******

    #   Set Window SDK path. See cmake/AddWin10SDK.cmake
    add_window10_sdk()

    # OpenCV

    #   Install OpenCV. See cmake/InstallOpenCV.cmake
    install_opencv()

    #   Link OpenCV
    include_directories( ${OpenCV_INCLUDE_DIRS} )
    link_directories( ${OpenCV_LIB_DIR} )

    #******Source******
    add_subdirectory("${DIRECTSHOWCAMERA_SOURCES_PATH}/src")

    #******Example******
    add_subdirectory("${DIRECTSHOWCAMERA_SOURCES_PATH}/examples")

    #******Test******

    include(CTest)
    if(BUILD_TESTING)
        enable_testing()
        add_subdirectory("${DIRECTSHOWCAMERA_SOURCES_PATH}/test")
    endif()

endif()
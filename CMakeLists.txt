cmake_minimum_required(VERSION 3.22)

# -------- Project --------
project(win_camera LANGUAGES CXX VERSION 2.0.0)

# -------- Setting --------
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#  C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")

# Path
set(WIN_CAMERA_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
Set(WIN_CAMERA_SOURCES_PATH "${WIN_CAMERA_ROOT_PATH}/src/win_camera")

# Add cmake module
list(APPEND CMAKE_MODULE_PATH "${WIN_CAMERA_ROOT_PATH}/cmake")

# Initialize variables
include(CMakeUtils)
init_variables()

# -------- Dependencies --------
# 1. Window SDK, See cmake/AddWin10SDK.cmake
include(AddWin10SDK)
add_window10_sdk()

# 2. OpenCV, See cmake/InstallOpenCV.cmake
include(InstallOpenCV)
install_opencv()

# -------- Source --------
add_subdirectory("${WIN_CAMERA_SOURCES_PATH}/src")

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    # Only build examples and test when this project is built as the main project

    # -------- Example --------
    add_subdirectory("${WIN_CAMERA_SOURCES_PATH}/examples")

    # -------- Test --------

    include(CTest)
    if(BUILD_TESTING)
        enable_testing()
        add_subdirectory("${WIN_CAMERA_SOURCES_PATH}/test")
    endif()
endif()
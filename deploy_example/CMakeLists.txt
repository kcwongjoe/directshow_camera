cmake_minimum_required(VERSION 3.22)

# -------- Project --------
set(PROJECT_NAME deploy_directshow_camera)
project(${PROJECT_NAME} LANGUAGES CXX VERSION 2.0.0)

# -------- Setting --------
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#  DirectShow Camera use C++20 internal but you can use other version such as C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")

# -------- Main --------

# Fetch DirectShow Camera
include(FetchContent)
FetchContent_Declare(
    DirectShowCamera
    GIT_REPOSITORY https://github.com/kcwongjoe/directshow_camera.git
    #GIT_TAG       hast_tag  // Uncomment this line if you want to use a specific version.
)

message("Start to download DirectShow Camera....")
FetchContent_MakeAvailable(DirectShowCamera)
message("Finished to download DirectShow Camera.")

# Add sources files
add_executable(${PROJECT_NAME} "main.cpp")

# Link source library
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC directshow_camera)
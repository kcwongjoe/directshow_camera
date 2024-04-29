cmake_minimum_required(VERSION 3.22)

# -------- Project --------
set(PROJECT_NAME deploy_win_camera)
project(${PROJECT_NAME} LANGUAGES CXX VERSION 2.0.0)

# -------- Setting --------
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#  Wincamera use C++20 internal but you can use other version such as C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")

# -------- Main --------

# Fetch WinCamera
include(FetchContent)
FetchContent_Declare(
    WinCamera
    GIT_REPOSITORY https://github.com/kcwongjoe/win_camera.git
    #GIT_TAG       bc7d09f2fef1d04744dc7c04607aa0f23e317483  // Uncomment this line if you want to use a specific version.
)

message("Start to download WinCamera....")
FetchContent_MakeAvailable(WinCamera)
message("Finished to download WinCamera.")

# Add sources files
add_executable(${PROJECT_NAME} "main.cpp")

# Link source library
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC win_camera)
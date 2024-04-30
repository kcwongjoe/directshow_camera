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

# Install OpenCV here if you want
# set(OpenCV_DIR "your_opencv_path")
# find_package(OpenCV REQUIRED)
# include_directories( ${OpenCV_INCLUDE_DIRS} )
# link_directories( ${OpenCV_LIB_DIR} )

# Fetch DirectShow Camera
include(FetchContent)
FetchContent_Declare(
    DirectShowCamera
    GIT_REPOSITORY https://github.com/kcwongjoe/directshow_camera.git
    #GIT_TAG       hash_tag  # Uncomment this line if you want to use a specific version.
)

message("Start to download DirectShow Camera....")
FetchContent_MakeAvailable(DirectShowCamera)
message("Finished to download DirectShow Camera.")

# Add sources files
add_executable(${PROJECT_NAME} "main.cpp")

# Link source library
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC directshow_camera)

# If you have installed OpenCV, you can use the following line to link OpenCV
# target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${OpenCV_LIBS})
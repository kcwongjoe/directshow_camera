# Create example project name
set(TEST_PROJECT_NAME ${PROJECT_NAME}_test)

# Get google test
include(FetchContent)
FetchContent_Declare(
  googletest
  # Updated on 25/2/2024, Update the commit if latest is available
  URL https://github.com/google/googletest/archive/f8d7d77c06936315286eb55f8de22cd23c188571.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Get example files
file(GLOB_RECURSE WIN_CAMERA_TEST_SOURCE_FILES "*.cpp")

# Add sources files
add_executable(${TEST_PROJECT_NAME} ${WIN_CAMERA_TEST_SOURCE_FILES})

# Link header files
target_include_directories(${TEST_PROJECT_NAME}
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Group source files
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${WIN_CAMERA_TEST_SOURCE_FILES}
)

# Link source library and doctest
target_link_libraries(${TEST_PROJECT_NAME} 
    LINK_PUBLIC
        ${PROJECT_NAME}
        ${OpenCV_LIBS}
        gtest_main
)

# Copy opencv dll
prebuild_copy_opencv_dll(${TEST_PROJECT_NAME} "${CMAKE_BINARY_DIR}/src/win_camera/test")

# Google test
include(GoogleTest)
gtest_discover_tests(${TEST_PROJECT_NAME})


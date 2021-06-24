# Create example project name
set(TEST_PROJECT_NAME ${PROJECT_NAME}_test)

# Get google test
include(FetchContent)
FetchContent_Declare(
  googletest
  # Updated on 22/6/2021, Update the commit if latest is available
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Get example files
file(GLOB_RECURSE MY_TEST_FILES "*.cpp")

# Add sources files
add_executable(${TEST_PROJECT_NAME} ${MY_TEST_FILES})

# Link header files
target_include_directories(${TEST_PROJECT_NAME}
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link source library and doctest
target_link_libraries(${TEST_PROJECT_NAME} 
    LINK_PUBLIC
        ${PROJECT_NAME}
        ${OpenCV_LIBS}
        gtest_main
)

# Post-build process to copy dll from vcpkg
prebuild_copy_vcpkg_opencv_dll(${TEST_PROJECT_NAME} "${CMAKE_BINARY_DIR}/test")

# Google test
include(GoogleTest)
gtest_discover_tests(${TEST_PROJECT_NAME})


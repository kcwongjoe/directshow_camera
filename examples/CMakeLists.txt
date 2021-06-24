# Create example project name
set(EXAMPLE_PROJECT_NAME ${PROJECT_NAME}_examples)

# Get example files
file(GLOB_RECURSE MY_EXAMPLE_FILES "*.h" "*.hpp" "*.cpp")

# Add sources files
add_executable(${EXAMPLE_PROJECT_NAME} ${MY_EXAMPLE_FILES})

# Link header files
target_include_directories(${EXAMPLE_PROJECT_NAME}
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link source library
target_link_libraries(${EXAMPLE_PROJECT_NAME} LINK_PUBLIC ${PROJECT_NAME} ${OpenCV_LIBS})

# Post-build process to copy dll from vcpkg
prebuild_copy_vcpkg_opencv_dll(${EXAMPLE_PROJECT_NAME} "${CMAKE_BINARY_DIR}/examples")
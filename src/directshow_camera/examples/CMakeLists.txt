# Create example project name
set(EXAMPLE_PROJECT_NAME ${PROJECT_NAME}_examples)

# Get example files
file(GLOB_RECURSE DIRECTSHO_CAMERA_EXAMPLE_SOURCE_FILES "*.h" "*.hpp" "*.cpp")

# Add sources files
add_executable(${EXAMPLE_PROJECT_NAME} ${DIRECTSHO_CAMERA_EXAMPLE_SOURCE_FILES})

# Link header files
target_include_directories(${EXAMPLE_PROJECT_NAME}
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Group source files
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${DIRECTSHO_CAMERA_EXAMPLE_SOURCE_FILES}
)

# Link source library
target_link_libraries(${EXAMPLE_PROJECT_NAME} LINK_PUBLIC ${PROJECT_NAME} ${OpenCV_LIBS})

# Copy opencv dll
prebuild_copy_opencv_dll(${EXAMPLE_PROJECT_NAME} "${CMAKE_BINARY_DIR}/src/directshow_camera/examples")

# Create example project name
set(EXAMPLE_PROJECT_NAME ${PROJECT_NAME}_examples)

# Get example files
file(GLOB_RECURSE WIN_CAMERA_EXAMPLE_SOURCE_FILES "*.h" "*.hpp" "*.cpp")

# Pre-compile header
set(EXAMPLE_PRECOMPILE_HEADERS
    stdafx.h
)

# Add sources files
add_executable(${EXAMPLE_PROJECT_NAME} ${WIN_CAMERA_EXAMPLE_SOURCE_FILES})

# Link header files
target_include_directories(${EXAMPLE_PROJECT_NAME}
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Add pre-compile header
target_precompile_headers(${EXAMPLE_PROJECT_NAME} PRIVATE
    ${EXAMPLE_PRECOMPILE_HEADERS}
)

# Group source files
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${WIN_CAMERA_EXAMPLE_SOURCE_FILES}
)

# Link source library
target_link_libraries(${EXAMPLE_PROJECT_NAME} LINK_PUBLIC ${PROJECT_NAME} ${OpenCV_LIBS})

# Post-build process to copy dll
prebuild_copy_opencv_dll(${EXAMPLE_PROJECT_NAME} "${CMAKE_BINARY_DIR}/src/win_camera/examples")

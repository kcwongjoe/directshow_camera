# Get sources file
file(GLOB_RECURSE WIN_CAMERA_SOURCE_FILES
    "*.h" "*.hpp" "*.cpp"
)

# Pre-compile header
set(PRECOMPILE_HEADERS
    stdafx.h
)

# Create static library
add_library(${PROJECT_NAME} SHARED STATIC ${WIN_CAMERA_SOURCE_FILES})

# Make sure the compiler can find include files
target_include_directories (${PROJECT_NAME}
    PUBLIC
        ./
)

# Add pre-compile header
target_precompile_headers(${PROJECT_NAME} PRIVATE
    ${PRECOMPILE_HEADERS}
)

# Group source files
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${WIN_CAMERA_SOURCE_FILES}
)
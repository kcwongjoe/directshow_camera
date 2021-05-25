# header folder
SET(MY_HEADER_DIR ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME})

# Get sources file
file(GLOB_RECURSE MY_SOURCE_FILES
    "*.h" "*.hpp" "*.cpp"
    "${MY_HEADER_DIR}/*.h" "${MY_HEADER_DIR}/*.hpp" "${MY_HEADER_DIR}/*.cpp"
)

# Create static library
add_library(${PROJECT_NAME} SHARED STATIC ${MY_SOURCE_FILES})

# Make sure the compiler can find include files
target_include_directories (${PROJECT_NAME}
    PUBLIC
        ${MY_HEADER_DIR}
)
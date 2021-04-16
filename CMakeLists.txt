cmake_minimum_required(VERSION 3.5)

#******Project******
project(directshow_camera LANGUAGES CXX)

#******Setting******
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#  C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++lastest")

# Check static library or executable
if(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    set(DIRECTSHOWCAMERA_AS_SUBDIRECTORY TRUE)
endif()

# Create source files list
set(DIRECTSHOWCAMERA_SOURCES_FILES)
LIST(APPEND DIRECTSHOWCAMERA_SOURCES_FILES "./src/ds_camera_utils.cpp")
LIST(APPEND DIRECTSHOWCAMERA_SOURCES_FILES "./src/uvc_camera.cpp")
LIST(APPEND DIRECTSHOWCAMERA_SOURCES_FILES "./src/uvc_camera_looper.cpp")
LIST(APPEND DIRECTSHOWCAMERA_SOURCES_FILES "./src/camera_device.cpp")
LIST(APPEND DIRECTSHOWCAMERA_SOURCES_FILES "./src/cv_mat_convertor.cpp")
LIST(APPEND DIRECTSHOWCAMERA_SOURCES_FILES "./src/ds_camera.cpp")
LIST(APPEND DIRECTSHOWCAMERA_SOURCES_FILES "./src/ds_camera_properties.cpp")
LIST(APPEND DIRECTSHOWCAMERA_SOURCES_FILES "./src/ds_camera_property.cpp")
LIST(APPEND DIRECTSHOWCAMERA_SOURCES_FILES "./src/ds_camera_device.cpp")
LIST(APPEND DIRECTSHOWCAMERA_SOURCES_FILES "./src/ds_video_format.cpp")
LIST(APPEND DIRECTSHOWCAMERA_SOURCES_FILES "./src/ds_grabber_callback.cpp")
# Add source files and link
if (${DIRECTSHOWCAMERA_AS_SUBDIRECTORY})
    # ---Static library---

    # Add sources files to library
    add_library(${PROJECT_NAME} STATIC ${DIRECTSHOWCAMERA_SOURCES_FILES})

    # Link header files
    target_include_directories(${PROJECT_NAME}
        PUBLIC 
            ${PROJECT_SOURCE_DIR}/include/directshow_camera
    )
else()
    # ---Build example---

    # ******OpenCV and Window SDK******

    #******Set vcpkg as submodule******

    # If the vcpkg folder is not located in current folder, change this variable.
    # For example: set(VCPKG_PATH "C:/Program Files")
    set(VCPKG_PATH ${CMAKE_CURRENT_SOURCE_DIR})

    set(CMAKE_TOOLCHAIN_FILE
        ${VCPKG_PATH}/vcpkg/scripts/buildsystems/vcpkg.cmake
        CACHE STRING "Vcpkg toolchain file")

    # Set Window SDK path
    set(CMAKE_WINDOWS_KITS_10_DIR "C:/Program Files (x86)/Windows Kits/10")

    if (CMAKE_SIZEOF_VOID_P EQUAL 4)
        # 32bit
        set( OpenCV_DIR "${VCPKG_PATH}/vcpkg/installed/x86-windows/share/opencv" )  # Opencv path

        if(NOT DEFINED OpenCV_LIB_DIR)
            set( OpenCV_LIB_DIR "${VCPKG_PATH}/vcpkg/installed/x86-windows/lib" "${VCPKG_PATH}/vcpkg/installed/x86-windows/debug/bin")  # Opencv lib path
        endif()
    elseif ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
        # 64bit
        set( OpenCV_DIR "${VCPKG_PATH}/vcpkg/installed/x64-windows/share/opencv" )  # Opencv path

        if(NOT DEFINED OpenCV_LIB_DIR)
            set( OpenCV_LIB_DIR "${VCPKG_PATH}/vcpkg/installed/x64-windows/lib" "${VCPKG_PATH}/vcpkg/installed/x64-windows/debug/bin")  # Opencv lib path
        endif()
    endif()

    #******Packages******

    # OpenCV
    find_package(OpenCV REQUIRED)
    include_directories( ${OpenCV_INCLUDE_DIRS} )
    link_directories( ${OpenCV_LIB_DIR} )

    #******Add source files******

    # Add examples
    LIST(APPEND DIRECTSHOWCAMERA_SOURCES_FILES "./examples/example.cpp")
    LIST(APPEND DIRECTSHOWCAMERA_SOURCES_FILES "./examples/eg1_capture.h")
    LIST(APPEND DIRECTSHOWCAMERA_SOURCES_FILES "./examples/eg1_capture.cpp")
    LIST(APPEND DIRECTSHOWCAMERA_SOURCES_FILES "./examples/eg2_properties.h")
    LIST(APPEND DIRECTSHOWCAMERA_SOURCES_FILES "./examples/eg2_properties.cpp")
    LIST(APPEND DIRECTSHOWCAMERA_SOURCES_FILES "./examples/eg3_camera_looper.h")
    LIST(APPEND DIRECTSHOWCAMERA_SOURCES_FILES "./examples/eg3_camera_looper.cpp")
    LIST(APPEND DIRECTSHOWCAMERA_SOURCES_FILES "./examples/eg4_exposure_fusion.h")
    LIST(APPEND DIRECTSHOWCAMERA_SOURCES_FILES "./examples/eg4_exposure_fusion.cpp")
    LIST(APPEND DIRECTSHOWCAMERA_SOURCES_FILES "./examples/eg5_disconnect_process.h")
    LIST(APPEND DIRECTSHOWCAMERA_SOURCES_FILES "./examples/eg5_disconnect_process.cpp")


    # Add sources files
    add_executable(${PROJECT_NAME} ${DIRECTSHOWCAMERA_SOURCES_FILES})

    # ******Link header files*******
    target_include_directories(${PROJECT_NAME}
        PRIVATE 
            ${PROJECT_SOURCE_DIR}/include/directshow_camera
            ${PROJECT_SOURCE_DIR}/examples
    )
    
    target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${OpenCV_LIBS}
    )
endif()
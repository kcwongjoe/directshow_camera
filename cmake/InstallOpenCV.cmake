# A Macro to install OpenCV
macro(install_opencv)

    # Check platform
    if (IS_BUILDING_X86)
        # Platform == x86
        set(${OpenCV_FOUND} FALSE)

        message("OpenCV no longer provides prebuilt in x86 versions. WinCamera can still works with no openCV. If you want to use Opencv feature, please download and install OpenCV manually. See https://docs.opencv.org/4.x/d3/d52/tutorial_windows_install.html")
    else()
        # Platform == x64

        # Fetch OpenCV
        include(FetchContent)
        FetchContent_Declare(
            opencv
            URL https://github.com/opencv/opencv/releases/download/4.9.0/opencv-4.9.0-windows.exe
        )

        message("Start to download Opencv....")
        FetchContent_MakeAvailable(opencv)
        message("Finished to download Opencv.")

        set(OpenCV_DIR "${CMAKE_BINARY_DIR}/_deps/opencv-src/build")  # Opencv path

        # Find OpenCV package
        if (EXISTS ${OpenCV_DIR})
            # Install
            find_package(OpenCV REQUIRED
                opencv_core
                opencv_imgcodecs
                opencv_imgproc
                opencv_photo
            )

            # Add define WITH_OPENCV2
            add_compile_definitions(WITH_OPENCV2)

            # Link OpenCV
            include_directories( ${OpenCV_INCLUDE_DIRS} )
            link_directories( ${OpenCV_LIB_DIR} )
        else()
            message("Fail to install OpenCV. Library is not found in ${OpenCV_DIR}.")
        endif()
    endif()


endmacro()

# Pre-build process: Copy opencv dll into the output folder
macro(prebuild_copy_opencv_dll PREBUILD_TARGET COPY_LOCATION)

    if (OpenCV_DIR)
        # Platform folder name
        if (CMAKE_SIZEOF_VOID_P EQUAL 4)
            # 32bit
            set(PLATFORM_FOLDER "x86")
        elseif ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
            # 64bit
            set(PLATFORM_FOLDER "x64")
        endif()

            # Copy opencv dll
            add_custom_command(TARGET ${PREBUILD_TARGET} PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${OpenCV_DIR}/${PLATFORM_FOLDER}/vc16/bin/opencv_world490d.dll "${COPY_LOCATION}/Debug/opencv_world490d.dll"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${OpenCV_DIR}/${PLATFORM_FOLDER}/vc16/bin/opencv_world490.dll "${COPY_LOCATION}/Release/opencv_world490.dll"
            )
    endif()
endmacro()
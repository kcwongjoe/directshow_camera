# Directshow Camera

A window-based camera library in c++.

### Features
* Work with OpenCV::Mat
* Support all camera settings
* Support Exposure Fusion
* A looper to handle the camera cycle
* Internal fake camera (Stub) for testing

# Table of contents

1. [Usage](#Usage)
2. [Looper](#Looper)
3. [Exposure Fusion](#Exposure-Fusion)
4. [Using camera without OpenCV](#Using-camera-without-OpenCV)
5. [Requirements](#Requirements)
6. [Installation](#Installation)
7. [Deploy example codes](#Deploy-example-codes)
8. [License](#License)

# Usage

```cpp
#include <uvc_camera.h>

using namespace DirectShowCamera;

// Get a empty camera
UVCCamera camera = UVCCamera();

// Get available camera list. You can use getResolutions() to acquire the resolutions
std::vector<CameraDevice> cameraDeivceList = camera.getCameras();

// Open the first camera
camera.open(cameraDeivceList[0]);

// Start Capture
camera.startCapture();

// Wait 1 second
std::this_thread::sleep_for(std::chrono::seconds(1));

// Capture a frame
cv::Mat frame = camera.getMat();

// Close the camera
camera.close();
```

*For more details, please see [example code](./examples) , [Documentation](https://www.kcwongjoe.com/directshow_camera/index.html) and [Software Diagrams](http://www.kcwongjoe.com/directshow_camera/software_diagrams.html)*

# Looper

Looper is a loop thread worked in detach mode, making the camera life cycle easy to handle.

### Usage

```cpp
#include <uvc_camera_looper.h>
#include <iostream>

using namespace DirectShowCamera;

// Create a looper
UVCCameraLooper cameraLooper = UVCCameraLooper();

// Open the first camera
std::vector<CameraDevice> cameraDeivceList = cameraLooper.getCamera()->getCameras();
cameraLooper.getCamera()->open(cameraDeivceList[0]);

// Set a process to handle the captured image
cameraLooper.setCapturedProcess(
    [](cv::Mat image)
    {
        std::cout << "Get a new image" << std::endl;
    }
);

// Start looper to capture image continously
cameraLooper.start();

// 1 second
std::this_thread::sleep_for(std::chrono::seconds(1));

// Stop looper
cameraLooper.stop();
```

# Exposure Fusion

Exposure fusion is a technique to capture a high quality image by fusing multi-exposure sequence.

```cpp
cv::Mat frame = camera.exposureFusion();
```

*Reference: [Tom Mertens, Jan Kautz, and Frank Van Reeth. Exposure fusion. In Computer Graphics and Applications, 2007. PG'07. 15th Pacific Conference on, pages 382–390. IEEE, 2007.](https://mericam.github.io/exposure_fusion/index.html)*

# Using camera without OpenCV

This library can be used without opencv.
1. Find **ds_libs_setting.h** in the **include/directshow_camera** folder.
2. Comment this line

```cpp
   //#define HAS_OPENCV
```

Then you can use the following code to get frame bytes in BGR order.

```cpp
int frameSize;
unsigned char* image = new unsigned char[1208 * 760 * 3];
camera.getFrame(&image, &size);
```

# Requirements

Minimum C++ 17

vcpkg commit version: #13bde2ff13192e1b2fdd37bd9b475c7665ae6ae5

OpenCV version: 4.8.0 (Option)

# Installation

### 1. Install Window SDK

1. Install: https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk/

### 2.1 Cmake

1. Create **libs** folder in you project
2. Clone this repository into the **libs** folder
   ```
   git clone https://github.com/kcwongjoe/directshow_camera.git
   ```
3. Add the code in your project *CMakeLists.txt*
   ```cmake
   # Set Window SDK Path. Change it to your Window SDK Path
    set(CMAKE_WINDOWS_KITS_10_DIR "C:/Program Files (x86)/Windows Kits/10") 

   # Camera library
    add_subdirectory(./libs/directshow_camera)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            directshow_camera
    )
   ```

4. Install OpenCV library in CMake from vcpgk (Option)
    1. Add the code in your project CMakeLists.txt
        ```cmake
        # Set vcpkg path. Change here if the vcpkg folder is not located in your solution folder.
        set(VCPKG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg)

        set(CMAKE_TOOLCHAIN_FILE
            ${VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake
            CACHE STRING "Vcpkg toolchain file")

        # OpenCV
        
        #   Install OpenCV from vcpkg. See cmake/InstallVcpkgOpenCV.cmake
        install_vcpkg_opencv()

        #   Link OpenCV
        include_directories( ${OpenCV_INCLUDE_DIRS} )
        link_directories( ${OpenCV_LIB_DIR} )

        target_link_libraries(${PROJECT_NAME}
            PRIVATE
                OpenCV_LIBS
        )
   

### 2.2 For dummy

1. Copy all files inside **src** folder and **include/directshow_camera** folder to your project.

# Deploy example codes

1. Install Window SDK

    1. Install: https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk/
    2. Go to *config* folder, Rename *window_sdk_path_example.txt* to *window_sdk_path.txt* and type your Window SDK path in the text file.

2. Clone this repository
   ```
   git clone --recurse-submodules https://github.com/kcwongjoe/directshow_camera.git
   ```

3. Install vcpkg in the project folder
    1. Clone the vcpkg repo from https://github.com/Microsoft/vcpkg
    2. Unzip and rename folder as *vcpkg*
    3. Place in the solution folder
    4. Run **bootstrap-vcpkg.bat** in the *vcpkg* folder
    5. If your vcpkg folder was not located in the solution folder, you have to specify the path in the *config* folder. Go to *config* folder, Rename *vcpkg_path_example.txt* to *vcpkg_path.txt* and type your vcpkg path in the text file.

4. Install OpenCV by vcpkg
    1. Go to directshow_camera project folder
    2. Type `vcpkg install`

5. Create a **build** folder if not existed. Run **build.bat** in Solution folder

   Type `build x86` or `build x64`

6. Go to *build* folder and open visual studio solution.
7. Build solution in visual studio.

# License
This project is licensed under [MIT](LICENSE) license.
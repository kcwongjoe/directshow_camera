# DirectShow Camera

A DirectShow-based camera library in c++ for window. Supported USB Camera.

## Table of contents

1. [Requirements](#Requirements)
2. [Deploy](#Deploy)
3. [Run Examples](#Run-Examples)
4. [Supported camera properties](#Supported-camera-properties)
5. [License](#License)

## Requirements

DirectShow Camera is based on DirectShow, which is a part of the Windows SDK. You have to install the Windows SDK to use this library.

1.  Install: https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk/

2.  If your Windows SDK has installed apart from C:/Program Files (x86)/Windows Kits/10, you have to set your Window SDK path manually.

    Go to *config* folder, Rename *window_sdk_path_example.txt* to *window_sdk_path.txt* and type your Window SDK path in the text file.

## Deploy

To add this library to your project, just use FetchContent_Declare() in your cmake file.

1.  Add the following code into your CMakeLists.txt file.
    ```cmake
    # Fetch DirectShowCamera
    include(FetchContent)
    FetchContent_Declare(
        DirectShowCamera
        GIT_REPOSITORY https://github.com/kcwongjoe/directshow_camera.git
    )
    FetchContent_MakeAvailable(DirectShowCamera)

    # Do something.......

    # Link source library
    target_link_libraries(${PROJECT_NAME} LINK_PUBLIC directshow_camera)
    ```

2.  Build solution from the cmake command line. Solution will be built in the *yourproject/build* folder.

    ```shell
    cmake -B ./build -G "Visual Studio 17 2022" -A x64
    cmake --build build/
    ```

See the *deploy_example* folder for details.

## Run Examples

Example code is a good start to learn how to use this library. You can find the example code in the *src/directshow_camera/examples* folder.

1.  Clone this repository
    ```
    git clone --recurse-submodules https://github.com/kcwongjoe/directshow_camera.git
    ```

2.  Build solution from the cmake command line. Solution will be built in the *build* folder.

    ```shell
    cmake -B ./build -G "Visual Studio 17 2022" -A x64
    cmake --build build/
    ```
    Change the `-A` to win32 for a x86 build. Notes that OpenCV doesn't provide a X86 pre-build, you have to build OpenCV by yourself if you want to use the OpenCV features.

3. Run *directshow_camera_examples* project to get start.

## Supported camera properties

Brightness, Contrast, Hue, Saturation, Sharpness, Gamma, Color Enable, White Balance, Backlight Compensation, Gain, Pan, Tilt, Roll, Zoom, Exposure, Iris, Focus, Powerline Frequency, Digital Zoom Level

## License
This project is licensed under [MIT](LICENSE) license.
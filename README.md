# Win Camera

A DirectShow-based camera library in c++ for window. Supported USB Camera.

# Table of contents

1. [Requirements](#Requirements)
2. [Deploy](#Deploy)
3. [License](#License)

# Requirements

Minimum C++ 17

OpenCV version: 4.9.0 (Option)

# Deploy

1.  Install Window SDK

    1.  Install: https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk/

    2.  If your Windows SDK has installed apart from C:/Program Files (x86)/Windows Kits/10, you have to set your Window SDK path manually.

        Go to *config* folder, Rename *window_sdk_path_example.txt* to *window_sdk_path.txt* and type your Window SDK path in the text file.

2.  Clone this repository
    ```
    git clone --recurse-submodules https://github.com/kcwongjoe/directshow_camera.git
    ```

3.  Build solution from the cmake command line. Solution will be built in the *build* folder.

    ```shell
    cmake -B ./build -G "Visual Studio 17 2022" -A win32
    cmake --build build/
    ```
    Change the `-A` to win32 for a x86 build. Notes that OpenCV doesn't provide a X86 pre-build, you have to build OpenCV by yourself if you want to use the OpenCV features.

4. Run *win_camera_examples* to get start.

# License
This project is licensed under [MIT](LICENSE) license.
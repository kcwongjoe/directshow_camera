/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#include <iostream>
#include <vector>
#include <functional>
#include "win_camera/win_camera.h"


int main(int argc, char *argv[])
{
    // Create a camera object
    WinCamera::WinCamera camera = WinCamera::WinCamera();

    // Print available camera list
    std::cout << "Start to list the available cameras..." << std::endl;
    std::vector< WinCamera::WinCameraDevice> cameraDeivceList = camera.getCameras();
    for (int i = 0; i < cameraDeivceList.size(); i++) {
        std::cout << cameraDeivceList[i] << std::endl;
    }
}
/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#include "examples/eg5_disconnect_process.h"

#include "win_camera/win_camera.h"

#include <iostream>

void eg5_disconnect_process()
{
    std::cout << "Example 5: Disconnect Process. This is example to handle a process when the USB camera is disconnected suddently." << std::endl;

    // Get a empty camera
    WinCamera::WinCamera camera = WinCamera::WinCamera();

    // Print camera list
    std::vector<WinCamera::WinCameraDevice> cameraDeivceList = camera.getCameras();
    for (int i = 0; i < cameraDeivceList.size(); i++) {
        std::cout << std::to_string(i) + ": " + cameraDeivceList[i].getFriendlyName() << std::endl;
    }

    std::cout << "Select your USB camera:" << std::endl;

    int cameraIndex;
    std::cin >> cameraIndex;

    // Open the camera
    std::cout << "Open the camera..." << std::endl;
    camera.Open(cameraDeivceList[cameraIndex]);

    camera.setDisconnectionProcess(
        []()
        {
            std::cout << "Camera was disconnected." << std::endl;
        }
    );

    // Start Capture
    std::cout << "Start capture..." << std::endl;
    camera.StartCapture();

    // Wait for 2 second
    std::cout << "You can unplug the USB camera now." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    // Stop Capture
    camera.StopCapture();

    // Close the camera.
    camera.Close();
}
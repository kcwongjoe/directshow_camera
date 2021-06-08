#include "eg5_disconnect_process.h"

#include <uvc_camera.h>
#include <iostream>

using namespace DirectShowCamera;

void eg5_disconnect_process()
{
    // Get a empty camera
    UVCCamera camera = UVCCamera();

    // Print camera list
    std::vector<CameraDevice> cameraDeivceList = camera.getCameras();
    for (int i = 0; i < cameraDeivceList.size(); i++) {
        std::cout << std::to_string(i) + ": " + cameraDeivceList[i].getFriendlyName() << std::endl;
    }

    std::cout << "Select your USB camera:" << std::endl;

    int cameraIndex;
    std::cin >> cameraIndex;

    // Open the camera
    std::cout << "Open the camera..." << std::endl;
    camera.open(cameraDeivceList[cameraIndex]);

    camera.setDisconnectionProcess(
        []()
        {
            std::cout << "Camera was disconnected." << std::endl;
        }
    );

    // Start Capture
    std::cout << "Start capture..." << std::endl;
    camera.startCapture();

    // Wait for 2 second
    std::cout << "You can unplug the USB camera now." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    // Stop Capture
    camera.stopCapture();

    // Close the camera.
    camera.close();
}
#include "eg5_disconnect_process.h"

#include "win_camera/win_camera.h"

void eg5_disconnect_process()
{
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
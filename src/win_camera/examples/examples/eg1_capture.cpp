/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#include "examples/eg1_capture.h"

#include "win_camera/win_camera.h"

#include <iostream>

void eg1_capture()
{
    std::cout << "Example 1: Capture frames and save as image.jpg" << std::endl;

    // Get a empty camera
    WinCamera::WinCamera camera = WinCamera::WinCamera();

    // Get available camera list
    std::cout << "Start to list the available cameras..." << std::endl;
    std::vector< WinCamera::WinCameraDevice> cameraDeivceList = camera.getCameras();
    for (int i = 0; i < cameraDeivceList.size(); i++) {
        std::cout << cameraDeivceList[i] << std::endl;
    }

    // Open the first camera
    std::cout << "Open the first camera..." << std::endl;
    camera.Open(cameraDeivceList[0]);

    // Start Capture
    std::cout << "Start capture..." << std::endl;
    camera.StartCapture();

    // Wait for 1 second
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Capture a frame
    std::cout << "Capture frame in cv::Mat and save as image.jpg" << std::endl;
    WinCamera::Frame frame;
    camera.getFrame(frame);
    frame.Save("image.jpg");

    // Capture a new frame
    std::cout << "Waiting for a new frame to be captured and save as image2.jpg" << std::endl;
    camera.getNewFrame(frame);
    frame.Save("image2.jpg");

    // You can vectical flip the image. The camera is default to vectical flip the image.
    std::cout << "Set camera to vertical flip the captured image." << std::endl;
    camera.getFrameSettings().VerticalFlip = false;

    std::cout << "Capture frame which has been vertical flip and save as image3.jpg" << std::endl;
    camera.getNewFrame(frame);
    frame.Save("image3.jpg");

    // Stop Capture
    camera.StopCapture();

    // Close the camera.
    camera.Close();
}
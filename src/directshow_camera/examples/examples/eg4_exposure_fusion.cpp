/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "examples/eg4_exposure_fusion.h"

#include "camera/camera.h"

#include <iostream>

void eg4_exposure_fusion()
{
    std::cout << "Example 4: Exposure Fusion" << std::endl;

    // Get a empty camera
    DirectShowCamera::Camera camera = DirectShowCamera::Camera();

    // Open the first camera
    std::cout << "Open the first camera..." << std::endl;
    std::vector<DirectShowCamera::CameraDevice> cameraDeivceList = camera.getCameras();
    camera.Open(cameraDeivceList[0]);

    // Start Capture
    std::cout << "Start capture..." << std::endl;
    camera.StartCapture();

    // Wait for 1 second
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Capture a exposure fusion image
    std::cout << "Start to capture in exposure fusion and save to image.jpg" << std::endl;
    cv::Mat frame = camera.ExposureFusion();
    cv::imwrite("image.jpg", frame);

    // Processing exposure fusion image is time consumed. Use async mode to capture.
    std::cout << "Use a async mode to capture image in exposure fusion and save to image2.jpg" << std::endl;
    std::atomic<bool> calculatedFusion = false;
    camera.ExposureFusion(
        [&calculatedFusion](cv::Mat fusionImage)
        {
            std::cout << "Captured in async mode." << std::endl;
            cv::imwrite("image2.jpg", fusionImage);

            // Set the finished flag as true
            calculatedFusion = true;
        }
    );

    // Waiting for processed
    int i = 0;
    while (!calculatedFusion.load())
    {
        std::cout << std::to_string(++i) + "s" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }    

    // Stop Capture
    camera.StopCapture();

    // Close the camera.
    camera.Close();
}
#include "eg4_exposure_fusion.h"

#include <uvc_camera.h>
#include <iostream>

using namespace DirectShowCamera;

void eg4_exposure_fusion()
{
    // Get a empty camera
    UVCCamera camera = UVCCamera();

    // Open the first camera
    std::cout << "Open the first camera..." << std::endl;
    std::vector<CameraDevice> cameraDeivceList = camera.getCameras();
    camera.open(cameraDeivceList[0]);

    // Start Capture
    std::cout << "Start capture..." << std::endl;
    camera.startCapture();

    // Wait for 1 second
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Capture a exposure fusion image
    std::cout << "Start to capture in exposure fusion..." << std::endl;
    cv::Mat frame = camera.exposureFusion();
    cv::imwrite("image.jpg", frame);

    // Processing exposure fusion image is time consumed. Use async mode to capture.
    std::cout << "Use a async mode to capture image in exposure fusion..." << std::endl;
    std::atomic<bool> calculatedFusion = false;
    camera.exposureFusion(
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
    camera.stopCapture();

    // Close the camera.
    camera.close();
}
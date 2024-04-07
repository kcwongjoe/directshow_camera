#include "examples/eg4_exposure_fusion.h"

#include "win_camera/win_camera.h"

void eg4_exposure_fusion()
{
    std::cout << "Example 4: Exposure Fusion" << std::endl;

    // Get a empty camera
    WinCamera::WinCamera camera = WinCamera::WinCamera();

    // Open the first camera
    std::cout << "Open the first camera..." << std::endl;
    std::vector<WinCamera::WinCameraDevice> cameraDeivceList = camera.getCameras();
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
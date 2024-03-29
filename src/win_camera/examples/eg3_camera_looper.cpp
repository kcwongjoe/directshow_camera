#include "eg3_camera_looper.h"

#include "win_camera/win_camera_looper.h"

void eg3_camera_looper()
{
    // Create a looper
    WinCamera::WinCameraLooper cameraLooper = WinCamera::WinCameraLooper();

    // Open the first camera
    std::cout << "Open the first camera..." << std::endl;
    std::vector<WinCamera::WinCameraDevice> cameraDeivceList = cameraLooper.getCamera()->getCameras();
    cameraLooper.getCamera()->Open(cameraDeivceList[0]);

    // Set a process to handle the captured image
    cameraLooper.setCapturedProcess(
        [](cv::Mat image)
        {
            std::cout << "Get a new image(" + std::to_string(image.cols) + " x " + std::to_string(image.rows) + ")" << std::endl;
        }
    );

    // Start looper to capture image continously
    std::cout << "Start looper..." << std::endl;
    cameraLooper.Start();

    // 1 second
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // You can save image automatically
    std::cout << "Start to save images..." << std::endl;
    cameraLooper.EnableSaveImage(true);

    std::this_thread::sleep_for(std::chrono::milliseconds(400));

    std::cout << "Stop to save images..." << std::endl;
    cameraLooper.EnableSaveImage(false);

    // Stop looper
    std::cout << "Stop looper..." << std::endl;
    cameraLooper.Stop();
}
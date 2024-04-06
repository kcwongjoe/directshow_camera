#include "eg3_camera_thread.h"

#include "win_camera/win_camera_thread.h"

void eg3_camera_thread()
{
    std::cout << "Example 3: Camera Thread" << std::endl;

    // Create a looper
    WinCamera::WinCameraThread cameraThread = WinCamera::WinCameraThread();

    // Open the first camera
    std::cout << "Open the first camera..." << std::endl;
    std::vector<WinCamera::WinCameraDevice> cameraDeivceList = cameraThread.getCamera()->getCameras();
    cameraThread.getCamera()->Open(cameraDeivceList[0]);

    // Set a process to handle the captured image
    cameraThread.setCapturedProcess(
        [](cv::Mat image)
        {
            std::cout << "Get a new image(" + std::to_string(image.cols) + " x " + std::to_string(image.rows) + ")" << std::endl;
        }
    );

    // Start the thread to capture image continously
    std::cout << "Start Thread..." << std::endl;
    cameraThread.Start();

    // Let the camera thread running 1 second
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // You can save image automatically
    std::cout << "Now start to save images in the thread..." << std::endl;
    cameraThread.EnableSaveImage(true);

    // Let the camera thread running 400ms
    std::this_thread::sleep_for(std::chrono::milliseconds(400));

    std::cout << "Stop to save images..." << std::endl;
    cameraThread.EnableSaveImage(false);

    // Stop the thread
    std::cout << "Stop Thread..." << std::endl;
    cameraThread.Stop();
}
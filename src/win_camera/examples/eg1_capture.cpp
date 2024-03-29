#include "eg1_capture.h"

#include "win_camera/win_camera.h"

void eg1_capture()
{
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
    cv::Mat frame = camera.getMat();
    cv::imwrite("image.jpg", frame);

    // Capture a new frame
    std::cout << "Waiting for a new frame to be captured and save as image2.jpg" << std::endl;
    frame = camera.getNewMat();
    cv::imwrite("image2.jpg", frame);

    // You can vectical flip the image. The camera is default to vectical flip the image.
    std::cout << "Stop to vectical flip the image." << std::endl;
    camera.VecticalFlipMat(false);
    frame = camera.getNewMat();
    cv::imwrite("image3.jpg", frame);

    // Stop Capture
    camera.StopCapture();

    // Close the camera.
    camera.Close();
}
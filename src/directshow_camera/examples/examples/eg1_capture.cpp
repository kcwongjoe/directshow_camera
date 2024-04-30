/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "examples/eg1_capture.h"

#include "camera/camera.h"

#include <iostream>

void eg1_capture()
{
    std::cout << "Example 1: Capture frames and save as image.jpg" << std::endl;

    // Get a empty camera
    DirectShowCamera::Camera camera = DirectShowCamera::Camera();

    // Get available camera list
    std::cout << "Start to list the available cameras..." << std::endl;
    std::vector< DirectShowCamera::CameraDevice> cameraDeivceList = camera.getCameras();
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
    DirectShowCamera::Frame frame;
    camera.getFrame(frame);
    frame.Save("image.jpg");

    // You can get the pixel data by the getFrameData function
    // const auto width = frame.getWidth();
    // const auto height = frame.getHeight();
    // const auto frameType = frame.getFrameType(); // Let say the frame type is ColorBGR24bit. If you want to get RGB data, Set frame.getFrameSettings().BGR = false;
    // int numOfBytes;
    // auto frameData = frame.getFrameData(numOfBytes);
    // int row = 0;
    // int col = 0;
    // for (int i = 0; i < numOfBytes; i+=3) {
    //     const auto b = frameData[i];
    //     const auto g = frameData[i+1];
    //     const auto r = frameData[i+2];
    //     std::cout << "Pixel at (" << row << "," << col << ") is (" << r << "," << g << "," << b << ")" << std::endl;
    //
    //     col++;
    //     if (col >= width) {
    //         col = 0;
    //         row++;
    //     }
    // }

    // Capture a new frame
    std::cout << "Waiting for a new frame to be captured and save as image2.jpg" << std::endl;
    camera.getNewFrame(frame);
    frame.Save("image2.jpg");

    // You can vectical flip the image. The camera is default to vectical flip the image.
    std::cout << "Set camera to vertical flip the captured image." << std::endl;
    camera.getFrameSettings().VerticalFlip = true;

    std::cout << "Capture frame which has been vertical flip and save as image3.jpg" << std::endl;
    camera.getNewFrame(frame);
    frame.Save("image3.jpg");

    // Stop Capture
    camera.StopCapture();

    // Close the camera.
    camera.Close();
}
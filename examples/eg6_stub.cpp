#include "eg6_stub.h"

#include <uvc_camera.h>
#include <iostream>
#include <ds_camera_stub.h>

using namespace DirectShowCamera;

void eg6_stub()
{

    // Define a flag to indicate whether open as stub.
    // So you just have to comment this line to disable the stub.
    // You can also add this flag in CMake for your testing project
    // target_compile_definitions(${ PROJECT_NAME } PUBLIC USE_STUB)
#define USE_STUB

    // Stub or directshow camera
#ifdef USE_STUB
    // Create Stub
    DirectShowCameraStub* stub = new DirectShowCameraStub();

    // Open
    UVCCamera camera = UVCCamera(stub);
#else
    // Use normal directshow camera
    UVCCamera camera = UVCCamera();
#endif

    // Get available camera list
    std::cout << "Start to list the available cameras..." << std::endl;
    std::vector<CameraDevice> cameraDeivceList = camera.getCameras();
    for (int i = 0; i < cameraDeivceList.size(); i++) {
        std::cout << cameraDeivceList[i] << std::endl;
    }

    // Open the first camera in the biggest resolution
    std::cout << "Open the first camera..." << std::endl;
    std::vector <std::pair<int, int>> resolutions = cameraDeivceList[0].getResolutions();
    camera.open(cameraDeivceList[0],
        resolutions[resolutions.size() - 1].first,
        resolutions[resolutions.size() - 1].second
    );

    // We set a disconnect process for later use
    camera.setDisconnectionProcess(
        []()
        {
            std::cout << "Camera was disconnected." << std::endl;
        }
    );
    
    // Print all properties
    std::cout << "Display the properties of the fake camera..." << std::endl;
    std::cout << (std::string)*camera.getDirectShowProperties() << std::endl;

    // ****** Change exposure******
    std::cout << "Change the exposure in the fake camera..." << std::endl;
    std::cout << "Exposure: " + std::to_string(camera.getExposure()) + "s" << std::endl;

    // Set exposure as the largest value
    std::vector<double> exposures = camera.getPossibleExposureValues();
    camera.setExposure(exposures[exposures.size()-1]);
    std::cout << "Set exposure to " + std::to_string(exposures[exposures.size() - 1]) + "s" << std::endl;

    // Get exposure again
    std::cout << "Exposure changed to: " + std::to_string(camera.getExposure()) + "s" << std::endl;

    // ****** Capture******
    std::cout << "Start capture in a fake camera..." << std::endl;
    camera.startCapture();

    // Wait for 1 second
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Capture a frame
    std::cout << "Capture frame in cv::Mat and save as image.jpg" << std::endl;
    cv::Mat frame = camera.getMat();
    cv::imwrite("image.jpg", frame);

    // User define a image
    int width = camera.getWidth();
    int height = camera.getHeight();
    stub->setGetFrameFunction([width, height](unsigned char* pixels, unsigned long* frameIndex, int* numOfBytes, bool copyNewFrameOnly, unsigned long previousFrameIndex)
        {
            // Frame index
            *frameIndex = ++previousFrameIndex;

            // Size
            int totalSize = width * height * 3;
            if (numOfBytes)
            {
                *numOfBytes = totalSize;
            }

            // Allocate image memory
            memset(pixels, 0, totalSize * sizeof(unsigned char));

            // Draw
            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {
                    int pixelIndex = (j * width + i) * 3;

                    // Blue
                    pixels[pixelIndex] = 175;

                    // Green
                    pixels[pixelIndex + 1] = 54;

                    // Red
                    pixels[pixelIndex + 2] = 60;
                }
            }
        }
    );

    // Capture a frame
    std::cout << "Capture a user defined frame in cv::Mat and save as image.jpg" << std::endl;
    frame = camera.getMat();
    cv::imwrite("image2.jpg", frame);

    // ******Disconnect process******
    std::cout << "The fake camera is disconnect accidently now..." << std::endl;
    stub->disconnetCamera();

    // Wait for 2 second
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    // Close the camera.
    camera.close();

}
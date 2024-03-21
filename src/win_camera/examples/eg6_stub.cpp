#include "eg6_stub.h"

#include "win_camera/win_camera.h"
#include <iostream>
#include "directshow_camera/ds_camera_stub.h"

void eg6_stub()
{

    // Define a flag to indicate whether open as stub.
    // So you just have to comment this line to disable the stub.
    // You can also add this flag in your testing project by CMake
    // target_compile_definitions(${ PROJECT_NAME } PUBLIC USE_STUB)
#define USE_STUB

    // Stub or directshow camera
#ifdef USE_STUB
    // Create Stub
    const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera> stub = std::make_shared<DirectShowCamera::DirectShowCameraStub>();
    DirectShowCamera::DirectShowCameraStub* cameraStub = dynamic_cast<DirectShowCamera::DirectShowCameraStub*>(stub.get());

    // Open
    WinCamera::WinCamera camera = WinCamera::WinCamera(stub);
#else
    // Use normal directshow camera
    UVCCamera camera = UVCCamera();
#endif

    // Get available camera list
    std::cout << "Start to list the available cameras..." << std::endl;
    std::vector<WinCamera::WinCameraDevice> cameraDeivceList = camera.getCameras();
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

#ifdef USE_STUB
    // User define a image
    int width = camera.getWidth();
    int height = camera.getHeight();
    cameraStub->setGetFrameFunction([width, height](unsigned char* pixels, int& numOfBytes, unsigned long& frameIndex, const bool copyNewFrameOnly, const unsigned long previousFrameIndex)
        {
            // Frame index
            frameIndex = previousFrameIndex + 1;

            // Size
            int totalSize = width * height * 3;
            numOfBytes = totalSize;

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
#endif

#ifdef USE_STUB
    // ******Disconnect process******
    std::cout << "The fake camera is disconnected accidently now..." << std::endl;
    cameraStub->disconnetCamera();

    // Wait for 2 second
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#endif

    // Close the camera.
    camera.close();

}
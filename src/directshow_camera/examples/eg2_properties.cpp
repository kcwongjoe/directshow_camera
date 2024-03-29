#include "eg2_properties.h"

#include <uvc_camera.h>
#include <iostream>

using namespace DirectShowCamera;

void eg2_properties()
{
    // Get a empty camera
    UVCCamera camera = UVCCamera();

    // Get available camera list
    std::cout << "Start to list the available cameras..." << std::endl;
    std::vector<CameraDevice> cameraDeivceList = camera.getCameras();
    for (int i = 0; i < cameraDeivceList.size(); i++) {
        std::cout << cameraDeivceList[i] << std::endl;
    }

    // Get first support resolution in the first camera
    std::vector <std::pair<int, int>> resolutions = cameraDeivceList[0].getResolutions();
    std::cout << "Width: " + std::to_string(resolutions[0].first) + ", Height: " + std::to_string(resolutions[0].second) << std::endl;



    // Open the first camera in the biggest resolution
    std::cout << "Open the first camera..." << std::endl;
    camera.open(cameraDeivceList[0], 
        resolutions[resolutions.size() - 1].first,
        resolutions[resolutions.size() - 1].second
    );

    // Get exposure in second
    if (camera.supportExposure())
    {
        std::cout << "Exposure: " + std::to_string(camera.getExposure()) + "s" << std::endl;
        // Set exposure as the largest value
        std::vector<double> exposures = camera.getPossibleExposureValues();
        camera.setExposure(exposures[exposures.size() - 1]);
        std::cout << "Set exposure to " + std::to_string(exposures[exposures.size() - 1]) + "s" << std::endl;

    }else
    {
        std::cout << "This device doesn't support exposure time" << std::endl;
    }


    // Reset all properties to default
    std::cout << "Reset properties to default..." << std::endl;
    camera.resetProperties();

    // Close the camera.
    camera.close();
    
}
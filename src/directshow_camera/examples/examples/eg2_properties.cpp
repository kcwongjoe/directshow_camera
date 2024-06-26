/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "examples/eg2_properties.h"

#include "camera/camera.h"

#include <iostream>

void eg2_properties()
{
    std::cout << "Example 2: Properties" << std::endl;

    // Get a empty camera
    DirectShowCamera::Camera camera = DirectShowCamera::Camera();

    // Get available camera list
    std::cout << "Start to list the available cameras..." << std::endl;
    std::vector< DirectShowCamera::CameraDevice> cameraDeivceList = camera.getCameras();
    for (int i = 0; i < cameraDeivceList.size(); i++) {
        std::cout << cameraDeivceList[i] << std::endl;
    }

    // Get first support resolution in the first camera
    std::vector <std::pair<int, int>> resolutions = cameraDeivceList[0].getResolutions();
    std::cout << "Width: " + std::to_string(resolutions[0].first) + ", Height: " + std::to_string(resolutions[0].second) << std::endl;

    // Open the first camera in the biggest resolution
    std::cout << "Open the first camera..." << std::endl;
    camera.Open(cameraDeivceList[0], 
        resolutions[resolutions.size() - 1].first,
        resolutions[resolutions.size() - 1].second
    );

    // Show Camera Properties
    std::cout << (*camera.getDirectShowProperties().get()) << std::endl;
    std::cout << "-----------------------------" << std::endl;

    // Get exposure in second
    if (camera.Exposure()->isSupported())
    {
        std::cout << "Exposure: " + std::to_string(camera.Exposure()->getValue()) + "s" << std::endl;
        // Set exposure as the largest value
        std::vector<double> exposures = camera.Exposure()->GetPossibleExposureValues();
        camera.Exposure()->setValue(exposures[exposures.size() - 1]);
        std::cout << "Set exposure to " + std::to_string(exposures[exposures.size() - 1]) + "s" << std::endl;
        std::cout << "Exposure now is " + std::to_string(camera.Exposure()->getValue()) + "s" << std::endl;

    }else
    {
        std::cout << "This device doesn't support exposure time" << std::endl;
    }

    // Reset all properties to default
    std::cout << "Reset properties to default..." << std::endl;
    camera.ResetProperties();
    std::cout << "Exposure now is " + std::to_string(camera.Exposure()->getValue()) + "s" << std::endl;

    // Close the camera.
    camera.Close();
    
}
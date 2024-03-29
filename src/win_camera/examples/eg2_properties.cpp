#include "eg2_properties.h"

#include "win_camera/win_camera.h"

void eg2_properties()
{
    // Get a empty camera
    WinCamera::WinCamera camera = WinCamera::WinCamera();

    // Get available camera list
    std::cout << "Start to list the available cameras..." << std::endl;
    std::vector< WinCamera::WinCameraDevice> cameraDeivceList = camera.getCameras();
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

    // Get exposure in second
    if (camera.Exposure()->isSupported())
    {
        std::cout << "Exposure: " + std::to_string(camera.Exposure()->getValue()) + "s" << std::endl;
        // Set exposure as the largest value
        std::vector<double> exposures = camera.Exposure()->GetPossibleExposureValues();
        camera.Exposure()->setValue(exposures[exposures.size() - 1]);
        std::cout << "Set exposure to " + std::to_string(exposures[exposures.size() - 1]) + "s" << std::endl;

    }else
    {
        std::cout << "This device doesn't support exposure time" << std::endl;
    }


    // Reset all properties to default
    std::cout << "Reset properties to default..." << std::endl;
    camera.ResetProperties();

    // Close the camera.
    camera.Close();
    
}
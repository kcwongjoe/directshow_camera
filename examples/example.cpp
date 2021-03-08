#include "eg1_capture.h"
#include "eg2_properties.h"
#include "eg3_camera_looper.h"
#include "eg4_exposure_fusion.h"
#include <iostream>

int main(int argc, char *argv[])
{
    std::cout << "Example 1: Capture Image." << std::endl;
    std::cout << "Example 2: Properties." << std::endl;
    std::cout << "Example 3: Camera Looper." << std::endl;
    std::cout << "Example 4: Exposure Fusion." << std::endl;
    std::cout << "Enter the example number: ";

    int example_index;
    std::cin >> example_index;

    if (example_index == 1)
    {
        eg1_capture();
    }
    else if (example_index == 2)
    {
        eg2_properties();
    }
    else if (example_index == 3)
    {
        eg3_camera_looper();
    }
    else if (example_index == 4)
    {
        eg4_exposure_fusion();
    }
}
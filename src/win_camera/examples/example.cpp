/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "examples/eg1_capture.h"
#include "examples/eg2_properties.h"
#include "examples/eg3_camera_thread.h"
#include "examples/eg4_exposure_fusion.h"
#include "examples/eg5_disconnect_process.h"
#include "examples/eg6_stub.h"
#include <iostream>

int main(int argc, char *argv[])
{
    std::cout << "Example 1: Capture Image." << std::endl;
    std::cout << "Example 2: Properties." << std::endl;
    std::cout << "Example 3: Camera Thread." << std::endl;
    std::cout << "Example 4: Exposure Fusion." << std::endl;
    std::cout << "Example 5: Disconnect Process." << std::endl;
    std::cout << "Example 6: Stub." << std::endl;
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
        eg3_camera_thread();
    }
    else if (example_index == 4)
    {
        eg4_exposure_fusion();
    }
    else if (example_index == 5)
    {
        eg5_disconnect_process();
    }
        else if (example_index == 6)
    {
        eg6_stub();
    }
}
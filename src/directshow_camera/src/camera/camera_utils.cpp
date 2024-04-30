/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "camera/camera_utils.h"

#include <math.h>

namespace DirectShowCamera
{
    double CameraUtils::DSExposureValueToSec(const long dsValue)
    {
        if (dsValue < 0)
        {
            return pow(0.5, (double)-dsValue);
        }
        else
        {
            return pow(2, (double)dsValue);
        }
    }

    long CameraUtils::DSExposureSecToValue(const double second)
    {
        if (second <= 0)
        {
            return 0;
        }
        else
        {
            return (long)lround(log2(second));
        }
    }
}
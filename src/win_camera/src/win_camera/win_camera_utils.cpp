#include "win_camera/win_camera_utils.h"

namespace WinCamera
{
    double WinCameraUtils::DSExposureValueToSec(const long dsValue)
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

    long WinCameraUtils::DSExposureSecToValue(const double second)
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
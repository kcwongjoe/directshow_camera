#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__WIN_CAMERA_UTILS_H
#define WIN_CAMERA__WIN_CAMERA__WIN_CAMERA_UTILS_H

//************Content************

#include <math.h>

namespace WinCamera
{
    class WinCameraUtils
    {
    public:

        /**
         * @brief Convert DirectShow exposure value to seconds
         * @param[in] dsValue DirectShow exposure value
         * @return Return exposure in second
        */
        static double DSExposureValueToSec(const long dsValue);

        /**
         * @brief Convert exposure value in second to DirectShow exposure value
         * @param[in] second Exposure value in second
         * @return Return DirectShow exposure value
        */
        static long DSExposureSecToValue(const double second);
    };

}

//*******************************

#endif
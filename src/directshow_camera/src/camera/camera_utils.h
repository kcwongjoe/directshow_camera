/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef DIRECTSHOW_CAMERA__CAMERA__CAMERA_UTILS_H
#define DIRECTSHOW_CAMERA__CAMERA__CAMERA_UTILS_H

//************Content************

namespace DirectShowCamera
{
    class CameraUtils
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
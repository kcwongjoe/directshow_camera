/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#pragma once
#ifndef WIN_CAMERA__OPENCV_UTILS__OPENCV_MAT_SETTINGS_H
#define WIN_CAMERA__OPENCV_UTILS__OPENCV_MAT_SETTINGS_H

//************Content************
#ifdef WITH_OPENCV2

namespace WinCamera
{
    class OpenCVMatSettings
    {
    public:

        /**
         * @brief Constructer
        */
        OpenCVMatSettings();

        /**
        * @brief Reset
        */
        void Reset();

        /**
         * @brief Set it as true if the input byte is BGR order, otherwise the input byte is RGB order. Default as false.
        */
        bool BGR = true;

        /**
         * @brief Set it as true to flip iamge vertically. Default as true
        */
        bool VerticalFlip = true;
    };

}

#endif

//*******************************

#endif
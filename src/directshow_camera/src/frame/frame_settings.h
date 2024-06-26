/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef DIRECTSHOW_CAMERA__OPENCV_UTILS__FRAME__FRAME_SETTINGS_H
#define DIRECTSHOW_CAMERA__OPENCV_UTILS__FRAME__FRAME_SETTINGS_H

//************Content************
namespace DirectShowCamera
{
    class FrameSettings
    {
    public:

        /**
         * @brief Constructer
        */
        FrameSettings();

        /**
        * @brief Reset
        */
        void Reset();

        /**
         * @brief Set it as true if the input byte is BGR order, otherwise the input byte is RGB order. Default as false.
        */
        bool BGR = true;

        /**
         * @brief Set it as true to flip image vertically. Default as false
        */
        bool VerticalFlip = false;

        /**
        * @brief equal operator
        */
        bool operator==(const FrameSettings& other) const
        {
            return BGR == other.BGR && VerticalFlip == other.VerticalFlip;
        }

        /**
        * @brief not equal operator
        */
        bool operator!=(const FrameSettings& other) const
        {
            return !(*this == other);
        }
    };

}

//*******************************

#endif // ndef DIRECTSHOW_CAMERA__OPENCV_UTILS__FRAME__FRAME_SETTINGS_H
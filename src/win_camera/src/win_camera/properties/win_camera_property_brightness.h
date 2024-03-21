#pragma once
#ifndef WIN_CAMERA_PROPERTY_BRIGHTNESS_H
#define WIN_CAMERA_PROPERTY_BRIGHTNESS_H

#include "win_camera/properties/win_camera_property.h"

namespace WinCamera { class WinCamera; }

namespace WinCamera
{
    /**
     * @brief A class to store WinCamera Device information
     */
    class WinCameraPropertyBrightness : public WinCameraProperty
    {
    public:
        WinCameraPropertyBrightness(
            const WinCamera& camera,
            const std::shared_ptr<DirectShowCamera::DirectShowCamera>& ds_camera
        );

        bool isPropertySupported() const;
        std::pair<long, long> getRange() const;
        long getStep() const;
        long getValue() const;
        void setValue(const long value);
    };
}

//*******************************

#endif
/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_BACKLIGHT_COMPENSATION_H
#define WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_BACKLIGHT_COMPENSATION_H

//************Content************

#include "win_camera/properties/win_camera_property.h"

namespace WinCamera { class WinCamera; }

namespace WinCamera
{
    class WinCameraPropertyBacklightCompensation : public WinCameraProperty
    {
    public:
        WinCameraPropertyBacklightCompensation(
            const WinCamera& camera,
            const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property backlight compensation is supported.
         * @return Retrun true if property backlight compensation is supported.
        */
        bool isSupported() const;

        /**
         * @brief Get current backlight compensation
         * @return Return current backlight compensation.
        */
        bool isBacklightCompensation() const;

        /**
         * @brief Set Contrast
         * @param isOn Backlight Compensation On = true
         * @return Return true if success.
        */
        void setBacklightCompensation(const bool isOn);

    protected:
        std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> getDirectShowProperty() const override;
    };
}

//*******************************

#endif
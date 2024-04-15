/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_COLOR_ENABLE_H
#define WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_COLOR_ENABLE_H

//************Content************

#include "win_camera/properties/win_camera_property.h"

namespace WinCamera { class WinCamera; }

namespace WinCamera
{
    class WinCameraPropertyColorEnable : public WinCameraProperty
    {
    public:
        WinCameraPropertyColorEnable(
            const WinCamera& camera,
            const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property brightness is supported.
         * @return Retrun true if property brightness is supported.
        */
        bool isSupported() const;

        /**
         * @brief Get current color enable
         * @return Return current color enable.
        */
        bool isColorEnable() const;

        /**
         * @brief Set Color Enable
         * @param isOn Color Enable On = true
        */
        void setColorEnable(const bool isOn);

    protected:
        std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> getDirectShowProperty() const override;
    };
}

//*******************************

#endif
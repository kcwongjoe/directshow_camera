#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_COLOR_ENABLE_H
#define WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_COLOR_ENABLE_H

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
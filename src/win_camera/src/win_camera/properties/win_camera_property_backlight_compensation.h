#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_BACKLIGHT_COMPENSATION_H
#define WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_BACKLIGHT_COMPENSATION_H

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
        bool IsSupported() const;

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
        std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> GetDirectShowProperty() const override;
    };
}

//*******************************

#endif
#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_HUE_H
#define WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_HUE_H

#include "win_camera/properties/win_camera_property.h"

namespace WinCamera { class WinCamera; }

namespace WinCamera
{
    class WinCameraPropertyHue : public WinCameraProperty
    {
    public:
        WinCameraPropertyHue(
            const WinCamera& camera,
            const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property hue is supported.
         * @return Retrun true if property hue is supported.
        */
        bool IsSupported() const;

        /**
         * @brief Get the range of the property - hue
         * @return Return (min,max).
        */
        std::pair<long, long> GetRange() const;

        /**
         * @brief Get the step of the property - Hue.
         * @return Return the step of the Hue.
        */
        long GetStep() const;

        /**
         * @brief Get current hue
         * @return Return current hue.
        */
        long GetValue() const;

        /**
         * @brief Set Hue
         * @param degree Value to be set in degree
         * @return Return true if success.
        */
        void SetValue(const long degree);

    protected:
        std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> GetDirectShowProperty() const override;
    };
}

//*******************************

#endif
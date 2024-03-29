#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_SATURATION_H
#define WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_SATURATION_H

#include "win_camera/properties/win_camera_property.h"

namespace WinCamera { class WinCamera; }

namespace WinCamera
{
    class WinCameraPropertySaturation : public WinCameraProperty
    {
    public:
        WinCameraPropertySaturation(
            const WinCamera& camera,
            const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property saturation is supported.
         * @return Retrun true if property saturation is supported.
        */
        bool IsSupported() const;

        /**
         * @brief Get the range of the property - saturation
         * @return Return (min,max)
        */
        std::pair<long, long> GetRange() const;

        /**
         * @brief Get the step of the property - Saturation.
         * @return Return the step of the Saturation
        */
        long GetStep() const;

        /**
         * @brief Get current saturation
         * @return Return current saturation.
        */
        long GetValue() const;

        /**
         * @brief Set Saturation
         * @param value Value to be set
         * @return Return true if success.
        */
        void SetValue(const long value);

    protected:
        std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> GetDirectShowProperty() const override;
    };
}

//*******************************

#endif
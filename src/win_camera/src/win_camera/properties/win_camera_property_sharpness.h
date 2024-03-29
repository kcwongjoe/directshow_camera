#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_SHARPNESS_H
#define WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_SHARPNESS_H

#include "win_camera/properties/win_camera_property.h"

namespace WinCamera { class WinCamera; }

namespace WinCamera
{
    class WinCameraPropertySharpness : public WinCameraProperty
    {
    public:
        WinCameraPropertySharpness(
            const WinCamera& camera,
            const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property sharpness is supported.
         * @return Retrun true if property sharpness is supported.
        */
        bool IsSupported() const;

        /**
         * @brief Get the range of the property - sharpness
         * @return Return (min,max).
        */
        std::pair<long, long> GetRange() const;

        /**
         * @brief Get the step of the property - Sharpness.
         * @return Return the step of the Sharpness.
        */
        long GetStep() const;

        /**
         * @brief Get current sharpness
         * @return Return current sharpness.
        */
        long GetValue() const;

        /**
         * @brief Set Sharpness
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
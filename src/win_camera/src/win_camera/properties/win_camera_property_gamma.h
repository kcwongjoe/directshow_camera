#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_GAMMA_H
#define WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_GAMMA_H

#include "win_camera/properties/win_camera_property.h"

namespace WinCamera { class WinCamera; }

namespace WinCamera
{
    class WinCameraPropertyGamma : public WinCameraProperty
    {
    public:
        WinCameraPropertyGamma(
            const WinCamera& camera,
            const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property gamma is supported.
         * @return Retrun true if property gamma is supported.
        */
        bool IsSupported() const;

        /**
         * @brief Get the range of the property - gamma
         * @return Return (min,max).
        */
        std::pair<long, long> GetRange() const;

        /**
         * @brief Get the step of the property - Gamma.
         * @return Return the step of the Gamma.
        */
        long GetStep() const;

        /**
         * @brief Get current gamma
         * @return Return current gamma.
        */
        long GetValue() const;

        /**
         * @brief Set Gamma
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
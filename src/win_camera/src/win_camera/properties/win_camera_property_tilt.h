#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_TILT_H
#define WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_TILT_H

#include "win_camera/properties/win_camera_property.h"

namespace WinCamera { class WinCamera; }

namespace WinCamera
{
    class WinCameraPropertyTilt : public WinCameraProperty
    {
    public:
        WinCameraPropertyTilt(
            const WinCamera& camera,
            const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property tilt is supported.
         * @return Retrun true if property tilt is supported.
        */
        bool IsSupported() const;

        /**
         * @brief Get the range of the property - tilt
         * @return Return (min,max).
        */
        std::pair<long, long> GetRange() const;

        /**
         * @brief Get the step of the property - tilt.
         * @return Return the step of the tilt.
        */
        long GetStep() const;

        /**
         * @brief Get current tilt
         * @return Return current tilt.
        */
        long GetValue() const;

        /**
         * @brief Set Tilt
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
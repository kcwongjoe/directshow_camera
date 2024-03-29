#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_ROLL_H
#define WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_ROLL_H

#include "win_camera/properties/win_camera_property.h"

namespace WinCamera { class WinCamera; }

namespace WinCamera
{
    class WinCameraPropertyRoll : public WinCameraProperty
    {
    public:
        WinCameraPropertyRoll(
            const WinCamera& camera,
            const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property roll is supported.
         * @return Retrun true if property roll is supported.
        */
        bool IsSupported() const;

        /**
         * @brief Get the range of the property - roll
         * @return Return (min,max).
        */
        std::pair<long, long> GetRange() const;

        /**
         * @brief Get the step of the property - roll.
         * @return Return the step of the roll.
        */
        long GetStep() const;

        /**
         * @brief Get current Roll
         * @return Return current Roll.
        */
        long GetValue() const;

        /**
         * @brief Set Roll
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
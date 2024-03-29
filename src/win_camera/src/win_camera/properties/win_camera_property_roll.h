#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_ROLL_H
#define WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_ROLL_H

//************Content************

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
        bool isSupported() const;

        /**
         * @brief Get the range of the property - roll
         * @return Return (min,max).
        */
        std::pair<long, long> getRange() const;

        /**
         * @brief Get the step of the property - roll.
         * @return Return the step of the roll.
        */
        long getStep() const;

        /**
         * @brief Get current Roll
         * @return Return current Roll.
        */
        long getValue() const;

        /**
         * @brief Set Roll
         * @param degree Value to be set in degree
         * @return Return true if success.
        */
        void setValue(const long degree);

    protected:
        std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> getDirectShowProperty() const override;
    };
}

//*******************************

#endif
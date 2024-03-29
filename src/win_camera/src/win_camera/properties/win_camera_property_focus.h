#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_FOCUS_H
#define WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_FOCUS_H

#include "win_camera/properties/win_camera_property.h"

namespace WinCamera { class WinCamera; }

namespace WinCamera
{
    class WinCameraPropertyFocus : public WinCameraProperty
    {
    public:
        WinCameraPropertyFocus(
            const WinCamera& camera,
            const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property focus is supported.
         * @return Retrun true if property focus is supported.
        */
        bool IsSupported() const;

        /**
         * @brief Get the range of the property - focus in millimeters.
         * @return Return (min,max).
        */
        std::pair<long, long> GetRange() const;

        /**
         * @brief Get the step of the property - focus in millimeters.
         * @return Return the step of the property - focus in millimeters.
        */
        long GetStep() const;

        /**
         * @brief Get current focus in millimeters.
         * @return Return current focus.
        */
        long GetValue() const;

        /**
         * @brief Set Focus
         * @param millimeter Value to be set in millimeters.
         * @return Return true if success.
        */
        void SetValue(const long millimeter);

        /**
         * @brief Retrun true if focus is auto mode, return false if it is manual mode.
         * @return Retrun true if focus is auto mode, return false if it is manual mode or error occurred.
        */
        bool IsAuto() const;

        /**
         * @brief Set focus to auto or manual mode
         * @param setToAuto Set it as true if you want to set as auto mode. Manual mode as false.
         * @return Return true if success.
        */
        void SetAuto(const bool setToAuto);

    protected:
        std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> GetDirectShowProperty() const override;
    };
}

//*******************************

#endif
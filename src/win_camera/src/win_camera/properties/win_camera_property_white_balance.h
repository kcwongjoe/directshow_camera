#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_WHITE_BALANCE_H
#define WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_WHITE_BALANCE_H

#include "win_camera/properties/win_camera_property.h"

namespace WinCamera { class WinCamera; }

namespace WinCamera
{
    class WinCameraPropertyWhiteBalance : public WinCameraProperty
    {
    public:
        WinCameraPropertyWhiteBalance(
            const WinCamera& camera,
            const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property white balance is supported.
         * @return Retrun true if property white balance is supported.
        */
        bool IsSupported() const;

        /**
         * @brief Get the range of the property - white balance in degree kelvin
         * @return Return (min,max).
        */
        std::pair<long, long> GetRange() const;

        /**
         * @brief Get the step of the property - White Balance.
         * @return Return the step of the White Balance.
        */
        long GetStep() const;

        /**
         * @brief Get current white balance in degree kelvin
         * @return Return current white balance.
        */
        long GetValue() const;

        /**
         * @brief Set WhiteBalance
         * @param kelvin Value to be set in degree kelvin
         * @return Return true if success.
        */
        void SetValue(const long kelvin);

        /**
         * @brief Retrun true if white balance is auto mode, return false if it is manual mode.
         * @return Retrun true if white balance is auto mode, return false if it is manual mode
        */
        bool IsAuto() const;

        /**
         * @brief Set white balance to auto or manual mode
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
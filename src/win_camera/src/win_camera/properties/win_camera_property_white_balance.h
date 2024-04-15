/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_WHITE_BALANCE_H
#define WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_WHITE_BALANCE_H

//************Content************

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
        bool isSupported() const;

        /**
         * @brief Get the range of the property - white balance in degree kelvin
         * @return Return (min,max).
        */
        std::pair<long, long> getRange() const;

        /**
         * @brief Get the step of the property - White Balance.
         * @return Return the step of the White Balance.
        */
        long getStep() const;

        /**
         * @brief Get current white balance in degree kelvin
         * @return Return current white balance.
        */
        long getValue() const;

        /**
         * @brief Set WhiteBalance
         * @param kelvin Value to be set in degree kelvin
         * @return Return true if success.
        */
        void setValue(const long kelvin);

        /**
         * @brief Retrun true if white balance is auto mode, return false if it is manual mode.
         * @return Retrun true if white balance is auto mode, return false if it is manual mode
        */
        bool isAuto() const;

        /**
         * @brief Set white balance to auto or manual mode
         * @param setToAuto Set it as true if you want to set as auto mode. Manual mode as false.
         * @return Return true if success.
        */
        void setAuto(const bool setToAuto);

    protected:
        std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> getDirectShowProperty() const override;
    };
}

//*******************************

#endif
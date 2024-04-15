/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_IRIS_H
#define WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_IRIS_H

//************Content************

#include "win_camera/properties/win_camera_property.h"

namespace WinCamera { class WinCamera; }

namespace WinCamera
{
    class WinCameraPropertyIris : public WinCameraProperty
    {
    public:
        WinCameraPropertyIris(
            const WinCamera& camera,
            const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property iris is supported.
         * @return Retrun true if property iris is supported.
        */
        bool isSupported() const;

        /**
         * @brief Get the range of the property - iris in units of f_stop * 10.
         * @return Return (min,max).
        */
        std::pair<long, long> getRange() const;

        /**
         * @brief Get the step of the property - iris in units of f_stop * 10.
         * @return Return the step of the iris.
        */
        long getStep() const;

        /**
         * @brief Get current iris in units of f_stop * 10.
         * @return Return current iris.
        */
        long getValue() const;

        /**
         * @brief Set Iris
         * @param value Value to be set in units of f_stop * 10.
         * @return Return true if success.
        */
        void setValue(const long value);

        /**
         * @brief Retrun true if iris is auto mode, return false if it is manual mode.
         * @return Retrun true if iris is auto mode, return false if it is manual mode or error occurred.
        */
        bool isAuto() const;

        /**
         * @brief Set iris to auto or manual mode
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
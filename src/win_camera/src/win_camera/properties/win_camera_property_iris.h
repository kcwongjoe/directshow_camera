#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_IRIS_H
#define WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_IRIS_H

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
        bool IsSupported() const;

        /**
         * @brief Get the range of the property - iris in units of f_stop * 10.
         * @return Return (min,max).
        */
        std::pair<long, long> GetRange() const;

        /**
         * @brief Get the step of the property - iris in units of f_stop * 10.
         * @return Return the step of the iris.
        */
        long GetStep() const;

        /**
         * @brief Get current iris in units of f_stop * 10.
         * @return Return current iris.
        */
        long GetValue() const;

        /**
         * @brief Set Iris
         * @param value Value to be set in units of f_stop * 10.
         * @return Return true if success.
        */
        void SetValue(const long value);

        /**
         * @brief Retrun true if iris is auto mode, return false if it is manual mode.
         * @return Retrun true if iris is auto mode, return false if it is manual mode or error occurred.
        */
        bool IsAuto() const;

        /**
         * @brief Set iris to auto or manual mode
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
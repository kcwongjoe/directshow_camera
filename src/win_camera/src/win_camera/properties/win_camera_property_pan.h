#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_PAN_H
#define WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_PAN_H

#include "win_camera/properties/win_camera_property.h"

namespace WinCamera { class WinCamera; }

namespace WinCamera
{
    class WinCameraPropertyPan : public WinCameraProperty
    {
    public:
        WinCameraPropertyPan(
            const WinCamera& camera,
            const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property pan is supported.
         * @return Retrun true if property pan is supported.
        */
        bool IsSupported() const;

        /**
         * @brief Get the range of the property - pan
         * @return Return (min,max).
        */
        std::pair<long, long> GetRange() const;

        /**
         * @brief Get the step of the property - pan.
         * @return Return the step of the pan.
        */
        long GetStep() const;

        /**
         * @brief Get current pan
         * @return Return current pan.
        */
        long GetValue() const;

        /**
         * @brief Set Pan
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
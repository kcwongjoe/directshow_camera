#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_GAIN_H
#define WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_GAIN_H

#include "win_camera/properties/win_camera_property.h"

namespace WinCamera { class WinCamera; }

namespace WinCamera
{
    class WinCameraPropertyGain : public WinCameraProperty
    {
    public:
        WinCameraPropertyGain(
            const WinCamera& camera,
            const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property gain is supported.
         * @return Retrun true if property gain is supported.
        */
        bool IsSupported() const;

        /**
         * @brief Get the range of the property - gain
         * @return Return (min,max).
        */
        std::pair<long, long> GetRange() const;

        /**
         * @brief Get the step of the property - gain.
         * @return Return the step of the gain.
        */
        long GetStep() const;

        /**
         * @brief Get current gain
         * @return Return current gain.
        */
        long GetValue() const;

        /**
         * @brief Set Gain
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
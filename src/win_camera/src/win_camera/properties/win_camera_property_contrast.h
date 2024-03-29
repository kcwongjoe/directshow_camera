#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_CONTRAST_H
#define WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_CONTRAST_H

#include "win_camera/properties/win_camera_property.h"

namespace WinCamera { class WinCamera; }

namespace WinCamera
{
    class WinCameraPropertyContrast : public WinCameraProperty
    {
    public:
        WinCameraPropertyContrast(
            const WinCamera& camera,
            const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property contrast is supported.
         * @return Retrun true if property contrast is supported.
        */
        bool IsSupported() const;

        /**
         * @brief Get the range of the property - contrast
         * @return Return (min,max).
        */
        std::pair<long, long> GetRange() const;

        /**
         * @brief Get the step of the property - contrast.
         * @return Return the step of the contrast.
        */
        long GetStep() const;

        /**
         * @brief Get current contrast
         * @return Return current contrast.
        */
        long GetValue() const;

        /**
         * @brief Set contrast
         * @param[in] value Value to be set
         * @return Return true if success.
        */
        void SetValue(const long value);

    protected:
        std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> GetDirectShowProperty() const override;
    };
}

//*******************************

#endif
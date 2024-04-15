/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_BRIGHTNESS_H
#define WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_BRIGHTNESS_H

//************Content************

#include "win_camera/properties/win_camera_property.h"

#include <functional>
#include <memory>

namespace WinCamera { class WinCamera; }

namespace WinCamera
{
    class WinCameraPropertyBrightness : public WinCameraProperty
    {
    public:
        WinCameraPropertyBrightness(
            const WinCamera& camera,
            const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property brightness is supported.
         * @return Retrun true if property brightness is supported.
        */
        bool isSupported() const;

        /**
         * @brief Get the range of the property - brightness
         * @return Return (min,max).
        */
        std::pair<long, long> getRange() const;

        /**
         * @brief Get the step of the property - Brightness.
         * @return Return the step of the Brightness.
        */
        long getStep() const;

        /**
         * @brief Get current brightness, from blanking(small value) to pure white(large value)
         * @return Return current brightness.
        */
        long getValue() const;

        /**
         * @brief Set brightness
         * @param[in] value Value to be set
         * @return Return true if success.
        */
        void setValue(const long value);

    protected:
        std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> getDirectShowProperty() const override;
    };
}

//*******************************

#endif
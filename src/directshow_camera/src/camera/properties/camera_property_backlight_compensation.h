/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef DIRECTSHOW_CAMERA__CAMERA__PROPERTIES__CAMERA_PROPERTY_BACKLIGHT_COMPENSATION_H
#define DIRECTSHOW_CAMERA__CAMERA__PROPERTIES__CAMERA_PROPERTY_BACKLIGHT_COMPENSATION_H

//************Content************

#include "camera/properties/camera_property.h"

#include <functional>
#include <memory>

namespace DirectShowCamera { class Camera; }

namespace DirectShowCamera
{
    class CameraPropertyBacklightCompensation : public CameraProperty
    {
    public:
        CameraPropertyBacklightCompensation(
            const Camera& camera,
            const std::shared_ptr<AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property backlight compensation is supported.
         * @return Retrun true if property backlight compensation is supported.
        */
        bool isSupported() const;

        /**
         * @brief Get current backlight compensation
         * @return Return current backlight compensation.
        */
        bool isBacklightCompensation() const;

        /**
         * @brief Set Contrast
         * @param isOn Backlight Compensation On = true
         * @return Return true if success.
        */
        void setBacklightCompensation(const bool isOn);

    protected:
        std::shared_ptr<DirectShowCameraProperty> getDirectShowProperty() const override;
    };
}

//*******************************

#endif
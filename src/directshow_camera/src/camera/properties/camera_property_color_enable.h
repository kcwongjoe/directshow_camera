/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef DIRECTSHOW_CAMERA__CAMERA__PROPERTIES__CAMERA_PROPERTY_COLOR_ENABLE_H
#define DIRECTSHOW_CAMERA__CAMERA__PROPERTIES__CAMERA_PROPERTY_COLOR_ENABLE_H

//************Content************

#include "camera/properties/camera_property.h"

#include <functional>
#include <memory>

namespace DirectShowCamera { class Camera; }

namespace DirectShowCamera
{
    class CameraPropertyColorEnable : public CameraProperty
    {
    public:
        CameraPropertyColorEnable(
            const Camera& camera,
            const std::shared_ptr<AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property brightness is supported.
         * @return Retrun true if property brightness is supported.
        */
        bool isSupported() const;

        /**
         * @brief Get current color enable
         * @return Return current color enable.
        */
        bool isColorEnable() const;

        /**
         * @brief Set Color Enable
         * @param isOn Color Enable On = true
        */
        void setColorEnable(const bool isOn);

    protected:
        std::shared_ptr<DirectShowCameraProperty> getDirectShowProperty() const override;
    };
}

//*******************************

#endif
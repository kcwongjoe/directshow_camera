/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef DIRECTSHOW_CAMERA__CAMERA__PROPERTIES__CAMERA_PROPERTY_CONTRAST_H
#define DIRECTSHOW_CAMERA__CAMERA__PROPERTIES__CAMERA_PROPERTY_CONTRAST_H

//************Content************

#include "camera/properties/camera_property.h"

#include <functional>
#include <memory>

namespace DirectShowCamera { class Camera; }

namespace DirectShowCamera
{
    class CameraPropertyContrast : public CameraProperty
    {
    public:
        CameraPropertyContrast(
            const Camera& camera,
            const std::shared_ptr<AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property contrast is supported.
         * @return Retrun true if property contrast is supported.
        */
        bool isSupported() const;

        /**
         * @brief Get the range of the property - contrast
         * @return Return (min,max).
        */
        std::pair<long, long> getRange() const;

        /**
         * @brief Get the step of the property - contrast.
         * @return Return the step of the contrast.
        */
        long getStep() const;

        /**
         * @brief Get current contrast
         * @return Return current contrast.
        */
        long getValue() const;

        /**
         * @brief Set contrast
         * @param[in] value Value to be set
         * @return Return true if success.
        */
        void setValue(const long value);

    protected:
        std::shared_ptr<DirectShowCameraProperty> getDirectShowProperty() const override;
    };
}

//*******************************

#endif
/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef DIRECTSHOW_CAMERA__CAMERA__PROPERTIES__CAMERA_PROPERTY_TILT_H
#define DIRECTSHOW_CAMERA__CAMERA__PROPERTIES__CAMERA_PROPERTY_TILT_H

//************Content************

#include "camera/properties/camera_property.h"

#include <functional>
#include <memory>

namespace DirectShowCamera { class Camera; }

namespace DirectShowCamera
{
    class CameraPropertyTilt : public CameraProperty
    {
    public:
        CameraPropertyTilt(
            const Camera& camera,
            const std::shared_ptr<AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property tilt is supported.
         * @return Retrun true if property tilt is supported.
        */
        bool isSupported() const;

        /**
         * @brief Get the range of the property - tilt
         * @return Return (min,max).
        */
        std::pair<long, long> getRange() const;

        /**
         * @brief Get the step of the property - tilt.
         * @return Return the step of the tilt.
        */
        long getStep() const;

        /**
         * @brief Get current tilt
         * @return Return current tilt.
        */
        long getValue() const;

        /**
         * @brief Set Tilt
         * @param degree Value to be set in degree
         * @return Return true if success.
        */
        void setValue(const long degree);

    protected:
        std::shared_ptr<DirectShowCameraProperty> getDirectShowProperty() const override;
    };
}

//*******************************

#endif
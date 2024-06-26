/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef DIRECTSHOW_CAMERA__CAMERA__PROPERTIES__CAMERA_PROPERTY_DIGITAL_ZOOM_LEVEL_H
#define DIRECTSHOW_CAMERA__CAMERA__PROPERTIES__CAMERA_PROPERTY_DIGITAL_ZOOM_LEVEL_H

//************Content************

#include "camera/properties/camera_property.h"

#include <functional>
#include <memory>

namespace DirectShowCamera { class Camera; }

namespace DirectShowCamera
{
    /**
     * @brief   Digital zoom is applied after the image is captured. The effect of
     *          digital zoom is to multiply the optical magnification by a factor m,
     *          which can be calculated as follows:
     *          m = ( (Z'cur - Z'min) * (m-max - 1) ) / (Z'max - Z'min) ) + 1
     *          where
     *          Z'cur is the current digital zoom level
     *          Z'min is the minimum digital zoom level
     *          Z'max is the maximum digital zoom level
     *          m-max is the Maximum digital magnification See KSPROPERTY_VIDEOPROCAMP_DIGITAL_MULTIPLIER_LIMIT
     *
     */
    class CameraPropertyDigitalZoomLevel : public CameraProperty
    {
    public:
        CameraPropertyDigitalZoomLevel(
            const Camera& camera,
            const std::shared_ptr<AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property digital zoom level is supported.
         * @return Retrun true if property digital zoom level is supported.
        */
        bool isSupported() const;

        /**
         * @brief Get the range of the property - digital zoom level
         * @return Return (min,max).
        */
        std::pair<long, long> getRange() const;

        /**
         * @brief Get the step of the property - digital zoom level
         * @return Return the step of the digital zoom level
        */
        long getStep() const;

        /**
         * @brief Get current digital zoom level
         * @return Return current digital zoom level
        */
        long getValue() const;

        /**
         * @brief Set digital zoom level
         * @param value Value to be set
         * @return Return true if success.
        */
        void setValue(const long value);

        /**
        * @brief    Get the factor m of digital zoom level. The effect of digital zoom is to
        *           multiply the optical magnification by a factor m. 
        *           m = ( (Z'cur - Z'min) * (m-max - 1) ) / (Z'max - Z'min) ) + 1
        *           where
        *           Z'cur is the current digital zoom level
        *           Z'min is the minimum digital zoom level
        *           Z'max is the maximum digital zoom level
        *           m-max is the Maximum digital magnification
        * @return Return factor m
        */
        double getFactorM() const;

    protected:
        std::shared_ptr<DirectShowCameraProperty> getDirectShowProperty() const override;
    };
}

//*******************************

#endif
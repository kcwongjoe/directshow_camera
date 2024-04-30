/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef DIRECTSHOW_CAMERA__CAMERA__PROPERTIES__CAMERA_PROPERTY_IRIS_H
#define DIRECTSHOW_CAMERA__CAMERA__PROPERTIES__CAMERA_PROPERTY_IRIS_H

//************Content************

#include "camera/properties/camera_property.h"

#include <functional>
#include <memory>

namespace DirectShowCamera { class Camera; }

namespace DirectShowCamera
{
    class CameraPropertyIris : public CameraProperty
    {
    public:
        CameraPropertyIris(
            const Camera& camera,
            const std::shared_ptr<AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property iris is supported.
         * @return Retrun true if property iris is supported.
        */
        bool isSupported() const;

        /**
         * @brief Get the range of the property - iris in units of f_stop * 10.
         * @return Return (min,max).
        */
        std::pair<long, long> getRange() const;

        /**
         * @brief Get the step of the property - iris in units of f_stop * 10.
         * @return Return the step of the iris.
        */
        long getStep() const;

        /**
         * @brief Get current iris in units of f_stop * 10.
         * @return Return current iris.
        */
        long getValue() const;

        /**
         * @brief Set Iris
         * @param value Value to be set in units of f_stop * 10.
         * @return Return true if success.
        */
        void setValue(const long value);

        /**
         * @brief Retrun true if iris is auto mode, return false if it is manual mode.
         * @return Retrun true if iris is auto mode, return false if it is manual mode or error occurred.
        */
        bool isAuto() const;

        /**
         * @brief Set iris to auto or manual mode
         * @param setToAuto Set it as true if you want to set as auto mode. Manual mode as false.
         * @return Return true if success.
        */
        void setAuto(const bool setToAuto);

    protected:
        std::shared_ptr<DirectShowCameraProperty> getDirectShowProperty() const override;
    };
}

//*******************************

#endif
/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef DIRECTSHOW_CAMERA__CAMERA__PROPERTIES__CAMERA_PROPERTY_FOCUS_H
#define DIRECTSHOW_CAMERA__CAMERA__PROPERTIES__CAMERA_PROPERTY_FOCUS_H

//************Content************

#include "camera/properties/camera_property.h"

#include <functional>
#include <memory>

namespace DirectShowCamera { class Camera; }

namespace DirectShowCamera
{
    class CameraPropertyFocus : public CameraProperty
    {
    public:
        CameraPropertyFocus(
            const Camera& camera,
            const std::shared_ptr<AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property focus is supported.
         * @return Retrun true if property focus is supported.
        */
        bool isSupported() const;

        /**
         * @brief Get the range of the property - focus in millimeters.
         * @return Return (min,max).
        */
        std::pair<long, long> getRange() const;

        /**
         * @brief Get the step of the property - focus in millimeters.
         * @return Return the step of the property - focus in millimeters.
        */
        long getStep() const;

        /**
         * @brief Get current focus in millimeters.
         * @return Return current focus.
        */
        long getValue() const;

        /**
         * @brief Set Focus
         * @param millimeter Value to be set in millimeters.
         * @return Return true if success.
        */
        void setValue(const long millimeter);

        /**
         * @brief Retrun true if focus is auto mode, return false if it is manual mode.
         * @return Retrun true if focus is auto mode, return false if it is manual mode or error occurred.
        */
        bool isAuto() const;

        /**
         * @brief Set focus to auto or manual mode
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
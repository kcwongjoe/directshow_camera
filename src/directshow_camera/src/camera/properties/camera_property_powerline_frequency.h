/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef DIRECTSHOW_CAMERA__CAMERA__PROPERTIES__CAMERA_PROPERTY_POWERLINE_FREQUENCY_H
#define DIRECTSHOW_CAMERA__CAMERA__PROPERTIES__CAMERA_PROPERTY_POWERLINE_FREQUENCY_H

//************Content************

#include "camera/properties/camera_property.h"

#include <functional>
#include <memory>

namespace DirectShowCamera { class Camera; }

namespace DirectShowCamera
{
    class CameraPropertyPowerlineFrequency : public CameraProperty
    {
    public:
        enum PowerlineFrequency
        {
            Disable,
            Hz50,
            Hz60
        };

    public:
        CameraPropertyPowerlineFrequency(
            const Camera& camera,
            const std::shared_ptr<AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property powerline frequency is supported.
         * @return Retrun true if property powerline frequency is supported.
        */
        bool isSupported() const;

        /**
         * @brief Get current powerline frequency
         * @return Return current powerline frequency.
        */
        PowerlineFrequency getValue() const;

        /**
         * @brief Set powerline frequency
         * @param powerlineFrequency Value to be set
         * @return Return true if success.
        */
        void setValue(const PowerlineFrequency powerlineFrequency);

    private:

        /**
        * @brief Convert long value to PowerlineFrequency enum
        * @param value Value to be converted
        * @return Return PowerlineFrequency
        **/
        PowerlineFrequency ValueToPowerlineFrequency(const long value) const;

        /**
        * @brief Convert PowerlineFrequency enum to long value
        * @param powerlineFrequency Value to be converted
        * @return Return long value
        **/
        long PowerlineFrequencyToValue(const PowerlineFrequency powerlineFrequency) const;

    protected:
        std::shared_ptr<DirectShowCameraProperty> getDirectShowProperty() const override;
    };
}

//*******************************

#endif
/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef DIRECTSHOW_CAMERA__CAMERA__PROPERTIES__CAMERA_PROPERTY_EXPOSURE_H
#define DIRECTSHOW_CAMERA__CAMERA__PROPERTIES__CAMERA_PROPERTY_EXPOSURE_H

//************Content************

#include "camera/properties/camera_property.h"

#include <functional>
#include <memory>

namespace DirectShowCamera { class Camera; }

namespace DirectShowCamera
{
    class CameraPropertyExposure : public CameraProperty
    {
    public:
        CameraPropertyExposure(
            const Camera& camera,
            const std::shared_ptr<AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property exposure is supported.
         * @return Retrun true if property exposure is supported.
        */
        bool isSupported() const;

        /**
         * @brief Get the range of the property - exposure in second
         * @return Return (min,max).
        */
        std::pair<double, double> getRange() const;

        /**
         * @brief Get current exposure in second
         * @return Return current exposure.
        */
        double getValue() const;

        /**
         * @brief Set Exposure
         * @param second Value to be set in second
         * @return Return true if success.
        */
        void setValue(const double second);

        /**
         * @brief Retrun true if exposure is auto mode, return false if it is manual mode.
         * @return Retrun true if exposure is auto mode, return false if it is manual mode or error occurred.
        */
        bool isAuto() const;

        /**
         * @brief Set exposure to auto or manual mode
         * @param setToAuto Set it as true if you want to set as auto mode. Manual mode as false.
         * @return Return true if success.
        */
        void setAuto(const bool setToAuto);

        /**
         * @brief Get all possible exposure values in second
         * @return Return the possible exposure values.
        */
        std::vector<double> GetPossibleExposureValues() const;

        /**
         * @brief Get current exposure index of the getPossibleExposureValues()
         * @return Return current exposure index of the getPossibleExposureValues().
        */
        int GetExposureIndex() const;


    protected:
        std::shared_ptr<DirectShowCameraProperty> getDirectShowProperty() const override;
    };
}

//*******************************

#endif
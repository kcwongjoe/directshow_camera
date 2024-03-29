#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_EXPOSURE_H
#define WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_EXPOSURE_H

#include "win_camera/properties/win_camera_property.h"

namespace WinCamera { class WinCamera; }

namespace WinCamera
{
    class WinCameraPropertyExposure : public WinCameraProperty
    {
    public:
        WinCameraPropertyExposure(
            const WinCamera& camera,
            const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property exposure is supported.
         * @return Retrun true if property exposure is supported.
        */
        bool IsSupported() const;

        /**
         * @brief Get the range of the property - exposure in second
         * @return Return (min,max).
        */
        std::pair<double, double> GetRange() const;

        /**
         * @brief Get current exposure in second
         * @return Return current exposure.
        */
        double GetValue() const;

        /**
         * @brief Set Exposure
         * @param second Value to be set in second
         * @return Return true if success.
        */
        void SetValue(const double second);

        /**
         * @brief Retrun true if exposure is auto mode, return false if it is manual mode.
         * @return Retrun true if exposure is auto mode, return false if it is manual mode or error occurred.
        */
        bool IsAuto() const;

        /**
         * @brief Set exposure to auto or manual mode
         * @param setToAuto Set it as true if you want to set as auto mode. Manual mode as false.
         * @return Return true if success.
        */
        void SetAuto(const bool setToAuto);

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
        std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> GetDirectShowProperty() const override;
    };
}

//*******************************

#endif
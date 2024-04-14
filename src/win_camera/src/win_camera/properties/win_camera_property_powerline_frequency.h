#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_POWERLINE_FREQUENCY_H
#define WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_POWERLINE_FREQUENCY_H

//************Content************

#include "win_camera/properties/win_camera_property.h"

namespace WinCamera { class WinCamera; }

namespace WinCamera
{
    class WinCameraPropertyPowerlineFrequency : public WinCameraProperty
    {
    public:
        enum PowerlineFrequency
        {
            Disable,
            Hz50,
            Hz60
        };

    public:
        WinCameraPropertyPowerlineFrequency(
            const WinCamera& camera,
            const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
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
        std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> getDirectShowProperty() const override;
    };
}

//*******************************

#endif
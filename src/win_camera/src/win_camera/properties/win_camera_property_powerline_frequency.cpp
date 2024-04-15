/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "win_camera/properties/win_camera_property_powerline_frequency.h"

#include "win_camera/win_camera.h"

namespace WinCamera
{
    WinCameraPropertyPowerlineFrequency::WinCameraPropertyPowerlineFrequency(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyPowerlineFrequency::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getPowerlineFrequency();
    }

    bool WinCameraPropertyPowerlineFrequency::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    WinCameraPropertyPowerlineFrequency::PowerlineFrequency WinCameraPropertyPowerlineFrequency::getValue() const
    {
        const auto value = getValueInternal();
        return ValueToPowerlineFrequency(value);
    }

    void WinCameraPropertyPowerlineFrequency::setValue(const PowerlineFrequency powerlineFrequency)
    {
        const auto value = PowerlineFrequencyToValue(powerlineFrequency);
        setValueInternal(value);
    }

    WinCameraPropertyPowerlineFrequency::PowerlineFrequency WinCameraPropertyPowerlineFrequency::ValueToPowerlineFrequency(const long value) const
    {
        if (value == 0)
        {
            return PowerlineFrequency::Disable;
        }
        else if (value == 1)
        {
            return PowerlineFrequency::Hz50;
        }
        else if (value == 2)
        {
            return PowerlineFrequency::Hz60;
        }
        else
        {
            throw std::invalid_argument("Invalid value(" + std::to_string(value) + ")");
        }
    }

    long WinCameraPropertyPowerlineFrequency::PowerlineFrequencyToValue(const PowerlineFrequency powerlineFrequency) const
    {
        if (powerlineFrequency == PowerlineFrequency::Disable)
        {
            return 0;
        }
        else if (powerlineFrequency == PowerlineFrequency::Hz50)
        {
            return 1;
        }
        else if (powerlineFrequency == PowerlineFrequency::Hz60)
        {
            return 2;
        }
        else
        {
            throw std::invalid_argument("Invalid PowerlineFrequency value.");
        }
    }
}
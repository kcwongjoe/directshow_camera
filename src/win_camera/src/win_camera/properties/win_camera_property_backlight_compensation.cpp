/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#include "win_camera/properties/win_camera_property_backlight_compensation.h"

#include "win_camera/win_camera.h"

namespace WinCamera
{
    WinCameraPropertyBacklightCompensation::WinCameraPropertyBacklightCompensation(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyBacklightCompensation::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getBacklightCompensation();
    }

    bool WinCameraPropertyBacklightCompensation::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    bool WinCameraPropertyBacklightCompensation::isBacklightCompensation() const
    {
        bool isBacklightCompensation = getValueInternal() == 0 ? false : true; // 0(off), 1(on)

        return isBacklightCompensation;
    }

    void WinCameraPropertyBacklightCompensation::setBacklightCompensation(const bool isOn)
    {
        long value = isOn ? 1 : 0; // 0(off), 1(on)

        setValueInternal(value);
    }
}
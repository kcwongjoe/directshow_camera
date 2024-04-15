/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#include "win_camera/properties/win_camera_property_color_enable.h"

#include "win_camera/win_camera.h"

namespace WinCamera
{
    WinCameraPropertyColorEnable::WinCameraPropertyColorEnable(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyColorEnable::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getColorEnable();
    }

    bool WinCameraPropertyColorEnable::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    bool WinCameraPropertyColorEnable::isColorEnable() const
    {
        bool isColorEnable = getValueInternal() == 0 ? false : true; // 0(off), 1(on)

        return isColorEnable;
    }

    void WinCameraPropertyColorEnable::setColorEnable(const bool isOn)
    {
        // Get value
        const auto range = getRangeInternal();
        long value = isOn ? range.second : range.first; // 0(off), 1(on)

        // Set
        setValueInternal(value);
    }
}
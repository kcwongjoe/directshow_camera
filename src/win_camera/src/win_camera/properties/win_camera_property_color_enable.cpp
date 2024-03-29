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

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyColorEnable::GetDirectShowProperty() const
    {
        return m_camera->getProperties()->getColorEnable();
    }

    bool WinCameraPropertyColorEnable::IsSupported() const
    {
        return IsPropertySupportedInternal();
    }

    bool WinCameraPropertyColorEnable::isColorEnable() const
    {
        bool isColorEnable = GetValueInternal() == 0 ? false : true; // 0(off), 1(on)

        return isColorEnable;
    }

    void WinCameraPropertyColorEnable::setColorEnable(const bool isOn)
    {
        // Get value
        const auto range = GetRangeInternal();
        long value = isOn ? range.second : range.first; // 0(off), 1(on)

        // Set
        SetValueInternal(value);
    }
}
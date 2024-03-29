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

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyBacklightCompensation::GetDirectShowProperty() const
    {
        return m_camera->getProperties()->getBacklightCompensation();
    }

    bool WinCameraPropertyBacklightCompensation::IsSupported() const
    {
        return IsPropertySupportedInternal();
    }

    bool WinCameraPropertyBacklightCompensation::isBacklightCompensation() const
    {
        bool isBacklightCompensation = GetValueInternal() == 0 ? false : true; // 0(off), 1(on)

        return isBacklightCompensation;
    }

    void WinCameraPropertyBacklightCompensation::setBacklightCompensation(const bool isOn)
    {
        long value = isOn ? 1 : 0; // 0(off), 1(on)

        SetValueInternal(value);
    }
}
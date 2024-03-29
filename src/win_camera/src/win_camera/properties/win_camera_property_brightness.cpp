#include "win_camera/properties/win_camera_property_brightness.h"

#include "win_camera/win_camera.h"

namespace WinCamera
{
    WinCameraPropertyBrightness::WinCameraPropertyBrightness(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyBrightness::GetDirectShowProperty() const
    {
        return m_camera->getProperties()->getBrightness();
    }

    bool WinCameraPropertyBrightness::IsSupported() const
    {
        return IsPropertySupportedInternal();
    }

    std::pair<long, long> WinCameraPropertyBrightness::GetRange() const
    {
        return GetRangeInternal();
    }

    long WinCameraPropertyBrightness::GetStep() const
    {
        return GetStepInternal();
    }

    long WinCameraPropertyBrightness::GetValue() const
    {
        return GetValueInternal();
    }

    void WinCameraPropertyBrightness::SetValue(const long value)
    {
        SetValueInternal(value);
    }
}
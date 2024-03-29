#include "win_camera/properties/win_camera_property_sharpness.h"

#include "win_camera/win_camera.h"

namespace WinCamera
{
    WinCameraPropertySharpness::WinCameraPropertySharpness(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertySharpness::GetDirectShowProperty() const
    {
        return m_camera->getProperties()->getSharpness();
    }

    bool WinCameraPropertySharpness::IsSupported() const
    {
        return IsPropertySupportedInternal();
    }

    std::pair<long, long> WinCameraPropertySharpness::GetRange() const
    {
        return GetRangeInternal();
    }

    long WinCameraPropertySharpness::GetStep() const
    {
        return GetStepInternal();
    }

    long WinCameraPropertySharpness::GetValue() const
    {
        return GetValueInternal();
    }

    void WinCameraPropertySharpness::SetValue(const long value)
    {
        SetValueInternal(value);
    }
}
#include "win_camera/properties/win_camera_property_saturation.h"

#include "win_camera/win_camera.h"

namespace WinCamera
{
    WinCameraPropertySaturation::WinCameraPropertySaturation(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertySaturation::GetDirectShowProperty() const
    {
        return m_camera->getProperties()->getSaturation();
    }

    bool WinCameraPropertySaturation::IsSupported() const
    {
        return IsPropertySupportedInternal();
    }

    std::pair<long, long> WinCameraPropertySaturation::GetRange() const
    {
        return GetRangeInternal();
    }

    long WinCameraPropertySaturation::GetStep() const
    {
        return GetStepInternal();
    }

    long WinCameraPropertySaturation::GetValue() const
    {
        return GetValueInternal();
    }

    void WinCameraPropertySaturation::SetValue(const long value)
    {
        SetValueInternal(value);
    }
}
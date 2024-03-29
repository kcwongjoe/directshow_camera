#include "win_camera/properties/win_camera_property_gamma.h"

#include "win_camera/win_camera.h"

namespace WinCamera
{
    WinCameraPropertyGamma::WinCameraPropertyGamma(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyGamma::GetDirectShowProperty() const
    {
        return m_camera->getProperties()->getGamma();
    }

    bool WinCameraPropertyGamma::IsSupported() const
    {
        return IsPropertySupportedInternal();
    }

    std::pair<long, long> WinCameraPropertyGamma::GetRange() const
    {
        return GetRangeInternal();
    }

    long WinCameraPropertyGamma::GetStep() const
    {
        return GetStepInternal();
    }

    long WinCameraPropertyGamma::GetValue() const
    {
        return GetValueInternal();
    }

    void WinCameraPropertyGamma::SetValue(const long value)
    {
        SetValueInternal(value);
    }
}
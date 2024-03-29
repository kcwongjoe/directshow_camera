#include "win_camera/properties/win_camera_property_gain.h"

#include "win_camera/win_camera.h"

namespace WinCamera
{
    WinCameraPropertyGain::WinCameraPropertyGain(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyGain::GetDirectShowProperty() const
    {
        return m_camera->getProperties()->getGain();
    }

    bool WinCameraPropertyGain::IsSupported() const
    {
        return IsPropertySupportedInternal();
    }

    std::pair<long, long> WinCameraPropertyGain::GetRange() const
    {
        return GetRangeInternal();
    }

    long WinCameraPropertyGain::GetStep() const
    {
        return GetStepInternal();
    }

    long WinCameraPropertyGain::GetValue() const
    {
        return GetValueInternal();
    }

    void WinCameraPropertyGain::SetValue(const long value)
    {
        SetValueInternal(value);
    }
}
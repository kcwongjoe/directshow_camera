#include "win_camera/properties/win_camera_property_contrast.h"

#include "win_camera/win_camera.h"

namespace WinCamera
{
    WinCameraPropertyContrast::WinCameraPropertyContrast(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyContrast::GetDirectShowProperty() const
    {
        return m_camera->getProperties()->getContrast();
    }

    bool WinCameraPropertyContrast::IsSupported() const
    {
        return IsPropertySupportedInternal();
    }

    std::pair<long, long> WinCameraPropertyContrast::GetRange() const
    {
        return GetRangeInternal();
    }

    long WinCameraPropertyContrast::GetStep() const
    {
        return GetStepInternal();
    }

    long WinCameraPropertyContrast::GetValue() const
    {
        return GetValueInternal();
    }

    void WinCameraPropertyContrast::SetValue(const long value)
    {
        SetValueInternal(value);
    }
}
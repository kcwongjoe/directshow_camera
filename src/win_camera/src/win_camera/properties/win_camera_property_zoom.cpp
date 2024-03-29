#include "win_camera/properties/win_camera_property_zoom.h"

#include "win_camera/win_camera.h"

namespace WinCamera
{
    WinCameraPropertyZoom::WinCameraPropertyZoom(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyZoom::GetDirectShowProperty() const
    {
        return m_camera->getProperties()->getZoom();
    }

    bool WinCameraPropertyZoom::IsSupported() const
    {
        return IsPropertySupportedInternal();
    }

    std::pair<long, long> WinCameraPropertyZoom::GetRange() const
    {
        return GetRangeInternal();
    }

    long WinCameraPropertyZoom::GetStep() const
    {
        return GetStepInternal();
    }

    long WinCameraPropertyZoom::GetValue() const
    {
        return GetValueInternal();
    }

    void WinCameraPropertyZoom::SetValue(const long millimeter)
    {
        SetValueInternal(millimeter);
    }
}
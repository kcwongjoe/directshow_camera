#include "win_camera/properties/win_camera_property_focus.h"

#include "win_camera/win_camera.h"

namespace WinCamera
{
    WinCameraPropertyFocus::WinCameraPropertyFocus(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyFocus::GetDirectShowProperty() const
    {
        return m_camera->getProperties()->getFocus();
    }

    bool WinCameraPropertyFocus::IsSupported() const
    {
        return IsPropertySupportedInternal();
    }

    std::pair<long, long> WinCameraPropertyFocus::GetRange() const
    {
        return GetRangeInternal();
    }

    long WinCameraPropertyFocus::GetStep() const
    {
        return GetStepInternal();
    }

    long WinCameraPropertyFocus::GetValue() const
    {
        return GetValueInternal();
    }

    void WinCameraPropertyFocus::SetValue(const long millimeter)
    {
        SetValueInternal(millimeter);
    }

    bool WinCameraPropertyFocus::IsAuto() const
    {
        return IsAutoInternal();
    }

    void WinCameraPropertyFocus::SetAuto(const bool setToAuto)
    {
        // Check mode support
        CheckModeSupport(setToAuto ? Mode::Auto : Mode::Manual);

        // Set value
        SetValueInternal(GetValueInternal(), setToAuto);
    }
}
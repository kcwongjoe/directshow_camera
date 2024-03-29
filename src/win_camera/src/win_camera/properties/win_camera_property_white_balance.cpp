#include "win_camera/properties/win_camera_property_white_balance.h"

#include "win_camera/win_camera.h"

namespace WinCamera
{
    WinCameraPropertyWhiteBalance::WinCameraPropertyWhiteBalance(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyWhiteBalance::GetDirectShowProperty() const
    {
        return m_camera->getProperties()->getWhiteBalance();
    }

    bool WinCameraPropertyWhiteBalance::IsSupported() const
    {
        return IsPropertySupportedInternal();
    }

    std::pair<long, long> WinCameraPropertyWhiteBalance::GetRange() const
    {
        return GetRangeInternal();
    }

    long WinCameraPropertyWhiteBalance::GetStep() const
    {
        return GetStepInternal();
    }

    long WinCameraPropertyWhiteBalance::GetValue() const
    {
        return GetValueInternal();
    }

    void WinCameraPropertyWhiteBalance::SetValue(const long kelvin)
    {
        SetValueInternal(kelvin);
    }

    bool WinCameraPropertyWhiteBalance::IsAuto() const
    {
        return IsAutoInternal();
    }

    void WinCameraPropertyWhiteBalance::SetAuto(const bool setToAuto)
    {
        // Check mode support
        CheckModeSupport(setToAuto ? Mode::Auto : Mode::Manual);

        // Set value
        SetValueInternal(GetValueInternal(), setToAuto);
    }
}
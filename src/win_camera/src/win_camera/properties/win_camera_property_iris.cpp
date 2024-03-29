#include "win_camera/properties/win_camera_property_iris.h"

#include "win_camera/win_camera.h"

namespace WinCamera
{
    WinCameraPropertyIris::WinCameraPropertyIris(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyIris::GetDirectShowProperty() const
    {
        return m_camera->getProperties()->getIris();
    }

    bool WinCameraPropertyIris::IsSupported() const
    {
        return IsPropertySupportedInternal();
    }

    std::pair<long, long> WinCameraPropertyIris::GetRange() const
    {
        return GetRangeInternal();
    }

    long WinCameraPropertyIris::GetStep() const
    {
        return GetStepInternal();
    }

    long WinCameraPropertyIris::GetValue() const
    {
        return GetValueInternal();
    }

    void WinCameraPropertyIris::SetValue(const long value)
    {
        SetValueInternal(value);
    }

    bool WinCameraPropertyIris::IsAuto() const
    {
        return IsAutoInternal();
    }

    void WinCameraPropertyIris::SetAuto(const bool setToAuto)
    {
        // Check mode support
        CheckModeSupport(setToAuto ? Mode::Auto : Mode::Manual);

        // Set value
        SetValueInternal(GetValueInternal(), setToAuto);
    }
}
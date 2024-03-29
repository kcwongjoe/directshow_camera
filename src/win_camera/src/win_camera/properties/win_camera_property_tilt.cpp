#include "win_camera/properties/win_camera_property_tilt.h"

#include "win_camera/win_camera.h"

#include "utils/math_utils.h"

namespace WinCamera
{
    WinCameraPropertyTilt::WinCameraPropertyTilt(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyTilt::GetDirectShowProperty() const
    {
        return m_camera->getProperties()->getTilt();
    }

    bool WinCameraPropertyTilt::IsSupported() const
    {
        return IsPropertySupportedInternal();
    }

    std::pair<long, long> WinCameraPropertyTilt::GetRange() const
    {
        return GetRangeInternal();
    }

    long WinCameraPropertyTilt::GetStep() const
    {
        return GetStepInternal();
    }

    long WinCameraPropertyTilt::GetValue() const
    {
        return GetValueInternal();
    }

    void WinCameraPropertyTilt::SetValue(const long degree)
    {
        // Make sure the degree is in the range of -180 to 180
        const auto degreeIn180 = Utils::MathUtils::ConfirmDegreeIn180Range(degree);

        // Set value
        SetValueInternal(degreeIn180);
    }
}
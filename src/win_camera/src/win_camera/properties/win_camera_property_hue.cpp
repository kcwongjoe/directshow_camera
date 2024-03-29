#include "win_camera/properties/win_camera_property_hue.h"

#include "win_camera/win_camera.h"

#include "utils/math_utils.h"

namespace WinCamera
{
    WinCameraPropertyHue::WinCameraPropertyHue(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyHue::GetDirectShowProperty() const
    {
        return m_camera->getProperties()->getHue();
    }

    bool WinCameraPropertyHue::IsSupported() const
    {
        return IsPropertySupportedInternal();
    }

    std::pair<long, long> WinCameraPropertyHue::GetRange() const
    {
        return GetRangeInternal();
    }

    long WinCameraPropertyHue::GetStep() const
    {
        return GetStepInternal();
    }

    long WinCameraPropertyHue::GetValue() const
    {
        return GetValueInternal();
    }

    void WinCameraPropertyHue::SetValue(const long degree)
    {
        // Make sure the degree is in the range of -180 to 180
        const auto degreeIn180 = Utils::MathUtils::ConfirmDegreeIn180Range(degree);

        // Set value
        SetValueInternal(degreeIn180);
    }
}
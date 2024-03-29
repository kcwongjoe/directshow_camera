#include "win_camera/properties/win_camera_property_roll.h"

#include "win_camera/win_camera.h"

#include "utils/math_utils.h"

namespace WinCamera
{
    WinCameraPropertyRoll::WinCameraPropertyRoll(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyRoll::GetDirectShowProperty() const
    {
        return m_camera->getProperties()->getRoll();
    }

    bool WinCameraPropertyRoll::IsSupported() const
    {
        return IsPropertySupportedInternal();
    }

    std::pair<long, long> WinCameraPropertyRoll::GetRange() const
    {
        return GetRangeInternal();
    }

    long WinCameraPropertyRoll::GetStep() const
    {
        return GetStepInternal();
    }

    long WinCameraPropertyRoll::GetValue() const
    {
        return GetValueInternal();
    }

    void WinCameraPropertyRoll::SetValue(const long degree)
    {
        // Make sure the degree is in the range of -180 to 180
        const auto degreeIn180 = Utils::MathUtils::ConfirmDegreeIn180Range(degree);

        // Set value
        SetValueInternal(degreeIn180);
    }
}
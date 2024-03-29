#include "win_camera/properties/win_camera_property_pan.h"

#include "win_camera/win_camera.h"

#include "utils/math_utils.h"

namespace WinCamera
{
    WinCameraPropertyPan::WinCameraPropertyPan(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyPan::GetDirectShowProperty() const
    {
        return m_camera->getProperties()->getPan();
    }

    bool WinCameraPropertyPan::IsSupported() const
    {
        return IsPropertySupportedInternal();
    }

    std::pair<long, long> WinCameraPropertyPan::GetRange() const
    {
        return GetRangeInternal();
    }

    long WinCameraPropertyPan::GetStep() const
    {
        return GetStepInternal();
    }

    long WinCameraPropertyPan::GetValue() const
    {
        return GetValueInternal();
    }

    void WinCameraPropertyPan::SetValue(const long degree)
    {
        // Make sure the degree is in the range of -180 to 180
        const auto degreeIn180 = Utils::MathUtils::ConfirmDegreeIn180Range(degree);

        // Set value
        SetValueInternal(degree);
    }
}
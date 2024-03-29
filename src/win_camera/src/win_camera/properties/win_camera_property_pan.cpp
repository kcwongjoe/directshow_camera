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

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyPan::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getPan();
    }

    bool WinCameraPropertyPan::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    std::pair<long, long> WinCameraPropertyPan::getRange() const
    {
        return getRangeInternal();
    }

    long WinCameraPropertyPan::getStep() const
    {
        return getStepInternal();
    }

    long WinCameraPropertyPan::getValue() const
    {
        return getValueInternal();
    }

    void WinCameraPropertyPan::setValue(const long degree)
    {
        // Make sure the degree is in the range of -180 to 180
        const auto degreeIn180 = Utils::MathUtils::ConfirmDegreeIn180Range(degree);

        // Set value
        setValueInternal(degree);
    }
}
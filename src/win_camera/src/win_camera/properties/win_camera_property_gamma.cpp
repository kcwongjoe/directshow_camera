#include "win_camera/properties/win_camera_property_gamma.h"

#include "win_camera/win_camera.h"

namespace WinCamera
{
    WinCameraPropertyGamma::WinCameraPropertyGamma(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyGamma::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getGamma();
    }

    bool WinCameraPropertyGamma::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    std::pair<long, long> WinCameraPropertyGamma::getRange() const
    {
        return getRangeInternal();
    }

    long WinCameraPropertyGamma::getStep() const
    {
        return getStepInternal();
    }

    long WinCameraPropertyGamma::getValue() const
    {
        return getValueInternal();
    }

    void WinCameraPropertyGamma::setValue(const long value)
    {
        setValueInternal(value);
    }
}
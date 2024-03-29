#include "win_camera/properties/win_camera_property_gain.h"

#include "win_camera/win_camera.h"

namespace WinCamera
{
    WinCameraPropertyGain::WinCameraPropertyGain(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyGain::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getGain();
    }

    bool WinCameraPropertyGain::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    std::pair<long, long> WinCameraPropertyGain::getRange() const
    {
        return getRangeInternal();
    }

    long WinCameraPropertyGain::getStep() const
    {
        return getStepInternal();
    }

    long WinCameraPropertyGain::getValue() const
    {
        return getValueInternal();
    }

    void WinCameraPropertyGain::setValue(const long value)
    {
        setValueInternal(value);
    }
}
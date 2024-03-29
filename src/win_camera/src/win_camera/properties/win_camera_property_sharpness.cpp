#include "win_camera/properties/win_camera_property_sharpness.h"

#include "win_camera/win_camera.h"

namespace WinCamera
{
    WinCameraPropertySharpness::WinCameraPropertySharpness(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertySharpness::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getSharpness();
    }

    bool WinCameraPropertySharpness::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    std::pair<long, long> WinCameraPropertySharpness::getRange() const
    {
        return getRangeInternal();
    }

    long WinCameraPropertySharpness::getStep() const
    {
        return getStepInternal();
    }

    long WinCameraPropertySharpness::getValue() const
    {
        return getValueInternal();
    }

    void WinCameraPropertySharpness::setValue(const long value)
    {
        setValueInternal(value);
    }
}
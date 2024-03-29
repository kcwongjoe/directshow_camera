#include "win_camera/properties/win_camera_property_contrast.h"

#include "win_camera/win_camera.h"

namespace WinCamera
{
    WinCameraPropertyContrast::WinCameraPropertyContrast(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyContrast::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getContrast();
    }

    bool WinCameraPropertyContrast::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    std::pair<long, long> WinCameraPropertyContrast::getRange() const
    {
        return getRangeInternal();
    }

    long WinCameraPropertyContrast::getStep() const
    {
        return getStepInternal();
    }

    long WinCameraPropertyContrast::getValue() const
    {
        return getValueInternal();
    }

    void WinCameraPropertyContrast::setValue(const long value)
    {
        setValueInternal(value);
    }
}
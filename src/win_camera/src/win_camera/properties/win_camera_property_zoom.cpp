#include "win_camera/properties/win_camera_property_zoom.h"

#include "win_camera/win_camera.h"

namespace WinCamera
{
    WinCameraPropertyZoom::WinCameraPropertyZoom(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyZoom::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getZoom();
    }

    bool WinCameraPropertyZoom::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    std::pair<long, long> WinCameraPropertyZoom::getRange() const
    {
        return getRangeInternal();
    }

    long WinCameraPropertyZoom::getStep() const
    {
        return getStepInternal();
    }

    long WinCameraPropertyZoom::getValue() const
    {
        return getValueInternal();
    }

    void WinCameraPropertyZoom::setValue(const long millimeter)
    {
        setValueInternal(millimeter);
    }
}
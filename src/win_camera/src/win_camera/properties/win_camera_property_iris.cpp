#include "win_camera/properties/win_camera_property_iris.h"

#include "win_camera/win_camera.h"

namespace WinCamera
{
    WinCameraPropertyIris::WinCameraPropertyIris(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyIris::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getIris();
    }

    bool WinCameraPropertyIris::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    std::pair<long, long> WinCameraPropertyIris::getRange() const
    {
        return getRangeInternal();
    }

    long WinCameraPropertyIris::getStep() const
    {
        return getStepInternal();
    }

    long WinCameraPropertyIris::getValue() const
    {
        return getValueInternal();
    }

    void WinCameraPropertyIris::setValue(const long value)
    {
        setValueInternal(value);
    }

    bool WinCameraPropertyIris::isAuto() const
    {
        return isAutoInternal();
    }

    void WinCameraPropertyIris::setAuto(const bool setToAuto)
    {
        // Check mode support
        CheckModeSupport(setToAuto ? Mode::Auto : Mode::Manual);

        // Set value
        setValueInternal(getValueInternal(), setToAuto);
    }
}
#include "win_camera/properties/win_camera_property_focus.h"

#include "win_camera/win_camera.h"

namespace WinCamera
{
    WinCameraPropertyFocus::WinCameraPropertyFocus(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyFocus::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getFocus();
    }

    bool WinCameraPropertyFocus::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    std::pair<long, long> WinCameraPropertyFocus::getRange() const
    {
        return getRangeInternal();
    }

    long WinCameraPropertyFocus::getStep() const
    {
        return getStepInternal();
    }

    long WinCameraPropertyFocus::getValue() const
    {
        return getValueInternal();
    }

    void WinCameraPropertyFocus::setValue(const long millimeter)
    {
        setValueInternal(millimeter);
    }

    bool WinCameraPropertyFocus::isAuto() const
    {
        return isAutoInternal();
    }

    void WinCameraPropertyFocus::setAuto(const bool setToAuto)
    {
        // Check mode support
        CheckModeSupport(setToAuto ? Mode::Auto : Mode::Manual);

        // Set value
        setValueInternal(getValueInternal(), setToAuto);
    }
}
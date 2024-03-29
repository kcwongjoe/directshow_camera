#include "win_camera/properties/win_camera_property_white_balance.h"

#include "win_camera/win_camera.h"

namespace WinCamera
{
    WinCameraPropertyWhiteBalance::WinCameraPropertyWhiteBalance(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyWhiteBalance::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getWhiteBalance();
    }

    bool WinCameraPropertyWhiteBalance::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    std::pair<long, long> WinCameraPropertyWhiteBalance::getRange() const
    {
        return getRangeInternal();
    }

    long WinCameraPropertyWhiteBalance::getStep() const
    {
        return getStepInternal();
    }

    long WinCameraPropertyWhiteBalance::getValue() const
    {
        return getValueInternal();
    }

    void WinCameraPropertyWhiteBalance::setValue(const long kelvin)
    {
        setValueInternal(kelvin);
    }

    bool WinCameraPropertyWhiteBalance::isAuto() const
    {
        return isAutoInternal();
    }

    void WinCameraPropertyWhiteBalance::setAuto(const bool setToAuto)
    {
        // Check mode support
        CheckModeSupport(setToAuto ? Mode::Auto : Mode::Manual);

        // Set value
        setValueInternal(getValueInternal(), setToAuto);
    }
}
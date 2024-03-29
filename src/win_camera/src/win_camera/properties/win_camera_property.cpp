#include "win_camera/properties/win_camera_property.h"

#include "win_camera/win_camera.h"

#include "exceptions/camera_not_opened_exception.h"
#include "exceptions/property_not_support_exception.h"

namespace WinCamera
{
    WinCameraProperty::WinCameraProperty(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) :
        m_camera(ds_camera),
        m_isCameraOpenedFunc(
            [this, &camera]() -> bool {return camera.isOpened();}
        )
    {
        
    }

    bool WinCameraProperty::isPropertySupportedInternal() const
    {
        // Throw if camera is not opened
        CheckOpening();

        // Get property support
        return getDirectShowProperty()->isSupported();
    }

    std::pair<long, long> WinCameraProperty::getRangeInternal() const
    {
        // Throw if camera is not opened or property is not supported
        CheckOpeningAndSupported();

        return getDirectShowProperty()->getRange();
    }

    long WinCameraProperty::getStepInternal() const
    {
        // Throw if camera is not opened or property is not supported
        CheckOpeningAndSupported();

        return getDirectShowProperty()->getStep();
    }

    long WinCameraProperty::getValueInternal() const
    {
        // Throw if camera is not opened or property is not supported
        CheckOpeningAndSupported();

        return getDirectShowProperty()->getValue();
    }

    void WinCameraProperty::setValueInternal(const long value, const bool isAuto)
    {
        // Throw if camera is not opened or property is not supported
        CheckOpeningAndSupported();

        // Set value
        const bool result = m_camera->setValue(getDirectShowProperty(), value, isAuto);

        // Throw if error
        if (!result)
        {
            // Do something with error message, throw it, etc.
            const auto error_message = m_camera->getLastError();
        }
    }

    bool WinCameraProperty::isAutoInternal() const
    {
        // Throw if camera is not opened or property is not supported
        CheckOpeningAndSupported();

        return getDirectShowProperty()->isAuto();
    }

    bool WinCameraProperty::SupportAutoModeInternal() const
    {
        // Throw if camera is not opened or property is not supported
        CheckOpeningAndSupported();

        return getDirectShowProperty()->supportAutoMode();
    }

    bool WinCameraProperty::ySupportManualModeInternal() const
    {
        // Throw if camera is not opened or property is not supported
        CheckOpeningAndSupported();

        return getDirectShowProperty()->supportManualMode();
    }

    void WinCameraProperty::CheckOpening() const
    {
        // Throw if camera is not opened
        if (!m_isCameraOpenedFunc()) throw CameraNotOpenedException();
    }

    void WinCameraProperty::CheckOpeningAndSupported() const
    {
        // Throw if camera is not opened
        if (!m_isCameraOpenedFunc()) throw CameraNotOpenedException();

        // Throw if property is not support
        if (!getDirectShowProperty()->isSupported())
        {
            throw PropertyNotSupportException(getDirectShowProperty()->getName());
        }
    }
    void WinCameraProperty::CheckModeSupport(Mode mode) const
    {
        // Throw if property does not support auto mode
        if (mode == Mode::Auto && !SupportAutoModeInternal())
        {
            throw PropertyNotSupportException("Auto " + getDirectShowProperty()->getName());
        }

        // Throw if property does not support manual mode
        if (mode == Mode::Manual && !ySupportManualModeInternal())
        {
            throw PropertyNotSupportException("Manual " + getDirectShowProperty()->getName());
        }
    }
}
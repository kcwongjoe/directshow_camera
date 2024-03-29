#include "win_camera/properties/win_camera_property.h"

#include "win_camera/win_camera.h"

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

    bool WinCameraProperty::IsPropertySupportedInternal() const
    {
        // Throw if camera is not opened
        CheckOpening();

        // Get property support
        return GetDirectShowProperty()->isSupported();
    }

    std::pair<long, long> WinCameraProperty::GetRangeInternal() const
    {
        // Throw if camera is not opened or property is not supported
        CheckOpeningAndSupported();

        return GetDirectShowProperty()->getRange();
    }

    long WinCameraProperty::GetStepInternal() const
    {
        // Throw if camera is not opened or property is not supported
        CheckOpeningAndSupported();

        return GetDirectShowProperty()->getStep();
    }

    long WinCameraProperty::GetValueInternal() const
    {
        // Throw if camera is not opened or property is not supported
        CheckOpeningAndSupported();

        return GetDirectShowProperty()->getValue();
    }

    void WinCameraProperty::SetValueInternal(const long value, const bool isAuto)
    {
        // Throw if camera is not opened or property is not supported
        CheckOpeningAndSupported();

        // Set value
        const bool result = m_camera->setValue(GetDirectShowProperty(), value, isAuto);

        // Throw if error
        if (!result)
        {
            // Do something with error message, throw it, etc.
            const auto error_message = m_camera->getLastError();
        }
    }

    bool WinCameraProperty::IsAutoInternal() const
    {
        // Throw if camera is not opened or property is not supported
        CheckOpeningAndSupported();

        return GetDirectShowProperty()->isAuto();
    }

    bool WinCameraProperty::SupportAutoModeInternal() const
    {
        // Throw if camera is not opened or property is not supported
        CheckOpeningAndSupported();

        return GetDirectShowProperty()->supportAutoMode();
    }

    bool WinCameraProperty::ySupportManualModeInternal() const
    {
        // Throw if camera is not opened or property is not supported
        CheckOpeningAndSupported();

        return GetDirectShowProperty()->supportManualMode();
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
        if (!GetDirectShowProperty()->isSupported())
        {
            throw PropertyNotSupportException(GetDirectShowProperty()->getName());
        }
    }
    void WinCameraProperty::CheckModeSupport(Mode mode) const
    {
        // Throw if property does not support auto mode
        if (mode == Mode::Auto && !SupportAutoModeInternal())
        {
            throw PropertyNotSupportException("Auto " + GetDirectShowProperty()->getName());
        }

        // Throw if property does not support manual mode
        if (mode == Mode::Manual && !ySupportManualModeInternal())
        {
            throw PropertyNotSupportException("Manual " + GetDirectShowProperty()->getName());
        }
    }
}
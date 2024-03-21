#include "win_camera/properties/win_camera_property.h"

#include "win_camera/win_camera.h"

namespace WinCamera
{
    WinCameraProperty::WinCameraProperty(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::DirectShowCamera>& ds_camera,
        const std::shared_ptr<DirectShowCamera::DirectShowCameraProperty>& property
    ) :
        m_property(property),
        m_camera(ds_camera),
        m_isCameraOpenedFunc(
            [this, &camera]() -> bool {return camera.isOpened();}
        )
    {
        
    }

    bool WinCameraProperty::isPropertySupportedInternal() const
    {
        // Throw if camera is not opened
        checkOpening();

        // Get property support
        return m_property->isSupported();
    }

    std::pair<long, long> WinCameraProperty::getRangeInternal() const
    {
        // Throw if camera is not opened or property is not supported
        checkOpeningAndSupported();

        return m_property->getRange();
    }

    long WinCameraProperty::getStepInternal() const
    {
        // Throw if camera is not opened or property is not supported
        checkOpeningAndSupported();

        return m_property->getStep();
    }

    long WinCameraProperty::getValueInternal() const
    {
        // Throw if camera is not opened or property is not supported
        checkOpeningAndSupported();

        return m_property->getValue();
    }

    void WinCameraProperty::setValueInternal(const long value)
    {
        // Throw if camera is not opened or property is not supported
        checkOpeningAndSupported();

        // Set value
        const bool result = m_camera->setValue(m_property, value, false);

        // Throw if error
        if (!result)
        {
            // Do something with error message, throw it, etc.
            const auto error_message = m_camera->getLastError();
        }
    }

    void WinCameraProperty::checkOpening() const
    {
        // Throw if camera is not opened
        if (!m_isCameraOpenedFunc()) throw CameraNotOpenedException();
    }

    void WinCameraProperty::checkOpeningAndSupported() const
    {
        // Throw if camera is not opened
        if (!m_isCameraOpenedFunc()) throw CameraNotOpenedException();

        // Throw if property is not support
        if (!m_property->isSupported())
        {
            throw PropertyNotSupportException(m_property->getName());
        }
    }
}
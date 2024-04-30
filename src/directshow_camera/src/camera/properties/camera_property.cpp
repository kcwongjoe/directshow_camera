/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "camera/properties/camera_property.h"

#include "camera/camera.h"

#include "exceptions/camera_not_opened_exception.h"
#include "exceptions/property_not_support_exception.h"

namespace DirectShowCamera
{
    CameraProperty::CameraProperty(
        const Camera& camera,
        const std::shared_ptr<AbstractDirectShowCamera>& ds_camera
    ) :
        m_camera(ds_camera),
        m_isCameraOpenedFunc(
            [this, &camera]() -> bool {return camera.isOpened();}
        )
    {
        
    }

    bool CameraProperty::isPropertySupportedInternal() const
    {
        // Throw if camera is not opened
        CheckOpening();

        // Get property support
        return getDirectShowProperty()->isSupported();
    }

    std::pair<long, long> CameraProperty::getRangeInternal() const
    {
        // Throw if camera is not opened or property is not supported
        CheckOpeningAndSupported();

        return getDirectShowProperty()->getRange();
    }

    long CameraProperty::getStepInternal() const
    {
        // Throw if camera is not opened or property is not supported
        CheckOpeningAndSupported();

        return getDirectShowProperty()->getStep();
    }

    long CameraProperty::getValueInternal() const
    {
        // Throw if camera is not opened or property is not supported
        CheckOpeningAndSupported();

        return getDirectShowProperty()->getValue();
    }

    void CameraProperty::setValueInternal(const long value, const bool isAuto)
    {
        // Throw if camera is not opened or property is not supported
        CheckOpeningAndSupported();

        // Set value
        auto property = getDirectShowProperty();
        auto result = m_camera->setPropertyValue(property, value, isAuto);

        // Throw if error
        if (!result)
        {
            // Do something with error message, throw it, etc.
            const auto error_message = m_camera->getLastError();
        }
    }

    bool CameraProperty::isAutoInternal() const
    {
        // Throw if camera is not opened or property is not supported
        CheckOpeningAndSupported();

        return getDirectShowProperty()->isAuto();
    }

    bool CameraProperty::SupportAutoModeInternal() const
    {
        // Throw if camera is not opened or property is not supported
        CheckOpeningAndSupported();

        return getDirectShowProperty()->isSupportAutoMode();
    }

    bool CameraProperty::ySupportManualModeInternal() const
    {
        // Throw if camera is not opened or property is not supported
        CheckOpeningAndSupported();

        return getDirectShowProperty()->isSupportManualMode();
    }

    void CameraProperty::CheckOpening() const
    {
        // Throw if camera is not opened
        if (!m_isCameraOpenedFunc()) throw CameraNotOpenedException();
    }

    void CameraProperty::CheckOpeningAndSupported() const
    {
        // Throw if camera is not opened
        if (!m_isCameraOpenedFunc()) throw CameraNotOpenedException();

        // Throw if property is not support
        if (!getDirectShowProperty()->isSupported())
        {
            throw PropertyNotSupportException(getDirectShowProperty()->getName());
        }
    }
    void CameraProperty::CheckModeSupport(Mode mode) const
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
/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "exceptions/property_not_support_exception.h"

namespace DirectShowCamera
{
    PropertyNotSupportException::PropertyNotSupportException(const std::string& propertyName) : 
        m_propertyName(propertyName), 
        m_errorMessage("Property(" + propertyName + ") is not supported.")
    {

    }

    std::string PropertyNotSupportException::getPropertyName() const
    {
        return m_propertyName;
    }

    std::string PropertyNotSupportException::getErrorMessage() const
    {
        return m_errorMessage;
    }

    const char* PropertyNotSupportException::what() const noexcept
    {
        return m_errorMessage.c_str();
    }
}
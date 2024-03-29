#include "exceptions/property_not_support_exception.h"

namespace WinCamera
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
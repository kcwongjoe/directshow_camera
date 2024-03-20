#include "exceptions/property_not_support_exception.h"

namespace DirectShowCamera
{
    PropertyNotSupportException::PropertyNotSupportException(const std::string& propertyName) : 
        m_propertyName(propertyName), 
        m_errorMessage("Property(" + propertyName + ") is not supported.")
    {

    }

    std::string PropertyNotSupportException::GetPropertyName() const
    {
        return m_propertyName;
    }

    std::string PropertyNotSupportException::GetErrorMessage() const
    {
        return m_errorMessage;
    }

    const char* PropertyNotSupportException::what() const noexcept
    {
        return m_errorMessage.c_str();
    }
}
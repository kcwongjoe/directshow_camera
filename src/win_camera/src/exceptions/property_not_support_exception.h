#pragma once
#ifndef PROPERTY_NOT_SUPPORT_EXCEPTION_H
#define PROPERTY_NOT_SUPPORT_EXCEPTION_H

#pragma once

#include <exception>

namespace WinCamera
{
    /**
     * @class PropertyNotSupportException
     * @brief PropertyNotSupportException indicates an error that a property is not supported by the camera
    */
    class PropertyNotSupportException : public std::exception
    {
    public:

        /**
         * @brief Constructor
         * @param propertyName The name of the property that is not supported
        */
        PropertyNotSupportException(const std::string& propertyName);

        /**
         * @brief Get the name of the property that is not supported
         * @return The name of the property that is not supported
        */
        std::string getPropertyName() const;

        /**
         * @brief Get the error message
        */
        std::string getErrorMessage() const;

        const char* what() const noexcept override;
    private:
        std::string m_propertyName;
        std::string m_errorMessage;
    };
}


//*******************************

#endif
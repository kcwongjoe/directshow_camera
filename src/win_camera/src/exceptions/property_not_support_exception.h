/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#pragma once
#ifndef WIN_CAMERA__EXCEPTIONS__PROPERTY_NOT_SUPPORT_EXCEPTION_H
#define WIN_CAMERA__EXCEPTIONS__PROPERTY_NOT_SUPPORT_EXCEPTION_H

//************Content************

#include <exception>
#include <string>

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
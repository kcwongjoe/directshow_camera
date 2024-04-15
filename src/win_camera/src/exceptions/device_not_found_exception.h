/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#pragma once
#ifndef WIN_CAMERA__EXCEPTIONS__DEVICE_NOT_FOUND_EXCEPTION_H
#define WIN_CAMERA__EXCEPTIONS__DEVICE_NOT_FOUND_EXCEPTION_H

//************Content************

#include <exception>
#include <string>

namespace WinCamera
{
    /**
     * @class DeviceNotFoundException
     * @brief DeviceNotFoundException indicates an error that the resolution is not supported by the camera
    */
    class DeviceNotFoundException : public std::exception
    {
    public:

        /**
         * @brief Constructor
         * @param m_deviceName The device name
        */
        DeviceNotFoundException(
            const std::string& deviceName
        );

        /**
         * @brief Get the error message
        */
        std::string getErrorMessage() const;

        const char* what() const noexcept override;
    private:
        std::string m_deviceName;
        std::string m_errorMessage;
    };
}


//*******************************

#endif
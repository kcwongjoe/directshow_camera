/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "exceptions/device_not_found_exception.h"

namespace DirectShowCamera
{
    DeviceNotFoundException::DeviceNotFoundException(
        const std::string& deviceName
    ) :
        m_deviceName(deviceName),
        m_errorMessage("Device not found: " + deviceName)
    {
    }

    std::string DeviceNotFoundException::getErrorMessage() const
    {
        return m_errorMessage;
    }

    const char* DeviceNotFoundException::what() const noexcept
    {
        return m_errorMessage.c_str();
    }
}
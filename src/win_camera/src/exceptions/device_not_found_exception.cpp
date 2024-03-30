#include "exceptions/device_not_found_exception.h"

namespace WinCamera
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
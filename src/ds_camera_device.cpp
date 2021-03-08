#include "ds_camera_device.h"

DirectShowCameraDevice::DirectShowCameraDevice(std::string friendlyName, std::string description, std::string devicePath, std::vector<DirectShowVideoFormat> videoFormats)
{
    m_videoFormats = videoFormats;
    m_friendlyName = friendlyName;
    m_description = description;
    m_devicePath = devicePath;    
}

#pragma region Getter

std::vector<DirectShowVideoFormat> DirectShowCameraDevice::getDirectShowVideoFormats()
{
    return m_videoFormats;
}

std::string DirectShowCameraDevice::getFriendlyName()
{
    return this->m_friendlyName;
}

std::string DirectShowCameraDevice::getDescription()
{
    return this->m_description;
}

std::string DirectShowCameraDevice::getDevicePath()
{
    return this->m_devicePath;
}

#pragma endregion Getter and Setter


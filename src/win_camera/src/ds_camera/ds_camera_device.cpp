#include "ds_camera/ds_camera_device.h"

namespace DirectShowCamera
{

    /**
     * @brief Constructor
     * 
     * @param friendlyName Friendly Name
     * @param description Description
     * @param devicePath Device Path
     * @param videoFormats Video Formats
     */
    DirectShowCameraDevice::DirectShowCameraDevice(std::string friendlyName, std::string description, std::string devicePath, std::vector<DirectShowVideoFormat> videoFormats)
    {
        m_videoFormats = videoFormats;
        m_friendlyName = friendlyName;
        m_description = description;
        m_devicePath = devicePath;    
    }

#pragma region Getter

    /**
     * @brief Get the supported DirectShowVideoFormats
     * 
     * @return Return the the supported DirectShowVideoFormats 
     */
    std::vector<DirectShowVideoFormat> DirectShowCameraDevice::getDirectShowVideoFormats() const
    {
        return m_videoFormats;
    }

    /**
     * @brief Get the camera friendly name
     * 
     * @return Return the camera friendly name
     */
    std::string DirectShowCameraDevice::getFriendlyName() const
    {
        return this->m_friendlyName;
    }

    /**
     * @brief Get the camera description
     * 
     * @return Return the camera description
     */
    std::string DirectShowCameraDevice::getDescription() const
    {
        return this->m_description;
    }

    /**
     * @brief Get the device path
     * 
     * @return Return the device path
     */
    std::string DirectShowCameraDevice::getDevicePath() const
    {
        return this->m_devicePath;
    }

#pragma endregion Getter and Setter

}
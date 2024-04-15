/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#include "directshow_camera/device/ds_camera_device.h"

namespace DirectShowCamera
{

#pragma region Constructor and Destructor

    DirectShowCameraDevice::DirectShowCameraDevice(
        const std::string friendlyName,
        const std::string description,
        const std::string devicePath,
        const std::vector<DirectShowVideoFormat> videoFormats
    )
    {
        m_videoFormats = videoFormats;
        m_friendlyName = friendlyName;
        m_description = description;
        m_devicePath = devicePath;    
    }

#pragma endregion Constructor and Destructor

#pragma region Getter

    std::vector<DirectShowVideoFormat> DirectShowCameraDevice::getDirectShowVideoFormats() const
    {
        return m_videoFormats;
    }

    std::string DirectShowCameraDevice::getFriendlyName() const
    {
        return m_friendlyName;
    }

    std::string DirectShowCameraDevice::getDescription() const
    {
        return m_description;
    }

    std::string DirectShowCameraDevice::getDevicePath() const
    {
        return m_devicePath;
    }

#pragma endregion Getter

}
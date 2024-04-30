/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "camera/camera_device.h"

#include <algorithm>

namespace DirectShowCamera
{

#pragma region constructor and destructor

    CameraDevice::CameraDevice(const DirectShowCameraDevice& directShowCamera)
    {
        m_friendlyName = directShowCamera.getFriendlyName();
        m_description = directShowCamera.getDescription();
        m_devicePath = directShowCamera.getDevicePath();

        // Get frame size
        std::vector<DirectShowVideoFormat> videoForamts = directShowCamera.getDirectShowVideoFormats();
        for (int i = 0; i < videoForamts.size(); i++)
        {
            int width = videoForamts[i].getWidth();
            int height = videoForamts[i].getHeight();
            GUID subType = videoForamts[i].getVideoType();

            if (width != 0 && height != 0)
            {
                if (DirectShowVideoFormatUtils::isSupportRGBConvertion(subType))
                {
                    // RGB

                    // Check existed
                    bool found = ContainRGBResolution(width, height);

                    // Add if not existed
                    if (!found)
                    {
                        m_rgbResolutions.push_back(std::pair<int, int>(width, height));
                    }
                }
                else if (DirectShowVideoFormatUtils::isMonochrome(subType))
                {
                    // Monochrome 

                    // Check existed
                    bool found = ContainMonochromeResolution(width, height);

                    // Add if not existed
                    if (!found)
                    {
                        m_monoResolutions.push_back(std::pair<int, int>(width, height));
                    }
                }
            }
        }

        // Sort
        if (m_monoResolutions.size() > 0) std::sort(m_monoResolutions.begin(), m_monoResolutions.end());
        if (m_rgbResolutions.size() > 0) std::sort(m_rgbResolutions.begin(), m_rgbResolutions.end());
    }

    CameraDevice::CameraDevice(const CameraDevice& cameraDevice)
    {
        *this = cameraDevice;
    }

#pragma endregion constructor and destructor

#pragma region Monochrome

    bool CameraDevice::isSupportMonochrome() const
    {
        return m_monoResolutions.size() > 0 ? true : false;
    }

    std::vector<std::pair<int, int>> CameraDevice::getMonoResolutions() const
    {
        return m_monoResolutions;
    }

    bool CameraDevice::ContainMonochromeResolution(const int width, const int height) const
    {
        for (int i = 0; i < m_monoResolutions.size(); i++)
        {
            if (m_monoResolutions[i].first == width && m_monoResolutions[i].second == height)
            {
                return true;
            }
        }
        return false;
    }

#pragma endregion Monochrome

#pragma region RGB

    bool CameraDevice::isSupportRGB() const
    {
        return m_rgbResolutions.size() > 0 ? true : false;
    }

    std::vector<std::pair<int, int>> CameraDevice::getRGBResolutions() const
    {
        return m_rgbResolutions;
    }

    bool CameraDevice::ContainRGBResolution(const int width, const int height) const
    {
        for (int i = 0; i < m_rgbResolutions.size(); i++)
        {
            if (m_rgbResolutions[i].first == width && m_rgbResolutions[i].second == height)
            {
                return true;
            }
        }

        return false;
    }

#pragma endregion RGB

#pragma region Resolution

    std::vector<std::pair<int,int>> CameraDevice::getResolutions() const
    {
        if (m_rgbResolutions.size() > 0)
        {
            return m_rgbResolutions;
        }
        else
        {
            return m_monoResolutions;
        }
    }

    bool CameraDevice::ContainResolution(const int width, const int height) const
    {
        const auto foundInMonochrome = ContainMonochromeResolution(width, height);
        const auto foundInRGB = ContainRGBResolution(width, height);

        return foundInMonochrome || foundInRGB;
    }

#pragma endregion Resolution

#pragma region Camera Getter

    std::string CameraDevice::getFriendlyName() const
    {
        return m_friendlyName;
    }

    std::string CameraDevice::getDescription() const
    {
        return m_description;
    }

    std::string CameraDevice::getDevicePath() const
    {
        return m_devicePath;
    }

#pragma endregion Camera Getter

    CameraDevice& CameraDevice::operator=(const CameraDevice& cameraDevice)
    {
        if (this != &cameraDevice)
        {
            m_friendlyName = cameraDevice.m_friendlyName;
            m_description = cameraDevice.m_description;
            m_devicePath = cameraDevice.m_devicePath;

            m_monoResolutions.clear();
            for (int i = 0; i < cameraDevice.m_monoResolutions.size(); i++)
            {
                m_monoResolutions.push_back(std::pair<int, int>(cameraDevice.m_monoResolutions[i].first, cameraDevice.m_monoResolutions[i].second));
            }

            m_rgbResolutions.clear();
            for (int i = 0; i < cameraDevice.m_rgbResolutions.size(); i++)
            {
                m_rgbResolutions.push_back(std::pair<int, int>(cameraDevice.m_rgbResolutions[i].first, cameraDevice.m_rgbResolutions[i].second));
            }
        }

        return *this;
    }

}
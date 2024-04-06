#include "win_camera/win_camera_device.h"

#include <algorithm>

namespace WinCamera
{

#pragma region constructor and destructor

    WinCameraDevice::WinCameraDevice(const DirectShowCamera::DirectShowCameraDevice& directShowCamera)
    {
        m_friendlyName = directShowCamera.getFriendlyName();
        m_description = directShowCamera.getDescription();
        m_devicePath = directShowCamera.getDevicePath();

        // Get frame size
        std::vector<DirectShowCamera::DirectShowVideoFormat> videoForamts = directShowCamera.getDirectShowVideoFormats();
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
                    bool found = false;
                    for (int j = 0; j < m_rgbResolutions.size(); j++) {
                        if (m_rgbResolutions[j].first == width && m_rgbResolutions[j].second == height)
                        {
                            found = true;
                            break;
                        }
                    }

                    // Add
                    if (!found)
                    {
                        m_rgbResolutions.push_back(std::pair<int, int>(width, height));
                    }
                }
                else if (DirectShowVideoFormatUtils::isMonochrome(subType))
                {
                    // Monochrome 

                    // Check existed
                    bool found = false;
                    for (int j = 0; j < m_monoResolutions.size(); j++) {
                        if (m_monoResolutions[j].first == width && m_monoResolutions[j].second == height)
                        {
                            found = true;
                            break;
                        }
                    }

                    // Add
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

    WinCameraDevice::WinCameraDevice(const WinCameraDevice& cameraDevice)
    {
        *this = cameraDevice;
    }

#pragma endregion constructor and destructor

#pragma region Monochrome

    bool WinCameraDevice::isSupportMonochrome() const
    {
        return m_monoResolutions.size() > 0 ? true : false;
    }

    std::vector<std::pair<int, int>> WinCameraDevice::getMonoResolutions() const
    {
        return m_monoResolutions;
    }

#pragma endregion Monochrome

#pragma region RGB

    bool WinCameraDevice::isSupportRGB() const
    {
        return m_rgbResolutions.size() > 0 ? true : false;
    }

    std::vector<std::pair<int, int>> WinCameraDevice::getRGBResolutions() const
    {
        return m_rgbResolutions;
    }

#pragma endregion RGB

#pragma region Resolution

    std::vector<std::pair<int,int>> WinCameraDevice::getResolutions() const
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

    bool WinCameraDevice::ContainResolution(const int width, const int height) const
    {
        for (int i=0;i< m_monoResolutions.size();i++)
        {
            if (m_monoResolutions[i].first == width && m_monoResolutions[i].second == height)
            {
                return true;
            }
        }

        for (int i = 0; i < m_rgbResolutions.size(); i++)
        {
            if (m_rgbResolutions[i].first == width && m_rgbResolutions[i].second == height)
            {
                return true;
            }
        }

        return false;
    }

#pragma endregion Resolution

#pragma region Camera Getter

    std::string WinCameraDevice::getFriendlyName() const
    {
        return m_friendlyName;
    }

    std::string WinCameraDevice::getDescription() const
    {
        return m_description;
    }

    std::string WinCameraDevice::getDevicePath() const
    {
        return m_devicePath;
    }

#pragma endregion Camera Getter

    WinCameraDevice& WinCameraDevice::operator=(const WinCameraDevice& cameraDevice)
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
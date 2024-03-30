#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__WIN_CAMERA_DEVICE_H
#define WIN_CAMERA__WIN_CAMERA__WIN_CAMERA_DEVICE_H

//************Content************
#include <utility>

#include "directshow_camera/ds_camera_device.h"
#include "directshow_camera/ds_video_format.h"

namespace WinCamera
{
    /**
     * @brief A class to store WinCamera Device information
     */
    class WinCameraDevice
    {
    public:
        WinCameraDevice(DirectShowCamera::DirectShowCameraDevice* directShowCameraDevice);
        WinCameraDevice(const WinCameraDevice& cameraDevice);

        bool supportMonochrome() const;
        std::vector<std::pair<int, int>> getMonoResolutions();

        bool supportRGB() const;
        std::vector<std::pair<int,int>> getRGBResolutions();

        std::vector<std::pair<int,int>> getResolutions();
        bool ContainResolution(int width, int height) const;

        std::string getFriendlyName() const;
        std::string getDescription() const;
        std::string getDevicePath() const;

        // Operator

        WinCameraDevice& operator=(const WinCameraDevice& cameraDevice);

        // To string

        operator std::string() const
        {
            std::string result = "Friend Name: " + m_friendlyName + "\n";
            result += "Description: " + m_description + "\n";
            result += "Device Path: " + m_devicePath + "\n";

            // Monochrome
            if (m_monoResolutions.size() > 0)
            {
                result += "---Support Monochrome Resolutions---\n";
                for (int i = 0; i < m_monoResolutions.size(); i++) {
                    result += std::to_string(m_monoResolutions[i].first) + " x " + std::to_string(m_monoResolutions[i].second) + "\n";
                }
            }
            else
            {
                result += "Support Monochrome: False\n";
            }

            // RGB
            if (m_rgbResolutions.size() > 0)
            {
                result += "---Support RGB Resolutions---\n";
                for (int i = 0; i < m_rgbResolutions.size(); i++) {
                    result += std::to_string(m_rgbResolutions[i].first) + " x " + std::to_string(m_rgbResolutions[i].second) + "\n";
                }
            }
            else
            {
                result += "Support RGB: False\n";
            }

            return result;
        }

        friend std::ostream& operator << (std::ostream& out, const WinCameraDevice& obj) {
            return out << (std::string)obj;
        }

    private:
        std::vector<std::pair<int, int>> m_monoResolutions;
        std::vector<std::pair<int, int>> m_rgbResolutions;
        std::string m_friendlyName;
        std::string m_description;
        std::string m_devicePath;

    };
}

//*******************************

#endif
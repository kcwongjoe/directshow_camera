#pragma once
#ifndef UVC_CAMERA_DEVICE_H
#define UVC_CAMERA_DEVICE_H

//************Content************
#include <string>
#include <vector>
#include <utility>

#include <ds_camera_device.h>
#include <ds_video_format.h>

namespace DirectShowCamera
{
    class CameraDevice
    {

        private:
            std::vector<std::pair<int, int>> m_monoResolutions;
            std::vector<std::pair<int, int>> m_rgbResolutions;
            std::string m_friendlyName;
            std::string m_description;
            std::string m_devicePath;

        public:
            CameraDevice(DirectShowCameraDevice* directShowCameraDevice);
            CameraDevice(const CameraDevice& cameraDevice);

            bool supportMonochrome();
            std::vector<std::pair<int, int>> getMonoResolutions();

            bool supportRGB();
            std::vector<std::pair<int,int>> getRGBResolutions();

            std::vector<std::pair<int,int>> getResolutions();

            std::string getFriendlyName();
            std::string getDescription();
            std::string getDevicePath();

            // Operator

            CameraDevice& operator=(const CameraDevice& cameraDevice);

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

            friend std::ostream& operator << (std::ostream& out, const CameraDevice& obj) {
                return out << (std::string)obj;
            }
    };
}

//*******************************

#endif
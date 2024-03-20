#pragma once
#ifndef DS_CAMERA_DEVICE_H
#define DS_CAMERA_DEVICE_H

//************Content************
#include "ds_camera/ds_video_format.h"

#include <string>
#include <vector>

namespace DirectShowCamera
{
    /**
     * @brief A Class to store DirectShow Camera Device information
     */
    class DirectShowCameraDevice
    {
    public:
        DirectShowCameraDevice
        (
            std::string friendlyName,
            std::string description,
            std::string devicePath,
            std::vector<DirectShowVideoFormat> videoFormats
        );

        std::vector<DirectShowVideoFormat> getDirectShowVideoFormats() const;
        std::string getFriendlyName() const;
        std::string getDescription() const;
        std::string getDevicePath() const;

        // Operator

        bool operator == (const DirectShowCameraDevice& device) const
        {
            bool result = true;
            if (result) result = (m_friendlyName == device.m_friendlyName);
            if (result) result = (m_description == device.m_description);
            if (result) result = (m_devicePath == device.m_devicePath);
            if (result) result = (m_videoFormats.size() == device.m_videoFormats.size());
            if (result)
            {
                for (int i = 0; i < m_videoFormats.size();i++) {
                    if (m_videoFormats != device.m_videoFormats)
                    {
                        result = false;
                        break;
                    }
                }
            }

            return result;
        }

        // To string

        operator std::string() const
        {
            std::string result = "Friend Name: " + m_friendlyName + "\n";
            result += "Description: " + m_description + "\n";
            result += "Device Path: " + m_devicePath + "\n";


            if (m_videoFormats.size() > 0)
            {
                result += "---Support Video Formats---\n";
                for (int i = 0; i < m_videoFormats.size();i++) {
                    result += (std::string)m_videoFormats[i] + "\n";
                }
            }        

            return result;
        }

        friend std::ostream& operator << (std::ostream& out, const DirectShowCameraDevice& obj) {
            return out << (std::string)obj;
        }

    private:
        std::vector<DirectShowVideoFormat> m_videoFormats;
        std::string m_friendlyName;
        std::string m_description;
        std::string m_devicePath;
    };
}

//*******************************

#endif
#pragma once
#ifndef DS_CAMERA_DEVICE_H
#define DS_CAMERA_DEVICE_H

//************Content************
#include <ds_video_format.h>

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
            DirectShowCameraDevice(std::string friendlyName, std::string description, std::string devicePath, std::vector<DirectShowVideoFormat> videoFormats);

            std::vector<DirectShowVideoFormat> getDirectShowVideoFormats();
            std::string getFriendlyName();
            std::string getDescription();
            std::string getDevicePath();

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
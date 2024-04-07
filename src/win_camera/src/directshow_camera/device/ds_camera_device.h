#pragma once
#ifndef WIN_CAMERA__DIRECTSHOW_CAMERA__DEVICE__DS_CAMERA_DEVICE_H
#define WIN_CAMERA__DIRECTSHOW_CAMERA__DEVICE__DS_CAMERA_DEVICE_H

//************Content************
#include "directshow_camera/video_format/ds_video_format.h"

namespace DirectShowCamera
{
    /**
     * @brief A Class to store DirectShow Camera Device information
     */
    class DirectShowCameraDevice
    {
    public:

#pragma region Constructor and Destructor
        /**
         * @brief Constructor
         *
         * @param[in] friendlyName Friendly Name
         * @param[in] description Description
         * @param[in] devicePath Device Path
         * @param[in] videoFormats Video Formats
         */
        DirectShowCameraDevice
        (
            const std::string friendlyName,
            const std::string description,
            const std::string devicePath,
            const std::vector<DirectShowVideoFormat> videoFormats
        );

#pragma endregion Constructor and Destructor

#pragma region Getter

        /**
         * @brief Get the supported DirectShowVideoFormats
         *
         * @return Return the the supported DirectShowVideoFormats
         */
        std::vector<DirectShowVideoFormat> getDirectShowVideoFormats() const;

        /**
         * @brief Get the camera friendly name
         *
         * @return Return the camera friendly name
         */
        std::string getFriendlyName() const;

        /**
         * @brief Get the camera description
         *
         * @return Return the camera description
         */
        std::string getDescription() const;

        /**
         * @brief Get the device path
         *
         * @return Return the device path
         */
        std::string getDevicePath() const;

#pragma endregion Getter

#pragma region operator

        bool operator == (const DirectShowCameraDevice& device) const
        {
            if (m_friendlyName != device.m_friendlyName) return false;
            if (m_description != device.m_description) return false;
            if (m_devicePath != device.m_devicePath) return false;
            if (m_videoFormats.size() != device.m_videoFormats.size()) return false;

            for (int i = 0; i < m_videoFormats.size(); i++) {
                if (m_videoFormats[i] != device.m_videoFormats[i])
                {
                    return false;
                }
            }

            return true;
        }

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

#pragma endregion operator

    private:
        std::vector<DirectShowVideoFormat> m_videoFormats;
        std::string m_friendlyName;
        std::string m_description;
        std::string m_devicePath;
    };
}

//*******************************

#endif
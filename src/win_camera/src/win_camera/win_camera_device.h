#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__WIN_CAMERA_DEVICE_H
#define WIN_CAMERA__WIN_CAMERA__WIN_CAMERA_DEVICE_H

//************Content************
#include <utility>

#include "directshow_camera/device/ds_camera_device.h"
#include "directshow_camera/video_format/ds_video_format.h"

namespace WinCamera
{
    /**
     * @brief A class to store WinCamera Device information
     */
    class WinCameraDevice
    {
    public:

#pragma region constructor and destructor

        /**
         * @brief Constructor
         * @param[in] directShowCamera Directshow Camera
        */
        WinCameraDevice(const DirectShowCamera::DirectShowCameraDevice& directShowCameraDevice);

        /**
         * @brief Copy constructor
         * @param[in] cameraDevice CameraDevice
        */
        WinCameraDevice(const WinCameraDevice& cameraDevice);

#pragma endregion constructor and destructor

#pragma region Monochrome

        /**
         * @brief Return true if camera supported monochrome image
         * @return Return true if camera supported monochrome image
        */
        bool isSupportMonochrome() const;

        /**
         * @brief Get the supported monochrome image resolution in (width, height)
         * @return Return (width, height)[]
        */
        std::vector<std::pair<int, int>> getMonoResolutions() const;

        /**
         * @brief Check whether resolution existed in monochrome
         *
         * @param[in] width Width
         * @param[in] height Height
         * @return Return true if existed.
        */
        bool ContainMonochromeResolution(const int width, const int height) const;

#pragma endregion Monochrome

#pragma region RGB

        /**
         * @brief Return true if camera supported rgb image.
         * @return Return true if camera supported rgb image.
        */
        bool isSupportRGB() const;

        /**
         * @brief Get the supported rgb image resolution in (width, height)
         * @return Return (width, height)[]
        */
        std::vector<std::pair<int,int>> getRGBResolutions() const;

        /**
         * @brief Check whether resolution existed in RGB
         *
         * @param[in] width Width
         * @param[in] height Height
         * @return Return true if existed.
        */
        bool ContainRGBResolution(const int width, const int height) const;

#pragma endregion RGB

#pragma region Resolution

        /**
         * @brief Get the supported resolution in (width, height)
         * @return Return (width, height)[]
        */
        std::vector<std::pair<int,int>> getResolutions() const;

        /**
         * @brief Check whether resolution existed
         *
         * @param[in] width Width
         * @param[in] height Height
         * @return Return true if existed.
        */
        bool ContainResolution(const int width, const int height) const;

#pragma endregion Resolution

#pragma region Camera Getter

        /**
         * @brief Get camera friendly name
         * @return Return the camera friendly name
        */
        std::string getFriendlyName() const;

        /**
         * @brief Get the camera description
         * @return Return camera description
        */
        std::string getDescription() const;

        /**
         * @brief Get the camera device path. It is the camera id.
         * @return Return the camera device path.
        */
        std::string getDevicePath() const;

#pragma endregion Camera Getter

#pragma region Operator

        /**
         * @brief Copy assignment operator
         * @param[in] cameraDevice CameraDevice
         * @return
        */
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

#pragma endregion Operator

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
/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#pragma once
#ifndef WIN_CAMERA__DIRECTSHOW_CAMERA__DIRECTSHOW_CAMERA_STUB_DEFAULT_H
#define WIN_CAMERA__DIRECTSHOW_CAMERA__DIRECTSHOW_CAMERA_STUB_DEFAULT_H

//************Content************
#include "directshow_camera/properties/ds_camera_properties.h"
#include "directshow_camera/video_format/ds_video_format.h"
#include "directshow_camera/device/ds_camera_device.h"

#include <cstring>
#include <set>
#include <vector>

namespace DirectShowCamera
{
    /**
     * @brief Directshow Camera Stub Default Setting.
     *
     */
    class DirectShowCameraStubDefaultSetting
    {
    public:

        /**
         * @brief frame per second
         * @return 
        */
        static double getFps()
        {
            return 0.5;
        }

        /**
         * @brief Get Direct show camera properties.
         * 
         * @param[out] properties Properties
        */
        static void getProperties(std::shared_ptr<DirectShowCameraProperties>& properties)
        {
            // Clear if necessary
            if (properties != nullptr) properties.reset();

            // Setting
            properties = std::make_shared<DirectShowCameraProperties>();
            properties->getBrightness()->ImportProperty(true, 0, 255, 1, 128, false, 128, false, true);
            properties->getContrast()->ImportProperty(true, 0, 255, 1, 32, false, 32, false, true);
            properties->getHue()->ImportProperty(true, -180, 180, 1, 0, false, 0, false, true);
            properties->getSaturation()->ImportProperty(true, 0, 100, 1, 64, false, 64, false, true);
            properties->getSharpness()->ImportProperty(true, 0, 7, 1, 0, false, 0, false, true);
            properties->getGamma()->ImportProperty(true, 90, 150, 1, 120, false, 120, false, true);
            properties->getColorEnable()->ImportProperty(true, 0, 1, 1, 1, false, 1, false, true);
            properties->getWhiteBalance()->ImportProperty(true, 2800, 6500, 1, 4600, true, 4600, true, true);
            properties->getBacklightCompensation()->ImportProperty(true, 0, 2, 1, 1, false, 1, false, true);
            properties->getGain()->ImportProperty(false, 0, 1, 1, 0, false, 0, false, true);
            properties->getPan()->ImportProperty(true, -16, 16, 1, 0, false, 0, false, true);
            properties->getTilt()->ImportProperty(true, -16, 16, 1, 0, false, 0, false, true);
            properties->getRoll()->ImportProperty(true, 0, 3, 1, 0, false, 0, false, true);
            properties->getZoom()->ImportProperty(true, 100, 400, 10, 100, false, 100, false, true);
            properties->getExposure()->ImportProperty(true, -12, -3, 1, -6, true, -6, true, true);
            properties->getIris()->ImportProperty(false, 0, 1, 1, 0, false, 0, false, true);
            properties->getFocus()->ImportProperty(false, 0, 1, 1, 0, false, 0, false, true);
            properties->MarkAsInitialized();
        }

        /**
         * @brief Get Video formats
         * @return Video formats
        */
        static std::vector<DirectShowVideoFormat> getVideoFormat()
        {
            std::vector<DirectShowVideoFormat> videoFormats = {
                DirectShowVideoFormat(MEDIASUBTYPE_MJPG, 320, 240, 24, 320 * 240 * 24 / 8),
                DirectShowVideoFormat(MEDIASUBTYPE_YUY2, 640, 480, 16, 640 * 480 * 16 / 8),
                DirectShowVideoFormat(MEDIASUBTYPE_MJPG, 1280, 720, 24, 1280 * 720 * 24 / 8)
            };

            return videoFormats;
        }

        /**
         * @brief Get Camera
         * @param[out] cameraDevices Camera
        */
        static void getCamera(std::vector<DirectShowCameraDevice>& cameraDevices)
        {
            // Initialize and clear
            cameraDevices.clear();

            // Get video format
            std::vector<DirectShowVideoFormat> videoFormats = getVideoFormat();

            // Add camera
            cameraDevices.push_back(DirectShowCameraDevice("Integrated Camera", 
                "A fake camera", 
                "\\\\?\\usb#vid_0000&pid_0000&mi_0000&0000000&0&0000#{00000000-0000-0000-0000-000000000000}\\global",
                videoFormats));
        }

        /**
         * @brief Get default frame
         * @param[out] frame Frame bytes
         * @param[out] frameIndex Frame index
         * @param[out] numOfBytes Number of bytes of this frame
         * @param[in] width Frame width
         * @param[in] height Frame height
         * @param[in] previousFrameIndex Previous frame index
        */
        static void getFrame(
            unsigned char* frame,
            int& numOfBytes,
            unsigned long& frameIndex,
            const int width,
            const int height,
            const unsigned long previousFrameIndex
        )
        {
            // Frame index
            frameIndex = previousFrameIndex + 1;

            // Size
            int totalSize = width * height * 3;
            numOfBytes = totalSize;

            // Allocate image memory
            memset(frame, 0, totalSize * sizeof(unsigned char));

            // Calculate Box Size
            double boxWidth = 0;
            int boxCol = 0;
            if (width % 4 == 0 && height % 3 == 0)
            {
                // 4:3
                boxWidth = width / 12.0;
                boxCol = 12;
            }
            else
            {
                // 16 : 9
                boxWidth = width / 16.0;
                boxCol = 16;
            }

            int rColorArray[24] = {115,194,98,87,133,103,214,80,193,94,157,224,56,70,175,231,187,8,243,200,160,122,85,52};
            int gColorArray[24] = {82,150,122,108,128,189,126,91,90,60,188,163,61,148,54,199,86,133,243,200,160,122,85,52};
            int bColorArray[24] = {68,130,157,67,177,170,44,166,99,108,64,46,150,73,60,31,149,161,242,200,160,121,85,52};

            // Draw
            for (int i=0;i<width;i++)
            {
                for (int j = 0; j < height; j++)
                {
                    int pixelIndex = (j * width + i) * 3;

                    // Get color
                    int widthIndex = static_cast<int>(std::floor(i / boxWidth));
                    int heightIndex = static_cast<int>(std::floor(j / boxWidth));
                    int colorIndex = (widthIndex + heightIndex * boxCol + frameIndex) % 24;

                    // Blue
                    frame[pixelIndex] = bColorArray[colorIndex];

                    // Green
                    frame[pixelIndex+1] = gColorArray[colorIndex];

                    // Red
                    frame[pixelIndex+2] = rColorArray[colorIndex]; 
                }
            }
        }
    };
}


//*******************************

#endif
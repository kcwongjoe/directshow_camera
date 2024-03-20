#pragma once
#ifndef DIRECTSHOW_CAMERA_STUB_DEFAULT_H
#define DIRECTSHOW_CAMERA_STUB_DEFAULT_H

//************Content************
#include "ds_camera/ds_camera_properties.h"
#include "ds_camera/ds_video_format.h"

#include <cstring>
#include <set>

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
            properties->getBrightness()->importProperty(true, 0, 255, 1, 128, false, 128, false, true);
            properties->getContrast()->importProperty(true, 0, 255, 1, 32, false, 32, false, true);
            properties->getHue()->importProperty(true, -180, 180, 1, 0, false, 0, false, true);
            properties->getSaturation()->importProperty(true, 0, 100, 1, 64, false, 64, false, true);
            properties->getSharpness()->importProperty(true, 0, 7, 1, 0, false, 0, false, true);
            properties->getGamma()->importProperty(true, 90, 150, 1, 120, false, 120, false, true);
            properties->getColorEnable()->importProperty(true, 0, 1, 1, 1, false, 1, false, true);
            properties->getWhiteBalance()->importProperty(true, 2800, 6500, 1, 4600, true, 4600, true, true);
            properties->getBacklightCompensation()->importProperty(true, 0, 2, 1, 1, false, 1, false, true);
            properties->getGain()->importProperty(false, 0, 1, 1, 0, false, 0, false, true);
            properties->getPan()->importProperty(true, -16, 16, 1, 0, false, 0, false, true);
            properties->getTilt()->importProperty(true, -16, 16, 1, 0, false, 0, false, true);
            properties->getRoll()->importProperty(true, 0, 3, 1, 0, false, 0, false, true);
            properties->getZoom()->importProperty(true, 100, 400, 10, 100, false, 100, false, true);
            properties->getExposure()->importProperty(true, -12, -3, 1, -6, true, -6, true, true);
            properties->getIris()->importProperty(false, 0, 1, 1, 0, false, 0, false, true);
            properties->getFocus()->importProperty(false, 0, 1, 1, 0, false, 0, false, true);
            properties->markAsInitialized();
        }

        /**
         * @brief Get Video formats
         * @param videoFormats 
        */
        static void getVideoFormat(std::vector<DirectShowVideoFormat*>** videoFormats)
        {
            // Clear if necessary
            DirectShowVideoFormat::release(*videoFormats);
            *videoFormats = NULL;

            // Push data
            *videoFormats = new std::vector<DirectShowVideoFormat*>();
            for (int i=0;i< getVideFormatSize();i++)
            {
                int width = getVideoFormatWidth(i);
                int height = getVideoFormatHeight(i);
                int bitPerPixel = getVideoFormatBitPerPixel(i);
                (*videoFormats)->push_back(new DirectShowVideoFormat(getVideoFormatFormat(i), width, height, bitPerPixel, width * height * bitPerPixel / 8));
            }
        }

        /**
         * @brief Get Camera
         * @param[out] cameraDevices Camera
        */
        static void getCamera(std::vector<DirectShowCameraDevice>* cameraDevices)
        {
            // Initialize and clear
            if (cameraDevices == nullptr) *cameraDevices = std::vector<DirectShowCameraDevice>();
            cameraDevices->clear();

            // Get video format
            std::vector<DirectShowVideoFormat> videoFormats;
            for (int i = 0; i < getVideFormatSize(); i++)
            {
                int width = getVideoFormatWidth(i);
                int height = getVideoFormatHeight(i);
                int bitPerPixel = getVideoFormatBitPerPixel(i);
                videoFormats.push_back(DirectShowVideoFormat(getVideoFormatFormat(i), width, height, bitPerPixel, width * height * bitPerPixel / 8));
            }

            // Add camera
            cameraDevices->push_back(DirectShowCameraDevice("Integrated Camera", 
                "A fake camera", 
                "\\\\?\\usb#vid_0000&pid_0000&mi_0000&0000000&0&0000#{00000000-0000-0000-0000-000000000000}\\global",
                videoFormats));
        }

        /**
         * @brief Get default frame
         * @param frame[out] Frame bytes
         * @param frameIndex[out] Frame index
         * @param numOfBytes[out] Number of bytes of this frame
         * @param width[in] Frame width
         * @param height[in] Frame height
         * @param previousFrameIndex[in] Previous frame index
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
    private:

        // Video Format Setting

        /**
         * @brief Number of video format
         * @return
        */
        static int getVideFormatSize()
        {
            return 3;
        }

        /**
         * @brief Video format
         * @param index Index
         * @return
        */
        static GUID getVideoFormatFormat(int index)
        {
            GUID videoFormatFormat[3] = { MEDIASUBTYPE_MJPG , MEDIASUBTYPE_YUY2, MEDIASUBTYPE_MJPG };
            return videoFormatFormat[index];
        }

        /**
         * @brief Video width
         * @param index Index
         * @return
        */
        static int getVideoFormatWidth(int index)
        {
            int videoFormatWidth[3] = { 320 , 640, 1280 };
            return videoFormatWidth[index];
        }

        /**
         * @brief Video height
         * @param index Index
         * @return
        */
        static int getVideoFormatHeight(int index)
        {
            int videoFormatHeight[3] = { 240 , 480, 720 };
            return videoFormatHeight[index];
        }

        /**
         * @brief Video bit per pixel
         * @param index Index
         * @return
        */
        static int getVideoFormatBitPerPixel(int index)
        {
            int videoFormatBitPerPixel[3] = { 24 , 16, 24 };
            return videoFormatBitPerPixel[index];
        }
    };
}


//*******************************

#endif
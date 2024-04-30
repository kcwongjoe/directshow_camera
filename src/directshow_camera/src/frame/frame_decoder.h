/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef DIRECTSHOW_CAMERA__OPENCV_UTILS__FRAME__FRAME_DECODER_H
#define DIRECTSHOW_CAMERA__OPENCV_UTILS__FRAME__FRAME_DECODER_H

//************Content************
#ifdef WITH_OPENCV2
#include <opencv2/opencv.hpp>
#endif

#include <guiddef.h>

#include <vector>
#include <memory>

#include "frame/frame_settings.h"

namespace DirectShowCamera
{
    /**
     * @brief A converter to convert byte[] to cv::Mat.
    */
    class FrameDecoder
    {
    public:

#pragma region Support Video Type

        /**
         * @brief Get the support video type
         * @return std::vector<GUID>
         */
        static std::vector<GUID> SupportVideoType();

        /**
        * @brief Check if the video type is supported
        * @param[in] videoType Video Type
        * @return bool Return true if the video type is supported
        */
        static bool isSupportedVideoType(const GUID videoType);

        /**
        * @brief Check if the video type is supported. If not supported, throw exception.
        * @param[in] videoType Video Type
        */
        static void CheckSupportVideoType(const GUID videoType);

#pragma endregion Support Video Type

#pragma region 8bit Monochrome

        /**
        * @brief Get the support monochrome video type
        * @return std::vector<GUID> Return the support monochrome video type
        */
        static std::vector<GUID> SupportMonochromeVideoType();

        /**
        * @brief Check if the video type is monochrome
        * @param[in] videoType Video Type
        * @return bool Return true if the video type is monochrome
        */
        static bool isMonochromeFrameType(const GUID videoType);

        /**
        * @brief Check if the video type is monochrome. If not monochrome, throw exception.
        * @param[in] videoType Video Type
        */
        static void CheckMonochromeFrameType(const GUID videoType);

        /**
        * @brief Clone the monochrome frame into another array
        * @param[in] inputData Input data. Image data is stored row by row and has been flipped vertically.
        * @param[out] outputData Output data. Image data is stored row by row.
        * @param[in] videoType Video Type
        * @param[in] width Width
        * @param[in] height Height
        * @param[in] verticalFlip (Optional) Flip the image vertically. Default as false.
        */
        static void DecodeMonochromeFrame(
            unsigned char* inputData,
            unsigned char* outputData,
            const GUID videoType,
            const int width,
            const int height,
            const bool verticalFlip = false
        );

        /**
        * @brief Clone the monochrome frame into another array
        * @param[in] data Input data. Image data is stored row by row and has been flipped vertically.
        * @param[in] videoType Video Type
        * @param[in] width Width
        * @param[in] height Height
        * @param[in] verticalFlip (Optional) Flip the image vertically. Default as false.
        */
        static std::shared_ptr<unsigned char[]> DecodeMonochromeFrame(
            unsigned char* data,
            const GUID videoType,
            const int width,
            const int height,
            const bool verticalFlip = false
        );

#pragma endregion 8bit Monochrome

#pragma region 16bit Monochrome

        /**
        * @brief Get the support 16bit monochrome video type
        * @return std::vector<GUID> Return the support 16bit monochrome video type
        */
        static std::vector<GUID> Support16BitMonochromeVideoType();

        /**
        * @brief Check if the video type is 16bit monochrome
        * @param[in] videoType Video Type
        * @return bool Return true if the video type is 16bit monochrome
        */
        static bool is16BitMonochromeFrameType(const GUID videoType);

        /**
        * @brief Check if the video type is 16bit monochrome. If not 16bit monochrome, throw exception.
        * @param[in] videoType Video Type
        */
        static void Check16BitMonochromeFrameType(const GUID videoType);

        /**
        * @brief Clone the 16bit monochrome frame into another array
        * @param[in] inputData Input data. Image data is stored row by row and has been flipped vertically.
        * @param[out] outputData Output data. Image data is stored row by row.
        * @param[in] videoType Video Type
        * @param[in] width Width
        * @param[in] height Height
        * @param[in] verticalFlip (Optional) Flip the image vertically. Default as false.
        */
        static void Decode16BitMonochromeFrame(
            unsigned char* inputData,
            unsigned short* outputData,
            const GUID videoType,
            const int width,
            const int height,
            const bool verticalFlip = false
        );

        /**
        * @brief Clone the 16bit monochrome frame into another array
        * @param[in] data Input data. Image data is stored row by row and has been flipped vertically.
        * @param[in] videoType Video Type
        * @param[in] width Width
        * @param[in] height Height
        * @param[in] verticalFlip (Optional) Flip the image vertically. Default as false.
        */
        static std::shared_ptr<unsigned short[]> Decode16BitMonochromeFrame(
            unsigned char* data,
            const GUID videoType,
            const int width,
            const int height,
            const bool verticalFlip = false
        );

#pragma endregion 16bit Monochrome

#pragma region RGB

        /**
        * @brief Get the support RGB video type
        * @return std::vector<GUID> Return the support RGB video type
        */
        static std::vector<GUID> SupportRGBVideoType();

        /**
        * @brief Check if the video type is RGB
        * @param[in] videoType Video Type
        * @return bool Return true if the video type is RGB
        */
        static bool isRGBFrameType(const GUID videoType);

        /**
        * @brief Check if the video type is RGB. If not RGB, throw exception.
        * @param[in] videoType Video Type
        */
        static void CheckRGBFrameType(const GUID videoType);

        /**
        * @brief Decode the RGB frame into another array
        * @param[in] inputData Input data. Image data is stored in pixel by pixel, row by row in BGR format and has been flipped vertically.
        * @param[out] outputData Output data. Image data is stored in pixel by pixel, row by row.
        * @param[in] videoType Video Type
        * @param[in] width Width
        * @param[in] height Height
        * @param[in] verticalFlip (Optional) Flip the image vertically. Default as false.
        * @param[in] outputRGB (Optional) Output as RGB. Default as false.
        */
        static void DecodeRGBFrame(
            unsigned char* inputData,
            unsigned char* outputData,
            const GUID videoType,
            const int width,
            const int height,
            const bool verticalFlip = false,
            const bool outputRGB = false
        );

        /**
        * @brief Decode the RGB frame into another array
        * @param[in] data Input data. Image data is stored in pixel by pixel, row by row in BGR format and has been flipped vertically.
        * @param[in] videoType Video Type
        * @param[in] width Width
        * @param[in] height Height
        * @param[in] verticalFlip (Optional) Flip the image vertically. Default as false.
        * @param[in] outputRGB (Optional) Output as RGB. Default as false.
        */
        static std::shared_ptr<unsigned char[]> DecodeRGBFrame(
            unsigned char* data,
            const GUID videoType,
            const int width,
            const int height,
            const bool verticalFlip = false,
            const bool outputRGB = false
        );

#pragma endregion RGB

        /**
        * @brief Decode the frame into another array
        * @param[in] inputData Input data. Image data is stored in pixel by pixel, row by row in BGR format(If color image) and has been flipped vertically.
        * @param[out] outputData Output data. Image data is stored in pixel by pixel, row by row.
        * @param[in] videoType Video Type
        * @param[in] width Width
        * @param[in] height Height
        * @param[in] verticalFlip (Optional) Flip the image vertically. Default as false.
        * @param[in] outputRGB (Optional) Output as RGB. Default as false.
        */
        static void DecodeFrame(
            unsigned char* inputData,
            unsigned char* outputData,
            const GUID videoType,
            const int width,
            const int height,
            const bool verticalFlip = false,
            const bool outputRGB = false
        );

#ifdef WITH_OPENCV2

        /**
        * @brief Decode the frame into cv::Mat
        * @param[in] data Input data. Image data is stored in pixel by pixel, row by row in BGR format(If color image) and has been flipped vertically.
        * @param[in] videoType Video Type
        * @param[in] width Width
        * @param[in] height Height
        * @param[in] verticalFlip (Optional) Flip the image vertically. Default as false.
        * @param[in] outputRGB (Optional) Output as RGB. Default as false.
        */
        static cv::Mat DecodeFrameToCVMat(
            unsigned char* data,
            const GUID videoType,
            const int width,
            const int height,
            const bool verticalFlip = false,
            const bool outputRGB = false
        );

        /**
        * @brief Decode the monochrome frame into cv::Mat
        * @param[in] data Input data. Image data is stored row by row and has been flipped vertically.
        * @param[in] videoType Video Type
        * @param[in] width Width
        * @param[in] height Height
        * @param[in] verticalFlip (Optional) Flip the image vertically. Default as false.
        */
        static cv::Mat DecodeMonochromeFrameToCVMat(
            unsigned char* data,
            const GUID videoType,
            const int width,
            const int height,
            const bool verticalFlip = false
        );

        /**
        * @brief Decode the 16bit monochrome frame into cv::Mat
        * @param[in] data Input data. Image data is stored row by row and has been flipped vertically.
        * @param[in] videoType Video Type
        * @param[in] width Width
        * @param[in] height Height
        * @param[in] verticalFlip (Optional) Flip the image vertically. Default as false.
        */
        static cv::Mat Decode16BitMonochromeFrameToCVMat(
            unsigned char* data,
            const GUID videoType,
            const int width,
            const int height,
            const bool verticalFlip = false
        );

        /**
        * @brief Decode the RGB frame into cv::Mat
        * @param[in] data Input data. Image data is stored in pixel by pixel, row by row in BGR format and has been flipped vertically.
        * @param[in] videoType Video Type
        * @param[in] width Width
        * @param[in] height Height
        * @param[in] verticalFlip (Optional) Flip the image vertically. Default as false.
        * @param[in] outputRGB (Optional) Output as RGB. Default as false.
        */
        static cv::Mat DecodeRGBFrameFrameToCVMat(
            unsigned char* data,
            const GUID videoType,
            const int width,
            const int height,
            const bool verticalFlip = false,
            const bool outputRGB = false
        );
#endif // def WITH_OPENCV2

    private:

        /**
        * @brief Clone the raw data into another array. It can be used to vertical flip the image.
        * @param[in] inputData Input data. Image data is stored row by row and has been flipped vertically.
        * @param[out] outputData Output data. Image data is stored row by row.
        * @param[in] width Width
        * @param[in] height Height
        * @param[in] bytesPerPixel Bytes per pixel
        * @param[in] verticalFlip (Optional) Flip the image vertically. Default as false.
        */
        static void CloneRawData(
            unsigned char* inputData,
            unsigned char* outputData,
            const int width,
            const int height,
            const int bytesPerPixel,
            const bool verticalFlip = false
        );

    private:
        GUID m_videoType;
        std::vector<GUID> m_supportVideoType;

        FrameSettings m_frameSettings;
    };

}

//*******************************

#endif // ndef DIRECTSHOW_CAMERA__OPENCV_UTILS__FRAME__FRAME_DECODER_H
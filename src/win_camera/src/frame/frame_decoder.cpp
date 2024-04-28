/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#include "frame/frame_decoder.h"

#include "directshow_camera/video_format/ds_guid.h"
#include "directshow_camera/utils/ds_video_format_utils.h"

#ifdef WITH_OPENCV2

namespace WinCamera
{
#pragma region Support Video Type
    std::vector<GUID> FrameDecoder::SupportVideoType()
    {
        const auto monochromeVideoType = SupportMonochromeVideoType();
        const auto monochrome16BitVideoType = Support16BitMonochromeVideoType();
        const auto rgbVideoType = SupportRGBVideoType();

        std::vector<GUID> result;
        result.insert(result.end(), monochromeVideoType.begin(), monochromeVideoType.end());
        result.insert(result.end(), monochrome16BitVideoType.begin(), monochrome16BitVideoType.end());
        result.insert(result.end(), rgbVideoType.begin(), rgbVideoType.end());
        return result;
    }

    bool FrameDecoder::isSupportedVideoType(const GUID videoType)
    {
        for (const auto supportedtype : SupportVideoType())
        {
            if (supportedtype == videoType) return true;
        }
        return false;
    }

    void FrameDecoder::CheckSupportVideoType(const GUID videoType)
    {
        if (!isSupportedVideoType(videoType))
        {
            throw std::invalid_argument("Video type(" + DirectShowVideoFormatUtils::ToString(videoType) + ") is not supported.");
        }
    }

#pragma endregion Support Video Type

#pragma region 8bit Monochrome

    std::vector<GUID> WinCamera::FrameDecoder::SupportMonochromeVideoType()
    {
        return { MEDIASUBTYPE_Y800, MEDIASUBTYPE_Y8, MEDIASUBTYPE_GREY };
    }

    bool FrameDecoder::isMonochromeFrameType(const GUID videoType)
    {
        for (const auto supportedtype : SupportMonochromeVideoType())
        {
            if (supportedtype == videoType) return true;
        }
        return false;
    }

    void FrameDecoder::CheckMonochromeFrameType(const GUID videoType)
    {
        // Check
        if (!isMonochromeFrameType(videoType))
        {
            throw std::invalid_argument("Video type(" + DirectShowVideoFormatUtils::ToString(videoType) + ") is not a Monochrome type.");
        }
    }

    void FrameDecoder::DecodeMonochromeFrame(
        unsigned char* inputData,
        unsigned char* outputData,
        const GUID videoType,
        const int width,
        const int height,
        const bool verticalFlip
    )
    {
        // Check
        CheckMonochromeFrameType(videoType);

        // Copy 1 byte per pixel
        CloneRawData(inputData, outputData, width, height, 1, verticalFlip);
    }

    std::shared_ptr<unsigned char[]> FrameDecoder::DecodeMonochromeFrame(
        unsigned char* data,
        const GUID videoType,
        const int width,
        const int height,
        const bool verticalFlip)
    {
        // Check
        CheckMonochromeFrameType(videoType);

        // Initialize result buffer
        auto result = std::make_shared<unsigned char[]>(height * width);

        // Decode
        DecodeMonochromeFrame(data, result.get(), videoType, width, height, verticalFlip);

        return result;
    }

#pragma endregion 8bit Monochrome

#pragma region 16bit Monochrome

    std::vector<GUID> WinCamera::FrameDecoder::Support16BitMonochromeVideoType()
    {
        return { MEDIASUBTYPE_Y16 };
    }

    bool FrameDecoder::is16BitMonochromeFrameType(const GUID videoType)
    {
        for (const auto supportedtype : Support16BitMonochromeVideoType())
        {
            if (supportedtype == videoType) return true;
        }
        return false;
    }

    void FrameDecoder::Check16BitMonochromeFrameType(const GUID videoType)
    {
        // Check
        if (!is16BitMonochromeFrameType(videoType))
        {
            throw std::invalid_argument("Video type(" + DirectShowVideoFormatUtils::ToString(videoType) + ") is not a 16Bit Monochrome type.");
        }
    }

    void FrameDecoder::Decode16BitMonochromeFrame(unsigned char* inputData, unsigned short* outputData, const GUID videoType, const int width, const int height, const bool verticalFlip)
    {
        // Check
        Check16BitMonochromeFrameType(videoType);

        // Copy 2 byte per pixel
        CloneRawData(inputData, (unsigned char*)outputData, width, height, 2, verticalFlip);
    }

    std::shared_ptr<unsigned short[]> FrameDecoder::Decode16BitMonochromeFrame(
        unsigned char* data,
        const GUID videoType,
        const int width,
        const int height,
        const bool verticalFlip)
    {
        // Check
        Check16BitMonochromeFrameType(videoType);

        // Initialize result buffer
        auto result = std::make_shared<unsigned short[]>(height * width);

        // Decode
        Decode16BitMonochromeFrame(data, result.get(), videoType, width, height, verticalFlip);

        return result;
    }

#pragma endregion 16bit Monochrome

#pragma region RGB

    std::vector<GUID> FrameDecoder::SupportRGBVideoType()
    {
        return { MEDIASUBTYPE_RGB8, MEDIASUBTYPE_YUY2, MEDIASUBTYPE_RGB565, MEDIASUBTYPE_RGB555, MEDIASUBTYPE_RGB24, MEDIASUBTYPE_MJPG };
    }

    bool FrameDecoder::isRGBFrameType(const GUID videoType)
    {
        for (const auto supportedtype : SupportRGBVideoType())
        {
            if (supportedtype == videoType) return true;
        }
        return false;
    }

    void FrameDecoder::CheckRGBFrameType(const GUID videoType)
    {
        // Check
        if (!isRGBFrameType(videoType))
        {
            throw std::invalid_argument("Video type(" + DirectShowVideoFormatUtils::ToString(videoType) + ") is not a RGB type.");
        }
    }

    void FrameDecoder::DecodeRGBFrame(
        unsigned char* inputData,
        unsigned char* outputData,
        const GUID videoType,
        const int width,
        const int height,
        const bool verticalFlip,
        const bool outputRGB
    )
    {
        // Check
        CheckRGBFrameType(videoType);

        if (!outputRGB)
        {
            // Copy 3 byte per pixel in BGR format
            CloneRawData(inputData, outputData, width, height, 3, verticalFlip);
        }
        else
        {
            // Convert to RGB
            if (verticalFlip) {
                // Notes: inputData default is vertical flipped. So do nothing if verticalFlip == true

                // Copy 3 byte per pixel from BGR to RGB format
                unsigned char* inputDataPtrTemp = inputData;
                unsigned char* outputDataPtrTemp = outputData;
                const int frameSize = width * height * 3;

                // 3 byte - BGR - No Vertical flip
                for (int i = 0; i < frameSize; i += 3) {

                    // Copy R to B
                    *outputDataPtrTemp = *(inputDataPtrTemp + 2);
                    outputDataPtrTemp++;

                    // Copy G
                    *outputDataPtrTemp = *(inputDataPtrTemp + 1);
                    outputDataPtrTemp++;

                    // Copy B to R
                    *outputDataPtrTemp = *inputDataPtrTemp;
                    outputDataPtrTemp++;

                    // Move to next pixel
                    inputDataPtrTemp += 3;
                }
            }
            else
            {
                // Notes: inputData default is vertical flipped. So image should be vertical flip if verticalFlip == false

                // Copy 3 byte per pixel from BGR to RGB format and Vertical flip

                const int numOfBytePerRow = width * 3;
                const int frameSize = width * height * 3;
                unsigned char* inputDataPtrTemp = inputData;    // Input start from the first row
                unsigned char* outputDataPtrTemp = outputData + (height - 1) * numOfBytePerRow; // Output start from the last row

                int x = 0; // x is the current pixel in the row
                for (int i = 0; i < frameSize; i += 3) {

                    // If x is out of bound, Output move up one row and x reset to 0
                    if (x >= width) {
                        x = 0;
                        outputDataPtrTemp -= numOfBytePerRow * 2; // Now outputDataPtrTemp is at the end of the previous row, so move 2 row up
                    }

                    // Copy R to B
                    *inputDataPtrTemp = *(outputDataPtrTemp + 2);
                    inputDataPtrTemp++;

                    // Copy G
                    *inputDataPtrTemp = *(outputDataPtrTemp + 1);
                    inputDataPtrTemp++;

                    // Copy B to R
                    *inputDataPtrTemp = *outputDataPtrTemp;
                    inputDataPtrTemp++;

                    // Move to next pixel
                    outputDataPtrTemp += 3;

                    // Update x
                    x++;
                }
            }
        }
    }

    std::shared_ptr<unsigned char[]> FrameDecoder::DecodeRGBFrame(
        unsigned char* data,
        const GUID videoType,
        const int width,
        const int height,
        const bool verticalFlip,
        const bool outputRGB
    )
    {
        // Check
        CheckRGBFrameType(videoType);

        // Initialize result buffer
        auto result = std::make_shared<unsigned char[]>(height * width * 3);

        // Decode
        DecodeRGBFrame(data, result.get(), videoType, width, height, verticalFlip, outputRGB);

        return result;
    }

    void FrameDecoder::DecodeFrame(
        unsigned char* inputData,
        unsigned char* outputData,
        const GUID videoType,
        const int width,
        const int height,
        const bool verticalFlip,
        const bool outputRGB
    )
    {
        // Check
        CheckSupportVideoType(videoType);

        if (isMonochromeFrameType(videoType))
        {
            // Monochrome
            DecodeMonochromeFrame(inputData, outputData, videoType, width, height, verticalFlip);
        }
        else if (is16BitMonochromeFrameType(videoType))
        {
            // 16bit Monochrome
            Decode16BitMonochromeFrame(inputData, (unsigned short*)outputData, videoType, width, height, verticalFlip);
        }
        else
        {
            // RGB
            DecodeRGBFrame(inputData, outputData, videoType, width, height, verticalFlip, outputRGB);
        }
    }

#pragma endregion RGB

#ifdef WITH_OPENCV2

    cv::Mat FrameDecoder::DecodeFrameToCVMat(
        unsigned char* data,
        const GUID videoType,
        const int width,
        const int height,
        const bool verticalFlip,
        const bool outputRGB
    )
    {
        // Check
        CheckSupportVideoType(videoType);

        if (isMonochromeFrameType(videoType))
        {
            // Monochrome
            return DecodeMonochromeFrameToCVMat(data, videoType, width, height, verticalFlip);
        }
        else if (is16BitMonochromeFrameType(videoType))
        {
            // 16bit Monochrome
            return Decode16BitMonochromeFrameToCVMat(data, videoType, width, height, verticalFlip);
        }
        else
        {
            // RGB
            return DecodeRGBFrameFrameToCVMat(data, videoType, width, height, verticalFlip, outputRGB);
        }
    }

    cv::Mat FrameDecoder::DecodeMonochromeFrameToCVMat(
        unsigned char* data,
        const GUID videoType,
        const int width,
        const int height,
        const bool verticalFlip
    )
    {
        // Check
        CheckMonochromeFrameType(videoType);

        // Initialize buffer
        auto result = cv::Mat(height, width, CV_8UC1);

        // Decode
        DecodeMonochromeFrame(data, result.ptr(), videoType, width, height, verticalFlip);

        return result;
    }

    cv::Mat FrameDecoder::Decode16BitMonochromeFrameToCVMat(
        unsigned char* data,
        const GUID videoType,
        const int width,
        const int height,
        const bool verticalFlip
    )
    {
        // Check
        Check16BitMonochromeFrameType(videoType);

        // Initialize buffer
        auto result = cv::Mat(height, width, CV_16UC1);

        // Decode
        Decode16BitMonochromeFrame(data, (unsigned short*)result.ptr(), videoType, width, height, verticalFlip);

        return result;
    }

    cv::Mat FrameDecoder::DecodeRGBFrameFrameToCVMat(
        unsigned char* data,
        const GUID videoType,
        const int width,
        const int height,
        const bool verticalFlip,
        const bool outputRGB
    )
    {
        // Check
        CheckRGBFrameType(videoType);

        // Initialize result buffer
        auto result = cv::Mat(height, width, CV_8UC3);

        // Decode
        DecodeRGBFrame(data, result.ptr(), videoType, width, height, verticalFlip, outputRGB);

        return result;
    }

#endif // def WITH_OPENCV2

    void FrameDecoder::CloneRawData(
        unsigned char* inputData,
        unsigned char* outputData,
        const int width,
        const int height,
        const int bytesPerPixel,
        const bool verticalFlip
    )
    {
        // Notes: inputData default is vertical flipped. So do nothing if verticalFlip == true

        if (verticalFlip)
        {
            // Return image in Vertical flip
            memcpy(outputData, inputData, (long)height * (long)width * (long)bytesPerPixel);
        }
        else
        {
            // Return image in non-Vertical flip
            int numOfBytePerRow = width * bytesPerPixel;
            for (int y = 0; y < height; y++) {
                memcpy(outputData + (numOfBytePerRow * (long)y), inputData + (numOfBytePerRow * (long)(height - y - 1)), numOfBytePerRow);
            }
        }
    }
}

#endif
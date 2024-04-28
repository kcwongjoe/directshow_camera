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

    OpenCVMatConverter::OpenCVMatConverter() :
        m_videoType(MEDIASUBTYPE_None)
    {
        m_supportVideoType = {
            MEDIASUBTYPE_RGB8,
            MEDIASUBTYPE_GREY,
            MEDIASUBTYPE_Y8,
            MEDIASUBTYPE_Y800,
            MEDIASUBTYPE_Y16,
            MEDIASUBTYPE_YUY2,
            MEDIASUBTYPE_RGB565,
            MEDIASUBTYPE_RGB555,
            MEDIASUBTYPE_RGB24,
            MEDIASUBTYPE_MJPG
        };
    }

    void OpenCVMatConverter::Reset()
    {
        m_videoType = MEDIASUBTYPE_None;
        m_frameSettings.Reset();
    }

    void OpenCVMatConverter::setFrameSettings(const FrameSettings settings)
    {
        m_frameSettings = settings;
    }

    void OpenCVMatConverter::setVideoType(const GUID videoType)
    {
        if (videoType != m_videoType)
        {
            if (std::find(m_supportVideoType.begin(), m_supportVideoType.end(), videoType) != m_supportVideoType.end()) {
                // Video type is supported, set it
                m_videoType = videoType;
            }
            else
            {
                throw std::invalid_argument("Video type(" + DirectShowVideoFormatUtils::ToString(videoType) + ") is not supported");
            }
        }
    }

    GUID OpenCVMatConverter::getVideoType() const
    {
        return m_videoType;
    }

    cv::Mat OpenCVMatConverter::convert(unsigned char* data, const int width, const int height)
    {
        // Initialize
        cv::Mat result;

        // Check if video type is not set
        if (m_videoType == MEDIASUBTYPE_None)   return result;

        // Convert to Mat
        if (m_videoType == MEDIASUBTYPE_Y800 || m_videoType == MEDIASUBTYPE_Y8 || m_videoType == MEDIASUBTYPE_GREY)
        {
            // 1 byte per pixel

            // Create Mat
            result = cv::Mat(height, width, CV_8UC1);

            // Copy
            memcpy(result.ptr(), data, (long)height * (long)width);
        }
        else if (m_videoType == MEDIASUBTYPE_Y16)
        {
            // 2 byte per pixel

            // Create Mat
            result = cv::Mat(height, width, CV_16UC1);
            unsigned char* matPtr = result.ptr();

            if (m_frameSettings.BGR)
            {
                // 2 byte - BGR
                memcpy(matPtr, data, (long)height * (long)width * 2L);
            }
            else
            {
                // 2 byte - RGB
                for (int i = 0; i < width * height; i++) {
                    *matPtr = (uint8_t)(*((uint16_t*)data) >> 8);
                    matPtr++;

                    *matPtr = (uint8_t)(*((uint16_t*)data) >> 8);
                    matPtr++;

                    *matPtr = (uint8_t)(*((uint16_t*)data) >> 8);
                    matPtr++;

                    data += 2;
                }
            }
        }
        else
        {
            // 3 byte per pixel

            // Create Mat
            result = cv::Mat(height, width, CV_8UC3);
            long numOfBytePerRow = width * 3;
            int frameSize = width * height * 3;
            unsigned char* dataPtrTemp = data;
            unsigned char* matPtr = result.ptr();

            if (m_frameSettings.BGR) {
                // BGR

                if (m_frameSettings.VerticalFlip)
                {
                    // 3 byte - RGB - Vertical flip
                    for (long y = 0; y < height; y++) {
                        memcpy(matPtr + (numOfBytePerRow * (long)y), data + (numOfBytePerRow * (long)(height - y - 1)), numOfBytePerRow);
                    }
                }
                else
                {
                    // 3 byte - RGB - No Vertical flip
                    memcpy(matPtr, data, frameSize);
                }
            }
            else
            {
                // RGB
                if (m_frameSettings.VerticalFlip) {
                    // 3 byte - BGR - Vertical flip

                    int x = 0;
                    int y = (height - 1) * numOfBytePerRow;
                    dataPtrTemp += y;

                    for (int i = 0; i < frameSize; i += 3) {
                        if (x >= width) {
                            x = 0;
                            dataPtrTemp -= numOfBytePerRow * 2;
                        }

                        *matPtr = *(dataPtrTemp + 2);
                        matPtr++;

                        *matPtr = *(dataPtrTemp + 1);
                        matPtr++;

                        *matPtr = *dataPtrTemp;
                        matPtr++;

                        dataPtrTemp += 3;
                        x++;
                    }
                }
                else
                {
                    // 3 byte - BGR - No Vertical flip
                    for (int i = 0; i < frameSize; i += 3) {
                        *matPtr = *(dataPtrTemp + 2);
                        matPtr++;

                        *matPtr = *(dataPtrTemp + 1);
                        matPtr++;

                        *matPtr = *dataPtrTemp;
                        matPtr++;

                        dataPtrTemp += 3;
                    }
                }
            }
        }

        return result;
    }

    std::vector<GUID> OpenCVMatConverter::getSupportVideoType() const
    {
        return m_supportVideoType;
    }
}

#endif
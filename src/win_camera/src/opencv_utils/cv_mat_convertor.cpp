#include "opencv_utils/cv_mat_convertor.h"

#include "directshow_camera/ds_guid.h"

#ifdef WITH_OPENCV2

namespace WinCamera
{

    /**
     * @brief Constructor
    */
    OpenCVMatConverter::OpenCVMatConverter() :
        m_videoType(MEDIASUBTYPE_None)
    {
        
        m_supportVideoType.push_back(MEDIASUBTYPE_RGB8);
        m_supportVideoType.push_back(MEDIASUBTYPE_GREY);
        m_supportVideoType.push_back(MEDIASUBTYPE_Y8);
        m_supportVideoType.push_back(MEDIASUBTYPE_Y800);
        m_supportVideoType.push_back(MEDIASUBTYPE_Y16);
        m_supportVideoType.push_back(MEDIASUBTYPE_YUY2);
        m_supportVideoType.push_back(MEDIASUBTYPE_RGB565);
        m_supportVideoType.push_back(MEDIASUBTYPE_RGB555);
        m_supportVideoType.push_back(MEDIASUBTYPE_RGB24);
        m_supportVideoType.push_back(MEDIASUBTYPE_MJPG);
        
    }

    void OpenCVMatConverter::setVideoType(const GUID videoType)
    {
        m_videoType = videoType;
    }

    GUID OpenCVMatConverter::getVideoType() const
    {
        return m_videoType;
    }

    cv::Mat OpenCVMatConverter::convert(unsigned char* data, const int width, const int height)
    {
        // Initialize
        cv::Mat result;

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

            if (isBGR)
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

            if (isBGR) {
                // BGR

                if (isVerticalFlip)
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
                if (isVerticalFlip) {
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
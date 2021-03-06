#include <cv_mat_convertor.h>

#ifdef HAS_OPENCV

namespace DirectShowCamera
{

    /**
     * @brief Constructor
    */
    OpenCVMatConverter::OpenCVMatConverter()
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

    /**
     * @brief Convet Byte[] to cv::Mat
     * @param data Byte[]
     * @param width Widht of frame
     * @param height Height of frmae
     * @return Return cv::Mat which store data in BGR
    */
    cv::Mat OpenCVMatConverter::convert(unsigned char* data, int width, int height)
    {
        // Initialize
        cv::Mat result;

        // Convert to Mat
        if (videoType == MEDIASUBTYPE_Y800 || videoType == MEDIASUBTYPE_Y8 || videoType == MEDIASUBTYPE_GREY)
        {
            // 1 byte per pixel

            // Create Mat
            result = cv::Mat(height, width, CV_8UC1);

            // Copy
            memcpy(result.ptr(), data, (long)height * (long)width);
        }
        else if (videoType == MEDIASUBTYPE_Y16)
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

    /**
     * @brief Get the support video type
     * 
     * @return std::vector<GUID> 
     */
    std::vector<GUID> OpenCVMatConverter::getSupportVideoType()
    {
        return m_supportVideoType;
    }
}

#endif
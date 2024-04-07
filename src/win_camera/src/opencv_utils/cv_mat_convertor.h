#pragma once
#ifndef WIN_CAMERA__OPENCV_UTILS__OPENCV_MAT_CONVERTOR_H
#define WIN_CAMERA__OPENCV_UTILS__OPENCV_MAT_CONVERTOR_H

//************Content************
#ifdef WITH_OPENCV2

#include <opencv2/opencv.hpp>

#include <guiddef.h>

namespace WinCamera
{

    /**
     * @brief A converter to convert byte[] to cv::Mat.
     * @details Reference: https://github.com/opencv/opencv/tree/master/modules/videoio/src/cap_dshow.cpp
    */
    class OpenCVMatConverter
    {
    public:

        OpenCVMatConverter();

        /**
         * @brief Set it as true if the input byte is BGR order, otherwise the input byte is RGB order. Default as false.
        */
        bool isBGR = true;

        /**
         * @brief Set it as true to flip iamge vertically. Default as true
        */
        bool isVerticalFlip = true;

        /**
        * @brief Set the video type
        * @param[in] videoType Video type
        */
        void setVideoType(const GUID videoType);

        /**
        * @brief Get the video type
        */
        GUID getVideoType() const;

        /**
         * @brief Convet Byte[] to cv::Mat
         * @param[out] data Byte[]
         * @param[in] width Widht of frame
         * @param[in] height Height of frmae
         * @return Return cv::Mat which store data in BGR
        */
        cv::Mat convert(unsigned char* data, const int width, const int height);

        /**
         * @brief Get the support video type
         *
         * @return std::vector<GUID>
         */
        std::vector<GUID> getSupportVideoType() const;
    private:
        GUID m_videoType;
        std::vector<GUID> m_supportVideoType;

    };

}

#endif

//*******************************

#endif
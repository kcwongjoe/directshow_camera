#pragma once
#ifndef WIN_CAMERA__OPENCV_UTILS__OPENCV_MAT_CONVERTOR_H
#define WIN_CAMERA__OPENCV_UTILS__OPENCV_MAT_CONVERTOR_H

//************Content************
#ifdef WITH_OPENCV2

#include <opencv2/opencv.hpp>

#include "directshow_camera/ds_video_format.h"

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

        GUID videoType = MEDIASUBTYPE_None;
            
        cv::Mat convert(unsigned char* data, int width, int height);

        std::vector<GUID> getSupportVideoType() const;
    private:
        std::vector<GUID> m_supportVideoType;

    };

}

#endif

//*******************************

#endif
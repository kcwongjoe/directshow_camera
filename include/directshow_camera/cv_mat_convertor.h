#pragma once
#ifndef OPENCV_MAT_CONVERTOR_H
#define OPENCV_MAT_CONVERTOR_H

//************Content************
#include <ds_libs_setting.h>

#ifdef HAS_OPENCV

#include <opencv2/opencv.hpp>

#include <ds_camera_utils.h>

#include <ds_video_format.h>

namespace DirectShowCamera
{

    /**
     * @brief A converter to convert byte[] to cv::Mat.
     * @details Reference: https://github.com/opencv/opencv/tree/master/modules/videoio/src/cap_dshow.cpp
    */
    class OpenCVMatConverter
    {
        private:
            std::vector<GUID> m_supportVideoType;

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

            std::vector<GUID> getSupportVideoType();
    };

}

#endif

//*******************************

#endif
#pragma once
#ifndef UVC_CAMERA_RESOLUTION_H
#define UVC_CAMERA_RESOLUTION_H

//************Content************
#include <string>
#include <vector>

class CameraResolution {

public:
    CameraResolution(int width, int height, int bytePerPixel);

    static CameraResolution Empty();
    bool isEmpty();
    int getWidth();
    int getHeight();
    int getBytePerPixel();
    int getNumOfPixel();
    int getTotalByteSize();

    operator std::string() const { return "Resolution: " + std::to_string(m_width) + " x " + std::to_string(m_height) + " x " + std::to_string(m_bytePerPixel); }

    static int getIndex(std::vector<CameraResolution>* resolutions, int width, int height);

private:
    int m_width= 0 ;
    int m_height = 0;
    int m_bytePerPixel = 0;
    int m_numOfPixel = 0;
    int m_TotalByteSize = 0;
};

//*******************************

#endif
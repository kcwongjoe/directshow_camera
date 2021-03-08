#include <camera_resolution.h>

CameraResolution::CameraResolution(int width, int height, int bytePerPixel)
{
    m_width = width;
    m_height = height;
    m_bytePerPixel = bytePerPixel;
    m_numOfPixel = width * height;
    m_TotalByteSize = m_numOfPixel * bytePerPixel;
}

CameraResolution CameraResolution::Empty()
{
    return CameraResolution(0,0,0);
}

bool CameraResolution::isEmpty()
{
    if (m_width <= 0 || m_height <= 0 || m_bytePerPixel <= 0)
        return true;
    else
        return false;
}

int CameraResolution::getWidth()
{
    return m_width;
}

int CameraResolution::getHeight()
{
    return m_height;
}

int CameraResolution::getBytePerPixel()
{
    return m_bytePerPixel;
}

int CameraResolution::getNumOfPixel()
{
    return m_numOfPixel;
}

int CameraResolution::getTotalByteSize()
{
    return m_TotalByteSize;
}

int CameraResolution::getIndex(std::vector<CameraResolution>* resolutions, int width, int height)
{
    int result = -1;
    for (int i=0;i<resolutions->size();i++)
    {
        if (resolutions->at(i).getWidth() == width && resolutions->at(i).getHeight() == height)
        {
            result = i;
            break;
        }
    }

    return result;
}
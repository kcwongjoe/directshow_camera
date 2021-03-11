#include <camera_resolution.h>

namespace DirectShowCamera
{

    /**
     * @brief Constructor
     * 
     * @param width Width
     * @param height Height
     * @param bytePerPixel Number of bytes per pixel
     */
    CameraResolution::CameraResolution(int width, int height, int bytePerPixel)
    {
        m_width = width;
        m_height = height;
        m_bytePerPixel = bytePerPixel;
    }

    /**
     * @brief Return a empty CameraResolution
     * 
     * @return Return a empty CameraResolution 
     */
    CameraResolution CameraResolution::Empty()
    {
        return CameraResolution(0,0,0);
    }

    /**
     * @brief Check whether CameraResolution empty
     * 
     * @return Return true if it is empty
     */
    bool CameraResolution::isEmpty()
    {
        if (m_width <= 0 || m_height <= 0 || m_bytePerPixel <= 0)
            return true;
        else
            return false;
    }

    /**
     * @brief Get the width
     * 
     * @return Return the width in number of pixel
     */
    int CameraResolution::getWidth()
    {
        return m_width;
    }

    /**
     * @brief Get the height
     * 
     * @return Return the height in number of pixel
     */
    int CameraResolution::getHeight()
    {
        return m_height;
    }

    /**
     * @brief Return number of bytes per pixel
     * 
     * @return Return number of bytes per pixel
     */
    int CameraResolution::getBytePerPixel()
    {
        return m_bytePerPixel;
    }

    /**
     * @brief Return number of pixel
     * 
     * @return Return number of pixel
     */
    int CameraResolution::getNumOfPixel()
    {
        return m_width * m_height;
    }

    /**
     * @brief Return the total size of a frame in bytes
     * 
     * @return Return the total size of a frame in bytes
     */
    int CameraResolution::getTotalByteSize()
    {
        return m_width * m_height * m_bytePerPixel;
    }

    /**
     * @brief Get the index of the CameraResolution which match the width and height
     * 
     * @param[in] resolutions Resolution list to be search
     * @param[in] width Width to be matched
     * @param[in] height Height to be matched
     * @return Return the index
     */
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
}
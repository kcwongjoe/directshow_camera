#include "directshow_camera/ds_video_format.h"

#include "directshow_camera/utils/check_hresult_utils.h"

#include <algorithm>

namespace DirectShowCamera
{

#pragma region Static Function

    DirectShowVideoFormat DirectShowVideoFormat::Create(const AM_MEDIA_TYPE* amMediaType)
    {
        if (amMediaType->majortype == MEDIATYPE_Video)
        {
            // Get Video Type
            const auto videoType = amMediaType->subtype;
            const auto totalSize = amMediaType->lSampleSize;

            // Get the width and height
            VIDEOINFOHEADER* videoInfoHeader = reinterpret_cast<VIDEOINFOHEADER*>(amMediaType->pbFormat);
            const auto width = videoInfoHeader->bmiHeader.biWidth;
            const auto height = videoInfoHeader->bmiHeader.biHeight;
            const auto bitPerPixel = videoInfoHeader->bmiHeader.biBitCount;

            return DirectShowVideoFormat(videoType, width, height, bitPerPixel, totalSize);
        }
        else
        {
            return DirectShowVideoFormat();
        }
    }

    /**
     * @brief Sort and unique the video format list.
     * @param[in, out] directShowVideoFormats Input the video format list, and sort it.
    */
    void DirectShowVideoFormat::sortAndUnique(std::vector<DirectShowVideoFormat>* directShowVideoFormats)
    {
        // Remove duplicates
        std::vector<DirectShowVideoFormat> videoFormatTemp;
        for (int i = 0; i < directShowVideoFormats->size(); i++)
        {
            // Check
            bool found = false;
            for (int j = i+1; j < directShowVideoFormats->size(); j++)
            {
                if (directShowVideoFormats->at(i) == directShowVideoFormats->at(j))
                {
                    found = true;
                    break;
                }
            }

            // Add to temp
            if (!found)
            {
                videoFormatTemp.push_back(directShowVideoFormats->at(i));
            }
        }

        // Sort
        directShowVideoFormats->assign(videoFormatTemp.begin(), videoFormatTemp.end());
        std::sort(directShowVideoFormats->begin(), directShowVideoFormats->end());
    }

#pragma endregion Static Function

#pragma region constructor and destructor

    /**
     * @brief Constructor to create a empty DirectShowVideoFormat
    */
    DirectShowVideoFormat::DirectShowVideoFormat()
    {
        m_isEmpty = true;
    }

    /**
     * @brief Constructor
     * 
     * @param mediaType Media Type
     * @param width Width in pixel
     * @param height Height in pixel
     * @param bitPerPixel Bit per pixel
     * @param totalSize Total bytes
    */
    DirectShowVideoFormat::DirectShowVideoFormat(GUID mediaType, int width, int height, int bitPerPixel, int totalSize)
    {
        m_videoType = mediaType;
        m_totalSize = totalSize;
        m_width = width;
        m_height = height;
        m_bitPerPixel = bitPerPixel;

        m_isEmpty = false;
    }

    /**
     * @brief Destructor
    */
    DirectShowVideoFormat::~DirectShowVideoFormat()
    {

    }

#pragma endregion constructor and destructor

#pragma region Getter

    /**
     * @brief Return true if this is empty.
     * @return Return true if this is empty. 
    */
    bool DirectShowVideoFormat::isEmpty() const
    {
        return m_isEmpty;
    }

    /**
     * @brief Return the frame width
     * @return Return the frame width
    */
    int DirectShowVideoFormat::getWidth() const
    {
        return m_width;
    }

    /**
     * @brief Return the frame height
     * @return Return the frame height
    */
    int DirectShowVideoFormat::getHeight() const
    {
        return m_height;
    }

    /**
     * @brief Return the bit per pixel
     * @return Return the bit per pixel
    */
    int DirectShowVideoFormat::getBitPerPixel() const
    {
        return m_bitPerPixel;
    }

    /**
     * @brief Return the frame size in byte
     * @return Return the frame size in byte
    */
    long DirectShowVideoFormat::getTotalSize() const
    {
        return m_totalSize;
    }

    /**
     * @brief Return the video type.
     * @return Return the video type.
    */
    GUID DirectShowVideoFormat::getVideoType() const
    {
        return m_videoType;
    }

#pragma endregion Getter

#pragma region Operator

    /**
     * @brief Copy assignment operator
     * @param directShowVideoFormat DirectShowVideoFormat
     * @return 
    */
    DirectShowVideoFormat& DirectShowVideoFormat::operator=(const DirectShowVideoFormat& directShowVideoFormat)
    {
        if (this != &directShowVideoFormat)
        {
            if (!directShowVideoFormat.m_isEmpty)
            {
                m_videoType = directShowVideoFormat.m_videoType;
                m_bitPerPixel = directShowVideoFormat.m_bitPerPixel;
                m_totalSize = directShowVideoFormat.m_totalSize;
                m_width = directShowVideoFormat.m_width;
                m_height = directShowVideoFormat.m_height;

                m_isEmpty = false;
            }
            else
            {
                m_isEmpty = true;
            }
        }	

        return *this;
    }

    /**
     * @brief Compare two video format in size, compare width first, then height, then bit per pixel
     * @param videoFormat DirectShowVideoFormat
     * @return 
    */
    bool DirectShowVideoFormat::operator < (const DirectShowVideoFormat& videoFormat) const
    {
        if (m_isEmpty && videoFormat.m_isEmpty)
        {
            return false;
        }
        else if (m_isEmpty)
        {
            return true;
        }
        else if (videoFormat.m_isEmpty)
        {
            return false;
        }
        else
        {
            if (m_width < videoFormat.m_width)
            {
                return true;
            }
            else if (m_width == videoFormat.m_width)
            {
                if (m_height < videoFormat.m_height)
                {
                    // m_height < height
                    return true;
                }
                else if (m_height == videoFormat.m_height)
                {
                    // m_height == height
                    if (m_bitPerPixel < videoFormat.m_bitPerPixel)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }				
                }
                else
                {
                    // m_height > height
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
    }

    /**
     * @brief Compare two video format in size, compare width first, then height, then bit per pixel
     * @param videoFormat DirectShowVideoFormat
     * @return
    */
    bool DirectShowVideoFormat::operator > (const DirectShowVideoFormat& videoFormat) const
    {
        if (*this == videoFormat || *this < videoFormat)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    /**
     * @brief Equal to operator
     * @param videoFormat DirectShowVideoFormat
     * @return Return tru if equal
    */
    bool DirectShowVideoFormat::operator == (const DirectShowVideoFormat& videoFormat) const
    {
        if (m_isEmpty && videoFormat.m_isEmpty)
        {
            return true;
        }
        else if (m_isEmpty || videoFormat.m_isEmpty)
        {
            return false;
        }
        else
        {
            if (m_width == videoFormat.m_width && m_height == videoFormat.m_height && m_videoType == videoFormat.m_videoType)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    /**
     * @brief Not equal to operator
     * @param videoFormat DirectShowVideoFormat
     * @return Return tru if equal
    */
    bool DirectShowVideoFormat::operator != (const DirectShowVideoFormat& videoFormat) const
    {
        if (*this == videoFormat)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    /**
     * @brief Equal to operator
     * @param am_MediaType AM_MEDIA_TYPE
     * @return Return true if equal
    */
    bool DirectShowVideoFormat::operator == (const AM_MEDIA_TYPE& am_MediaType) const
    {
        if (m_isEmpty)
        {
            return true;
        }
        else
        {
            // Get the width and height
            VIDEOINFOHEADER* videoInfoHeader = reinterpret_cast<VIDEOINFOHEADER*>(am_MediaType.pbFormat);
            int mediaTypeWidth = videoInfoHeader->bmiHeader.biWidth;
            int mediaTypeHeight = videoInfoHeader->bmiHeader.biHeight;

            if (m_width == mediaTypeWidth && m_height == mediaTypeHeight && m_videoType == am_MediaType.subtype)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    /**
     * @brief Not equal to operator
     * @param am_MediaType AM_MEDIA_TYPE
     * @return Return true if equal
    */
    bool DirectShowVideoFormat::operator != (const AM_MEDIA_TYPE& am_MediaType) const
    {
        if (*this == am_MediaType)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

#pragma endregion Operator
}

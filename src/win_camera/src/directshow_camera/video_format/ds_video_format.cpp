/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#include "directshow_camera/video_format/ds_video_format.h"

#include "directshow_camera/utils/check_hresult_utils.h"

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

#pragma endregion Static Function

#pragma region constructor and destructor

    DirectShowVideoFormat::DirectShowVideoFormat()
    {
        m_isEmpty = true;
    }

    DirectShowVideoFormat::DirectShowVideoFormat(GUID mediaType, int width, int height, int bitPerPixel, int totalSize)
    {
        m_videoType = mediaType;
        m_totalSize = totalSize;
        m_width = width;
        m_height = height;
        m_bitPerPixel = bitPerPixel;

        m_isEmpty = false;
    }

#pragma endregion constructor and destructor

#pragma region Getter

    bool DirectShowVideoFormat::isEmpty() const
    {
        return m_isEmpty;
    }

    int DirectShowVideoFormat::getWidth() const
    {
        return m_width;
    }

    int DirectShowVideoFormat::getHeight() const
    {
        return m_height;
    }

    int DirectShowVideoFormat::getBitPerPixel() const
    {
        return m_bitPerPixel;
    }

    long DirectShowVideoFormat::getTotalSize() const
    {
        return m_totalSize;
    }

    GUID DirectShowVideoFormat::getVideoType() const
    {
        return m_videoType;
    }

#pragma endregion Getter

#pragma region Operator

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

    bool DirectShowVideoFormat::operator == (const AM_MEDIA_TYPE& am_MediaType) const
    {
        if (m_isEmpty)
        {
            return true;
        }
        else
        {
            const auto videoFormat = Create(&am_MediaType);

            if (*this == videoFormat)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }

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

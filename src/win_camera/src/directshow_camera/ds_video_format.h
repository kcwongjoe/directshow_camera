#pragma once
#ifndef WIN_CAMERA__DIRECTSHOW_CAMERA__DIRECTSHOW_VIDEO_FORMAT_H
#define WIN_CAMERA__DIRECTSHOW_CAMERA__DIRECTSHOW_VIDEO_FORMAT_H

//************Content************

#include "directshow_camera/utils/ds_video_format_utils.h"

namespace DirectShowCamera
{
    /**
     * @brief A class to store the Directshow Video Format information
     * 
     */
    class DirectShowVideoFormat
    {
    // Static function
    public:

        static DirectShowVideoFormat Create(const AM_MEDIA_TYPE* amMediaType);

        static void sortAndUnique(std::vector<DirectShowVideoFormat>* directShowVideoFormats);

    public:

        // Constuctor
        DirectShowVideoFormat();
        DirectShowVideoFormat(GUID mediaType, int width, int height, int bitPerPixel, int totalSize);

        ~DirectShowVideoFormat();

        // Getter
        bool isEmpty() const;
        int getWidth() const;
        int getHeight() const;
        int getBitPerPixel() const;
        long getTotalSize() const;
        GUID getVideoType() const;

        // Operator

        DirectShowVideoFormat& operator=(const DirectShowVideoFormat& directShowVideoFormat);

        bool operator < (const DirectShowVideoFormat& videoFormat) const;
        bool operator > (const DirectShowVideoFormat& videoFormat) const;

        bool operator == (const DirectShowVideoFormat& videoFormat) const;
        bool operator == (const AM_MEDIA_TYPE& am_MediaType) const;

        bool operator != (const DirectShowVideoFormat& videoFormat) const;
        bool operator != (const AM_MEDIA_TYPE& am_MediaType) const;

        // To string

        operator std::string() const
        {
            std::string result;
            if (!m_isEmpty)
            {
                result = "(" + std::to_string(m_width) + " x " + std::to_string(m_height) + " x " + std::to_string(m_bitPerPixel) + "bits )" + DirectShowVideoFormatUtils::ToString(m_videoType);
            }
            else
            {
                result = "Empty video format.";
            }

            return result;
        }

        friend std::ostream& operator << (std::ostream& out, const DirectShowVideoFormat& obj) {
            return out << (std::string)obj;
        }

    private:
        GUID m_videoType = MEDIASUBTYPE_None;
        int m_bitPerPixel = 0;
        long m_totalSize = 0;
        int m_width = 0;
        int m_height = 0;
        bool m_isEmpty = true;

    };
}

//*******************************

#endif

/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef WIN_CAMERA__DIRECTSHOW_CAMERA__VIDEO_FORMAT__DIRECTSHOW_VIDEO_FORMAT_H
#define WIN_CAMERA__DIRECTSHOW_CAMERA__VIDEO_FORMAT__DIRECTSHOW_VIDEO_FORMAT_H

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

        /**
        * @brief Create a DirectShowVideoFormat object from a AM_MEDIA_TYPE object
        * 
        * @param[in] amMediaType The AM_MEDIA_TYPE object
        * 
        * @return The DirectShowVideoFormat object
        */
        static DirectShowVideoFormat Create(const AM_MEDIA_TYPE* amMediaType);

    public:

        // Constuctor

        /**
         * @brief Constructor to create a empty DirectShowVideoFormat
        */
        DirectShowVideoFormat();

        /**
         * @brief Constructor
         *
         * @param[in] mediaType Media Type
         * @param[in] width Width in pixel
         * @param[in] height Height in pixel
         * @param[in] bitPerPixel Bit per pixel
         * @param[in] totalSize Total bytes
        */
        DirectShowVideoFormat(
            const GUID mediaType,
            const int width,
            const int height,
            const int bitPerPixel,
            const int totalSize
        );

        // Getter

        /**
         * @brief Return true if this is empty.
         * @return Return true if this is empty.
        */
        bool isEmpty() const;

        /**
         * @brief Return the frame width
         * @return Return the frame width
        */
        int getWidth() const;

        /**
         * @brief Return the frame height
         * @return Return the frame height
        */
        int getHeight() const;

        /**
         * @brief Return the bit per pixel
         * @return Return the bit per pixel
        */
        int getBitPerPixel() const;

        /**
         * @brief Return the frame size in byte
         * @return Return the frame size in byte
        */
        long getTotalSize() const;

        /**
         * @brief Return the video type.
         * @return Return the video type.
        */
        GUID getVideoType() const;

        // Operator

        /**
         * @brief Copy assignment operator
         * @param[in] directShowVideoFormat DirectShowVideoFormat
         * @return
        */
        DirectShowVideoFormat& operator=(const DirectShowVideoFormat& directShowVideoFormat);

        /**
         * @brief Compare two video format in size, compare width first, then height, then bit per pixel
         * @param[in] videoFormat DirectShowVideoFormat
         * @return
        */
        bool operator < (const DirectShowVideoFormat& videoFormat) const;

        /**
         * @brief Compare two video format in size, compare width first, then height, then bit per pixel
         * @param[in] videoFormat DirectShowVideoFormat
         * @return
        */
        bool operator > (const DirectShowVideoFormat& videoFormat) const;

        /**
         * @brief Equal to operator
         * @param[in] videoFormat DirectShowVideoFormat
         * @return Return true if equal
        */
        bool operator == (const DirectShowVideoFormat& videoFormat) const;

        /**
         * @brief Equal to operator
         * @param[in] am_MediaType AM_MEDIA_TYPE
         * @return Return true if equal
        */
        bool operator == (const AM_MEDIA_TYPE& am_MediaType) const;

        /**
         * @brief Not equal to operator
         * @param[in] videoFormat DirectShowVideoFormat
         * @return Return tru if equal
        */
        bool operator != (const DirectShowVideoFormat& videoFormat) const;

        /**
         * @brief Not equal to operator
         * @param[in] am_MediaType AM_MEDIA_TYPE
         * @return Return true if equal
        */
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

/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#pragma once
#ifndef WIN_CAMERA__DIRECTSHOW_CAMERA__UTILS__DIRECTSHOW_VIDEO_FORMAT_UTILS_H
#define WIN_CAMERA__DIRECTSHOW_CAMERA__UTILS__DIRECTSHOW_VIDEO_FORMAT_UTILS_H

//************Content************
#include "directshow_camera/ds_header.h"

namespace DirectShowVideoFormatUtils
{
    /**
     * @brief Convert AM_MEDIA_TYPE to string
     * @param[in] amMediaType AM_MEDIA_TYPE
     * @return Return th string
    */
    std::string ToString(AM_MEDIA_TYPE* amMediaType);

    /**
     * @brief Convert Media type and subtype GUID to description.
     * @param[in] guid GUID
     * @return Return the description
    */
    std::string ToString(GUID guid);

    /**
    * @brief Convert KsTopologyInfo nodetype GUID to string
    * @param[in] guid GUID
    * @return Return the description
    */
    std::string IKsTopologyInfoNodeToString(GUID guid);

    /**
     * @brief Get the supported monochrome media subtype. Order by the less convertion.
     * @return Return the monochrome list
    */
    std::vector<GUID> SupportMonochromeSubType();

    /**
     * @brief Return true if the media subtype is a monochrome type.
     * @param[in] guid Media subtype
     * @return Return true if the media subtype is a monochrome type.
    */
    bool isMonochrome(GUID guid);

    /**
     * @brief Get the supported RGB media subtype. Order by the less convertion.
     * @return Return the RGB list
    */
    std::vector<GUID> SupportRGBSubType();

    /**
     * @brief Return true if the media subtype can be converted to RGB in directshow filter
     * @param[in] guid Media subtype
     * @return
    */
    bool isSupportRGBConvertion(GUID guid);
}

//*******************************

#endif
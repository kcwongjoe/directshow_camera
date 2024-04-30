/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef DIRECTSHOW_CAMERA__DIRECTSHOW_CAMERA__UTILS__WIN32_UTILS_H
#define DIRECTSHOW_CAMERA__DIRECTSHOW_CAMERA__UTILS__WIN32_UTILS_H

//************Content************

#include <atlconv.h>
#include <string>

namespace Win32Utils
{

    /**
     * @brief Convert BSTR to string
     * @param[in] bstr BSTR
     * @param[in] cp (Optional) CodePage, Default as CP_UTF8
     * @return Return string
    */
    std::string BSTRToString(const BSTR bstr, const int cp = CP_UTF8);

    /**
     * @brief Casting GUID to string
     * @param[in] guid GUID
     * @return Return GUID string {}
    */
    std::string ToString(const GUID guid);
}

//*******************************

#endif
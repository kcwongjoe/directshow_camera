#pragma once
#ifndef WIN_CAMERA__DIRECTSHOW_CAMERA__UTILS__WIN32_UTILS_H
#define WIN_CAMERA__DIRECTSHOW_CAMERA__UTILS__WIN32_UTILS_H

#include <atlconv.h>

//************Content************

namespace Win32Utils
{

    /**
     * @brief Convert BSTR to string
     * @param[in] bstr BSTR
     * @param[in] cp (Option) CodePage, Default as CP_UTF8
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
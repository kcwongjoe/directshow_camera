/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#pragma once
#ifndef WIN_CAMERA__UTILS__STRING_UTILS_H
#define WIN_CAMERA__UTILS__STRING_UTILS_H

//************Content************

#include <string>

namespace Utils
{
    class StringUtils
    {
    public:

        /**
        * Convert a string to lowercase
        * @param[in,out] str The string to be converted
        */
        static void ToLowercase(std::string& str);

        /**
        * Trim the left of a string
        * @param[in,out] str The string to be trimmed
        */
        static void LeftTrim(std::string& str);

        /**
        * Trim the right of a string
        * @param[in,out] str The string to be trimmed
        */
        static void RightTrim(std::string& str);

        /**
        * Trim the left and right of a string
        * @param[in,out] str The string to be trimmed
        */
        static void Trim(std::string& str);
    };
}

//*******************************

#endif // ndef WIN_CAMERA__UTILS__STRING_UTILS_H
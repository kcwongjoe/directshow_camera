/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "utils/string_utils.h"

#include <algorithm>

namespace Utils
{
    void StringUtils::ToLowercase(std::string& str)
    {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    }

    void StringUtils::LeftTrim(std::string& str)
    {
        str.erase(
            str.begin(),
            std::find_if(
                str.begin(),
                str.end(), 
                [](unsigned char ch)
                {
                    return !std::isspace(ch);
                }
            )
        );
    }
    void StringUtils::RightTrim(std::string& str)
    {
        str.erase(
            std::find_if(
                str.rbegin(), 
                str.rend(), 
                [](unsigned char ch)
                {
                    return !std::isspace(ch);
                }
            ).base(), 
            str.end()
        );
    }

    void StringUtils::Trim(std::string& str)
    {
        LeftTrim(str);
        RightTrim(str);
    }
}
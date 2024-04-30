/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef DIRECTSHOW_CAMERA__UTILS__PATH_UTILS_H
#define DIRECTSHOW_CAMERA__UTILS__PATH_UTILS_H

//************Content************

#include <string>
#include <filesystem>

namespace Utils
{
    class PathUtils
    {
    public:

        /**
        * @brief Get the extension of the file
        * @param path The path of the file
        * @return The extension of the file in lower case
        */
        static std::string getExtension(std::filesystem::path path);
    };
}

//*******************************

#endif // ndef DIRECTSHOW_CAMERA__UTILS__PATH_UTILS_H
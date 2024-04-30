/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef DIRECTSHOW_CAMERA__DIRECTSHOW_CAMERA__UTILS__COM_LIB_UTILS_H
#define DIRECTSHOW_CAMERA__DIRECTSHOW_CAMERA__UTILS__COM_LIB_UTILS_H

//************Content************

#include <string>

namespace COMLibUtils
{
    class COMLibUtils
    {
    public:
        /**
         * @brief Initialize COM library
         * 
         * @param[out] errorString Error message
         *
         * @return Return the status of initialized.
        */
        static bool InitCOMLib(std::string& errorString);

        /**
         * @brief Uninitialize COM library
        */
        static void UninitCOMLib();

        static bool s_isInitializedCOMLib;
    };
}

//*******************************

#endif
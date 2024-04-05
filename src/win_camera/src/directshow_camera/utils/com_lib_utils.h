#pragma once
#ifndef WIN_CAMERA__DIRECTSHOW_CAMERA__UTILS__COM_LIB_UTILS_H
#define WIN_CAMERA__DIRECTSHOW_CAMERA__UTILS__COM_LIB_UTILS_H

//************Content************

namespace COMLibUtils
{
    static bool s_isInitializedCOMLib = false;

    class COMLibUtils
    {
    public:
        /**
         * @brief Initialize COM library
         *
         * @return Return the status of initialized.
        */
        static bool InitCOMLib();

        /**
         * @brief Uninitialize COM library
        */
        static void UninitCOMLib();
    };
}

//*******************************

#endif
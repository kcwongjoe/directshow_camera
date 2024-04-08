#pragma once
#ifndef WIN_CAMERA__DIRECTSHOW_CAMERA__UTILS__COM_LIB_UTILS_H
#define WIN_CAMERA__DIRECTSHOW_CAMERA__UTILS__COM_LIB_UTILS_H

//************Content************

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
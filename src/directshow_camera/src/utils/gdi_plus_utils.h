/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef DIRECTSHOW_CAMERA__UTILS__GDI_PLUS_UTILS_H
#define DIRECTSHOW_CAMERA__UTILS__GDI_PLUS_UTILS_H

//************Content************

#include <windows.h>
#include <gdiplus.h>

#pragma comment( lib, "gdiplus.lib" )
#pragma warning( disable : 4018 )
#pragma warning( disable : 4996 )

#include <string>
#include <exception>

namespace Utils
{

#pragma region GDIPlusUtils

    class GDIPLUSUtils
    {
    public:

        /**
        * @brief Start GDIPlus
        *
        * @return Gdiplus::Status
        */
        static Gdiplus::Status StartGDIPlus();

        static void DrawBitmap(
            Gdiplus::Bitmap& bitmap,
            const unsigned char* image,
            const long imageSize
        );

        /**
        * @brief Get PNG encoder CLSID
        * @param[out] pClsid CLSID
        * @return bool True if success
        */
        static bool GetPngEncoderClsid(CLSID& pClsid);

        /**
        * @brief Get JPEG encoder CLSID
        * @param[out] pClsid CLSID
        * @return bool True if success
        */
        static bool GetJpegEncoderClsid(CLSID& pClsid);

        /**
        * @brief Get BMP encoder CLSID
        * @param[out] pClsid CLSID
        * @return bool True if success
        */
        static bool GetBmpEncoderClsid(CLSID& pClsid);

        /**
        * @brief Get TIFF encoder CLSID
        * @param[out] pClsid CLSID
        * @return bool True if success
        */
        static bool GetTiffEncoderClsid(CLSID& pClsid);

        /**
        * @brief Get image encoder CLSID        * 
        * @param[out] pClsid CLSID
        * @param[in] format Format, such as "image/png"        * 
        * @return bool True if success
        */
        static bool GetImageEncoderClsid(CLSID& pClsid, const WCHAR* format);

        /**
        * @brief Stop GDIPlus
        */
        static void StopGDIPlus();

    private:
        static ULONG_PTR s_gdiplusToken;

        /**
        * Count the number of GDI+ start has been called. Called StartGDIPlus(), +1. Called StopGDIPlus(), -1.
        * So that GDIPLUSUtils will works on multiThread. If thread 1 call StartGDIPlus() and thread 2 call StartGDIPlus().
        * Then GDI+ will not stopped even thread 2 call StopGDIPlus(). It will only stop until thread 1 call StopGDIPlus() as well.
        */
        static int s_gdiplusCount;

    };

#pragma endregion GDIPlusUtils

#pragma region GDIPlusException

    /**
         * @class GDIPlusException
         * @brief GDIPlusException indicates an error in GDI+ library
        */
    class GDIPlusException : public std::exception
    {
    public:

        /**
         * @brief Constructor. throw a error in the string format of "errorMessage + : + str(status)"
         * @param errorMessage The error message
         * @param status The Gdiplus::Status
        */
        GDIPlusException(
            const std::string errorMessage,
            const Gdiplus::Status status
        );

        /**
         * @brief Get the error message
        */
        std::string getErrorMessage() const;

        const char* what() const noexcept override;

    private:
        /**
        * @brief Convert Gdiplus::Status to string
        * @param status Gdiplus::Status
        * @return std::string
        */
        std::string StatusToString(const Gdiplus::Status status) const;

    private:
        std::string m_errorMessage;
    };

#pragma endregion GDIPlusException
}

//*******************************

#endif
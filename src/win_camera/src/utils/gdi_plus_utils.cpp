/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#include "utils/gdi_plus_utils.h"

namespace Utils
{

#pragma region GDIPlusUtils

    // Initialize the static variable
    ULONG_PTR GDIPLUSUtils::s_gdiplusToken = NULL;

    Gdiplus::Status Utils::GDIPLUSUtils::StartGDIPlus()
    {
        if (s_gdiplusToken == NULL)
        {
            // Start GDI+
            Gdiplus::GdiplusStartupInput gdiplusStartupInput;
            const auto status = Gdiplus::GdiplusStartup(&s_gdiplusToken, &gdiplusStartupInput, NULL);
            return status;
        }
        else
        {
            // GDI+ has already been started
            return Gdiplus::Ok;
        }

    }

    void GDIPLUSUtils::DrawBitmap(Gdiplus::Bitmap& bitmap, const unsigned char* image, const long imageSize)
    {
        // Get bitmap information
        const auto width = bitmap.GetWidth();
        const auto height = bitmap.GetHeight();
        const auto pixelFormat = bitmap.GetPixelFormat();

        // Draw image into bitmap
        Gdiplus::BitmapData bitmapData;
        Gdiplus::Rect rect(0, 0, width, height);
        bitmap.LockBits(&rect, Gdiplus::ImageLockModeWrite, pixelFormat, &bitmapData);
        memcpy(bitmapData.Scan0, image, imageSize);
        bitmap.UnlockBits(&bitmapData);
    }

    bool GDIPLUSUtils::GetPngEncoderClsid(CLSID& pClsid)
    {
        return GetImageEncoderClsid(pClsid, L"image/png");
    }

    bool GDIPLUSUtils::GetJpegEncoderClsid(CLSID& pClsid)
    {
        return GetImageEncoderClsid(pClsid, L"image/jpeg");
    }

    bool GDIPLUSUtils::GetBmpEncoderClsid(CLSID& pClsid)
    {
        return GetImageEncoderClsid(pClsid, L"image/bmp");
    }

    bool GDIPLUSUtils::GetTiffEncoderClsid(CLSID& pClsid)
    {
        return GetImageEncoderClsid(pClsid, L"image/tiff");
    }

    bool GDIPLUSUtils::GetImageEncoderClsid(CLSID& pClsid, const WCHAR* format)
    {
        // Get the number of image encoders
        UINT num = 0;          // number of image encoders
        UINT size = 0;         // size of the image encoder array in bytes
        Gdiplus::GetImageEncodersSize(&num, &size);
        if (size == 0)
            return false;

        // Create image encoder array
        Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
        pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
        if (pImageCodecInfo == NULL)
            return false;

        // Get image encoders
        GetImageEncoders(num, size, pImageCodecInfo);

        // Find the encoder
        for (UINT j = 0; j < num; ++j)
        {
#pragma warning(push)
#pragma warning(disable:6385)
            if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
#pragma warning(pop)
            {
                pClsid = pImageCodecInfo[j].Clsid;
                free(pImageCodecInfo);
                return true;
            }
        }

        free(pImageCodecInfo);
        return false;
    }

    void GDIPLUSUtils::StopGDIPlus()
    {
        if (s_gdiplusToken != NULL)
        {
            Gdiplus::GdiplusShutdown(s_gdiplusToken);
        }
    }

#pragma endregion GDIPlusUtils

#pragma region GDIPlusException

    GDIPlusException::GDIPlusException(
        const std::string errorMessage,
        const Gdiplus::Status status
    )
    {
        m_errorMessage = errorMessage + " : " + StatusToString(status);
    }

    std::string GDIPlusException::getErrorMessage() const
    {
        return m_errorMessage;
    }

    const char* GDIPlusException::what() const noexcept
    {
        return m_errorMessage.c_str();
    }

    std::string GDIPlusException::StatusToString(const Gdiplus::Status status) const
    {
        if (status == Gdiplus::Status::GenericError)
        {
            return "GenericError";
        }
        else if (status == Gdiplus::Status::InvalidParameter)
        {
            return "InvalidParameter";
        }
        else if (status == Gdiplus::Status::OutOfMemory)
        {
            return "OutOfMemory";
        }
        else if (status == Gdiplus::Status::ObjectBusy)
        {
            return "ObjectBusy";
        }
        else if (status == Gdiplus::Status::InsufficientBuffer)
        {
            return "InsufficientBuffer";
        }
        else if (status == Gdiplus::Status::NotImplemented)
        {
            return "NotImplemented";
        }
        else if (status == Gdiplus::Status::Win32Error)
        {
            return "Win32Error";
        }
        else if (status == Gdiplus::Status::WrongState)
        {
            return "WrongState";
        }
        else if (status == Gdiplus::Status::Aborted)
        {
            return "Aborted";
        }
        else if (status == Gdiplus::Status::FileNotFound)
        {
            return "FileNotFound";
        }
        else if (status == Gdiplus::Status::ValueOverflow)
        {
            return "ValueOverflow";
        }
        else if (status == Gdiplus::Status::AccessDenied)
        {
            return "AccessDenied";
        }
        else if (status == Gdiplus::Status::UnknownImageFormat)
        {
            return "UnknownImageFormat";
        }
        else if (status == Gdiplus::Status::FontFamilyNotFound)
        {
            return "FontFamilyNotFound";
        }
        else if (status == Gdiplus::Status::FontStyleNotFound)
        {
            return "FontStyleNotFound";
        }
        else if (status == Gdiplus::Status::NotTrueTypeFont)
        {
            return "NotTrueTypeFont";
        }
        else if (status == Gdiplus::Status::UnsupportedGdiplusVersion)
        {
            return "UnsupportedGdiplusVersion";
        }
        else if (status == Gdiplus::Status::GdiplusNotInitialized)
        {
            return "GdiplusNotInitialized";
        }
        else if (status == Gdiplus::Status::PropertyNotFound)
        {
            return "PropertyNotFound";
        }
        else if (status == Gdiplus::Status::PropertyNotSupported)
        {
            return "PropertyNotSupported";
        }
#if (GDIPVER >= 0x0110)
        else if (status == Gdiplus::Status::ProfileNotFound)
        {
            return "ProfileNotFound";
        }
#endif //(GDIPVER >= 0x0110)
        else
        {
            return "Unknown Error";
        }
    }

#pragma endregion GDIPlusException
}
#include "exceptions/camera_not_opened_exception.h"

namespace WinCamera
{
    CameraNotOpenedException::CameraNotOpenedException() :
        m_errorMessage("Camera is not opened.")
    {

    }

    std::string CameraNotOpenedException::GetErrorMessage() const
    {
        return m_errorMessage;
    }

    const char* CameraNotOpenedException::what() const noexcept
    {
        return m_errorMessage.c_str();
    }
}
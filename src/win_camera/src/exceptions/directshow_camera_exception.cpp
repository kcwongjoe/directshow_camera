#include "exceptions/directshow_camera_exception.h"

namespace WinCamera
{
    DriectShowCameraException::DriectShowCameraException(
        const std::string errorMessage
    ) :
        m_errorMessage(errorMessage)
    {
    }

    std::string DriectShowCameraException::getErrorMessage() const
    {
        return m_errorMessage;
    }

    const char* DriectShowCameraException::what() const noexcept
    {
        return m_errorMessage.c_str();
    }
}
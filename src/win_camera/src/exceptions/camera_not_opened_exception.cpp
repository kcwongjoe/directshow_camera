/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "exceptions/camera_not_opened_exception.h"

namespace WinCamera
{
    CameraNotOpenedException::CameraNotOpenedException() :
        m_errorMessage("Camera is not opened.")
    {

    }

    CameraNotOpenedException::CameraNotOpenedException(std::string codeLocation) :
        m_errorMessage("In the " + codeLocation + ", Camera is not opened.")
    {
    }

    std::string CameraNotOpenedException::getErrorMessage() const
    {
        return m_errorMessage;
    }

    const char* CameraNotOpenedException::what() const noexcept
    {
        return m_errorMessage.c_str();
    }
}
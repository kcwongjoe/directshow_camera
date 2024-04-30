/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "exceptions/directshow_camera_exception.h"

namespace DirectShowCamera
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
/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "exceptions/resolution_not_support_exception.h"

namespace WinCamera
{
    ResolutionNotSupportException::ResolutionNotSupportException(
        const std::string& cameraName,
        const int width,
        const int height,
        const bool isRGB
    ) :
        m_cameraName(cameraName),
        m_width(width),
        m_height(height),
        m_isRGB(isRGB)
    {
        if (isRGB)
        {
            m_errorMessage = "Camera " + cameraName + " does not supported resolution (" + std::to_string(width) + " x " + std::to_string(height) + " x 3).";
        }
        else
        {
            m_errorMessage = "Camera " + cameraName + " does not supported resolution (" + std::to_string(width) + " x " + std::to_string(height) + " x 1).";
        }
    }

    std::string ResolutionNotSupportException::getErrorMessage() const
    {
        return m_errorMessage;
    }

    const char* ResolutionNotSupportException::what() const noexcept
    {
        return m_errorMessage.c_str();
    }
}
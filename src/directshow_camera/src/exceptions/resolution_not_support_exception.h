/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef DIRECTSHOW_CAMERA__EXCEPTIONS__RESOLUTION_NOT_SUPPORT_EXCEPTION_H
#define DIRECTSHOW_CAMERA__EXCEPTIONS__RESOLUTION_NOT_SUPPORT_EXCEPTION_H

//************Content************

#include <exception>
#include <string>

namespace DirectShowCamera
{
    /**
     * @class ResolutionNotSupportException
     * @brief ResolutionNotSupportException indicates an error that the resolution is not supported by the camera
    */
    class ResolutionNotSupportException : public std::exception
    {
    public:

        /**
         * @brief Constructor
         * @param propertyName The name of the property that is not supported
        */
        ResolutionNotSupportException(
            const std::string& cameraName,
            const int width,
            const int height,
            const bool isRGB
        );

        /**
         * @brief Get the error message
        */
        std::string getErrorMessage() const;

        const char* what() const noexcept override;
    private:
        std::string m_cameraName;
        int m_width;
        int m_height;
        bool m_isRGB;
        std::string m_errorMessage;
    };
}


//*******************************

#endif
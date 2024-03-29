#pragma once
#ifndef WIN_CAMERA__EXCEPTIONS__RESOLUTION_NOT_SUPPORT_EXCEPTION_H
#define WIN_CAMERA__EXCEPTIONS__RESOLUTION_NOT_SUPPORT_EXCEPTION_H

#pragma once

#include <exception>

namespace WinCamera
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
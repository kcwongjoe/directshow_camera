#pragma once
#ifndef WIN_CAMERA__EXCEPTIONS__CAMERA_NOT_OPEN_EXCEPTION_H
#define WIN_CAMERA__EXCEPTIONS__CAMERA_NOT_OPEN_EXCEPTION_H

#pragma once

#include <exception>

namespace WinCamera
{
    /**
     * @class CameraNotOpenedException
     * @brief CameraNotOpenedException indicates an error that a property is not supported by the camera
    */
    class CameraNotOpenedException : public std::exception
    {
    public:

        /**
         * @brief Constructor
        */
        CameraNotOpenedException();

        /**
         * @brief Get the error message
        */
        std::string getErrorMessage() const;

        const char* what() const noexcept override;

    private:
        std::string m_errorMessage;

    };
}


//*******************************

#endif
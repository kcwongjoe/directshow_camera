/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef DIRECTSHOW_CAMERA__EXCEPTIONS__CAMERA_NOT_OPEN_EXCEPTION_H
#define DIRECTSHOW_CAMERA__EXCEPTIONS__CAMERA_NOT_OPEN_EXCEPTION_H

//************Content************

#include <exception>
#include <string>

namespace DirectShowCamera
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
        * @brief Constructor
        * @param codeLocation The location where the exception is thrown. This will be added to the error message.
        */
        CameraNotOpenedException(std::string codeLocation);

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
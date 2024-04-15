/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#pragma once
#ifndef WIN_CAMERA__EXCEPTIONS__DIRECTSHOW_CAMERA_EXCEPTION_H
#define WIN_CAMERA__EXCEPTIONS__DIRECTSHOW_CAMERA_EXCEPTION_H

//************Content************

#include <exception>
#include <string>

namespace WinCamera
{
    /**
     * @class DriectShowCameraException
     * @brief DriectShowCameraException indicates an error from the DirectShow camera
    */
    class DriectShowCameraException : public std::exception
    {
    public:

        /**
         * @brief Constructor
         * @param errorMessage The error message
        */
        DriectShowCameraException(
            const std::string errorMessage
        );

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
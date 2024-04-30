/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef DIRECTSHOW_CAMERA__DIRECTSHOW_CAMERA__CAMERA__I_MEDIA_CONTROL_HANDLER_H
#define DIRECTSHOW_CAMERA__DIRECTSHOW_CAMERA__CAMERA__I_MEDIA_CONTROL_HANDLER_H

#include "directshow_camera/ds_header.h"

#include <string>

//************Content************


namespace DirectShowCamera
{
    /**
     * @brief A handler for IMediaControl
     * 
     */
    class IMediaControlHandler
    {
    public:

#pragma region Constructor and Destructor

        /**
         * @brief Constructor
        */
        IMediaControlHandler();

        /**
         * @brief Desctructor
        */
        ~IMediaControlHandler();

        /**
         * @brief Release the media control
        */
        void Release();

#pragma endregion Constructor and Destructor

#pragma region Media Control

        /**
        * @brief Create a media control object
        * 
        * @param[in] graphBuilder The graph builder
        * @param[out] errorMessage Error message
        * 
        * @return Return true if the media control object is created
        */
        bool CreateMediaControl(IGraphBuilder* graphBuilder, std::string& errorMessage);

        /**
        * @brief Run the media control
        * 
        * @param[out] errorMessage Error message
        */
        bool Run(std::string& errorMessage);

        /**
        * @brief Pause the media control
        * 
        * @param[out] errorMessage Error message
        */
        bool Pause(std::string& errorMessage);

        /**
        * @brief Stop the media control
        * 
        * @param[out] errorMessage Error message
        */
        bool Stop(std::string& errorMessage);

#pragma endregion Media Control

    private:

        /**
        * @breif Wait for the completion of the media control operation
        * 
        * @param[in] succeedState The state of the media control to be completed
        * @param[in] processStr The processing string which will be shown in the error message
        * @param[out] errorMessage Error message
        * 
        * @return Return true if the media control operation is completed
        */
        bool WaitForCompletion(
            const FILTER_STATE succeedState,
            const std::string processStr,
            std::string& errorMessage
        );

    private:
        IMediaControl* m_mediaControl = NULL;
    };
}


//*******************************

#endif
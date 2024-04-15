/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "directshow_camera/camera/i_media_control_handler.h"

#include <directshow_camera/utils/ds_camera_utils.h>

#include <chrono>
#include <thread>

namespace DirectShowCamera
{
    
#pragma region Constructor and Destructor

    IMediaControlHandler::IMediaControlHandler()
    {
        
    }

    IMediaControlHandler::~IMediaControlHandler()
    {
        Release();
    }

    void IMediaControlHandler::Release()
    {
        // Release media control
        DirectShowCameraUtils::SafeRelease(&m_mediaControl);
    }

#pragma endregion Constructor and Destructor

#pragma region Media Control

    bool IMediaControlHandler::CreateMediaControl(IGraphBuilder* graphBuilder, std::string& errorMessage)
    {
        const auto hr = graphBuilder->QueryInterface(IID_IMediaControl, (void**)&m_mediaControl);
        if (hr != S_OK)
        {
            if (hr == E_NOINTERFACE)
            {
                errorMessage = "Could not create media control object(Interface is not supported).";
            }
            else if (hr == E_POINTER)
            {
                errorMessage = "Could not create media control object(The media control address is NULL).";
            }
            else
            {
                errorMessage = "Could not create media control object(hr = " + std::to_string(hr) + ").";
            }

            return false;
        }

        return true;
    }

    bool IMediaControlHandler::Run(std::string& errorMessage)
    {
        // Run
        HRESULT hr = m_mediaControl->Run();

        // Check succeeded
        if (!SUCCEEDED(hr))
        {
            errorMessage = "Could not start the graph (hr = " + std::to_string(hr) + ").";
            return false;
        }

        if (hr == S_FALSE)
        {
            // The graph is preparing to run, but some filters have not completed the transition to a running state.
            // Wait for the filters to complete the transition to a running state.
            return WaitForCompletion(State_Running, "IMediaControl::Run", errorMessage);
        }

        return true;
    }

    bool IMediaControlHandler::Pause(std::string& errorMessage)
    {
        // Run
        HRESULT hr = m_mediaControl->Pause();

        // Check succeeded
        if (!SUCCEEDED(hr))
        {
            errorMessage = "Could not pause media contol.(hr = " + std::to_string(hr) + ").";
            return false;
        }

        if (hr == S_FALSE)
        {
            // The graph paused successfully, but some filters have not completed the state transition.
            // Wait for the filters to complete the transition to a pause state.
            return WaitForCompletion(State_Paused, "IMediaControl::Pause", errorMessage);
        }

        return true;
    }

    bool IMediaControlHandler::Stop(std::string& errorMessage)
    {
        // Run
        HRESULT hr = m_mediaControl->Stop();

        // The state transition might be asynchronous. If the method returns before the transition completes, the return value is S_FALSE.
        // Ignore S_FALSE here and not wait for completion. As we do nothing after Stop., we will not capture any frame after Stop.
        if (!SUCCEEDED(hr))
        {
            // Notes: This method always sets the filter's state to State_Stopped, even if the method returns an error code. 

            errorMessage = "Could not stop media contol.(hr = " + std::to_string(hr) + ").";
            return false;
        }

        return true;
    }

    bool IMediaControlHandler::WaitForCompletion(
        const FILTER_STATE succeedState,
        const std::string processStr,
        std::string& errorMessage
    )
    {
        // Initialize Filter State
        FILTER_STATE fs = State_Stopped;
        if (succeedState == State_Stopped) fs = State_Running;

        // Wait for completion
        int tryCount = 0;
        while (fs != succeedState && tryCount < 20) // Maximum try 20 times
        {
            // Wait for 100ms
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            // Get State
            const auto hr = m_mediaControl->GetState(
                1000, // 1 second timeout
                (OAFilterState*)&fs
            );

            // Throw if GetState return error
            if (hr != S_OK &&
                !(succeedState == State_Paused && hr == VFW_S_CANT_CUE) // Ignore VFW_S_CANT_CUE when pause
                )
            {
                if (hr == VFW_S_STATE_INTERMEDIATE)
                {
                    errorMessage = "Error in waiting " + processStr + " completed(The filter graph is still in transition to the indicated state.)";
                }
                else if (hr == VFW_S_CANT_CUE)
                {
                    errorMessage = "Error in waiting " + processStr + " completed(The filter graph is paused, but cannot cue data.)";
                }
                else if(hr == E_FAIL)
                {
                    errorMessage = "Error in waiting " + processStr + " completed(Failure.)";
                }
                else
                {
                    errorMessage = "Error in waiting " + processStr + " completed(hr=" + std::to_string(hr) + ")";
                }

                return false;
            }

            // Increase try count
            tryCount++;
        }

        // Check if completed
        if (fs != succeedState)
        {
            errorMessage = "Error in waiting " + processStr + " completed(Timeout.)";
            return false;
        }

        return true;
    }

#pragma endregion Media Control
}
/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef DIRECTSHOW_CAMERA__DIRECTSHOW_CAMERA__GRABBER__DIRECTSHOW_GRABBER_CALLBACK_H
#define DIRECTSHOW_CAMERA__DIRECTSHOW_CAMERA__GRABBER__DIRECTSHOW_GRABBER_CALLBACK_H

//************Content************

#include <windows.h>

#include "directshow_camera/grabber/qedit.h"

#include "directshow_camera/video_format/ds_guid.h"

#include <mutex>
#include <chrono>
#include <memory>

namespace DirectShowCamera
{
    /**
     * @brief A Callback for image sampling
     * 
     */
    class SampleGrabberCallback : public ISampleGrabberCB
    {
    public:

#pragma region Constructor and Destructor
        /**
         * @brief Constructor
        */
        SampleGrabberCallback();

        /**
         * @brief Destructor
        */
        ~SampleGrabberCallback();

#pragma endregion Constructor and Destructor

#pragma region Buffer Size

        /**
         * @brief Set the buffer size.
         * @paraml[in] numOfBytes Number of bytes of a frame
        */
        void setBufferSize(const int numOfBytes);

        /**
        * @brief Get the buffer size.
        * 
        * @return The buffer size
        */
        int getBufferSize() const;

#pragma endregion Buffer Size

#pragma region Frame

        /**
         * @brief Get the current frame
         * @param[out] frame Frame in bytes
         * @param[out] numOfBytes Number of the byte of the frame. It will change if the size is change in 5 frame.
         * @param[out] frameIndex (Optional) A frame index,such as a frame id. It can be use to identify whether it is a new frame.
         * @return Return true if the current is copied. If error occurred, it return false.
        */
        bool getFrame(
            unsigned char* frame,
            int& numOfBytes,
            unsigned long& frameIndex
        );

        /**
        * @brief Get the last frame index. It can be used to identify whether a new frame. Index will only be updated when you call getFrame()
        * @return Return the last frame index.
        */
        unsigned long getLastFrameIndex() const;

        /**
         * @brief Get frame per second
         * @return Return fps. Return 0 if 1/(current time - last frame time) < MinimumFPS
        */
        double getFPS() const;

        /**
        * @brief Get the last frame capture time
        * @return Return the last frame capture time
        */
        std::chrono::system_clock::time_point getLastFrameCaptureTime() const;

        /**
        * @brief Set the minimum FPS. FPS below this value will be identified as 0.
        * 
        * @param[in] minimumFPS Minimum FPS. FPS below this value will be identified as 0.
        * @return Return true if the minimum FPS is set successfully.
        */
        bool setMinimumFPS(const double minimumFPS);

        /**
        * @brief Get the minimum FPS. FPS below this value will be identified as 0.
        * 
        * @return Return the minimum FPS. FPS below this value will be identified as 0.
        */
        double getMinimumFPS() const;

#pragma endregion Frame

        //------------------------------------------------
        STDMETHODIMP_(ULONG) AddRef() override;
        STDMETHODIMP_(ULONG) Release() override;

        //------------------------------------------------
        STDMETHODIMP QueryInterface(REFIID, void** ppvObject) override;

        //------------------------------------------------
        STDMETHODIMP SampleCB(double, IMediaSample* pSample) override;

        // Not implemented
        STDMETHODIMP BufferCB(double, BYTE*, long) override;
    private:

        /**
         * @brief A mutex for the rame copying.
        */
        std::mutex m_bufferMutex;

        /**
         * @brief Current frame data.
        */
        std::unique_ptr<unsigned char[]> m_pixelsBuffer;

        /**
         * @brief The current frame size in bytes.
        */
        int m_bufferSize = 0;

        /**
         * @brief Current Frame index. Such as ID of the current frame. It use to identify whether a new frame.
        */
        unsigned long m_frameIndex = 1;

        int m_latestPixelCount = 0;
        int m_numOfRepeatPixelCount = 0;
        static const int m_resetBufferCount = 5;

        std::chrono::system_clock::time_point m_lastFrameTime;
        double m_fps = 0;

        double m_minimumFPS = 0.5;

        /**
        * @brief Current reference count. If it reaches 0 we delete ourself
        */
        unsigned long m_refCount = 0;

    };
}
//*******************************

#endif
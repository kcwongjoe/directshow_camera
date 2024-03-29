#pragma once
#ifndef DIRECTSHOW_GRABBER_CALLBACK_H
#define DIRECTSHOW_GRABBER_CALLBACK_H

//************Content************

#include <mutex>
#include <chrono>

#include <windows.h>
#include "directshow_camera/ds_guid.h"

namespace DirectShowCamera
{
    /**
     * @brief A Callback for image sampling
     * 
     */
    class SampleGrabberCallback : public ISampleGrabberCB
    {
    public:

        SampleGrabberCallback();
        ~SampleGrabberCallback();

        // Buffer size
        void setBufferSize(int numOfBytes);
        int getBufferSize();

        /**
         * @brief Get the current frame
         * @param[out] frame Frame in bytes
         * @param[out] numOfBytes Number of the byte of the frame. It will change if the size is change in 5 frame.
         * @param[out] frameIndex (Option) A frame index,such as a frame id. It can be use to identify whether it is a new frame.
         * @param[in] copyNewFrameOnly (Option) Set as true if only interesting on new frame and it will not copy the frame. It should be used with previousFrameIndex. Default as false.
         * @param[in] previousFrameIndex (Option) The previous frame index. It use to identify whether the current frame is a new frame. Default as 0.
         * @return Return true if the current is copied. If a new frame is required and the frame doesn't change, it will returns false. If eroor occurred, it return false.
        */
        bool getFrame(
            unsigned char* frame,
            int& numOfBytes,
            unsigned long& frameIndex,
            const bool copyNewFrameOnly = false,
            const unsigned long previousFrameIndex = 0
        );
        double getFPS();
        std::chrono::system_clock::time_point getLastFrameCaptureTime();

        /**
            * @brief Minimum FPS. FPS below this value will be identified as 0.
        */
        double minimumFPS = 0.5;

        //------------------------------------------------
        STDMETHODIMP_(ULONG) AddRef();
        STDMETHODIMP_(ULONG) Release();

        //------------------------------------------------
        STDMETHODIMP QueryInterface(REFIID, void** ppvObject);

        //------------------------------------------------
        STDMETHODIMP SampleCB(double, IMediaSample* pSample);


        // Not implemented
        STDMETHODIMP BufferCB(double, BYTE*, long);
    private:

        /**
         * @brief A mutex for the rame copying.
        */
        std::mutex m_bufferMutex;

        /**
         * @brief Current frame data.
        */
        unsigned char* m_pixelsBuffer;

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

        /**
        * @brief Current reference count. If it reaches 0 we delete ourself
        */
        unsigned long m_refCount = 0;

    };
}
//*******************************

#endif
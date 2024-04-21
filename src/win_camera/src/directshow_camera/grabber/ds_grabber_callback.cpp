/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#include "directshow_camera/grabber/ds_grabber_callback.h"

namespace DirectShowCamera
{

#pragma region Constructor and Destructor
    SampleGrabberCallback::SampleGrabberCallback()
    {
        // initialize buffer
        m_pixelsBuffer = std::make_unique<unsigned char[]>(m_bufferSize);
        AddRef();
    }

    SampleGrabberCallback::~SampleGrabberCallback()
    {

    }

#pragma endregion Constructor and Destructor

#pragma region Buffer Size

    void SampleGrabberCallback::setBufferSize(const int numOfBytes)
    {
        // Lock all buffer
        m_bufferMutex.lock();

        // Reallocate buffer
        m_bufferSize = numOfBytes;
        m_pixelsBuffer.reset();
        m_pixelsBuffer = std::make_unique<unsigned char[]>(m_bufferSize);

        // Set all bytes as 0
        memset(m_pixelsBuffer.get(), 0, m_bufferSize);

        // Release all lock
        m_bufferMutex.unlock();
    }

    int SampleGrabberCallback::getBufferSize() const
    {
        return m_bufferSize;
    }

#pragma endregion Buffer Size

#pragma region Frame

    bool SampleGrabberCallback::getFrame(
        unsigned char* frame,
        int& numOfBytes,
        unsigned long& frameIndex
    )
    {
        // Check
        if (frame == nullptr)  return false;

        // Copy frame
        try
        {
            //     Lock mutex
            m_bufferMutex.lock();

            //     Copy
            memcpy(frame, m_pixelsBuffer.get(), m_bufferSize);

            //     Return frame size
            numOfBytes = m_bufferSize;

            //     Return frame index
            frameIndex = m_frameIndex;

            //     Unlock mutex
            m_bufferMutex.unlock();
        }
        catch (...)
        {
            // Unlock mutex
            m_bufferMutex.unlock();
            return false;
        }

        return true;
    }

    unsigned long SampleGrabberCallback::getLastFrameIndex() const
    {
        return m_frameIndex;
    }

    double SampleGrabberCallback::getFPS() const
    {
        auto nowTime = std::chrono::system_clock::now();
        double timeDiff = (std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - m_lastFrameTime)).count() / 1000.0;
        if (1/ timeDiff < m_minimumFPS)
        {
            return 0;
        }
        else
        {
            if (m_fps < m_minimumFPS)
            {
                return 0;
            }
            else
            {
                return m_fps;
            }
            
        }
    }

    std::chrono::system_clock::time_point SampleGrabberCallback::getLastFrameCaptureTime() const
    {
        return m_lastFrameTime;
    }

    bool SampleGrabberCallback::setMinimumFPS(const double minimumFPS)
    {
        bool success = false;
        if (minimumFPS >= 0)
        {
            m_minimumFPS = minimumFPS;
            success = true;
        }

        return success;
    }

    double SampleGrabberCallback::getMinimumFPS() const
    {
        return m_minimumFPS;
    }

#pragma endregion Frame

    ULONG SampleGrabberCallback::AddRef()
    {
        m_refCount++;
        return m_refCount;
    }

    ULONG SampleGrabberCallback::Release()
    {
        m_refCount--;
        if (!m_refCount)
        {
            delete this;
            return 0;
        }
        return m_refCount;
    }

    STDMETHODIMP SampleGrabberCallback::QueryInterface(REFIID, void** ppvObject) {
        *ppvObject = static_cast<ISampleGrabberCB*>(this);
        return S_OK;
    }

    STDMETHODIMP SampleGrabberCallback::SampleCB(double, IMediaSample* pSample) {

        // Get data
        unsigned char* directShowBufferPointer;
        HRESULT hr = pSample->GetPointer(&directShowBufferPointer);

        if (hr == S_OK) {

            // Get frame data size
            int currentPixelSize = pSample->GetActualDataLength();

            if (currentPixelSize == m_bufferSize) {
                
                // Lock
                m_bufferMutex.lock();

                // Copy to buffer
                memcpy(m_pixelsBuffer.get(), directShowBufferPointer, m_bufferSize);

                // Update frame index
                if (m_frameIndex >= ULONG_MAX - 1)
                {
                    m_frameIndex = 1;
                }				
                else
                {
                    m_frameIndex++;
                }

                // Release Lock
                m_bufferMutex.unlock();

                // Update fps
                auto nowTime = std::chrono::system_clock::now();
                double timeDiff = (std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - m_lastFrameTime)).count() / 1000.0;
                m_fps = 1 / timeDiff;
                m_lastFrameTime = nowTime;

                // Reset variable
                m_numOfRepeatPixelCount = 0;
            }
            else
            {
                // Pixel count not match the buffer size, rest buffer size after 5 same pixel count
                if (currentPixelSize == m_latestPixelCount)
                {
                    m_numOfRepeatPixelCount++;

                    if (m_numOfRepeatPixelCount > m_resetBufferCount)
                    {
                        // Reset size
                        setBufferSize(currentPixelSize);
                        m_numOfRepeatPixelCount = 0;
                    }
                }
                else
                {
                    m_numOfRepeatPixelCount = 0;
                }
            }

            // Update latest pixel size
            m_latestPixelCount = currentPixelSize;

        }

        return S_OK;
    }

    STDMETHODIMP SampleGrabberCallback::BufferCB(double, BYTE*, long) {
        // prevent nodiscard warning by casting to void
        static_cast<void>(std::chrono::system_clock::now());
        return E_NOTIMPL;
    }
}
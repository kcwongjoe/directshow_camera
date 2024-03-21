#include "directshow_camera/ds_grabber_callback.h"

namespace DirectShowCamera
{

    /**
     * @brief Constructor
    */
    SampleGrabberCallback::SampleGrabberCallback()
    {
        // initialize buffer
        m_pixelsBuffer = new unsigned char[m_bufferSize];
        AddRef();
    }

    /**
     * @brief Destructor
    */
    SampleGrabberCallback::~SampleGrabberCallback()
    {
        // Release buffers
        delete[] m_pixelsBuffer;
    }

    /**
     * @brief Set the buffer size.
     * @param numOfBytes Number of bytes of a frame
    */
    void SampleGrabberCallback::setBufferSize(int numOfBytes)
    {
        // Lock all buffer
        m_bufferMutex.lock();

        // Reallocate buffer
        m_bufferSize = numOfBytes;
        delete[] m_pixelsBuffer;
        m_pixelsBuffer = new unsigned char[m_bufferSize];

        // Set all bytes as 0
        memset(m_pixelsBuffer, 0, m_bufferSize);

        // Release all lock
        m_bufferMutex.unlock();
    }

    int SampleGrabberCallback::getBufferSize()
    {
        return m_bufferSize;
    }

    bool SampleGrabberCallback::getFrame(
        unsigned char* frame,
        int& numOfBytes,
        unsigned long& frameIndex,
        const bool copyNewFrameOnly,
        const unsigned long previousFrameIndex
    )
    {
        int currentframeIndex = m_frameIndex;
        bool result = false;

        if (frame)
        {
            // Copy data
            if (!copyNewFrameOnly || (copyNewFrameOnly && currentframeIndex != previousFrameIndex))
            {
                // Lock mutex
                m_bufferMutex.lock();

                // Copy
                memcpy(frame, m_pixelsBuffer, m_bufferSize);
                    
                // Return frame size
                numOfBytes = m_bufferSize;

                // Unlock mutex
                m_bufferMutex.unlock();

                // Return success
                result = true;
            }

            // Return frame index
            frameIndex = currentframeIndex;
        }

        return result;
    }

    /**
     * @brief Get frame per second
     * @return Return fps. Return 0 if 1/(current time - last frame time) < minimumFPS
    */
    double SampleGrabberCallback::getFPS()
    {
        auto nowTime = std::chrono::system_clock::now();
        double timeDiff = (std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - m_lastFrameTime)).count() / 1000.0;
        if (1/ timeDiff < minimumFPS)
        {
            return 0;
        }
        else
        {
            if (m_fps < minimumFPS)
            {
                return 0;
            }
            else
            {
                return m_fps;
            }
            
        }
    }

    std::chrono::system_clock::time_point SampleGrabberCallback::getLastFrameCaptureTime()
    {
        return m_lastFrameTime;
    }

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
                memcpy(m_pixelsBuffer, directShowBufferPointer, m_bufferSize);

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
/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#include "directshow_camera/stub/ds_camera_stub.h"

namespace DirectShowCamera
{

#pragma region Life cycle

    DirectShowCameraStub::DirectShowCameraStub()
    {
        m_fps = DirectShowCameraStubDefaultSetting::getFps();
    }

    DirectShowCameraStub::~DirectShowCameraStub()
    {
        Release();
    }

    void DirectShowCameraStub::Release()
    {
        if (this)
        {
            HRESULT hr = NOERROR;
            
            Stop();

            // Release video format
            m_videoFormats.Clear();
            m_currentVideoFormatIndex = -1;

            //Release Properties
            m_properties.reset();

            m_isOpening = false;
        }
    }

#pragma endregion Life cycle

#pragma region Connection

    bool DirectShowCameraStub::Open(
        IBaseFilter** directShowFilter,
        std::optional<const DirectShowVideoFormat> videoFormat
    )
    {
        m_isOpening = true;

        // Create camera properties
        DirectShowCameraStubDefaultSetting::getProperties(m_properties);

        // Update video format
        UpdateVideoFormatList();
        if (videoFormat != std::nullopt && videoFormat.has_value()) {
            m_currentVideoFormatIndex = getVideoFormatIndex(videoFormat.value());
        }
        else
        {
            throw std::exception("DirectShowVideoFormat can't be null");
        }

        return true;
    }

    void DirectShowCameraStub::Close()
    {
        Release();
    }

    bool DirectShowCameraStub::isOpening() const
    {
        return m_isOpening;
    }

    bool DirectShowCameraStub::isDisconnecting()
    {
        // Device doesn't started
        if (!m_isOpening) return true;

        return m_disconnectCamera;
    }

    void DirectShowCameraStub::setDisconnectionProcess(std::function<void()> func)
    {
        m_disconnectionProcess = func;

        StartCheckConnectionThread();
    }

    void DirectShowCameraStub::StartCheckConnectionThread()
    {
        if (m_isCapturing && m_disconnectionProcess)
        {
            if (!m_isRunningCheckConnectionThread)
            {
                m_stopCheckConnectionThread = false;

                m_checkConnectionThread = std::thread(
                    [this]()
                    {
                        // Parameter
                        int step = 1000;

                        // Thread start running
                        m_isRunningCheckConnectionThread = true;

                        while (!m_stopCheckConnectionThread)
                        {
                            // Sleep
                            std::this_thread::sleep_for(std::chrono::milliseconds(step));

                            // Check disconnection
                            if (isDisconnecting())
                            {
                                m_disconnectCamera = false;

                                // Do something
                                m_disconnectionProcess();

                                // Stop capture
                                Stop();

                                m_stopCheckConnectionThread = true;
                            }
                        }

                        // Thread stoped
                        m_isRunningCheckConnectionThread = false;
                    }
                );
                m_checkConnectionThread.detach();
            }
        }
    }

    void DirectShowCameraStub::DisconnetCamera()
    {
        m_disconnectCamera = true;
    }

#pragma endregion Connection

#pragma region start/stop

    bool DirectShowCameraStub::Start()
    {
        bool result = true;

        if (m_isOpening)
        {
            m_isCapturing = true;

            // Start check disconnection thread
            StartCheckConnectionThread();
        }
        else
        {
            result = false;
            m_errorString = "Graph is not initialized, please call initialize().";
        }

        return result;
    }

    bool DirectShowCameraStub::Stop()
    {
        HRESULT hr = NOERROR;
        bool result = true;

        if (this)
        {
            if (m_isOpening)
            {
                if (m_isCapturing)
                {
                    // Reset isCapturing
                    m_isCapturing = false;

                    // Stop the check disconnection thread
                    m_stopCheckConnectionThread = true;
                }
            }
            else
            {
                result = false;
                m_errorString = "Graph is not initialized, please call initialize().";
            }
        }

        return result;
    }

    bool DirectShowCameraStub::isCapturing() const
    {
        return m_isCapturing;
    }

#pragma endregion start/stop

#pragma region Frame

    void DirectShowCameraStub::setGetFrameFunction(GetFrameFunc func)
    {
        m_getFrameFunc = func;
    }

    bool DirectShowCameraStub::getFrame(
        unsigned char* frame,
        int& numOfBytes,
        unsigned long& frameIndex
    )
    {
        if (m_isCapturing && frame)
        {
            if (m_getFrameFunc)
            {
                // Return the user define image
                m_getFrameFunc(frame, numOfBytes, frameIndex, m_frameIndex);

                // Update frame index
                m_frameIndex = frameIndex;
            }
            else
            {
                // Update frame index
                if (UpdateFrameIndexAfterGetFrame)
                {
                    m_frameIndex = m_frameIndex + 1;
                }

                frameIndex = m_frameIndex;

                // Return the default image, image will be generated based on the frame index value
                DirectShowCameraStubDefaultSetting::getFrame(
                    frame,
                    numOfBytes,
                    frameIndex,
                    m_videoFormats.getVideoFormat(m_currentVideoFormatIndex).getWidth(),
                    m_videoFormats.getVideoFormat(m_currentVideoFormatIndex).getHeight()
                );
            }

            return true;
        }
        else
        {
            return false;
        }
    }

    unsigned long DirectShowCameraStub::getLastFrameIndex() const
    {
        return m_frameIndex;
    }

    void DirectShowCameraStub::setMinimumFPS(const double minimumFPS)
    {
        if (minimumFPS < 0)
        {
            m_fps = 0;
        }
        else
        {
            m_fps = minimumFPS;
        }
    }

    double DirectShowCameraStub::getFPS() const
    {
        if (isOpening())
        {
            return m_fps;
        }
        else
        {
            return 0;
        }
    }

    long DirectShowCameraStub::getFrameTotalSize() const
    {

        if (m_isCapturing)
        {
            const auto heigth = m_videoFormats.getVideoFormat(m_currentVideoFormatIndex).getHeight();
            const auto width = m_videoFormats.getVideoFormat(m_currentVideoFormatIndex).getWidth();
            const int result = heigth * width * 3;
            return result;
        }
        else
        {
            return 0;
        }
    }

    GUID DirectShowCameraStub::getFrameType() const
    {
        return MEDIASUBTYPE_RGB24;
    }

#pragma endregion Frame

#pragma region Video Format

    bool DirectShowCameraStub::UpdateVideoFormatList()
    {
        // Create video format
        const auto videoFormat = DirectShowCameraStubDefaultSetting::getVideoFormat();
        m_videoFormats = DirectShowVideoFormatList(videoFormat);

        return true;
    }

    int DirectShowCameraStub::getVideoFormatIndex(const DirectShowVideoFormat videoFormat) const
    {
        int result = -1;
        for (int i = 0; i < m_videoFormats.Size(); i++)
        {
            if (m_videoFormats.getVideoFormat(i) == videoFormat)
            {
                result = i;
                break;
            }
        }

        return result;
    }

    int DirectShowCameraStub::getCurrentVideoFormatIndex() const
    {
        return m_currentVideoFormatIndex;
    }

    DirectShowVideoFormat DirectShowCameraStub::getCurrentVideoFormat() const
    {
        if (m_currentVideoFormatIndex >= 0)
        {
            return m_videoFormats.getVideoFormat(m_currentVideoFormatIndex);
        }
        else
        {
            return DirectShowVideoFormat();
        }

    }

    DirectShowVideoFormat DirectShowCameraStub::getCurrentGrabberFormat() const
    {
        return getCurrentVideoFormat();
    }

    std::vector<DirectShowVideoFormat> DirectShowCameraStub::getVideoFormatList() const
    {
        return m_videoFormats.getVideoFormatList();
    }

    bool DirectShowCameraStub::setVideoFormat(const DirectShowVideoFormat videoFormat)
    {
        bool result = false;
        UpdateVideoFormatList();
        int index = getVideoFormatIndex(videoFormat);
        if (index >= 0)
        {
            result = setVideoFormat(index);
        }

        return result;
    }

    bool DirectShowCameraStub::setVideoFormat(const int videoFormatIndex)
    {
        bool result = true;

        if (result)
        {
            if (!m_isOpening)
            {
                result = false;
                m_errorString = "Graph is not initialized, please call initialize().";
            }
            else if (videoFormatIndex < 0 || videoFormatIndex >= m_videoFormats.Size())
            {
                result = false;
                m_errorString = "Video format index(" + std::to_string(videoFormatIndex) + ") is out of range(0," + std::to_string(m_videoFormats.Size()) + ").";
            }
            else
            {
                // Set
                m_currentVideoFormatIndex = videoFormatIndex;
            }
        }

        return result;
    }

#pragma endregion Video Format

#pragma region Properties

    void DirectShowCameraStub::RefreshProperties()
    {
        
    }

    std::shared_ptr<DirectShowCameraProperties> DirectShowCameraStub::getProperties() const
    {
        return m_properties;
    }

    void DirectShowCameraStub::ResetPropertiesToDefault(const bool asAuto)
    {
        if (m_isOpening && m_properties != nullptr)
        {
            m_properties->ResetToDefaultValue(NULL, m_errorString, asAuto);
        }
    }

    bool DirectShowCameraStub::setPropertyValue(
        std::shared_ptr<DirectShowCameraProperty>& property,
        const long value,
        const bool isAuto
    )
    {
        if (m_isOpening)
        {
            return property->setValue(NULL, value, isAuto, m_errorString);
        }
        else
        {
            m_errorString = "Error on setting " + property->getName() + " property: Camera is not open.";
            return false;
        }
    }

#pragma endregion Properties

#pragma region getCamera

    bool DirectShowCameraStub::getCameras(std::vector<DirectShowCameraDevice>& cameraDevices)
    {
        DirectShowCameraStubDefaultSetting::getCamera(cameraDevices);
        return true;
    }

    bool DirectShowCameraStub::getCamera(const int cameraIndex, IBaseFilter** directShowFilter)
    {
        *directShowFilter = NULL;
        return true;
    }

    bool DirectShowCameraStub::getCamera(const std::string devicePath, IBaseFilter** directShowFilter)
    {
        *directShowFilter = NULL;
        return true;
    }

    bool DirectShowCameraStub::getCamera(const DirectShowCameraDevice device, IBaseFilter** directShowFilter)
    {
        return getCamera(device.getDevicePath(), directShowFilter);
    }

#pragma endregion getCamera

    void DirectShowCameraStub::ResetLastError()
    {
        m_errorString.clear();
    }

    std::string DirectShowCameraStub::getLastError() const
    {
        return m_errorString;
    }

}
#include "directshow_camera/ds_camera.h"

#include "directshow_camera/utils/check_hresult_utils.h"
#include "directshow_camera/utils/win32_utils.h"

namespace DirectShowCamera
{
    
#pragma region Life cycle

    /**
     * @brief Constructor
    */
    DirectShowCamera::DirectShowCamera()
    {
        
    }

    /**
     * @brief Desctructor
    */
    DirectShowCamera::~DirectShowCamera()
    {
        release();
    }

    /**
     * @brief Release
    */
    void DirectShowCamera::release()
    {
        if (this)
        {
            HRESULT hr = NOERROR;

            stop();

            // Release video format
            m_videoFormats.Clear();
            m_currentVideoFormatIndex = -1;

            // assigning a newly created DirectShowVideoFormat will reset m_isEmpty
            m_sampleGrabberVideoFormat = DirectShowVideoFormat();

            // Release stream config
            DirectShowCameraUtils::SafeRelease(&m_streamConfig);

            // Release sample grabber callback
            if (m_sampleGrabber != NULL) m_sampleGrabber->SetCallback(NULL, 1);
            DirectShowCameraUtils::SafeRelease(&m_sampleGrabberCallback);

            // Release sample grabber
            DirectShowCameraUtils::SafeRelease(&m_sampleGrabber);

            // Disconnect filters from capture device
            DirectShowCameraUtils::NukeDownStream(m_filterGraphManager, m_videoInputFilter);

            // Release media event
            DirectShowCameraUtils::SafeRelease(&m_mediaEvent);

            // Release media control
            DirectShowCameraUtils::SafeRelease(&m_mediaControl);

            // Release null renderer filter
            DirectShowCameraUtils::SafeRelease(&m_nullRendererFilter);

            // Release grabber filter
            DirectShowCameraUtils::SafeRelease(&m_grabberFilter);

            // Release video input filter
            DirectShowCameraUtils::SafeRelease(&m_videoInputFilter);

            // Release capture graph builder
            DirectShowCameraUtils::DestroyGraph(m_filterGraphManager);
            DirectShowCameraUtils::SafeRelease(&m_filterGraphManager);

            // Release capture graph builder
            DirectShowCameraUtils::SafeRelease(&m_captureGraphBuilder);

            //Release Properties
            m_property.reset();

            m_grabberMediaSubType = MEDIASUBTYPE_None;

            m_isOpening = false;
        }		
    }

#pragma endregion Life cycle

#pragma region Connection

    /**
     * @brief Build the directshow graph
     * @param videoInputFilter Video input filter. Look up from DirectShowCamera::getCamera()
     * @param videoFormat Video Format. Look up from DirectShowCameraDevice::getDirectShowVideoFormats()
     * @return Return true if success
     * 
     * @startuml {directshow_diagram.svg} "DirectShow Camera Diagram"
     *	skinparam linetype polyline
     *	skinparam linetype ortho
     *	"Capture Graph Builder" .d.>[Manage] "Filter Graph Manager"
     *	"Filter Graph Manager" .d.>[QueryInterface] "Media Control"
     *	note left
     *		Use to control the camera
     *	end note
     *	"Filter Graph Manager" .d.>[QueryInterface] "Media Event"
     *	note right
     * 		Check the camera disconnection state
     *	end note
     * 	"Filter Graph Manager" -r->[Contain] "Filters"
     *	partition "Filters" #00b9a0 {
     *		"Video Input Filter" .d.> "Sample Grabber Filter"
     *		"Sample Grabber Filter" .d.> "Null Renderer Filter"
     *	}
     *	"Sample Grabber Filter" -r->[Set] "SampleGrabberCallback"
     * @enduml
    */
    bool DirectShowCamera::open(
        IBaseFilter** videoInputFilter,
        std::optional<const DirectShowVideoFormat> videoFormat
    )
    {
        // Initialize variable
        HRESULT hr = NOERROR;
        bool result = true;

        if (!m_isOpening)
        {
            m_videoInputFilter = *videoInputFilter;

            m_sampleGrabberCallback = new SampleGrabberCallback();
            // Create the capture graph builder
            if (result)
            {
                hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void**)&m_captureGraphBuilder);
                result = CheckHResultUtils::CheckCoCreateInstanceResult(hr, m_errorString, "Error on creating the Capture Graph Builder");
            }

            // Create the Filter Graph Manager.
            if (result)
            {
                hr = CoCreateInstance(CLSID_FilterGraph, 0, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&m_filterGraphManager);
                result = CheckHResultUtils::CheckCoCreateInstanceResult(hr, m_errorString, "Error on creating the Filter Graph Manager");
            }

            // Set a media event. We can use this to check device disconnection.
            if (result)
            {
                hr = m_filterGraphManager->QueryInterface(IID_IMediaEventEx, (void**)&m_mediaEvent);
                if (FAILED(hr))
                {
                    result = false;
                    m_errorString = " Could not create media event object for device disconnection(hr = " + std::to_string(hr) + ").";
                }
            }

            // Set the Filter Graph Manager into Capture Graph Builder
            if (result)
            {
                hr = m_captureGraphBuilder->SetFiltergraph(m_filterGraphManager);
                result = CheckHResultUtils::CheckICGB2SetFiltergraphResult(hr, m_errorString, "Error on setting the Filter Graph Manager to Capture Graph Builder");
            }

            // Set the media control
            if (result)
            {
                hr = m_filterGraphManager->QueryInterface(IID_IMediaControl, (void**)&m_mediaControl);
                if (FAILED(hr))
                {
                    result = false;
                    m_errorString = "Could not create media control object(hr = " + std::to_string(hr) + ").";
                }
            }

            // Add video input filter
            if (result)
            {
                hr = m_filterGraphManager->AddFilter(m_videoInputFilter, NULL);
                result = CheckHResultUtils::CheckIGBAddFilterResult(hr, m_errorString, "Error on adding video input filter");
            }

            // Set capture pin
            if (result)
            {
                hr = m_captureGraphBuilder->FindInterface(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, m_videoInputFilter, IID_IAMStreamConfig, (void**)&m_streamConfig);
                result = CheckHResultUtils::CheckICGB2FindInterfaceResult(hr, m_errorString, "Error on setting capture pin");
            }

            // Get media type
            AM_MEDIA_TYPE* amMediaType = NULL;
            if (result)
            {
                // Set video format
                if (videoFormat != std::nullopt && videoFormat.has_value())
                {
                    // Update the video format list so that we can find the AM_Media_Type in the list.
                    updateVideoFormatList();

                    // Set format
                    setVideoFormat(videoFormat.value());

                    // Todo: Check if the video format was set incorrectly, throw error if it was.
                }

                // Get format
                hr = m_streamConfig->GetFormat(&amMediaType);
                result = CheckHResultUtils::CheckIIAMSCGetFormatResult(hr, m_errorString, "Error on getting media type");
            }

            // Create the Sample Grabber.
            if (result)
            {
                hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&m_grabberFilter);
                result = CheckHResultUtils::CheckCoCreateInstanceResult(hr, m_errorString, "Error on creating the Sample Grabber");
            }

            // Add Sample Grabber
            if (result)
            {
                hr = m_filterGraphManager->AddFilter(m_grabberFilter, L"Sample Grabber");
                result = CheckHResultUtils::CheckIGBAddFilterResult(hr, m_errorString, "Error on adding Sample Grabber");
            }

            // Connect Sample Grabber
            if (result)
            {
                hr = m_grabberFilter->QueryInterface(IID_ISampleGrabber, (void**)&m_sampleGrabber);
                if (FAILED(hr))
                {
                    result = false;
                    m_errorString = "Could not query Sample Grabber(hr = " + std::to_string(hr) + ").";
                }
            }

            // Set Grabber callback function
            if (result)
            {
                m_sampleGrabber->SetOneShot(FALSE);
                m_sampleGrabber->SetBufferSamples(FALSE);
                hr = m_sampleGrabber->SetCallback(m_sampleGrabberCallback, 0);// 0 is for SampleCB and 1 for BufferCB
                if (FAILED(hr))
                {
                    result = false;
                    m_errorString = "Could not set sample grabber callback function(hr = " + std::to_string(hr) + ").";
                }
            }

            // Set grabber filter media type
            if (result)
            {
                updateGrabberFilterVideoFormat();
            }

            // Create a null renderer filter to discard the samples after you are done with them.
            if (result)
            {
                hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)(&m_nullRendererFilter));
                result = CheckHResultUtils::CheckCoCreateInstanceResult(hr, m_errorString, "Error on creating the null renderer filter");
            }

            // Add null renderer filter to graph
            if (result)
            {
                hr = m_filterGraphManager->AddFilter(m_nullRendererFilter, L"NullRenderer");
                result = CheckHResultUtils::CheckIGBAddFilterResult(hr, m_errorString, "Error on adding Null Renderer filter");
            }

            //  Connect all filter as stream : Video Input Filter - Grabber Filter - Null Renderer Filter
            if (result)
            {
                hr = m_captureGraphBuilder->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, m_videoInputFilter, m_grabberFilter, m_nullRendererFilter);
                result = CheckHResultUtils::CheckICGB2RenderStreamResult(hr, m_errorString, "Error on connecting filter (Video Input Filter - Grabber Filter - Null Renderer Filter)");
            }

            // get video format of connected grabber filter
            AM_MEDIA_TYPE grabberMediaType;
            ZeroMemory(&grabberMediaType, sizeof(grabberMediaType));
            hr = m_sampleGrabber->GetConnectedMediaType(&grabberMediaType);
            if (hr == S_OK)
            {
                m_sampleGrabberVideoFormat = DirectShowVideoFormat::Create(&grabberMediaType);
            }

            // Try setting the sync source to null - and make it run as fast as possible
            bool syncSourceAsNull = false;
            if (result && syncSourceAsNull)
            {
                IMediaFilter* iMediaFilter = 0;
                hr = m_filterGraphManager->QueryInterface(IID_IMediaFilter, (void**)&iMediaFilter);
                if (hr == S_OK) {
                    iMediaFilter->SetSyncSource(NULL);
                    iMediaFilter->Release();
                }
            }

            // ****Release****

            // Free media type
            if (amMediaType != NULL)
            {
                DirectShowCameraUtils::DeleteMediaType(&amMediaType);
            }

            if (result)
            {
                m_isOpening = true;

                // Get property
                if (m_property != nullptr) m_property.reset();
                m_property = std::make_shared<DirectShowCameraProperties>(m_videoInputFilter, m_errorString);

                // Update video format
                updateVideoFormatList();
                updateVideoFormatIndex();
            }
            else
            {
                // Release everything if error
                release();
            }
        }

        return result;
    }

    /**
     * @brief Close
    */
    void DirectShowCamera::close()
    {
        release();
    }

    /**
     * @brief Return true if camera was opened
     * @return Return true if camera was opened
    */
    bool DirectShowCamera::isOpening() const
    {
        return m_isOpening;
    }

    /**
     * @brief It can be check camera whether disconnected accidently. 
     * @return Return true if camera disconnected.
    */
    bool DirectShowCamera::checkDisconnection()
    {
        // Device doesn't started
        if (!m_isOpening) return true;

        long eventCode;
        LONG_PTR eventParameter1, eventParameter2;
        bool disconnected = false;

        // Get all event code
        // See https://docs.microsoft.com/en-us/windows/win32/directshow/event-notification-codes
        while (m_mediaEvent->GetEvent(&eventCode, &eventParameter1, &eventParameter2, 0) == S_OK)
        {
            //std::cout << "Event: Code: " + std::to_string(eventCode) + " Params: " + std::to_string(eventParameter1) + ", "  + std::to_string(eventParameter2) + "\n";

            // Free event parameters
            m_mediaEvent->FreeEventParams(eventCode, eventParameter1, eventParameter2);

            // Check device lost
            if (eventCode == EC_DEVICE_LOST)
            {
                EC_BANDWIDTHCHANGE;
                disconnected = true;
            }
        }
        return disconnected;
    }

    /**
     * @brief Set the disconnection process. When the process was set, a thread will start to keep check the connection. If camera is disconnected, this process will run and then run stop() internally.
     * @param func void()
    */
    void DirectShowCamera::setDisconnectionProcess(std::function<void()> func)
    {
        m_disconnectionProcess = func;

        startCheckConnectionThread();
    }

    /**
     * @brief Start a thread to check the device connection
    */
    void DirectShowCamera::startCheckConnectionThread()
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
                            
                            // Check last frame time
                            auto lastFrameTime = m_sampleGrabberCallback->getLastFrameCaptureTime();
                            auto timeDiff = (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastFrameTime)).count(); // in ms
                            double fps = m_sampleGrabberCallback->getFPS();
                            double fpsInTime = 1.0 / fps * 1000;

                            // Check disconnection
                            if (timeDiff > 10000 || fps == 0 || timeDiff >= fpsInTime * 2)
                            {
                                bool disconnected = checkDisconnection();

                                if (disconnected)
                                {
                                    // Do something
                                    m_disconnectionProcess();

                                    // Stop capture
                                    stop();

                                    m_stopCheckConnectionThread = true;
                                }
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

#pragma endregion Connection

#pragma region start/stop

    /**
     * @brief Start capture
     * @return Return true if success
    */
    bool DirectShowCamera::start()
    {
        bool result = true;

        if (m_isOpening)
        {
            // Set callback buffer
            updateGrabberFilterVideoFormat();

            // Run
            HRESULT hr = m_mediaControl->Run();
            if (FAILED(hr))
            {
                result = false;
                m_errorString = "Could not start the graph (hr = " + std::to_string(hr) + ").";
            }
            m_isCapturing = true;

            // Start check disconnection thread
            startCheckConnectionThread();
        }
        else
        {
            result = false;
            m_errorString = "Graph is not initialized, please call initialize().";
        }

        return result;
    }

    /**
     * @brief Stop Capture
     * @return Return true if success
    */
    bool DirectShowCamera::stop()
    {
        HRESULT hr = NOERROR;
        bool result = true;

        if (this)
        {
            if (m_isOpening)
            {
                if (m_isCapturing)
                {
                    // Pause
                    if (result)
                    {
                        hr = m_mediaControl->Pause();
                        if (FAILED(hr))
                        {
                            result = false;
                            m_errorString = " Could not pause media contol.(hr = " + std::to_string(hr) + ").";
                        }
                    }

                    // Stop
                    if (result)
                    {
                        hr = m_mediaControl->Stop();
                        if (FAILED(hr))
                        {
                            result = false;
                            m_errorString = " Could not stop media contol.(hr = " + std::to_string(hr) + ").";
                        }
                    }

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

    /**
     * @brief Return true if camera is capturing
     * @return Return true if camera is capturing 
    */
    bool DirectShowCamera::isCapturing() const
    {
        return m_isCapturing;
    }

#pragma endregion start/stop

#pragma region Frame


    bool DirectShowCamera::getFrame
    (
        unsigned char* frame,
        int& numOfBytes,
        unsigned long& frameIndex,
        const bool copyNewFrameOnly,
        const unsigned long previousFrameIndex
    )
    {
        if (m_isCapturing && frame)
        {
            m_sampleGrabberCallback->getFrame(frame, numOfBytes, frameIndex, copyNewFrameOnly, previousFrameIndex);
            return true;
        }
        else
        {
            return false;
        }
    }

    /**
     * @brief Minimum FPS. FPS below this value will be identified as 0. Default as 0.5
     * @param minimumFPS 
    */
    void DirectShowCamera::setMinimumPFS(double minimumFPS)
    {
        if (m_sampleGrabberCallback)
        {
            if (minimumFPS < 0) minimumFPS = 0;
            m_sampleGrabberCallback->minimumFPS = minimumFPS;
        }
    }

    /**
     * @brief Get Frame per second.
     * @return Return fps.
    */
    double DirectShowCamera::getFPS() const
    {
        if (isOpening())
        {
            return m_sampleGrabberCallback->getFPS();
        }
        else
        {
            return 0;
        }
    }

    /**
     * @brief Get frame size in bytes.
     * @return 
    */
    long DirectShowCamera::getFrameTotalSize() const
    {

        if (m_isCapturing)
        {
            int result = m_sampleGrabberCallback->getBufferSize();
            return result;
        }
        else
        {
            return 0;
        }
    }

    /**
     * @brief Get frame type. Such as MEDIASUBTYPE_RGB24
     * @return 
    */
    GUID DirectShowCamera::getFrameType() const
    {
        return m_grabberMediaSubType;
    }

#pragma endregion Frame

#pragma region Video Format

    /**
     * @brief Update teh grabber filter buffer size and the media type.
    */
    void DirectShowCamera::updateGrabberFilterVideoFormat()
    {
        if (m_sampleGrabber)
        {
            // Get frame size
            int frameTotalSize = 0;
            GUID mediaSubType;
            DirectShowCameraUtils::AmMediaTypeDecorator(m_streamConfig,
                [this, &frameTotalSize, &mediaSubType](AM_MEDIA_TYPE* mediaType)
                {
                    VIDEOINFOHEADER* videoInfoHeader = reinterpret_cast<VIDEOINFOHEADER*>(mediaType->pbFormat);
                    int width = videoInfoHeader->bmiHeader.biWidth;
                    int height = videoInfoHeader->bmiHeader.biHeight;

                    if (DirectShowVideoFormatUtils::isSupportRGBConvertion(mediaType->subtype))
                    {
                        // Transform to RGB in the grabber filter
                        frameTotalSize = width * height * 3;
                        mediaSubType = MEDIASUBTYPE_RGB24;
                    }
                    else
                    {
                        // Do not convert
                        frameTotalSize = mediaType->lSampleSize;
                        mediaSubType = mediaType->subtype;
                    }
                },
                m_errorString
            );

            // Set grabber media type
            AM_MEDIA_TYPE grabberMediaType;
            ZeroMemory(&grabberMediaType, sizeof(grabberMediaType));

            grabberMediaType.majortype = MEDIATYPE_Video;
            grabberMediaType.formattype = FORMAT_VideoInfo;
            grabberMediaType.subtype = mediaSubType;
            grabberMediaType.lSampleSize = frameTotalSize;

            HRESULT hr = m_sampleGrabber->SetMediaType(&grabberMediaType);
            if (hr == S_OK)
            {
                m_grabberMediaSubType = mediaSubType;

                // get video format of grabber filter - this can fail if the graph is not yet connected
                hr = m_sampleGrabber->GetConnectedMediaType(&grabberMediaType);
                if (hr == S_OK)
                {
                    m_sampleGrabberVideoFormat = DirectShowVideoFormat::Create(&grabberMediaType);
                }

                // Set buffer size
                m_sampleGrabberCallback->setBufferSize(frameTotalSize);
            }
            else
            {
                m_errorString = "Could not set media type to RGB24.(hr = " + std::to_string(hr) + ").";
            }

            DirectShowCameraUtils::FreeMediaType(grabberMediaType);
        }	
    }

    /**
     * @brief Update video formats
    */
    bool DirectShowCamera::updateVideoFormatList()
    {
        bool result = false;
        if (m_streamConfig != NULL)
        {
            result = m_videoFormats.Update(m_streamConfig, m_errorString);
        }
        else
        {
            result = false;
        }

        return result;
    }

    /**
     * @brief Update the current video foramt index from Steam Config
    */
    void DirectShowCamera::updateVideoFormatIndex()
    {
        if (m_videoFormats.Size() > 0)
        {
            DirectShowCameraUtils::AmMediaTypeDecorator(m_streamConfig,
                [this](AM_MEDIA_TYPE* mediaType)
                {
                    m_currentVideoFormatIndex = getVideoFormatIndex(mediaType);
                },
                m_errorString
            );
        }
        else
        {
            m_currentVideoFormatIndex = -1;
        }
    }

    /**
     * @brief Get index from the Video Format list
     * @param mediaType 
     * @return Return -1 if not found
    */
    int DirectShowCamera::getVideoFormatIndex(AM_MEDIA_TYPE* mediaType) const
    {
        int result = -1;
        for (int i = 0; i < m_videoFormats.Size(); i++)
        {
            if (m_videoFormats.getAMMediaType(i) == mediaType)
            {
                result = i;
                break;
            }
        }

        return result;
    }

    /**
     * @brief Get index from the Video Format list
     * @param videoFormat 
     * @return Return -1 if not found
    */
    int DirectShowCamera::getVideoFormatIndex(const DirectShowVideoFormat videoFormat) const
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

    /**
     * @brief Get current video format index.
     * @return 
    */
    int DirectShowCamera::getCurrentVideoFormatIndex() const
    {
        return m_currentVideoFormatIndex;
    }

    /**
     * @brief Get current video format
     * @return 
    */
    DirectShowVideoFormat DirectShowCamera::getCurrentVideoFormat() const
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

    /**
    * @brief Get current grabber format
    * @return
    */
    DirectShowVideoFormat DirectShowCamera::getCurrentGrabberFormat() const
    {
        return m_sampleGrabberVideoFormat;
    }

    /**
     * @brief Get current video format list of this opened camera.
     * @return 
    */
    std::vector<DirectShowVideoFormat> DirectShowCamera::getVideoFormatList() const
    {
        return m_videoFormats.getVideoFormatList();
    }

    /**
     * @brief Set video format. It is suggested to set video format in the open(). It may not succes to change the video format after opened camera.
     * @param videoFormat Video format to be set
     * @return Return true if success.
    */
    bool DirectShowCamera::setVideoFormat(DirectShowVideoFormat videoFormat)
    {
        bool result = false;
        updateVideoFormatList();
        int index = getVideoFormatIndex(videoFormat);
        if (index >= 0)
        {
            result = setVideoFormat(index);
        }

        return result;
    }

    /**
     * @brief Set video format. It is suggested to set video format in the open(). It may not succes to change the video format after opening camera.
     * @param videoFormatIndex Index of the video foramt list.
     * @return Return true if success.
    */
    bool DirectShowCamera::setVideoFormat(int videoFormatIndex)
    {
        bool result = true;
        HRESULT hr = NO_ERROR;

        if (result)
        {
            if (m_streamConfig == NULL)
            {
                result = false;
                m_errorString = "Graph is not initialized, please call initialize().";
            }
            else if (videoFormatIndex < 0 || videoFormatIndex >= m_videoFormats.Size())
            {
                result = false;
                m_errorString = "Video format index("+ std::to_string(videoFormatIndex) +") is out of range(0," + std::to_string(m_videoFormats.Size()) + ").";
            }
            else
            {
                // Set
                hr = m_streamConfig->SetFormat(m_videoFormats.getAMMediaType(videoFormatIndex));
                result = CheckHResultUtils::CheckIIAMSCSetFormatResult(hr, m_errorString, "Error on setting camera resolution");

                if (result)
                {
                    updateGrabberFilterVideoFormat();
                    m_currentVideoFormatIndex = videoFormatIndex;
                }
            }
        }

        return result;
    }

#pragma endregion Video Format

#pragma region Properties

    /**
     * @brief Refresh properties
     */
    void DirectShowCamera::refreshProperties()
    {
        if (m_isOpening && m_property != nullptr)
        {
            m_property->refresh(m_videoInputFilter, m_errorString);
        }
    }

    /**
     * @brief Get properties
     * @return Return properties
    */
    std::shared_ptr<DirectShowCameraProperties> DirectShowCamera::getProperties() const
    {
        return m_property;
    }

    /**
     * @brief Reset properties to default
     * 
     * @param[in] asAuto (Option) Set as true if you also want to set properties to auto. Default as true.
     */
    void DirectShowCamera::resetDefault(bool asAuto)
    {
        if (m_videoInputFilter != NULL && m_property != nullptr)
        {
            m_property->resetDefault(m_videoInputFilter, m_errorString, asAuto);
        }
    }

    /**
     * @brief Set property value
     * @param property Property
     * @param value Value to be set
     * @param isAuto Set as true for auto mode, false for manual mode
     * @return Return true if success
    */
    bool DirectShowCamera::setValue(
        const std::shared_ptr<DirectShowCameraProperty>& property,
        const long value,
        const bool isAuto
    )
    {
        if (m_videoInputFilter != NULL)
        {
            return property->setValue(m_videoInputFilter, value, isAuto, m_errorString);
        }
        else
        {
            m_errorString = "Error on setting " + property->getName() + " property: Camera is not open.";
            return false;
        }
    }

#pragma endregion Properties

#pragma region getCamera

    /**
     * @brief Get the available camera list
     * @param[out] cameraDevices Camera Devices.
     * @return Return true if success
    */
    bool DirectShowCamera::getCameras(std::vector<DirectShowCameraDevice>* cameraDevices)
    {
        // Initialize and clear
        if (cameraDevices == nullptr) cameraDevices = new std::vector<DirectShowCameraDevice>();
        cameraDevices->clear();

        bool success = DirectShowCameraUtils::IPropertyDecorator(
            CLSID_VideoInputDeviceCategory,
            [this, cameraDevices](IMoniker* moniker, IPropertyBag* propertyBag)
            {
                // Initialize variables
                HRESULT hr;
                VARIANT var;
                VariantInit(&var);

                std::string description = "";
                std::string friendlyName = "";
                std::string devicePath = "";

                // Get description
                hr = propertyBag->Read(L"Description", &var, 0);
                if (hr == S_OK)
                {
                    description = Win32Utils::BSTRToString(var.bstrVal);
                    VariantClear(&var);
                }

                // Get friendly name
                hr = propertyBag->Read(L"FriendlyName", &var, 0);
                if (hr == S_OK)
                {
                    friendlyName = Win32Utils::BSTRToString(var.bstrVal);
                    VariantClear(&var);
                }

                // Get device path
                hr = propertyBag->Read(L"DevicePath", &var, 0);
                if (hr == S_OK)
                {
                    // Get device path
                    devicePath = Win32Utils::BSTRToString(var.bstrVal);
                    VariantClear(&var);
                }

                // Get all video format
                std::vector<DirectShowVideoFormat> videoFormats;
                DirectShowCameraUtils::IPinDecorator(
                    moniker,
                    [this, &videoFormats](IPin* iPin, PIN_INFO* pinInfo)
                    {
                        DirectShowCameraUtils::AmMediaTypeDecorator(
                            iPin,
                            [this, &videoFormats](AM_MEDIA_TYPE* amMediaType)
                            {
                                videoFormats.push_back(DirectShowVideoFormat::Create(amMediaType));
                            },
                            m_errorString
                        );
                    },
                    m_errorString
                );

                // Sort and erase duplicates
                if (videoFormats.size() > 0)
                {
                    DirectShowVideoFormat::sortAndUnique(&videoFormats);
                }

                // Push into device
                cameraDevices->push_back(DirectShowCameraDevice(friendlyName, description, devicePath, videoFormats));
            },
            m_errorString
        );

        return success;
    }

    /**
     * @brief Get the video input filter based on the camera index 
     * @param[in] cameraIndex Camera index
     * @param[out] videoInputFilter Output video input filter
     * @return Return true if success.
    */
    bool DirectShowCamera::getCamera(int cameraIndex, IBaseFilter** videoInputFilter)
    {
        int count = 0;
        bool found = false;
        bool success = DirectShowCameraUtils::IPropertyDecorator(
            CLSID_VideoInputDeviceCategory,
            [&videoInputFilter, &cameraIndex, &count, &found](IMoniker* moniker, IPropertyBag* propertyBag)
            {
                // Found, obtain the video input filter
                if (count == cameraIndex)
                {
#pragma warning( push )
#pragma warning( disable : 6387)
                    moniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void**)videoInputFilter);
#pragma warning( pop )
                    found = true;
                }

                // Move to next
                count++;
            },
            m_errorString
        );

        return success && found;
    }

    /**
     * @brief Get the video input filter based on the Camera device path
     * @param[in] devicePath Camera device path
     * @param[out] videoInputFilter Output video input filter
     * @return Return true if success.
    */
    bool DirectShowCamera::getCamera(std::string devicePath, IBaseFilter** videoInputFilter)
    {
        bool found = false;
        bool success = DirectShowCameraUtils::IPropertyDecorator(
            CLSID_VideoInputDeviceCategory,
            [&videoInputFilter, &devicePath, &found](IMoniker* moniker, IPropertyBag* propertyBag)
            {
                // Initialize variables
                HRESULT hr;
                VARIANT var;
                VariantInit(&var);

                // Get device path
                hr = propertyBag->Read(L"DevicePath", &var, 0);
                if (hr == S_OK)
                {
                    std::string currentDevicePath = Win32Utils::BSTRToString(var.bstrVal);
                    VariantClear(&var);

                    // Found, obtain the video input filter
                    if (currentDevicePath == devicePath)
                    {
                        // Get device name
#pragma warning( push )
#pragma warning( disable : 6387)
                        moniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void**)videoInputFilter);
#pragma warning( pop )
                        found = true;
                    }
                }
            },
            m_errorString
        );

        return success && found;
    }

    /**
     * @brief Get the video input filter based on the Camera device object 
     * @param[in] device Camera device
     * @param[out] videoInputFilter Output video input filter
     * @return Return true if success.
    */
    bool DirectShowCamera::getCamera(DirectShowCameraDevice device, IBaseFilter** videoInputFilter)
    {
        // try to match via device path first, fallback to friendly name
        bool found = getCamera(device.getDevicePath(), videoInputFilter);
        if (!found)
        {
            DirectShowCameraUtils::IPropertyDecorator(
                CLSID_VideoInputDeviceCategory,
                [&videoInputFilter, &device, &found](IMoniker* moniker, IPropertyBag* propertyBag)
                {
                    // Initialize variables
                    HRESULT hr;
                    VARIANT var;
                    VariantInit(&var);

                    // Get friendly name
                    hr = propertyBag->Read(L"FriendlyName", &var, 0);
                    if (hr == S_OK)
                    {
                        std::string friendly_name = Win32Utils::BSTRToString(var.bstrVal);
                        VariantClear(&var);

                        if (friendly_name == device.getFriendlyName())
                        {
#pragma warning( push )
#pragma warning( disable : 6387)
                            moniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void**)videoInputFilter);
#pragma warning( pop )
                            found = true;
                        }
                    }
                },
                m_errorString
            );
        }
        return found;
    }

#pragma endregion getCamera

    /**
     * @brief Get the last error
     * @return Return the last error
    */
    std::string DirectShowCamera::getLastError() const
    {
        return m_errorString;
    }
}
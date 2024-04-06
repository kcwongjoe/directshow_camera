#include "win_camera/win_camera.h"

#include "directshow_camera/utils/com_lib_utils.h"

#include "exceptions/resolution_not_support_exception.h"
#include "exceptions/device_not_found_exception.h"

namespace WinCamera
{

#pragma region Constructor and Destructor

    WinCamera::WinCamera() :
        m_directShowCamera(std::make_shared<DirectShowCamera::DirectShowCamera>())
    {
        Constructor();
    }

    WinCamera::WinCamera(
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& abstractDirectShowCamera
    ) :
        m_directShowCamera(abstractDirectShowCamera)
    {
        Constructor();
    }

    void WinCamera::Constructor()
    {
        COMLibUtils::COMLibUtils::InitCOMLib();

#ifdef WITH_OPENCV2
        m_matConvertor = OpenCVMatConverter();
        m_matConvertor.isBGR = true;
        m_matConvertor.isVerticalFlip = true;
#endif
        InitProperties();
    }

    WinCamera::~WinCamera()
    {
        Close();
        
        // Uninitialize COM Library
        COMLibUtils::COMLibUtils::UninitCOMLib();
    }

#pragma endregion Constructor and Destructor

#pragma region Connection

    bool WinCamera::Open(
        const WinCameraDevice& device,
        const int width,
        const int height,
        const bool rgb
    )
    {
        // Find the DirectShowCameraDevice
        std::vector<DirectShowCamera::DirectShowCameraDevice> possibleCamera = getDirectShowCameras();
        int cameraIndex = -1;
        for (int i = 0; i < possibleCamera.size(); i++)
        {
            if (possibleCamera[i].getDevicePath() == device.getDevicePath())
            {
                // Found
                cameraIndex = i;
                break;
            }
        }
        if (cameraIndex == -1) throw DeviceNotFoundException(device.getFriendlyName());

        // Get DirectShowVideoFormat
        bool result = false;
        if (width <= 0 || height <= 0)
        {
            // No specific format
            result = Open(possibleCamera[cameraIndex]);
        }
        else
        {
            // Lookup format
            std::vector<DirectShowCamera::DirectShowVideoFormat> videoFormats = possibleCamera[cameraIndex].getDirectShowVideoFormats();

            // Lookup size
            std::vector<DirectShowCamera::DirectShowVideoFormat> possibleVideoFormat;
            for (int i = 0; i < videoFormats.size(); i++)
            {
                if (videoFormats[i].getWidth() == width && videoFormats[i].getHeight() == height)
                {
                    possibleVideoFormat.push_back(videoFormats[i]);
                }
            }

            if (possibleVideoFormat.size() > 0)
            {
                // Get media type list
                int videoFormatIndex = -1;
                std::vector<GUID> mediaType;
                if (rgb)
                {
                    // RGB
                    mediaType = DirectShowVideoFormatUtils::SupportRGBSubType();
                }
                else
                {
                    // MonoChrome
                    mediaType = DirectShowVideoFormatUtils::SupportMonochromeSubType();
                }

                // Match with list
                for (int i = 0; i < mediaType.size(); i++)
                {
                    for (int j = 0; j < possibleVideoFormat.size(); j++)
                    {
                        if (possibleVideoFormat[j].getVideoType() == mediaType[i])
                        {
                            // Found
                            videoFormatIndex = j;
                            break;
                        }
                    }

                    // Exit if found
                    if (videoFormatIndex >= 0) break;
                }

                // If success
                if (videoFormatIndex >= 0)
                {
                    // Open
                    result = Open(possibleCamera[cameraIndex], possibleVideoFormat[videoFormatIndex]);
                }
                else
                {
                    // Not Found the size
                    result = false;
                    throw ResolutionNotSupportException(device.getFriendlyName(), width, height, rgb);

                }
            }
            else
            {
                // Not Found the size
                result = false;
                throw ResolutionNotSupportException(device.getFriendlyName(), width, height, false);
            }
        }

        return result;
    }

    bool WinCamera::Open(
        const DirectShowCamera::DirectShowCameraDevice& device,
        std::optional<const DirectShowCamera::DirectShowVideoFormat> videoFormat
    )
    {
        bool result = false;

        // Get device input filter
        IBaseFilter* videoInputFilter;
        result = m_directShowCamera->getCamera(device, &videoInputFilter);

        // Open
        if (result)
        {
            result = Open(&videoInputFilter, videoFormat);
        }
        else
        {
            // Release device input filter
            DirectShowCameraUtils::SafeRelease(&videoInputFilter);

            copyError(result);
        }

        return result;
    }

    bool WinCamera::Open(
        IBaseFilter** videoInputFilter,
        std::optional<const DirectShowCamera::DirectShowVideoFormat> videoFormat
    )
    {
        bool result = false;

        // Initialize camera
        result = m_directShowCamera->open(videoInputFilter, videoFormat);

    #ifdef WITH_OPENCV2
        if (result)
        {
            AllocateMatBuffer();
        }
    #endif

        copyError(result);

        return result;
    }

    bool WinCamera::isOpened() const
    {
        if (m_directShowCamera)
            return m_directShowCamera->isOpening();
        else
            return false;
    }

    bool WinCamera::Close()
    {
        bool result = true;
        if (m_directShowCamera)
        {
            if (this && result && isCapturing())
            {
                result = StopCapture();
            }

            if (this && result && isOpened())
            {
                m_directShowCamera->close();
            }
        }


        return result;
    }

    bool WinCamera::CheckDisconnection()
    {
        return m_directShowCamera->checkDisconnection();
    }

    void WinCamera::setDisconnectionProcess(std::function<void()> func)
    {
        m_directShowCamera->setDisconnectionProcess(func);
    }

#pragma endregion Connection

#pragma region Capture

    bool WinCamera::StartCapture()
    {
        if (m_directShowCamera->isOpening())
        {
            bool success = m_directShowCamera->start();

            copyError(success);

            return success;
        }
        else
        {
            m_errorString = "Error on startCapture() : Camera is not opened.";
            return false;
        }
    }

    bool WinCamera::StopCapture()
    {
        if (m_directShowCamera->isOpening())
        {
            bool success = m_directShowCamera->stop();

            copyError(success);

            return success;
        }
        else
        {
            m_errorString = "Error on stopCapture() : Camera is not opened.";
            return false;
        }
    }

    bool WinCamera::isCapturing() const
    {
        if (m_directShowCamera)
        {
            return m_directShowCamera->isCapturing();
        }
        else
        {
            return false;
        }
        
    }

#pragma endregion Capture

#pragma region Properties

    void WinCamera::ResetProperties(const bool asAuto)
    {
        m_directShowCamera->resetDefault(asAuto);
    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperties> WinCamera::getDirectShowProperties() const
    {
        return m_directShowCamera->getProperties();
    }

    void WinCamera::InitProperties() 
    {
        m_brightness = std::make_shared<WinCameraPropertyBrightness>( *this, m_directShowCamera);
        m_brightness = std::make_shared<WinCameraPropertyBrightness>(*this, m_directShowCamera);
        m_contrast = std::make_shared<WinCameraPropertyContrast>(*this, m_directShowCamera);
        m_hue = std::make_shared<WinCameraPropertyHue>(*this, m_directShowCamera);
        m_saturation = std::make_shared<WinCameraPropertySaturation>(*this, m_directShowCamera);
        m_sharpness = std::make_shared<WinCameraPropertySharpness>(*this, m_directShowCamera);
        m_gamma = std::make_shared<WinCameraPropertyGamma>(*this, m_directShowCamera);
        m_color_enable = std::make_shared<WinCameraPropertyColorEnable>(*this, m_directShowCamera);
        m_white_balance = std::make_shared<WinCameraPropertyWhiteBalance>(*this, m_directShowCamera);
        m_backlight_compensation = std::make_shared<WinCameraPropertyBacklightCompensation>(*this, m_directShowCamera);
        m_gain = std::make_shared<WinCameraPropertyGain>(*this, m_directShowCamera);
        m_pan = std::make_shared<WinCameraPropertyPan>(*this, m_directShowCamera);
        m_tilt = std::make_shared<WinCameraPropertyTilt>(*this, m_directShowCamera);
        m_roll = std::make_shared<WinCameraPropertyRoll>(*this, m_directShowCamera);
        m_zoom = std::make_shared<WinCameraPropertyZoom>(*this, m_directShowCamera);
        m_exposure = std::make_shared<WinCameraPropertyExposure>(*this, m_directShowCamera);
        m_iris = std::make_shared<WinCameraPropertyIris>(*this, m_directShowCamera);
        m_focus = std::make_shared<WinCameraPropertyFocus>(*this, m_directShowCamera);
    }

    std::shared_ptr<WinCameraPropertyBrightness> WinCamera::Brightness()
    {
        return m_brightness;
    }

    std::shared_ptr<WinCameraPropertyContrast> WinCamera::Contrast()
    {
        return m_contrast;
    }

    std::shared_ptr<WinCameraPropertyHue> WinCamera::Hue()
    {
        return m_hue;
    }

    std::shared_ptr<WinCameraPropertySaturation> WinCamera::Saturation()
    {
        return m_saturation;
    }

    std::shared_ptr<WinCameraPropertySharpness> WinCamera::Sharpness()
    {
        return m_sharpness;
    }

    std::shared_ptr<WinCameraPropertyGamma> WinCamera::Gamma()
    {
        return m_gamma;
    }

    std::shared_ptr<WinCameraPropertyColorEnable> WinCamera::ColorEnable()
    {
        return m_color_enable;
    }

    std::shared_ptr<WinCameraPropertyWhiteBalance> WinCamera::WhiteBalance()
    {
        return m_white_balance;
    }

    std::shared_ptr<WinCameraPropertyBacklightCompensation> WinCamera::BacklightCompensation()
    {
        return m_backlight_compensation;
    }

    std::shared_ptr<WinCameraPropertyGain> WinCamera::Gain()
    {
        return m_gain;
    }

    std::shared_ptr<WinCameraPropertyPan> WinCamera::Pan()
    {
        return m_pan;
    }

    std::shared_ptr<WinCameraPropertyTilt> WinCamera::Tilt()
    {
        return m_tilt;
    }

    std::shared_ptr<WinCameraPropertyRoll> WinCamera::Roll()
    {
        return m_roll;
    }

    std::shared_ptr<WinCameraPropertyZoom> WinCamera::Zoom()
    {
        return m_zoom;
    }

    std::shared_ptr<WinCameraPropertyExposure> WinCamera::Exposure()
    {
        return m_exposure;
    }

    std::shared_ptr<WinCameraPropertyIris> WinCamera::Iris()
    {
        return m_iris;
    }

    std::shared_ptr<WinCameraPropertyFocus> WinCamera::Focus()
    {
        return m_focus;
    }

#pragma endregion Properties

    std::string WinCamera::getLastError() const
    {
        return m_errorString;
    }

#pragma region Cameras

    std::vector<DirectShowCamera::DirectShowCameraDevice> WinCamera::getDirectShowCameras()
    {
        std::vector<DirectShowCamera::DirectShowCameraDevice> result;
        bool success = m_directShowCamera->getCameras(&result);

        copyError(success);

        return result;
    }

    std::vector<WinCameraDevice> WinCamera::getCameras()
    {
        // Get DirectShowCameraDevice
        std::vector<DirectShowCamera::DirectShowCameraDevice> directShowCameras = getDirectShowCameras();

        // Convert to CameraDevice
        std::vector<WinCameraDevice> result;
        for (int i = 0; i < directShowCameras.size(); i++)
        {
            result.push_back(WinCameraDevice(&directShowCameras[i]));
        }

        return result;
    }

#pragma endregion Cameras

#pragma region DirectShow Video Format

    std::vector<DirectShowCamera::DirectShowVideoFormat> WinCamera::getSupportDirectShowVideoFormats() const
    {
        return m_directShowCamera->getVideoFormatList();
    }

    bool WinCamera::setDirectShowVideoFormat(DirectShowCamera::DirectShowVideoFormat videoFormat)
    {
        if (m_directShowCamera->isOpening())
        {
            bool requireStart = m_directShowCamera->isCapturing();

            // Stop capturing
            m_directShowCamera->stop();

            // Set resolution
            bool result = m_directShowCamera->setVideoFormat(videoFormat);

            // Copy error
            copyError(result);

#ifdef WITH_OPENCV2
            AllocateMatBuffer();
#endif

            // Restart capturing
            if (requireStart)
            {
                m_directShowCamera->start();
            }

            return result;
        }
        else
        {
            // Copy error
            copyError(false);

            return false;
        }
    }

    DirectShowCamera::DirectShowVideoFormat WinCamera::getDirectShowVideoFormat() const
    {
        return m_directShowCamera->getCurrentGrabberFormat();
    }

#pragma endregion DirectShow Video Format

#pragma region Frame

    bool WinCamera::getFrame(unsigned char* frame, int& numOfBytes, const bool onlyGetNewFrame)
    {
        bool result = false;

        if (m_directShowCamera->isCapturing())
        {
            unsigned long frameIndex;
            bool success = m_directShowCamera->getFrame(frame, numOfBytes, frameIndex, onlyGetNewFrame, m_lastFrameIndex);


            if (onlyGetNewFrame && frameIndex == m_lastFrameIndex)
            {
                // Get old frame but we want new frame only.
                result = false;
            }
            else if (!success)
            {
                result = false;
            }
            else
            {
                result = true;
            }

            // Update frame index
            m_lastFrameIndex = frameIndex;
        }
        else
        {
            result = false;
        }

        return result;
    }

    long WinCamera::getFrameIndex() const
    {
        return m_lastFrameIndex;
    }

    double WinCamera::getFPS() const
    {
        return m_directShowCamera->getFPS();
    }

#pragma region Opencv Function

#ifdef WITH_OPENCV2

    void WinCamera::setMatAsBGR(const bool asBGR)
    {
        m_matConvertor.isBGR = asBGR;
    }

    void WinCamera::VecticalFlipMat(const bool verticalFlip)
    {
        m_matConvertor.isVerticalFlip = verticalFlip;
    }

    bool WinCamera::AllocateMatBuffer()
    {
        bool result = false;

        if (isOpened())
        {
            // Get frame size
            const long bufferSize = m_directShowCamera->getFrameTotalSize();

            if (m_matBufferSize != bufferSize)
            {
                // Allocate buffer
                if (m_matBuffer != NULL)
                {
                    delete[] m_matBuffer;
                    m_matBuffer = NULL;
                }

                m_matBuffer = new unsigned char[bufferSize];
                m_matBufferSize = bufferSize;

                result = true;
            }

            // Update convertor media type
            m_matConvertor.videoType = m_directShowCamera->getFrameType();
        }

        return result;
    }

    cv::Mat WinCamera::getMat(const bool onlyGetNewMat)
    {
        // Reallocate frame buffer size if changed
        if (m_matBufferSize != m_directShowCamera->getFrameTotalSize())
        {
            AllocateMatBuffer();
        }

        // Get frame
        int numOfBytes;
        bool success = getFrame(m_matBuffer, numOfBytes, onlyGetNewMat);

        if (success)
        {
            return m_matConvertor.convert(m_matBuffer, getWidth(), getHeight());
        }
        else
        {
            cv::Mat result;
            return result;
        }
    }

    cv::Mat WinCamera::getLastMat()
    {
        return m_matConvertor.convert(m_matBuffer, getWidth(), getHeight());
    }

    cv::Mat WinCamera::getNewMat(const int step, const int timeout, const int skip)
    {
        auto lastFrameIndex = m_lastFrameIndex;
        int pastTime = 0;
        cv::Mat result;

        // Wait for new frame
        while (m_lastFrameIndex < lastFrameIndex + 1 + skip && pastTime <= timeout)
        {
            // Sleep
            std::this_thread::sleep_for(std::chrono::milliseconds(step));

            // Get mat and update m_lastFrameIndex
            result = getMat(true);

            // Update past time
            pastTime += step;
        }

        // Output
        if (pastTime > timeout)
        {
            // Timeout
            return cv::Mat();
        }
        else
        {
            // New frame collected
            if (result.empty())
            {
                return getLastMat();
            }
            else
            {
                return result;
            }
            
        }
    }

    cv::Mat WinCamera::ExposureFusion(
        const ExposureFusionAsyncResult exposureFusionAsyncResult,
        std::vector<cv::Mat>* exposureImages,
        const int minSetExposureDelay
    )
    {
        std::vector<double> exposures = Exposure()->GetPossibleExposureValues();
        
        return ExposureFusion(exposures, exposureFusionAsyncResult, exposureImages, minSetExposureDelay);
    }

    cv::Mat WinCamera::ExposureFusion(
        const std::vector<double> exposures,
        const ExposureFusionAsyncResult exposureFusionAsyncResult,
        std::vector<cv::Mat>* exposureImages,
        const int minSetExposureDelay
    )
    {
        // Save current exposure
        double currentExposure = Exposure()->getValue();
        bool isAutoExposureNow = Exposure()->isAuto();

        bool releaseExposureImages = false;
        if (exposureImages == nullptr || exposureImages == NULL)
        {
            exposureImages = new std::vector<cv::Mat>();
            releaseExposureImages = true;
        }

        // Get images in different exposure
        for (int i = 0; i < exposures.size(); i++)
        {
            Exposure()->setValue(exposures[i]);

            std::this_thread::sleep_for(std::chrono::milliseconds(minSetExposureDelay + (int)(exposures[i] * 1000)));

            cv::Mat newMat = getMat(false);
            if (!newMat.empty())
            {
                exposureImages->push_back(newMat);
            }
        }

        // Recovery exposure
        if (isAutoExposureNow)
        {
            Exposure()->setAuto(true);
        }
        else
        {
            Exposure()->setValue(currentExposure);
        }

        // Run exposure fusion	
        cv::Mat fusion;
        if (exposureImages)
        {
            // Exposure fusion process
            auto exposureFusionProcess = [exposureImages, releaseExposureImages]()
            {
                cv::Mat fusion;

                // Fusion
                cv::Ptr<cv::MergeMertens> merge_mertens = cv::createMergeMertens();
                merge_mertens->process(*exposureImages, fusion);

                // Convert to 8UC3
                cv::Mat fusion8UC3;
                fusion.convertTo(fusion8UC3, CV_8U, 255, 0);
                fusion = fusion8UC3;

                // Release
                if (releaseExposureImages)
                {
                    exposureImages->clear();
                    delete exposureImages;
                }

                return fusion;
            };

            // Run in Sync or Async mode
            if (exposureFusionAsyncResult == nullptr)
            {
                // Run in sync mode
                fusion = exposureFusionProcess();
            }
            else
            {
                // Run in async mode
                std::thread([exposureFusionProcess, exposureFusionAsyncResult]()
                    {
                        cv::Mat fusion = exposureFusionProcess();
                        exposureFusionAsyncResult(fusion);
                    }
                ).detach();
            }
        }

        return fusion;
    }

    #endif

#pragma endregion Opencv Function

    int WinCamera::getWidth() const
    {
        return getDirectShowVideoFormat().getWidth();
    }

    int WinCamera::getHeight() const
    {
        return getDirectShowVideoFormat().getHeight();
    }

    int WinCamera::getFrameSize() const
    {
        return getDirectShowVideoFormat().getTotalSize();
    }

    int WinCamera::getNumOfBytePerPixel() const
    {
        return getDirectShowVideoFormat().getBitPerPixel() / 8;
    }

    int WinCamera::getNumOfPixel() const
    {
        return getDirectShowVideoFormat().getWidth() * getDirectShowVideoFormat().getHeight();
    }

#pragma endregion Frame

#pragma region Utils

    /**
     * @brief Copy error from DirectShowCamera to this
     * @param success Only copy error if success = false
    */
    void WinCamera::copyError(bool success)
    {
        if (!success) m_errorString = m_directShowCamera->getLastError();
    }

#pragma endregion Utils
}
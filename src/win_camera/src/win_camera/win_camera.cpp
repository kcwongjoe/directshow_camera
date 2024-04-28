/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#include "win_camera/win_camera.h"

#include "directshow_camera/utils/ds_camera_utils.h"

#include "exceptions/resolution_not_support_exception.h"
#include "exceptions/device_not_found_exception.h"
#include "exceptions/directshow_camera_exception.h"
#include "exceptions/camera_not_opened_exception.h"

#include "utils/gdi_plus_utils.h"


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
        // Check
        assert(
            (abstractDirectShowCamera != nullptr) &&
            "abstractDirectShowCamera in WinCamera::WinCamera() should never be null."
        );

        // Initialize
        Constructor();
    }

    void WinCamera::Constructor()
    {
        // Check if DirectShowCamera has error during initialization
        ThrowDirectShowException();

        // Start GDI+
#ifndef DONT_INIT_GDIPLUS_IN_WINCAMERA
        const auto GDIPlusStatus = Utils::GDIPLUSUtils::StartGDIPlus();
        if (GDIPlusStatus != Gdiplus::Status::Ok)
        {
            // Throw GDIPlusException
            throw Utils::GDIPlusException("Failed to start GDI+", GDIPlusStatus);
        }
#endif

        // Initialize properties
        InitProperties();
    }

    WinCamera::~WinCamera()
    {
        Close();

        // Stop GDI+
#ifndef DONT_INIT_GDIPLUS_IN_WINCAMERA
        Utils::GDIPLUSUtils::StopGDIPlus();
#endif
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
        IBaseFilter* directShowFilter;
        result = m_directShowCamera->getCamera(device, &directShowFilter);

        // Open
        if (result)
        {
            result = Open(&directShowFilter, videoFormat);
        }
        else
        {
            // Release device input filter
            DirectShowCameraUtils::SafeRelease(&directShowFilter);

            // Throw DirectShow Camera Exception
            if (!result) ThrowDirectShowException();
        }

        return result;
    }

    bool WinCamera::Open(
        IBaseFilter** directShowFilter,
        std::optional<const DirectShowCamera::DirectShowVideoFormat> videoFormat
    )
    {
        bool result = false;

        // Initialize camera
        result = m_directShowCamera->Open(directShowFilter, videoFormat);

        // Throw DirectShow Camera Exception
        if (!result) ThrowDirectShowException();

        return result;
    }

    bool WinCamera::isOpened() const
    {
        if (m_directShowCamera != nullptr)
            return m_directShowCamera->isOpening();
        else
            return false;
    }

    bool WinCamera::Close()
    {
        bool result = true;
        if (m_directShowCamera != nullptr)
        {
            if (this && result && isCapturing())
            {
                result = StopCapture();
            }

            if (this && result && isOpened())
            {
                m_directShowCamera->Close();
            }
        }


        return result;
    }

    bool WinCamera::CheckDisconnection()
    {
        return m_directShowCamera->isDisconnecting();
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
            const auto result = m_directShowCamera->Start();

            // Throw DirectShow Camera Exception
            if (!result) ThrowDirectShowException();

            return result;
        }
        else
        {
            throw CameraNotOpenedException("WinCamera::StartCapture()");
            return false;
        }
    }

    bool WinCamera::StopCapture()
    {
        if (m_directShowCamera->isOpening())
        {
            const auto result = m_directShowCamera->Stop();

            // Throw DirectShow Camera Exception
            if (!result) ThrowDirectShowException();

            return result;
        }
        else
        {
            throw CameraNotOpenedException("WinCamera::StopCapture()");
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
        m_directShowCamera->ResetPropertiesToDefault(asAuto);
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
        m_powerline_frequency = std::make_shared<WinCameraPropertyPowerlineFrequency>(*this, m_directShowCamera);
        m_digital_zoom_level = std::make_shared<WinCameraPropertyDigitalZoomLevel>(*this, m_directShowCamera);
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

    std::shared_ptr<WinCameraPropertyPowerlineFrequency> WinCamera::PowerlineFrequency()
    {
        return m_powerline_frequency;
    }

    std::shared_ptr<WinCameraPropertyDigitalZoomLevel> WinCamera::DigitalZoomLevel()
    {
        return m_digital_zoom_level;
    }

#pragma endregion Properties

#pragma region Cameras

    std::vector<DirectShowCamera::DirectShowCameraDevice> WinCamera::getDirectShowCameras()
    {
        std::vector<DirectShowCamera::DirectShowCameraDevice> result;
        bool success = m_directShowCamera->getCameras(result);

        // Throw DirectShow Camera Exception
        if (!success) ThrowDirectShowException();

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
            result.push_back(WinCameraDevice(directShowCameras[i]));
        }

        return result;
    }

#pragma endregion Cameras

#pragma region DirectShow Video Format

    std::vector<DirectShowCamera::DirectShowVideoFormat> WinCamera::getSupportDirectShowVideoFormats() const
    {
        return m_directShowCamera->getVideoFormatList();
    }

    bool WinCamera::setDirectShowVideoFormat(const DirectShowCamera::DirectShowVideoFormat videoFormat)
    {
        if (m_directShowCamera->isOpening())
        {
            bool requireStart = m_directShowCamera->isCapturing();

            // Stop capturing
            const auto resultStop = m_directShowCamera->Stop();
            if (!resultStop) ThrowDirectShowException();

            // Set resolution
            const auto resultSetVideoFormat = m_directShowCamera->setVideoFormat(videoFormat);
            if (!resultSetVideoFormat) ThrowDirectShowException();

            // Restart capturing
            if (requireStart)
            {
                const auto resultStart = m_directShowCamera->Start();
                if (!resultStart) ThrowDirectShowException();
            }

            return true;
        }
        else
        {
            // Throw DirectShow Camera Exception
            ThrowDirectShowException();

            return false;
        }
    }

    DirectShowCamera::DirectShowVideoFormat WinCamera::getDirectShowVideoFormat() const
    {
        return m_directShowCamera->getCurrentGrabberFormat();
    }

#pragma endregion DirectShow Video Format

#pragma region Frame

    bool WinCamera::getFrame(Frame& frame, const bool onlyGetNewFrame)
    {
        // Check
        if (!m_directShowCamera->isCapturing()) return false;

        // Check frame index if user only want a new Frame
        if (onlyGetNewFrame)
        {
            if (m_lastFrameIndex == m_directShowCamera->getLastFrameIndex())
            {
                // No new frame
                return false;
            }
        }

        // Get frame information
        const auto width = getDirectShowVideoFormat().getWidth();
        const auto height = getDirectShowVideoFormat().getHeight();
        const long bufferSize = m_directShowCamera->getFrameTotalSize();
        const auto frameType = m_directShowCamera->getFrameType();

        // Get frame
        frame.ImportData(
            bufferSize,
            width,
            height,
            frameType,
            [this, onlyGetNewFrame](unsigned char* data, unsigned long& frameIndex)
            {   
                int numOfBytes;
                bool success = m_directShowCamera->getFrame(
                    data, 
                    numOfBytes,
                    frameIndex
                );
            }
        );

        // Set frame settings
        frame.setFrameSettings(m_frameSettings);

        // Update frame index
        m_lastFrameIndex = frame.getFrameIndex();

        return true;
    }

    bool WinCamera::getNewFrame(Frame& frame, const int step, const int timeout, const int skip)
    {
        const unsigned long lastFrameIndex = m_lastFrameIndex;
        const unsigned long expectedFrameIndex = lastFrameIndex + 1 + skip;
        bool expectedFrameIndexOverFlow = false;
        if (lastFrameIndex > expectedFrameIndex)
        {
            // Expected frame index overflow
            expectedFrameIndexOverFlow = true;
        }

        int pastTime = 0;

        // Wait for new frame
        while (
            (
                (expectedFrameIndexOverFlow && m_lastFrameIndex > 2147483647) ||
                (m_lastFrameIndex < expectedFrameIndex)
            ) &&
            pastTime <= timeout
        )
        {
            // Sleep
            std::this_thread::sleep_for(std::chrono::milliseconds(step));

            // Get frame and update the last frame index
            const auto success = getFrame(frame, true);

            // Update past time
            pastTime += step;
        }

        // Return
        if (pastTime > timeout)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    long WinCamera::getLastFrameIndex() const
    {
        return m_lastFrameIndex;
    }

    double WinCamera::getFPS() const
    {
        return m_directShowCamera->getFPS();
    }

#pragma region Opencv Function

#ifdef WITH_OPENCV2

    FrameSettings& WinCamera::getFrameSettings()
    {
        return m_frameSettings;
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
        if (exposureImages == nullptr)
        {
            exposureImages = new std::vector<cv::Mat>();
            releaseExposureImages = true;
        }

        // Get images in different exposure
        for (int i = 0; i < exposures.size(); i++)
        {
            Exposure()->setValue(exposures[i]);

            std::this_thread::sleep_for(std::chrono::milliseconds(minSetExposureDelay + (int)(exposures[i] * 1000)));

            Frame frame;
            getFrame(frame, false);
            cv::Mat newMat = frame.getMat();
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

#pragma region Exception

    void WinCamera::ThrowDirectShowException()
    {
        const auto lastError = m_directShowCamera->getLastError();

        if (!lastError.empty())
        {
            // Clear last error in DirectShowCamera
            m_directShowCamera->ResetLastError();

            // Throw exception
            throw DriectShowCameraException(lastError);
        }
    }

#pragma endregion Exception
}
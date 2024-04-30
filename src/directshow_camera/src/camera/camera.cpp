/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "camera/camera.h"

#include "directshow_camera/utils/ds_camera_utils.h"

#include "exceptions/resolution_not_support_exception.h"
#include "exceptions/device_not_found_exception.h"
#include "exceptions/directshow_camera_exception.h"
#include "exceptions/camera_not_opened_exception.h"

#include "utils/gdi_plus_utils.h"

#include <cassert>


namespace DirectShowCamera
{

#pragma region Constructor and Destructor

    Camera::Camera() :
        m_directShowCamera(std::make_shared<DirectShowCamera>())
    {
        Constructor();
    }

    Camera::Camera(
        const std::shared_ptr<AbstractDirectShowCamera>& abstractDirectShowCamera
    ) :
        m_directShowCamera(abstractDirectShowCamera)
    {
        // Check
        assert(
            (abstractDirectShowCamera != nullptr) &&
            "abstractDirectShowCamera in Camera() should never be null."
        );

        // Initialize
        Constructor();
    }

    void Camera::Constructor()
    {
        // Check if DirectShowCamera has error during initialization
        ThrowDirectShowException();

        // Start GDI+
#ifndef DONT_INIT_GDIPLUS_IN_Camera
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

    Camera::~Camera()
    {
        Close();

        // Stop GDI+
#ifndef DONT_INIT_GDIPLUS_IN_Camera
        Utils::GDIPLUSUtils::StopGDIPlus();
#endif
    }

#pragma endregion Constructor and Destructor

#pragma region Connection

    bool Camera::Open(
        const CameraDevice& device,
        const int width,
        const int height,
        const bool rgb
    )
    {
        // Find the DirectShowCameraDevice
        std::vector<DirectShowCameraDevice> possibleCamera = getDirectShowCameras();
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
            std::vector<DirectShowVideoFormat> videoFormats = possibleCamera[cameraIndex].getDirectShowVideoFormats();

            // Lookup size
            std::vector<DirectShowVideoFormat> possibleVideoFormat;
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

    bool Camera::Open(
        const DirectShowCameraDevice& device,
        std::optional<const DirectShowVideoFormat> videoFormat
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

    bool Camera::Open(
        IBaseFilter** directShowFilter,
        std::optional<const DirectShowVideoFormat> videoFormat
    )
    {
        bool result = false;

        // Initialize camera
        result = m_directShowCamera->Open(directShowFilter, videoFormat);

        // Throw DirectShow Camera Exception
        if (!result) ThrowDirectShowException();

        return result;
    }

    bool Camera::isOpened() const
    {
        if (m_directShowCamera != nullptr)
            return m_directShowCamera->isOpening();
        else
            return false;
    }

    bool Camera::Close()
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

    bool Camera::CheckDisconnection()
    {
        return m_directShowCamera->isDisconnecting();
    }

    void Camera::setDisconnectionProcess(std::function<void()> func)
    {
        m_directShowCamera->setDisconnectionProcess(func);
    }

#pragma endregion Connection

#pragma region Capture

    bool Camera::StartCapture()
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
            throw CameraNotOpenedException("Camera::StartCapture()");
            return false;
        }
    }

    bool Camera::StopCapture()
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
            throw CameraNotOpenedException("Camera::StopCapture()");
            return false;
        }
    }

    bool Camera::isCapturing() const
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

    void Camera::ResetProperties(const bool asAuto)
    {
        m_directShowCamera->ResetPropertiesToDefault(asAuto);
    }

    std::shared_ptr<DirectShowCameraProperties> Camera::getDirectShowProperties() const
    {
        return m_directShowCamera->getProperties();
    }

    void Camera::InitProperties()
    {
        m_brightness = std::make_shared<CameraPropertyBrightness>( *this, m_directShowCamera);
        m_brightness = std::make_shared<CameraPropertyBrightness>(*this, m_directShowCamera);
        m_contrast = std::make_shared<CameraPropertyContrast>(*this, m_directShowCamera);
        m_hue = std::make_shared<CameraPropertyHue>(*this, m_directShowCamera);
        m_saturation = std::make_shared<CameraPropertySaturation>(*this, m_directShowCamera);
        m_sharpness = std::make_shared<CameraPropertySharpness>(*this, m_directShowCamera);
        m_gamma = std::make_shared<CameraPropertyGamma>(*this, m_directShowCamera);
        m_color_enable = std::make_shared<CameraPropertyColorEnable>(*this, m_directShowCamera);
        m_white_balance = std::make_shared<CameraPropertyWhiteBalance>(*this, m_directShowCamera);
        m_backlight_compensation = std::make_shared<CameraPropertyBacklightCompensation>(*this, m_directShowCamera);
        m_gain = std::make_shared<CameraPropertyGain>(*this, m_directShowCamera);
        m_pan = std::make_shared<CameraPropertyPan>(*this, m_directShowCamera);
        m_tilt = std::make_shared<CameraPropertyTilt>(*this, m_directShowCamera);
        m_roll = std::make_shared<CameraPropertyRoll>(*this, m_directShowCamera);
        m_zoom = std::make_shared<CameraPropertyZoom>(*this, m_directShowCamera);
        m_exposure = std::make_shared<CameraPropertyExposure>(*this, m_directShowCamera);
        m_iris = std::make_shared<CameraPropertyIris>(*this, m_directShowCamera);
        m_focus = std::make_shared<CameraPropertyFocus>(*this, m_directShowCamera);
        m_powerline_frequency = std::make_shared<CameraPropertyPowerlineFrequency>(*this, m_directShowCamera);
        m_digital_zoom_level = std::make_shared<CameraPropertyDigitalZoomLevel>(*this, m_directShowCamera);
    }

    std::shared_ptr<CameraPropertyBrightness> Camera::Brightness()
    {
        return m_brightness;
    }

    std::shared_ptr<CameraPropertyContrast> Camera::Contrast()
    {
        return m_contrast;
    }

    std::shared_ptr<CameraPropertyHue> Camera::Hue()
    {
        return m_hue;
    }

    std::shared_ptr<CameraPropertySaturation> Camera::Saturation()
    {
        return m_saturation;
    }

    std::shared_ptr<CameraPropertySharpness> Camera::Sharpness()
    {
        return m_sharpness;
    }

    std::shared_ptr<CameraPropertyGamma> Camera::Gamma()
    {
        return m_gamma;
    }

    std::shared_ptr<CameraPropertyColorEnable> Camera::ColorEnable()
    {
        return m_color_enable;
    }

    std::shared_ptr<CameraPropertyWhiteBalance> Camera::WhiteBalance()
    {
        return m_white_balance;
    }

    std::shared_ptr<CameraPropertyBacklightCompensation> Camera::BacklightCompensation()
    {
        return m_backlight_compensation;
    }

    std::shared_ptr<CameraPropertyGain> Camera::Gain()
    {
        return m_gain;
    }

    std::shared_ptr<CameraPropertyPan> Camera::Pan()
    {
        return m_pan;
    }

    std::shared_ptr<CameraPropertyTilt> Camera::Tilt()
    {
        return m_tilt;
    }

    std::shared_ptr<CameraPropertyRoll> Camera::Roll()
    {
        return m_roll;
    }

    std::shared_ptr<CameraPropertyZoom> Camera::Zoom()
    {
        return m_zoom;
    }

    std::shared_ptr<CameraPropertyExposure> Camera::Exposure()
    {
        return m_exposure;
    }

    std::shared_ptr<CameraPropertyIris> Camera::Iris()
    {
        return m_iris;
    }

    std::shared_ptr<CameraPropertyFocus> Camera::Focus()
    {
        return m_focus;
    }

    std::shared_ptr<CameraPropertyPowerlineFrequency> Camera::PowerlineFrequency()
    {
        return m_powerline_frequency;
    }

    std::shared_ptr<CameraPropertyDigitalZoomLevel> Camera::DigitalZoomLevel()
    {
        return m_digital_zoom_level;
    }

#pragma endregion Properties

#pragma region Cameras

    std::vector<DirectShowCameraDevice> Camera::getDirectShowCameras()
    {
        std::vector<DirectShowCameraDevice> result;
        bool success = m_directShowCamera->getCameras(result);

        // Throw DirectShow Camera Exception
        if (!success) ThrowDirectShowException();

        return result;
    }

    std::vector<CameraDevice> Camera::getCameras()
    {
        // Get DirectShowCameraDevice
        std::vector<DirectShowCameraDevice> directShowCameras = getDirectShowCameras();

        // Convert to CameraDevice
        std::vector<CameraDevice> result;
        for (int i = 0; i < directShowCameras.size(); i++)
        {
            result.push_back(CameraDevice(directShowCameras[i]));
        }

        return result;
    }

#pragma endregion Cameras

#pragma region DirectShow Video Format

    std::vector<DirectShowVideoFormat> Camera::getSupportDirectShowVideoFormats() const
    {
        return m_directShowCamera->getVideoFormatList();
    }

    bool Camera::setDirectShowVideoFormat(const DirectShowVideoFormat videoFormat)
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

    DirectShowVideoFormat Camera::getDirectShowVideoFormat() const
    {
        return m_directShowCamera->getCurrentGrabberFormat();
    }

#pragma endregion DirectShow Video Format

#pragma region Frame

    bool Camera::getFrame(Frame& frame, const bool onlyGetNewFrame)
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
            m_frameSettings,
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

        // Update frame index
        m_lastFrameIndex = frame.getFrameIndex();

        return true;
    }

    bool Camera::getNewFrame(Frame& frame, const int step, const int timeout, const int skip)
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

    long Camera::getLastFrameIndex() const
    {
        return m_lastFrameIndex;
    }

    double Camera::getFPS() const
    {
        return m_directShowCamera->getFPS();
    }

#pragma region Opencv Function

#ifdef WITH_OPENCV2

    FrameSettings& Camera::getFrameSettings()
    {
        return m_frameSettings;
    }

    cv::Mat Camera::ExposureFusion(
        const ExposureFusionAsyncResult exposureFusionAsyncResult,
        std::vector<cv::Mat>* exposureImages,
        const int minSetExposureDelay
    )
    {
        std::vector<double> exposures = Exposure()->GetPossibleExposureValues();
        
        return ExposureFusion(exposures, exposureFusionAsyncResult, exposureImages, minSetExposureDelay);
    }

    cv::Mat Camera::ExposureFusion(
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

    int Camera::getWidth() const
    {
        return getDirectShowVideoFormat().getWidth();
    }

    int Camera::getHeight() const
    {
        return getDirectShowVideoFormat().getHeight();
    }

    int Camera::getFrameSize() const
    {
        return getDirectShowVideoFormat().getTotalSize();
    }

    int Camera::getNumOfBytePerPixel() const
    {
        return getDirectShowVideoFormat().getBitPerPixel() / 8;
    }

    int Camera::getNumOfPixel() const
    {
        return getDirectShowVideoFormat().getWidth() * getDirectShowVideoFormat().getHeight();
    }

#pragma endregion Frame

#pragma region Exception

    void Camera::ThrowDirectShowException()
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
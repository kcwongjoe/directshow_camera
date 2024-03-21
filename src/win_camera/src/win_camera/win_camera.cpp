#include "win_camera/win_camera.h"
#include "directshow_camera/ds_camera.h"
#include "properties/win_camera_property_brightness.h"

namespace WinCamera
{

#pragma region Constructor and Destructor

    WinCamera::WinCamera() : 
        m_directShowCamera(std::make_shared<DirectShowCamera::DirectShowCamera>())
    {
        constructor();
    }

    WinCamera::WinCamera(
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& abstractDirectShowCamera
    ) :
        m_directShowCamera(abstractDirectShowCamera)
    {
        constructor();
    }

    void WinCamera::constructor()
    {
        DirectShowCameraUtils::initCOMLib();

#ifdef HAS_OPENCV
        m_matConvertor = OpenCVMatConverter();
        m_matConvertor.isBGR = true;
        m_matConvertor.isVerticalFlip = true;
#endif
    }

    WinCamera::~WinCamera()
    {
        close();
        
        // Uninitialize COM Library
        DirectShowCameraUtils::uninitCOMLib();
    }

#pragma endregion Constructor and Destructor

#pragma region Connection

    bool WinCamera::open(
        const WinCameraDevice& device,
        const int width,
        const int height,
        const bool rgb
    )
    {
        bool result = false;

        // Get DirectShowCameraDevice
        std::vector<DirectShowCamera::DirectShowCameraDevice> possibleCamera = getDirectShowCameras();
        int cameraIndex = -1;
        for (int i = 0; i < possibleCamera.size(); i++)
        {
            if (possibleCamera[i].getDevicePath() == device.getDevicePath())
            {
                // Found
                cameraIndex = i;
                result = true;
                break;
            }
        }

        // Get DirectShowVideoFormat
        if (result)
        {
            if (width <= 0 || height <= 0)
            {
                // No specific format
                result = open(possibleCamera[cameraIndex], NULL);
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
                        mediaType = DirectShowCamera::DirectShowVideoFormat::getSupportRGBSubType();
                    }
                    else
                    {
                        // MonoChrome
                        mediaType = DirectShowCamera::DirectShowVideoFormat::getMonochromeSubType();
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
                        result = open(possibleCamera[cameraIndex], &possibleVideoFormat[videoFormatIndex]);
                    }
                    else
                    {
                        // Not Found the size
                        result = false;
                        if (rgb)
                        {
                            m_errorString = "Camera" + device.getFriendlyName() + " does not supported size (" + std::to_string(width) + " x " + std::to_string(height) + " x 3).";
                        }
                        else
                        {
                            m_errorString = "Camera" + device.getFriendlyName() + " does not supported size (" + std::to_string(width) + " x " + std::to_string(height) + " x 1).";
                        }

                    }
                }
                else
                {
                    // Not Found the size
                    result = false;
                    m_errorString = "Camera" + device.getFriendlyName() + " does not supported size (" + std::to_string(width) + " x " + std::to_string(height) + ").";
                }
            }
        }

        return result;
    }

    bool WinCamera::open(const DirectShowCamera::DirectShowCameraDevice& device, DirectShowCamera::DirectShowVideoFormat* videoFormat)
    {
        bool result = false;

        // Get device input filter
        IBaseFilter* videoInputFilter;
        result = m_directShowCamera->getCamera(device, &videoInputFilter);

        // Open
        if (result)
        {
            result = open(&videoInputFilter, videoFormat);
        }
        else
        {
            // Release device input filter
            DirectShowCameraUtils::SafeRelease(&videoInputFilter);

            copyError(result);
        }

        return result;
    }

    bool WinCamera::open(IBaseFilter** videoInputFilter, DirectShowCamera::DirectShowVideoFormat* videoFormat)
    {
        bool result = false;

        // Initialize camera
        result = m_directShowCamera->open(videoInputFilter, videoFormat);

    #ifdef HAS_OPENCV
        if (result)
        {
            allocateMatBuffer();
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

    bool WinCamera::close()
    {
        bool result = true;
        if (m_directShowCamera)
        {
            if (this && result && isCapturing())
            {
                result = stopCapture();
            }

            if (this && result && isOpened())
            {
                m_directShowCamera->close();
            }
        }


        return result;
    }

    bool WinCamera::checkDisconnection()
    {
        return m_directShowCamera->checkDisconnection();
    }

    void WinCamera::setDisconnectionProcess(std::function<void()> func)
    {
        m_directShowCamera->setDisconnectionProcess(func);
    }

#pragma endregion Connection

#pragma region Capture

    bool WinCamera::startCapture()
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

    bool WinCamera::stopCapture()
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

    void WinCamera::resetProperties(const bool asAuto)
    {
        m_directShowCamera->resetDefault(asAuto);
    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperties> WinCamera::getDirectShowProperties() const
    {
        return m_directShowCamera->getProperties();
    }

#pragma region Brightness

    bool WinCamera::supportBrightness() const
    {
        const auto brightness = m_directShowCamera->getProperties()->getBrightness();
        return supportPropertyTemplate(brightness);
    }

    std::pair<long, long> WinCamera::getBrightnessRange() const
    {
        const auto brightness = m_directShowCamera->getProperties()->getBrightness();

        std::pair<long, long> result;
        bool success = getPropertyTemplate(
            [this, &result, &brightness]()
            {
                result = brightness->getRange();
            },
            brightness
        );

        return result;
    }

    long WinCamera::getBrightnessStep() const
    {
        const auto brightness = m_directShowCamera->getProperties()->getBrightness();

        long result = -1;
        bool success = getPropertyTemplate(
            [this, &result, &brightness]()
            {
                result = brightness->getStep();
            },
            brightness
        );

        return result;
    }

    long WinCamera::getBrightness() const
    {
        const auto brightness = m_directShowCamera->getProperties()->getBrightness();

        long result;
        bool success = getPropertyTemplate(
            [this, &result, &brightness]()
            {
                result = brightness->getValue();
            },
            brightness
        );

        if (!success) result = -1;

        return result;
    }

    bool WinCamera::setBrightness(const long value)
    {
        const auto brightness = m_directShowCamera->getProperties()->getBrightness();

        bool result = false;
        bool success = getPropertyTemplate(
            [this, value, &result, &brightness]()
            {
                result = m_directShowCamera->setValue(brightness, value, false);

                copyError(result);
            },
            brightness
        );

        success = success && result;

        return success;
    }

#pragma endregion Brightness

#pragma region Contrast

    /**
     * @brief Retrun true if property contrast is supported.
     * @return Retrun true if property contrast is supported.
    */
    bool WinCamera::supportContrast() const
    {
        const auto contrast = m_directShowCamera->getProperties()->getContrast();

        return supportPropertyTemplate(contrast);
    }

    /**
     * @brief Get the range of the property - contrast
     * @return Return (min,max). (0,0) return if error occurred.
    */
    std::pair<long, long> WinCamera::getContrastRange() const
    {
        const auto contrast = m_directShowCamera->getProperties()->getContrast();

        std::pair<long, long> result;
        bool success = getPropertyTemplate(
            [this, &result, &contrast]()
            {
                result = contrast->getRange();
            },
            contrast
        );

        return result;
    }

    /**
     * @brief Get the step of the property - Contrast.
     * @return Return the step of the Contrast. -1 return if error occurred.
    */
    long WinCamera::getContrastStep() const
    {
        const auto contrast = m_directShowCamera->getProperties()->getContrast();

        long result = -1;
        bool success = getPropertyTemplate(
            [this, &result, &contrast]()
            {
                result = contrast->getStep();
            },
            contrast
        );

        return result;
    }

    /**
     * @brief Get current contrast
     * @return Return current contrast. -1 return if error occurred.
    */
    long WinCamera::getContrast() const
    {
        const auto contrast = m_directShowCamera->getProperties()->getContrast();

        long result;
        bool success = getPropertyTemplate(
            [this, &result, &contrast]()
            {
                result = contrast->getValue();
            },
            contrast
        );

        if (!success) result = -1;

        return result;
    }

    /**
     * @brief Set Contrast
     * @param value Value to be set
     * @return Return true if success.
    */
    bool WinCamera::setContrast(const long value)
    {
        const auto contrast = m_directShowCamera->getProperties()->getContrast();

        bool result = false;
        bool success = getPropertyTemplate(
            [this, value, &result, &contrast]()
            {
                result = m_directShowCamera->setValue(contrast, value, false);

                copyError(result);
            }, 
            contrast
        );
        success = success && result;

        return success;
    }

#pragma endregion Contrast

#pragma region Hue

    /**
     * @brief Retrun true if property hue is supported.
     * @return Retrun true if property hue is supported.
    */
    bool WinCamera::supportHue() const
    {
        const auto hue = m_directShowCamera->getProperties()->getHue();

        return supportPropertyTemplate(hue);
    }

    /**
     * @brief Get the range of the property - hue
     * @return Return (min,max). (0,0) return if error occurred.
    */
    std::pair<long, long> WinCamera::getHueRange() const
    {
        const auto hue = m_directShowCamera->getProperties()->getHue();

        std::pair<long, long> result;
        bool success = getPropertyTemplate(
            [this, &result, &hue]()
            {
                result = hue->getRange();
            },
            hue
        );

        return result;
    }

    /**
     * @brief Get the step of the property - Hue.
     * @return Return the step of the Hue. -1 return if error occurred.
    */
    long WinCamera::getHueStep() const
    {
        const auto hue = m_directShowCamera->getProperties()->getHue();

        long result = -1;
        bool success = getPropertyTemplate(
            [this, &result, &hue]()
            {
                result = hue->getStep();
            },
            hue
        );

        return result;
    }

    /**
     * @brief Get current hue
     * @return Return current hue.
    */
    long WinCamera::getHue() const
    {
        const auto hue = m_directShowCamera->getProperties()->getHue();

        long result;
        bool success = getPropertyTemplate(
            [this, &result, &hue]()
            {
                result = hue->getValue();
            },
            hue
        );

        return result;
    }

    /**
     * @brief Set Hue
     * @param degree Value to be set in degree
     * @return Return true if success.
    */
    bool WinCamera::setHue(const long degree)
    {
        const auto hue = m_directShowCamera->getProperties()->getHue();

        bool result = false;
        bool success = getPropertyTemplate(
            [this, degree, &result, &hue]()
            {
                long value = confirmDegreeRange(degree);
                result = m_directShowCamera->setValue(hue, value, false);

                copyError(result);
            },
            hue
        );
        success = success && result;

        return success;
    }

#pragma endregion Hue

#pragma region Saturation

    /**
     * @brief Retrun true if property saturation is supported.
     * @return Retrun true if property saturation is supported.
    */
    bool WinCamera::supportSaturation() const
    {
        const auto saturation = m_directShowCamera->getProperties()->getSaturation();

        return supportPropertyTemplate(saturation);
    }

    /**
     * @brief Get the range of the property - saturation
     * @return Return (min,max). (0,0) return if error occurred.
    */
    std::pair<long, long> WinCamera::getSaturationRange() const
    {
        const auto saturation = m_directShowCamera->getProperties()->getSaturation();

        std::pair<long, long> result;
        bool success = getPropertyTemplate(
            [this, &result, &saturation]()
            {
                result = saturation->getRange();
            },
            saturation
        );

        return result;
    }

    /**
     * @brief Get the step of the property - Saturation.
     * @return Return the step of the Saturation. -1 return if error occurred.
    */
    long WinCamera::getSaturationStep() const
    {
        const auto saturation = m_directShowCamera->getProperties()->getSaturation();

        long result = -1;
        bool success = getPropertyTemplate(
            [this, &result, &saturation]()
            {
                result = saturation->getStep();
            },
            saturation
        );

        return result;
    }

    /**
     * @brief Get current saturation
     * @return Return current saturation. -1 return if error occurred.
    */
    long WinCamera::getSaturation() const
    {
        const auto saturation = m_directShowCamera->getProperties()->getSaturation();

        long result;
        bool success = getPropertyTemplate(
            [this, &result, &saturation]()
            {
                result = saturation->getValue();
            },
            saturation
        );

        if (!success) result = -1;

        return result;
    }

    /**
     * @brief Set Saturation
     * @param value Value to be set
     * @return Return true if success.
    */
    bool WinCamera::setSaturation(const long value)
    {
        const auto saturation = m_directShowCamera->getProperties()->getSaturation();

        bool result = false;
        bool success = getPropertyTemplate(
            [this, value, &result, &saturation]()
            {
                result = m_directShowCamera->setValue(saturation, value, false);

                copyError(result);
            },
            saturation
        );
        success = success && result;

        return success;
    }

#pragma endregion Saturation

#pragma region Sharpness

    /**
     * @brief Retrun true if property sharpness is supported.
     * @return Retrun true if property sharpness is supported.
    */
    bool WinCamera::supportSharpness() const
    {
        const auto sharpness = m_directShowCamera->getProperties()->getSharpness();

        return supportPropertyTemplate(sharpness);
    }

    /**
     * @brief Get the range of the property - sharpness
     * @return Return (min,max). (0,0) return if error occurred.
    */
    std::pair<long, long> WinCamera::getSharpnessRange() const
    {
        const auto sharpness = m_directShowCamera->getProperties()->getSharpness();

        std::pair<long, long> result;
        bool success = getPropertyTemplate(
            [this, &result, &sharpness]()
            {
                result = sharpness->getRange();
            },
            sharpness
        );

        return result;
    }

    /**
     * @brief Get the step of the property - Sharpness.
     * @return Return the step of the Sharpness. -1 return if error occurred.
    */
    long WinCamera::getSharpnessStep() const
    {
        const auto sharpness = m_directShowCamera->getProperties()->getSharpness();

        long result = -1;
        bool success = getPropertyTemplate(
            [this, &result, &sharpness]()
            {
                result = sharpness->getStep();
            },
            sharpness
        );

        return result;
    }

    /**
     * @brief Get current sharpness
     * @return Return current sharpness. -1 return if error occurred.
    */
    long WinCamera::getSharpness() const
    {
        const auto sharpness = m_directShowCamera->getProperties()->getSharpness();

        long result;
        bool success = getPropertyTemplate(
            [this, &result, &sharpness]()
            {
                result = sharpness->getValue();
            },
            sharpness
        );

        if (!success) result = -1;

        return result;
    }

    /**
     * @brief Set Sharpness
     * @param value Value to be set
     * @return Return true if success.
    */
    bool WinCamera::setSharpness(const long value)
    {
        const auto sharpness = m_directShowCamera->getProperties()->getSharpness();

        bool result = false;
        bool success = getPropertyTemplate(
            [this, value, &result, &sharpness]()
            {
                result = m_directShowCamera->setValue(sharpness, value, false);

                copyError(result);
            },
            sharpness
        );
        success = success && result;

        return success;
    }

#pragma endregion Sharpness

#pragma region Gamma

    /**
     * @brief Retrun true if property gamma is supported.
     * @return Retrun true if property gamma is supported.
    */
    bool WinCamera::supportGamma() const
    {
        const auto gamma = m_directShowCamera->getProperties()->getGamma();

        return supportPropertyTemplate(gamma);
    }

    /**
     * @brief Get the range of the property - gamma
     * @return Return (min,max). (0,0) return if error occurred.
    */
    std::pair<long, long> WinCamera::getGammaRange() const
    {
        const auto gamma = m_directShowCamera->getProperties()->getGamma();

        std::pair<long, long> result;
        bool success = getPropertyTemplate(
            [this, &result, &gamma]()
            {
                result = gamma->getRange();
            },
            gamma
        );

        return result;
    }

    /**
     * @brief Get the step of the property - Gamma.
     * @return Return the step of the Gamma. -1 return if error occurred.
    */
    long WinCamera::getGammaStep() const
    {
        const auto gamma = m_directShowCamera->getProperties()->getGamma();

        long result = -1;
        bool success = getPropertyTemplate(
            [this, &result, &gamma]()
            {
                result = gamma->getStep();
            },
            gamma
        );

        return result;
    }

    /**
     * @brief Get current gamma
     * @return Return current gamma. -1 return if error occurred.
    */
    long WinCamera::getGamma() const
    {
        const auto gamma = m_directShowCamera->getProperties()->getGamma();

        long result;
        bool success = getPropertyTemplate(
            [this, &result, &gamma]()
            {
                result = gamma->getValue();
            },
            gamma
        );

        if (!success) result = -1;

        return result;
    }

    /**
     * @brief Set Gamma
     * @param value Value to be set
     * @return Return true if success.
    */
    bool WinCamera::setGamma(const long value)
    {
        const auto gamma = m_directShowCamera->getProperties()->getGamma();

        bool result = false;
        bool success = getPropertyTemplate(
            [this, value, &result, &gamma]()
            {
                result = m_directShowCamera->setValue(gamma, value, false);

                copyError(result);

                return result;
            },
            gamma
        );

        success = success && result;

        return success;
    }

#pragma endregion Gamma

#pragma region ColorEnable

    /**
     * @brief Retrun true if property Color Enable is supported.
     * @return Retrun true if property Color Enable is supported.
    */
    bool WinCamera::supportColorEnable() const
    {
        const auto colorEnable = m_directShowCamera->getProperties()->getColorEnable();

        return supportPropertyTemplate(colorEnable);
    }

    /**
     * @brief Get current color enable
     * @return Return current color enable. -1 return if error occurred.
    */
    bool WinCamera::isColorEnable() const
    {
        const auto colorEnable = m_directShowCamera->getProperties()->getColorEnable();

        long result;
        bool success = getPropertyTemplate(
            [this, &result, &colorEnable]()
            {
                result = colorEnable->getValue();
            },
            colorEnable
        );

        bool isColorEnable = result == 0 ? false : true; // 0(off), 1(on)
        if (!success) isColorEnable = false;

        return isColorEnable;
    }

    /**
     * @brief Set Color Enable
     * @param isOn Color Enable On = true
     * @return Return true if success.
    */
    bool WinCamera::setColorEnable(const bool isOn)
    {
        const auto colorEnable = m_directShowCamera->getProperties()->getColorEnable();

        bool result = false;
        bool success = getPropertyTemplate(
            [this, isOn, &result, &colorEnable]()
            {
                // Get value
                std::pair<long, long> range = colorEnable->getRange();
                long value = isOn ? range.second : range.first; // 0(off), 1(on)

                // Set
                result = m_directShowCamera->setValue(colorEnable, value, false);

                copyError(result);
            },
            colorEnable
        );

        success = success && result;

        return success;
    }

#pragma endregion Color Enable

#pragma region WhiteBalance

    /**
     * @brief Retrun true if property white balance is supported.
     * @return Retrun true if property white balance is supported.
    */
    bool WinCamera::supportWhiteBalance() const
    {
        const auto whiteBalance = m_directShowCamera->getProperties()->getWhiteBalance();

        return supportPropertyTemplate(whiteBalance);
    }

    /**
     * @brief Get the range of the property - white balance in degree kelvin
     * @return Return (min,max). (0,0) return if error occurred.
    */
    std::pair<long, long> WinCamera::getWhiteBalanceRange() const
    {
        const auto whiteBalance = m_directShowCamera->getProperties()->getWhiteBalance();

        std::pair<long, long> result;
        bool success = getPropertyTemplate(
            [this, &result, &whiteBalance]()
            {
                result = whiteBalance->getRange();
            },
            whiteBalance
        );

        return result;
    }

    /**
     * @brief Get the step of the property - White Balance.
     * @return Return the step of the White Balance. -1 return if error occurred.
    */
    long WinCamera::getWhiteBalanceStep() const
    {
        const auto whiteBalance = m_directShowCamera->getProperties()->getWhiteBalance();

        long result = -1;
        bool success = getPropertyTemplate(
            [this, &result, &whiteBalance]()
            {
                result = whiteBalance->getStep();
            },
            whiteBalance
        );

        return result;
    }

    /**
     * @brief Get current white balance in degree kelvin
     * @return Return current white balance. -1 return if error occurred.
    */
    long WinCamera::getWhiteBalance() const
    {
        const auto whiteBalance = m_directShowCamera->getProperties()->getWhiteBalance();

        long result;
        bool success = getPropertyTemplate(
            [this, &result, &whiteBalance]()
            {
                result = whiteBalance->getValue();
            },
            whiteBalance
        );

        if (!success) result = -1;

        return result;
    }

    /**
     * @brief Set WhiteBalance
     * @param kelvin Value to be set in degree kelvin
     * @return Return true if success.
    */
    bool WinCamera::setWhiteBalance(const long kelvin)
    {
        const auto whiteBalance = m_directShowCamera->getProperties()->getWhiteBalance();

        bool result = false;
        bool success = getPropertyTemplate(
            [this, kelvin, &result, &whiteBalance]()
            {
                result = m_directShowCamera->setValue(whiteBalance, kelvin, false);

                copyError(result);
            },
            whiteBalance
        );

        success = success && result;

        return success;
    }

    /**
     * @brief Retrun true if white balance is auto mode, return false if it is manual mode.
     * @return Retrun true if white balance is auto mode, return false if it is manual mode or error occurred.
    */
    bool WinCamera::isAutoWhiteBalance() const
    {
        const auto whiteBalance = m_directShowCamera->getProperties()->getWhiteBalance();

        bool isAuto = false;
        bool success = getPropertyTemplate(
            [this, &isAuto, &whiteBalance]()
            {
                isAuto = whiteBalance->isAuto();
            },
            whiteBalance,
            "AutoWhiteBalance"
        );

        if (!success) isAuto = false;

        return isAuto;
    }

    /**
     * @brief Set white balance to auto or manual mode
     * @param setToAuto Set it as true if you want to set as auto mode. Manual mode as false.
     * @return Return true if success.
    */
    bool WinCamera::setAutoWhiteBalance(const bool setToAuto)
    {
        const auto whiteBalance = m_directShowCamera->getProperties()->getWhiteBalance();

        bool result = false;
        bool success = getPropertyTemplate(
            [this, setToAuto, &result, &whiteBalance]()
            {
                if (setToAuto && !(whiteBalance->supportAutoMode()))
                {
                    // Not support auto mode
                    result = false;
                    m_errorString = "White Balance does not support auto mode.";
                }
                else if (!setToAuto && !(whiteBalance->supportManualMode()))
                {
                    // Not support manual mode
                    result = false;
                    m_errorString = "White Balance does not support manual mode.";
                }
                else
                {
                    // Set
                    result = m_directShowCamera->setValue(m_directShowCamera->getProperties()->getWhiteBalance(),
                        m_directShowCamera->getProperties()->getWhiteBalance()->getValue(),
                        setToAuto);

                    copyError(result);
                }
            },
            whiteBalance
        );
        success = success && result;

        return success;
    }

#pragma endregion WhiteBalance

#pragma region BacklightCompensation

    /**
     * @brief Retrun true if property backlight compensation is supported.
     * @return Retrun true if property backlight compensation is supported.
    */
    bool WinCamera::supportBacklightCompensation() const
    {
        const auto backlightCompensation = m_directShowCamera->getProperties()->getBacklightCompensation();

        return supportPropertyTemplate(backlightCompensation);
    }


    /**
     * @brief Get current backlight compensation
     * @return Return current backlight compensation. -1 return if error occurred.
    */
    bool WinCamera::isBacklightCompensation() const
    {
        const auto backlightCompensation = m_directShowCamera->getProperties()->getBacklightCompensation();

        long result;
        bool success = getPropertyTemplate(
            [this, &result, &backlightCompensation]()
            {
                result = backlightCompensation->getValue();
            },
            backlightCompensation
        );

        bool isBacklightCompensation = result == 0 ? false : true; // 0(off), 1(on)
        if (!success) isBacklightCompensation = false;

        return isBacklightCompensation;
    }

    /**
     * @brief Set Contrast
     * @param isOn Backlight Compensation On = true
     * @return Return true if success.
    */
    bool WinCamera::setBacklightCompensation(const bool isOn)
    {
        const auto backlightCompensation = m_directShowCamera->getProperties()->getBacklightCompensation();

        bool result = false;
        bool success = getPropertyTemplate(
            [this, isOn, &result, &backlightCompensation]()
            {
                // Get value
                long value = isOn ? 1 : 0; // 0(off), 1(on)

                result = m_directShowCamera->setValue(backlightCompensation, value, false);

                copyError(result);
            },
            backlightCompensation
        );

        success = success && result;

        return success;
    }

#pragma endregion BacklightCompensation

#pragma region Gain

    /**
     * @brief Retrun true if property gain is supported.
     * @return Retrun true if property gain is supported.
    */
    bool WinCamera::supportGain() const
    {
        const auto gain = m_directShowCamera->getProperties()->getGain();

        return supportPropertyTemplate(gain);
    }

    /**
     * @brief Get the range of the property - gain
     * @return Return (min,max). (0,0) return if error occurred.
    */
    std::pair<long, long> WinCamera::getGainRange() const
    {
        const auto gain = m_directShowCamera->getProperties()->getGain();

        std::pair<long, long> result;
        bool success = getPropertyTemplate(
            [this, &result, &gain]()
            {
                result = gain->getRange();
            },
            gain
        );

        return result;
    }

    /**
     * @brief Get the step of the property - gain.
     * @return Return the step of the gain. -1 return if error occurred.
    */
    long WinCamera::getGainStep() const
    {
        const auto gain = m_directShowCamera->getProperties()->getGain();

        long result = -1;
        bool success = getPropertyTemplate(
            [this, &result, &gain]()
            {
                result = gain->getStep();
            },
            gain
        );

        return result;
    }

    /**
     * @brief Get current gain
     * @return Return current gain. -1 return if error occurred.
    */
    long WinCamera::getGain() const
    {
        const auto gain = m_directShowCamera->getProperties()->getGain();

        long result;
        bool success = getPropertyTemplate(
            [this, &result, &gain]()
            {
                result = gain->getValue();
            },
            gain
        );

        if (!success) result = -1;

        return result;
    }

    /**
     * @brief Set Gain
     * @param value Value to be set
     * @return Return true if success.
    */
    bool WinCamera::setGain(const long value)
    {
        const auto gain = m_directShowCamera->getProperties()->getGain();

        bool result = false;
        bool success = getPropertyTemplate(
            [this, value, &result, &gain]()
            {
                result = m_directShowCamera->setValue(gain, value, false);

                copyError(result);
            },
            gain
        );

        success = success && result;

        return success;
    }

#pragma endregion Gain

#pragma region Pan

    /**
     * @brief Retrun true if property pan is supported.
     * @return Retrun true if property pan is supported.
    */
    bool WinCamera::supportPan() const
    {
        const auto pan = m_directShowCamera->getProperties()->getPan();

        return supportPropertyTemplate(pan);
    }

    /**
     * @brief Get the range of the property - pan
     * @return Return (min,max). (0,0) return if error occurred.
    */
    std::pair<long, long> WinCamera::getPanRange() const
    {
        const auto pan = m_directShowCamera->getProperties()->getPan();

        std::pair<long, long> result;
        bool success = getPropertyTemplate(
            [this, &result, &pan]()
            {
                result = pan->getRange();
            },
            pan
        );

        return result;
    }

    /**
     * @brief Get the step of the property - pan.
     * @return Return the step of the pan. -1 return if error occurred.
    */
    long WinCamera::getPanStep() const
    {
        const auto pan = m_directShowCamera->getProperties()->getPan();

        long result = -1;
        bool success = getPropertyTemplate(
            [this, &result, &pan]()
            {
                result = pan->getStep();
            },
            pan
        );

        return result;
    }

    /**
     * @brief Get current pan
     * @return Return current pan. 0 return if error occurred.
    */
    long WinCamera::getPan() const
    {
        const auto pan = m_directShowCamera->getProperties()->getPan();

        long result;
        bool success = getPropertyTemplate(
            [this, &result, &pan]()
            {
                result = pan->getValue();
            },
            pan
        );

        if (!success) result = 0;

        return result;
    }

    /**
     * @brief Set Pan
     * @param degree Value to be set in degree
     * @return Return true if success.
    */
    bool WinCamera::setPan(const long degree)
    {
        const auto pan = m_directShowCamera->getProperties()->getPan();

        bool result = false;
        bool success = getPropertyTemplate(
            [this, degree, &result, &pan]()
            {
                long value = confirmDegreeRange(degree);
                result = m_directShowCamera->setValue(pan, value, false);

                copyError(result);
            },
            pan
        );

        success = success && result;

        return success;
    }

#pragma endregion Pan

#pragma region Tilt

    /**
     * @brief Retrun true if property tilt is supported.
     * @return Retrun true if property tilt is supported.
    */
    bool WinCamera::supportTilt() const
    {
        const auto tilt = m_directShowCamera->getProperties()->getTilt();

        return supportPropertyTemplate(tilt);
    }

    /**
     * @brief Get the range of the property - tilt
     * @return Return (min,max). (0,0) return if error occurred.
    */
    std::pair<long, long> WinCamera::getTiltRange() const
    {
        const auto tilt = m_directShowCamera->getProperties()->getTilt();

        std::pair<long, long> result;
        bool success = getPropertyTemplate(
            [this, &result, &tilt]()
            {
                result = tilt->getRange();
            },
            tilt
        );

        return result;
    }

    /**
     * @brief Get the step of the property - tilt.
     * @return Return the step of the tilt. -1 return if error occurred.
    */
    long WinCamera::getTiltStep() const
    {
        const auto tilt = m_directShowCamera->getProperties()->getTilt();

        long result = -1;
        bool success = getPropertyTemplate(
            [this, &result, &tilt]()
            {
                result = tilt->getStep();
            },
            tilt
        );

        return result;
    }

    /**
     * @brief Get current tilt
     * @return Return current tilt. 0 return if error occurred.
    */
    long WinCamera::getTilt() const
    {
        const auto tilt = m_directShowCamera->getProperties()->getTilt();

        long result;
        bool success = getPropertyTemplate(
            [this, &result, &tilt]()
            {
                result = tilt->getValue();
            },
            tilt
        );

        if (!success) result = 0;

        return result;
    }

    /**
     * @brief Set Tilt
     * @param degree Value to be set in degree
     * @return Return true if success.
    */
    bool WinCamera::setTilt(const long degree)
    {
        const auto tilt = m_directShowCamera->getProperties()->getTilt();

        bool result = false;
        bool success = getPropertyTemplate(
            [this, degree, &result, &tilt]()
            {
                long value = confirmDegreeRange(degree);
                result = m_directShowCamera->setValue(tilt, value, false);

                copyError(result);
            },
            tilt
        );

        success = success && result;

        return success;
    }

#pragma endregion Tilt

#pragma region Roll

    /**
     * @brief Retrun true if property roll is supported.
     * @return Retrun true if property roll is supported.
    */
    bool WinCamera::supportRoll() const
    {
        const auto roll = m_directShowCamera->getProperties()->getRoll();

        return supportPropertyTemplate(roll);
    }

    /**
     * @brief Get the range of the property - roll
     * @return Return (min,max). (0,0) return if error occurred.
    */
    std::pair<long, long> WinCamera::getRollRange() const
    {
        const auto roll = m_directShowCamera->getProperties()->getRoll();

        std::pair<long, long> result;
        bool success = getPropertyTemplate(
            [this, &result, &roll]()
            {
                result = roll->getRange();
            },
            roll
        );

        return result;
    }

    /**
     * @brief Get the step of the property - roll.
     * @return Return the step of the roll. -1 return if error occurred.
    */
    long WinCamera::getRollStep() const
    {
        const auto roll = m_directShowCamera->getProperties()->getRoll();

        long result = -1;
        bool success = getPropertyTemplate(
            [this, &result, &roll]()
            {
                result = roll->getStep();
            },
            roll
        );

        return result;
    }

    /**
     * @brief Get current Roll
     * @return Return current Roll. 0 return if error occurred.
    */
    long WinCamera::getRoll() const
    {
        const auto roll = m_directShowCamera->getProperties()->getRoll();

        long result;
        bool success = getPropertyTemplate(
            [this, &result, &roll]()
            {
                result = roll->getValue();
            },
            roll
        );

        if (!success) result = 0;

        return result;
    }

    /**
     * @brief Set Roll
     * @param degree Value to be set in degree
     * @return Return true if success.
    */
    bool WinCamera::setRoll(const long degree)
    {
        const auto roll = m_directShowCamera->getProperties()->getRoll();

        bool result = false;
        bool success = getPropertyTemplate(
            [this, degree, &result, &roll]()
            {
                long value = confirmDegreeRange(degree);
                result = m_directShowCamera->setValue(roll, value, false);

                copyError(result);
            },
            roll
        );

        success = success && result;

        return success;
    }

#pragma endregion Roll

#pragma region Zoom

    /**
     * @brief Retrun true if property zoom is supported.
     * @return Retrun true if property zoom is supported.
    */
    bool WinCamera::supportZoom() const
    {
        const auto zoom = m_directShowCamera->getProperties()->getZoom();

        return supportPropertyTemplate(zoom);
    }

    /**
     * @brief Get the range of the property - zoom
     * @return Return (min,max). (0,0) return if error occurred.
    */
    std::pair<long, long> WinCamera::getZoomRange() const
    {
        const auto zoom = m_directShowCamera->getProperties()->getZoom();

        std::pair<long, long> result;
        bool success = getPropertyTemplate(
            [this, &result, &zoom]()
            {
                result = zoom->getRange();
            },
            zoom
        );

        return result;
    }

    /**
     * @brief Get the step of the property - zoom.
     * @return Return the step of the zoom. -1 return if error occurred.
    */
    long WinCamera::getZoomStep() const
    {
        const auto zoom = m_directShowCamera->getProperties()->getZoom();

        long result = -1;
        bool success = getPropertyTemplate(
            [this, &result, &zoom]()
            {
                result = zoom->getStep();
            },
            zoom
        );

        return result;
    }

    /**
     * @brief Get current zoom
     * @return Return current zoom. -1 return if error occurred.
    */
    long WinCamera::getZoom() const
    {
        const auto zoom = m_directShowCamera->getProperties()->getZoom();

        long result;
        bool success = getPropertyTemplate(
            [this, &result, &zoom]()
            {
                result = zoom->getValue();
            },
            zoom
        );

        if (!success) result = -1;

        return result;
    }

    /**
     * @brief Set Zoom
     * @param millimeter Value to be set in millimeters
     * @return Return true if success.
    */
    bool WinCamera::setZoom(const long millimeter)
    {
        const auto zoom = m_directShowCamera->getProperties()->getZoom();

        bool result = false;
        bool success = getPropertyTemplate(
            [this, millimeter, &result, &zoom]()
            {
                result = m_directShowCamera->setValue(zoom, millimeter, false);

                copyError(result);
            },
            zoom
        );
        success = success && result;

        return success;
    }

#pragma endregion Zoom

#pragma region Exposure

    /**
     * @brief Retrun true if property exposure is supported.
     * @return Retrun true if property exposure is supported.
    */
    bool WinCamera::supportExposure() const
    {
        const auto exposure = m_directShowCamera->getProperties()->getExposure();

        return supportPropertyTemplate(exposure);
    }

    /**
     * @brief Get the range of the property - exposure in second
     * @return Return (min,max). (0,0) return if error occurred.
    */
    std::pair<double, double> WinCamera::getExposureRange() const
    {
        const auto exposure = m_directShowCamera->getProperties()->getExposure();

        std::pair<double, double> result;
        bool success = getPropertyTemplate(
            [this, &result, &exposure]()
            {
                //Get
                std::pair<long, long> range = exposure->getRange();

                // Convert to second
                result.first = exposureConvertion(range.first);
                result.second = exposureConvertion(range.second);
            },
            exposure
        );

        return result;
    }

    /**
     * @brief Get current exposure in second
     * @return Return current exposure. -1 return if error occurred.
    */
    double WinCamera::getExposure() const
    {
        const auto exposure = m_directShowCamera->getProperties()->getExposure();

        double result;
        bool success = getPropertyTemplate(
            [this, &result, &exposure]()
            {
                // Get value
                long value = exposure->getValue();

                // Convert to second
                result = exposureConvertion(value);
            },
            exposure
        );

        if (!success) result = -1;

        return result;
    }

    /**
     * @brief Set Exposure
     * @param second Value to be set in second
     * @return Return true if success.
    */
    bool WinCamera::setExposure(const double second)
    {
        const auto exposure = m_directShowCamera->getProperties()->getExposure();

        bool result = false;
        bool success = getPropertyTemplate(
            [this, second, &result, &exposure]()
            {
                // Convert to DirectShow value
                long dsValue = exposureConvertion(second);

                // Set
                result = m_directShowCamera->setValue(exposure, dsValue, false);

                copyError(result);
            },
            exposure
        );
        success = success && result;

        return success;
    }

    /**
     * @brief Get all possible exposure values in second
     * @return Return the possible exposure values. Return empty vector if error occurred.
    */
    std::vector<double> WinCamera::getPossibleExposureValues() const
    {
        const auto exposure = m_directShowCamera->getProperties()->getExposure();

        std::vector<double> result;
        std::vector<long> exposureValues;
        bool success = getPropertyTemplate(
            [this, &exposureValues, &exposure]()
            {
                // Get range and step
                std::pair<long, long> range = exposure->getRange();
                long step = exposure->getStep();

                // Add possible value
                long value = range.first;
                while (value < range.second)
                {
                    // Convert to second
                    exposureValues.push_back(value);

                    // To next value
                    value += step;
                }
            },
            exposure
        );

        if (success && exposureValues.size() > 0)
        {
            // Sort and Unique
            std::sort(exposureValues.begin(), exposureValues.end());
            exposureValues.erase(std::unique(exposureValues.begin(), exposureValues.end()), exposureValues.end());

            // Convert to second
            for (int i = 0; i < exposureValues.size(); i++)
            {
                result.push_back(exposureConvertion(exposureValues[i]));
            }

        }

        return result;
    }

    /**
     * @brief Get current exposure index of the getPossibleExposureValues()
     * @return Return current exposure index of the getPossibleExposureValues(). Return -1 if error occurred.
    */
    int WinCamera::getExposureIndex() const
    {
        std::vector<double> exposureList = getPossibleExposureValues();
        double exposureValue = getExposure();
        int result = -1;

        if (exposureList.size() > 0)
        {
            // Find the closest value in the exposureList
            result = 0;
            double distance = abs(exposureList[0] - exposureValue);

            for (int i = 1;i< exposureList.size();i++)
            {
                double currentDistance = abs(exposureList[i] - exposureValue);
                if (currentDistance < distance)
                {
                    result = i;
                    distance = currentDistance;
                }
            }
        }
        
        return result;
    }

    /**
     * @brief Retrun true if exposure is auto mode, return false if it is manual mode.
     * @return Retrun true if exposure is auto mode, return false if it is manual mode or error occurred.
    */
    bool WinCamera::isAutoExposure() const
    {
        const auto exposure = m_directShowCamera->getProperties()->getExposure();

        bool isAuto = false;
        bool success = getPropertyTemplate(
            [this, &isAuto, &exposure]()
            {
                isAuto = exposure->isAuto();
            },
            exposure
        );

        if (!success) isAuto = false;

        return isAuto;
    }

    /**
     * @brief Set exposure to auto or manual mode
     * @param setToAuto Set it as true if you want to set as auto mode. Manual mode as false.
     * @return Return true if success.
    */
    bool WinCamera::setAutoExposure(const bool setToAuto)
    {
        const auto exposure = m_directShowCamera->getProperties()->getExposure();

        bool result = false;
        bool success = getPropertyTemplate(
            [this, setToAuto, &result, &exposure]()
            {
                if (setToAuto && !(exposure->supportAutoMode()))
                {
                    // Not support auto mode
                    result = false;
                    m_errorString = "Exposure does not support auto mode.";
                }
                else if (!setToAuto && !(exposure->supportManualMode()))
                {
                    // Not support manual mode
                    result = false;
                    m_errorString = "Exposure does not support manual mode.";
                }
                else
                {
                    // Set
                    result = m_directShowCamera->setValue(m_directShowCamera->getProperties()->getExposure(),
                        m_directShowCamera->getProperties()->getExposure()->getValue(),
                        setToAuto);

                    copyError(result);
                }
            },
            exposure
        );

        success = success && result;

        return success;
    }

#pragma endregion Exposure

#pragma region Iris

    /**
     * @brief Retrun true if property iris is supported.
     * @return Retrun true if property iris is supported.
    */
    bool WinCamera::supportIris() const
    {
        const auto iris = m_directShowCamera->getProperties()->getIris();

        return supportPropertyTemplate(iris);
    }

    /**
     * @brief Get the range of the property - iris in units of f_stop * 10.
     * @return Return (min,max). (0,0) return if error occurred.
    */
    std::pair<long, long> WinCamera::getIrisRange() const
    {
        const auto iris = m_directShowCamera->getProperties()->getIris();

        std::pair<long, long> result;
        bool success = getPropertyTemplate(
            [this, &result, &iris]()
            {
                result = iris->getRange();
            },
            iris
        );

        return result;
    }

    /**
     * @brief Get the step of the property - iris in units of f_stop * 10.
     * @return Return the step of the iris. -1 return if error occurred.
    */
    long WinCamera::getIrisStep() const
    {
        const auto iris = m_directShowCamera->getProperties()->getIris();

        long result = -1;
        bool success = getPropertyTemplate(
            [this, &result, &iris]()
            {
                result = iris->getStep();
            },
            iris
        );

        return result;
    }

    /**
     * @brief Get current iris in units of f_stop * 10.
     * @return Return current iris. -1 return if error occurred.
    */
    long WinCamera::getIris() const
    {
        const auto iris = m_directShowCamera->getProperties()->getIris();

        long result;
        bool success = getPropertyTemplate(
            [this, &result, &iris]()
            {
                result = iris->getValue();
            },
            iris
        );

        if (!success) result = -1;

        return result;
    }

    /**
     * @brief Set Iris
     * @param value Value to be set in units of f_stop * 10.
     * @return Return true if success.
    */
    bool WinCamera::setIris(const long value)
    {
        const auto iris = m_directShowCamera->getProperties()->getIris();

        bool result = false;
        bool success = getPropertyTemplate(
            [this, value, &result, &iris]()
            {
                result = m_directShowCamera->setValue(iris, value, false);

                copyError(result);
            },
            iris
        );

        success = success && result;

        return success;
    }

    /**
     * @brief Retrun true if iris is auto mode, return false if it is manual mode.
     * @return Retrun true if iris is auto mode, return false if it is manual mode or error occurred.
    */
    bool WinCamera::isAutoIris() const
    {
        const auto iris = m_directShowCamera->getProperties()->getIris();

        bool isAuto = false;
        bool success = getPropertyTemplate(
            [this, &isAuto, &iris]()
            {
                isAuto = iris->isAuto();
            },
            iris
        );

        if (!success) isAuto = false;

        return isAuto;
    }

    /**
     * @brief Set iris to auto or manual mode
     * @param setToAuto Set it as true if you want to set as auto mode. Manual mode as false.
     * @return Return true if success.
    */
    bool WinCamera::setAutoIris(const bool setToAuto)
    {
        const auto iris = m_directShowCamera->getProperties()->getIris();

        bool result = false;
        bool success = getPropertyTemplate(
            [this, setToAuto, &result, &iris]()
            {
                if (setToAuto && !(iris->supportAutoMode()))
                {
                    // Not support auto mode
                    result = false;
                    m_errorString = "Iris does not support auto mode.";
                }
                else if (!setToAuto && !(m_directShowCamera->getProperties()->getIris()->supportManualMode()))
                {
                    // Not support manual mode
                    result = false;
                    m_errorString = "Iris does not support manual mode.";
                }
                else
                {
                    // Set
                    result = m_directShowCamera->setValue(m_directShowCamera->getProperties()->getIris(),
                        m_directShowCamera->getProperties()->getIris()->getValue(),
                        setToAuto);

                    copyError(result);
                }
            },
            iris
        );

        success = success && result;

        return success;
    }

#pragma endregion Iris

#pragma region Focus

    /**
     * @brief Retrun true if property focus is supported.
     * @return Retrun true if property focus is supported.
    */
    bool WinCamera::supportFocus() const
    {
        const auto focus = m_directShowCamera->getProperties()->getFocus();

        return supportPropertyTemplate(focus);
    }

    /**
     * @brief Get the range of the property - focus in millimeters.
     * @return Return (min,max). (0,0) return if error occurred.
    */
    std::pair<long, long> WinCamera::getFocusRange() const
    {
        const auto focus = m_directShowCamera->getProperties()->getFocus();

        std::pair<long, long> result;
        bool success = getPropertyTemplate(
            [this, &result, &focus]()
            {
                result = focus->getRange();
            },
            focus
        );

        return result;
    }

    /**
     * @brief Get the step of the property - focus in millimeters.
     * @return Return the step of the property - focus in millimeters. -1 return if error occurred.
    */
    long WinCamera::getFocusStep() const
    {
        const auto focus = m_directShowCamera->getProperties()->getFocus();

        long result = -1;
        bool success = getPropertyTemplate(
            [this, &result, &focus]()
            {
                result = focus->getStep();
            },
            focus
        );

        return result;
    }

    /**
     * @brief Get current focus in millimeters.
     * @return Return current focus. -1 return if error occurred.
    */
    long WinCamera::getFocus() const
    {
        const auto focus = m_directShowCamera->getProperties()->getFocus();

        long result;
        bool success = getPropertyTemplate(
            [this, &result, &focus]()
            {
                result = focus->getValue();
            },
            focus
        );

        if (!success) result = -1;

        return result;
    }

    /**
     * @brief Set Focus
     * @param millimeter Value to be set in millimeters.
     * @return Return true if success.
    */
    bool WinCamera::setFocus(const long millimeter)
    {
        const auto focus = m_directShowCamera->getProperties()->getFocus();

        bool result = false;
        bool success = getPropertyTemplate(
            [this, millimeter, &result, &focus]()
            {
                result = m_directShowCamera->setValue(focus, millimeter, false);

                copyError(result);
            },
            focus
        );
        success = success && result;

        return success;
    }

    /**
     * @brief Retrun true if focus is auto mode, return false if it is manual mode.
     * @return Retrun true if focus is auto mode, return false if it is manual mode or error occurred.
    */
    bool WinCamera::isAutoFocus() const
    {
        const auto focus = m_directShowCamera->getProperties()->getFocus();

        bool isAuto = false;
        bool success = getPropertyTemplate(
            [this, &isAuto, &focus]()
            {
                isAuto = focus->isAuto();
            },
            focus
        );

        if (!success) isAuto = false;

        return isAuto;
    }

    /**
     * @brief Set focus to auto or manual mode
     * @param setToAuto Set it as true if you want to set as auto mode. Manual mode as false.
     * @return Return true if success.
    */
    bool WinCamera::setAutoFocus(const bool setToAuto)
    {
        const auto focus = m_directShowCamera->getProperties()->getFocus();

        bool result = false;
        bool success = getPropertyTemplate(
            [this, setToAuto, &result, &focus]()
            {
                if (setToAuto && !(focus->supportAutoMode()))
                {
                    // Not support auto mode
                    result = false;
                    m_errorString = "Focus does not support auto mode.";
                }
                else if (!setToAuto && !(focus->supportManualMode()))
                {
                    // Not support manual mode
                    result = false;
                    m_errorString = "Focus does not support manual mode.";
                }
                else
                {
                    // Set
                    result = m_directShowCamera->setValue(focus,
                        focus->getValue(),
                        setToAuto);

                    copyError(result);
                }
            },
            focus
        );
        success = success && result;

        return success;
    }

#pragma endregion Focus

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

    bool WinCamera::setDirectShowVideoFormat(DirectShowCamera::DirectShowVideoFormat* videoFormat)
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

#ifdef HAS_OPENCV
            allocateMatBuffer();
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

#ifdef HAS_OPENCV

    void WinCamera::setMatAsBGR(const bool asBGR)
    {
        m_matConvertor.isBGR = asBGR;
    }

    void WinCamera::vecticalFlipMat(const bool verticalFlip)
    {
        m_matConvertor.isVerticalFlip = verticalFlip;
    }

    bool WinCamera::allocateMatBuffer()
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
            allocateMatBuffer();
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

    cv::Mat WinCamera::exposureFusion(
        const ExposureFusionAsyncResult exposureFusionAsyncResult,
        std::vector<cv::Mat>* exposureImages,
        const int minSetExposureDelay
    )
    {
        std::vector<double> exposures = getPossibleExposureValues();
        
        return exposureFusion(exposures, exposureFusionAsyncResult, exposureImages, minSetExposureDelay);
    }

    cv::Mat WinCamera::exposureFusion(
        const std::vector<double> exposures,
        const ExposureFusionAsyncResult exposureFusionAsyncResult,
        std::vector<cv::Mat>* exposureImages,
        const int minSetExposureDelay
    )
    {
        // Save current exposure
        double currentExposure = getExposure();
        bool isAutoExposureNow = isAutoExposure();

        bool releaseExposureImages = false;
        if (!exposureImages)
        {
            exposureImages = new std::vector<cv::Mat>();
            releaseExposureImages = true;
        }

        // Get images in different exposure
        for (int i = 0; i < exposures.size(); i++)
        {
            setExposure(exposures[i]);

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
            setAutoExposure(true);
        }
        else
        {
            setExposure(currentExposure);
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

    /**
     * @brief A utils to convert degree to -180 to 180
     * @param degree Degree to be converted
     * @return Return a degree within -180 and 180
    */
    int WinCamera::confirmDegreeRange(int degree)
    {
        int result = 0;
        if (degree % 360 != 0)
        {
            bool positiveRegion = sin(degree * 0.01745329251994329576923690722222) > 0.0 ? true : false;

            if (degree > 0)
            {
                if (positiveRegion)
                {
                    // 0 to 180
                    result = degree % 180;
                }
                else
                {
                    // 0 to -180
                    result = degree % 180 - 180;
                }
            }
            else
            {
                if (positiveRegion)
                {
                    // 0 to 180
                    result = 180 + degree % 180;
                }
                else
                {
                    // 0 to -180
                    result = degree % 180;
                }
            }
        }

        return result;
    }

    /**
     * @brief Convert DirectShow exposure value to seconds
     * @param dsValue DirectShow exposure value
     * @return Return exposure in second
    */
    double WinCamera::exposureConvertion(const long dsValue) const
    {
        if (dsValue < 0)
        {
            return pow(0.5, (double)-dsValue);
        }
        else
        {
            return pow(2, (double)dsValue);
        }
    }

    /**
     * @brief Convert exposure value in second to DirectShow exposure value
     * @param second Exposure value in second
     * @return Return DirectShow exposure value
    */
    long WinCamera::exposureConvertion(const double second) const
    {
        if (second <= 0)
        {
            return 0;
        }
        else
        {
            return (long)lround(log2(second));
        }

    }

#pragma endregion Utils
}
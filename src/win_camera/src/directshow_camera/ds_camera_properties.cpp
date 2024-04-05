#include "directshow_camera/ds_camera_properties.h"

#include "directshow_camera/utils/ds_camera_utils.h"


namespace DirectShowCamera
{
#pragma region Constructor

    /**
     * @brief Dummy constructor, use DirectShowCameraProperties(IBaseFilter*, std::string*) instead.
    */
    DirectShowCameraProperties::DirectShowCameraProperties()
    {
        construct();
        m_isinitialized = false;
    }

    /**
     * @brief Constuctor
     * @param[in] videoInputFilter Video input filter. Property will be load from this filter.
     * @param[out] errorString Error string
    */
    DirectShowCameraProperties::DirectShowCameraProperties(IBaseFilter* videoInputFilter, std::string& errorString)
    {
        construct();
        refresh(videoInputFilter, errorString);
    }

    /**
     * @brief Destructor
    */
    DirectShowCameraProperties::~DirectShowCameraProperties()
    {

    }

    void DirectShowCameraProperties::construct()
    {
        m_brightness = std::make_shared<DirectShowCameraProperty>("Brightness", VideoProcAmp_Brightness, DirectShowCameraProperty::USE_AM_VIDEO_PROC_AMP);
        m_contrast = std::make_shared<DirectShowCameraProperty>("Contrast", VideoProcAmp_Contrast, DirectShowCameraProperty::USE_AM_VIDEO_PROC_AMP);
        m_hue = std::make_shared<DirectShowCameraProperty>("Hue", VideoProcAmp_Hue, DirectShowCameraProperty::USE_AM_VIDEO_PROC_AMP);
        m_saturation = std::make_shared<DirectShowCameraProperty>("Saturation", VideoProcAmp_Saturation, DirectShowCameraProperty::USE_AM_VIDEO_PROC_AMP);
        m_sharpness = std::make_shared<DirectShowCameraProperty>("Sharpness", VideoProcAmp_Sharpness, DirectShowCameraProperty::USE_AM_VIDEO_PROC_AMP);
        m_gamma = std::make_shared<DirectShowCameraProperty>("Gamma", VideoProcAmp_Gamma, DirectShowCameraProperty::USE_AM_VIDEO_PROC_AMP);
        m_colorEnable = std::make_shared<DirectShowCameraProperty>("Color Enable", VideoProcAmp_ColorEnable, DirectShowCameraProperty::USE_AM_VIDEO_PROC_AMP); // 0(off), 1(on)
        m_whiteBalance = std::make_shared<DirectShowCameraProperty>("White Balance", VideoProcAmp_WhiteBalance, DirectShowCameraProperty::USE_AM_VIDEO_PROC_AMP);
        m_backlightCompensation = std::make_shared<DirectShowCameraProperty>("Backlight Compensation", VideoProcAmp_BacklightCompensation, DirectShowCameraProperty::USE_AM_VIDEO_PROC_AMP);  // 0(off), 1(on)
        m_gain = std::make_shared<DirectShowCameraProperty>("Gain", VideoProcAmp_Gain, DirectShowCameraProperty::USE_AM_VIDEO_PROC_AMP);
        m_pan = std::make_shared<DirectShowCameraProperty>("Pan", CameraControl_Pan, DirectShowCameraProperty::USE_AM_CAMERA_CONTROL);
        m_tilt = std::make_shared<DirectShowCameraProperty>("Tilt", CameraControl_Tilt, DirectShowCameraProperty::USE_AM_CAMERA_CONTROL);
        m_roll = std::make_shared<DirectShowCameraProperty>("Roll", CameraControl_Roll, DirectShowCameraProperty::USE_AM_CAMERA_CONTROL);
        m_zoom = std::make_shared<DirectShowCameraProperty>("Zoom", CameraControl_Zoom, DirectShowCameraProperty::USE_AM_CAMERA_CONTROL);
        m_exposure = std::make_shared<DirectShowCameraProperty>("Exposure", CameraControl_Exposure, DirectShowCameraProperty::USE_AM_CAMERA_CONTROL);
        m_iris = std::make_shared<DirectShowCameraProperty>("Iris", CameraControl_Iris, DirectShowCameraProperty::USE_AM_CAMERA_CONTROL);
        m_focus = std::make_shared<DirectShowCameraProperty>("Focus", CameraControl_Focus, DirectShowCameraProperty::USE_AM_CAMERA_CONTROL);
    }

    /**
     * @brief Reset variables
    */
    void DirectShowCameraProperties::reset()
    {
        m_brightness->reset();
        m_contrast->reset();
        m_hue->reset();
        m_saturation->reset();
        m_sharpness->reset();
        m_gamma->reset();
        m_colorEnable->reset();
        m_whiteBalance->reset();
        m_backlightCompensation->reset();
        m_gain->reset();
        m_pan->reset();
        m_tilt->reset();
        m_roll->reset();
        m_zoom->reset();
        m_exposure->reset();
        m_iris->reset();
        m_focus->reset();

        m_isinitialized = false;
    }

    void DirectShowCameraProperties::resetDefault(IBaseFilter* videoInputFilter, std::string& errorString, const bool asAuto)
    {
        PropertySetAsDefault(m_brightness,videoInputFilter, errorString, asAuto);
        PropertySetAsDefault(m_contrast, videoInputFilter, errorString, asAuto);
        PropertySetAsDefault(m_hue, videoInputFilter, errorString, asAuto);
        PropertySetAsDefault(m_saturation, videoInputFilter, errorString, asAuto);
        PropertySetAsDefault(m_sharpness, videoInputFilter, errorString, asAuto);
        PropertySetAsDefault(m_gamma, videoInputFilter, errorString, asAuto);
        PropertySetAsDefault(m_colorEnable, videoInputFilter, errorString, asAuto);
        PropertySetAsDefault(m_whiteBalance, videoInputFilter, errorString, asAuto);
        PropertySetAsDefault(m_backlightCompensation, videoInputFilter, errorString, asAuto);
        PropertySetAsDefault(m_gain, videoInputFilter, errorString, asAuto);
        PropertySetAsDefault(m_pan, videoInputFilter, errorString, asAuto);
        PropertySetAsDefault(m_tilt, videoInputFilter, errorString, asAuto);
        PropertySetAsDefault(m_roll, videoInputFilter, errorString, asAuto);
        PropertySetAsDefault(m_zoom, videoInputFilter, errorString, asAuto);
        PropertySetAsDefault(m_exposure, videoInputFilter, errorString, asAuto);
        PropertySetAsDefault(m_iris, videoInputFilter, errorString, asAuto);
        PropertySetAsDefault(m_focus, videoInputFilter, errorString, asAuto);

        m_isinitialized = true;
    }

    bool DirectShowCameraProperties::PropertySetAsDefault(std::shared_ptr<DirectShowCameraProperty>& property, IBaseFilter* videoInputFilter, std::string& errorString, const bool asAuto)
    {
        std::string errorStr;
        bool result =  property->setAsDefault(videoInputFilter, errorStr);

        // Amend error string
        if (!errorStr.empty())
        {
            errorString = errorString + '\n' + errorStr;
        }

        return result;
    }

    /**
     * @brief Refresh properties from video input filter.
     * @param[in] videoInputFilter Video input filter. Property will be load from this filter.
     * @param[out] errorString Error string
    */
    void DirectShowCameraProperties::refresh(IBaseFilter* videoInputFilter, std::string& errorString)
    {
        reset();
        bool success = true;

        // Get properties
        success = DirectShowCameraUtils::AmVideoProcAmpDecorator(videoInputFilter,
            [this, errorString](IAMVideoProcAmp* videoProcAmp)
            {
                // Import property
                m_brightness->importProperty(videoProcAmp);
                m_contrast->importProperty(videoProcAmp);
                m_hue->importProperty(videoProcAmp);
                m_saturation->importProperty(videoProcAmp);
                m_sharpness->importProperty(videoProcAmp);
                m_gamma->importProperty(videoProcAmp);
                m_colorEnable->importProperty(videoProcAmp);
                m_whiteBalance->importProperty(videoProcAmp);
                m_backlightCompensation->importProperty(videoProcAmp);
                m_gain->importProperty(videoProcAmp);
            },
            errorString
        );

        success = DirectShowCameraUtils::AmCameraControlDecorator(videoInputFilter,
            [this, errorString](IAMCameraControl* cameraControl)
            {
                // Import property
                m_pan->importProperty(cameraControl);
                m_tilt->importProperty(cameraControl);
                m_roll->importProperty(cameraControl);
                m_zoom->importProperty(cameraControl);
                m_exposure->importProperty(cameraControl);
                m_iris->importProperty(cameraControl);
                m_focus->importProperty(cameraControl);
            },
            errorString
        );

        m_isinitialized = true;
    }

    /**
     * @brief Mark this object as initialized.
    */
    void DirectShowCameraProperties::markAsInitialized()
    {
        m_isinitialized = true;
    }

#pragma endregion Constructor

#pragma region Getter

    /**
     * @brief Get brightness, from blanking(small value) to pure white(large value)
     * @return Return brightness
    */
    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getBrightness() const
    {
        return m_brightness;
    }

    /**
     * @brief Get contrast
     * @return Return contrast
    */
    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getContrast() const
    {
        return m_contrast;
    }

    /**
     * @brief Get hue (-180 to +180 degrees)
     * 
     * @return Return hue
    */
    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getHue() const
    {
        return m_hue;
    }

    /**
     * @brief Get saturation
     * @return Return saturation
    */
    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getSaturation() const
    {
        return m_saturation;
    }

    /**
     * @brief Get sharpness
     * @return Return sharpness
    */
    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getSharpness() const
    {
        return m_sharpness;
    }

    /**
     * @brief Get gamma
     * @return Return gamma
    */
    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getGamma() const
    {
        return m_gamma;
    }

    /**
     * @brief Get color enable, 0(off) or 1(on)
     * @return 
    */
    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getColorEnable() const
    {
        return m_colorEnable;
    }

    /**
     * @brief Get white balance
     * @return Return white balance
    */
    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getWhiteBalance() const
    {
        return m_whiteBalance;
    }

    /**
     * @brief Get backlight compensation, 0(off) or 1(on)
     * @return Return backlight compensation
    */
    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getBacklightCompensation() const
    {
        return m_backlightCompensation;
    }

    /**
     * @brief Get gain, +ve are brighter and -ve are darker
     * @return Get gain
    */
    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getGain() const
    {
        return m_gain;
    }

    /**
     * @brief Get pan, in degree?
     * @return Return pan
    */
    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getPan() const
    {
        return m_pan;
    }

    /**
     * @brief Get tilt, in degree?
     * @return Return tilt
    */
    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getTilt() const
    {
        return m_tilt;
    }

    /**
     * @brief Get roll, in degree?
     * @return Return roll
    */
    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getRoll() const
    {
        return m_roll;
    }

    /**
     * @brief Get zoom, in millimeters
     * @return Return zoom
    */
    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getZoom() const
    {
        return m_zoom;
    }

    /**
     * @brief Get exposure, value = log2(sec) which means sec = 0.5^value(value < 0) or 2^value(value > 0) , eg. value = -3 sec = 0.125s, value = 2 sec = 4
     * @return Return exposure
    */
    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getExposure() const
    {
        return m_exposure;
    }

    /**
     * @brief Get iris, fstop * 10
     * @return Return iris
    */
    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getIris() const
    {
        return m_iris;
    }

    /**
     * @brief Get focus, in millimeters
     * @return Return focus
    */
    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getFocus() const
    {
        return m_focus;
    }

#pragma endregion Getter
}

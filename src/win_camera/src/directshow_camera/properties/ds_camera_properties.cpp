#include "directshow_camera/properties/ds_camera_properties.h"

#include "directshow_camera/utils/ds_camera_utils.h"

namespace DirectShowCamera
{
#pragma region Constructor

    DirectShowCameraProperties::DirectShowCameraProperties()
    {
        Construct();
        m_isinitialized = false;
    }

    DirectShowCameraProperties::DirectShowCameraProperties(IBaseFilter* videoInputFilter, std::string& errorString)
    {
        Construct();
        Refresh(videoInputFilter, errorString);
    }

    DirectShowCameraProperties::~DirectShowCameraProperties()
    {

    }

    void DirectShowCameraProperties::Construct()
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

#pragma endregion Constructor and Destructor

#pragma region Properties Update

    void DirectShowCameraProperties::Reset()
    {
        m_brightness->Reset();
        m_contrast->Reset();
        m_hue->Reset();
        m_saturation->Reset();
        m_sharpness->Reset();
        m_gamma->Reset();
        m_colorEnable->Reset();
        m_whiteBalance->Reset();
        m_backlightCompensation->Reset();
        m_gain->Reset();
        m_pan->Reset();
        m_tilt->Reset();
        m_roll->Reset();
        m_zoom->Reset();
        m_exposure->Reset();
        m_iris->Reset();
        m_focus->Reset();

        m_isinitialized = false;
    }

    void DirectShowCameraProperties::ResetToDefaultValue(IBaseFilter* videoInputFilter, std::string& errorString, const bool asAuto)
    {
        SetToDefaultValue(m_brightness,videoInputFilter, errorString, asAuto);
        SetToDefaultValue(m_contrast, videoInputFilter, errorString, asAuto);
        SetToDefaultValue(m_hue, videoInputFilter, errorString, asAuto);
        SetToDefaultValue(m_saturation, videoInputFilter, errorString, asAuto);
        SetToDefaultValue(m_sharpness, videoInputFilter, errorString, asAuto);
        SetToDefaultValue(m_gamma, videoInputFilter, errorString, asAuto);
        SetToDefaultValue(m_colorEnable, videoInputFilter, errorString, asAuto);
        SetToDefaultValue(m_whiteBalance, videoInputFilter, errorString, asAuto);
        SetToDefaultValue(m_backlightCompensation, videoInputFilter, errorString, asAuto);
        SetToDefaultValue(m_gain, videoInputFilter, errorString, asAuto);
        SetToDefaultValue(m_pan, videoInputFilter, errorString, asAuto);
        SetToDefaultValue(m_tilt, videoInputFilter, errorString, asAuto);
        SetToDefaultValue(m_roll, videoInputFilter, errorString, asAuto);
        SetToDefaultValue(m_zoom, videoInputFilter, errorString, asAuto);
        SetToDefaultValue(m_exposure, videoInputFilter, errorString, asAuto);
        SetToDefaultValue(m_iris, videoInputFilter, errorString, asAuto);
        SetToDefaultValue(m_focus, videoInputFilter, errorString, asAuto);

        m_isinitialized = true;
    }

    bool DirectShowCameraProperties::SetToDefaultValue(
        std::shared_ptr<DirectShowCameraProperty>& property,
        IBaseFilter* videoInputFilter,
        std::string& errorString,
        const bool asAuto
    )
    {
        std::string errorStr;
        bool result =  property->setToDefaultValue(videoInputFilter, errorStr);

        // Amend error string
        if (!errorStr.empty())
        {
            errorString = errorString + '\n' + errorStr;
        }

        return result;
    }

    void DirectShowCameraProperties::Refresh(IBaseFilter* videoInputFilter, std::string& errorString)
    {
        Reset();
        bool success = true;

        // Get properties
        success = DirectShowCameraUtils::AmVideoProcAmpDecorator(videoInputFilter,
            [this, errorString](IAMVideoProcAmp* videoProcAmp)
            {
                // Import property
                m_brightness->ImportProperty(videoProcAmp);
                m_contrast->ImportProperty(videoProcAmp);
                m_hue->ImportProperty(videoProcAmp);
                m_saturation->ImportProperty(videoProcAmp);
                m_sharpness->ImportProperty(videoProcAmp);
                m_gamma->ImportProperty(videoProcAmp);
                m_colorEnable->ImportProperty(videoProcAmp);
                m_whiteBalance->ImportProperty(videoProcAmp);
                m_backlightCompensation->ImportProperty(videoProcAmp);
                m_gain->ImportProperty(videoProcAmp);
            },
            errorString
        );

        success = DirectShowCameraUtils::AmCameraControlDecorator(videoInputFilter,
            [this, errorString](IAMCameraControl* cameraControl)
            {
                // Import property
                m_pan->ImportProperty(cameraControl);
                m_tilt->ImportProperty(cameraControl);
                m_roll->ImportProperty(cameraControl);
                m_zoom->ImportProperty(cameraControl);
                m_exposure->ImportProperty(cameraControl);
                m_iris->ImportProperty(cameraControl);
                m_focus->ImportProperty(cameraControl);
            },
            errorString
        );

        m_isinitialized = true;
    }

    void DirectShowCameraProperties::MarkAsInitialized()
    {
        m_isinitialized = true;
    }

#pragma endregion Properties Update

#pragma region Properties

    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getBrightness() const
    {
        return m_brightness;
    }

    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getContrast() const
    {
        return m_contrast;
    }

    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getHue() const
    {
        return m_hue;
    }

    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getSaturation() const
    {
        return m_saturation;
    }

    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getSharpness() const
    {
        return m_sharpness;
    }

    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getGamma() const
    {
        return m_gamma;
    }

    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getColorEnable() const
    {
        return m_colorEnable;
    }

    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getWhiteBalance() const
    {
        return m_whiteBalance;
    }

    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getBacklightCompensation() const
    {
        return m_backlightCompensation;
    }

    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getGain() const
    {
        return m_gain;
    }

    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getPan() const
    {
        return m_pan;
    }

    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getTilt() const
    {
        return m_tilt;
    }

    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getRoll() const
    {
        return m_roll;
    }

    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getZoom() const
    {
        return m_zoom;
    }

    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getExposure() const
    {
        return m_exposure;
    }

    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getIris() const
    {
        return m_iris;
    }

    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getFocus() const
    {
        return m_focus;
    }

#pragma endregion Properties
}

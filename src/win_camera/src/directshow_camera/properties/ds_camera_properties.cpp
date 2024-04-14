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
        m_brightness = std::make_shared<DirectShowCameraAMVideoProcAmpProperty>("Brightness", VideoProcAmp_Brightness);
        m_contrast  = std::make_shared<DirectShowCameraAMVideoProcAmpProperty>("Contrast", VideoProcAmp_Contrast);
        m_hue = std::make_shared<DirectShowCameraAMVideoProcAmpProperty>("Hue", VideoProcAmp_Hue);
        m_saturation = std::make_shared<DirectShowCameraAMVideoProcAmpProperty>("Saturation", VideoProcAmp_Saturation);
        m_sharpness = std::make_shared<DirectShowCameraAMVideoProcAmpProperty>("Sharpness", VideoProcAmp_Sharpness);
        m_gamma = std::make_shared<DirectShowCameraAMVideoProcAmpProperty>("Gamma", VideoProcAmp_Gamma);
        m_colorEnable = std::make_shared<DirectShowCameraAMVideoProcAmpProperty>("Color Enable", VideoProcAmp_ColorEnable); // 0(off), 1(on)
        m_whiteBalance = std::make_shared<DirectShowCameraAMVideoProcAmpProperty>("White Balance", VideoProcAmp_WhiteBalance);
        m_backlightCompensation = std::make_shared<DirectShowCameraAMVideoProcAmpProperty>("Backlight Compensation", VideoProcAmp_BacklightCompensation);  // 0(off), 1(on)
        m_gain = std::make_shared<DirectShowCameraAMVideoProcAmpProperty>("Gain", VideoProcAmp_Gain);
        m_pan = std::make_shared<DirectShowCameraCameraControlProperty>("Pan", CameraControl_Pan);
        m_tilt = std::make_shared<DirectShowCameraCameraControlProperty>("Tilt", CameraControl_Tilt);
        m_roll = std::make_shared<DirectShowCameraCameraControlProperty>("Roll", CameraControl_Roll);
        m_zoom = std::make_shared<DirectShowCameraCameraControlProperty>("Zoom", CameraControl_Zoom);
        m_exposure = std::make_shared<DirectShowCameraCameraControlProperty>("Exposure", CameraControl_Exposure);
        m_iris = std::make_shared<DirectShowCameraCameraControlProperty>("Iris", CameraControl_Iris);
        m_focus = std::make_shared<DirectShowCameraCameraControlProperty>("Focus", CameraControl_Focus);
        m_powerlineFrequency = std::make_shared<DirectShowCameraPowerlineFrequencyProperty>();
        m_digitalZoomLevel = std::make_shared<DirectShowCameraDigitalZoomLevelProperty>();
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
        m_powerlineFrequency->Reset();
        m_digitalZoomLevel->Reset();

        m_isinitialized = false;
    }

    void DirectShowCameraProperties::ResetToDefaultValue(IBaseFilter* videoInputFilter, std::string& errorString, const bool asAuto)
    {
        ResetToDefaultValue(m_brightness,videoInputFilter, errorString, asAuto);
        ResetToDefaultValue(m_contrast, videoInputFilter, errorString, asAuto);
        ResetToDefaultValue(m_hue, videoInputFilter, errorString, asAuto);
        ResetToDefaultValue(m_saturation, videoInputFilter, errorString, asAuto);
        ResetToDefaultValue(m_sharpness, videoInputFilter, errorString, asAuto);
        ResetToDefaultValue(m_gamma, videoInputFilter, errorString, asAuto);
        ResetToDefaultValue(m_colorEnable, videoInputFilter, errorString, asAuto);
        ResetToDefaultValue(m_whiteBalance, videoInputFilter, errorString, asAuto);
        ResetToDefaultValue(m_backlightCompensation, videoInputFilter, errorString, asAuto);
        ResetToDefaultValue(m_gain, videoInputFilter, errorString, asAuto);
        ResetToDefaultValue(m_pan, videoInputFilter, errorString, asAuto);
        ResetToDefaultValue(m_tilt, videoInputFilter, errorString, asAuto);
        ResetToDefaultValue(m_roll, videoInputFilter, errorString, asAuto);
        ResetToDefaultValue(m_zoom, videoInputFilter, errorString, asAuto);
        ResetToDefaultValue(m_exposure, videoInputFilter, errorString, asAuto);
        ResetToDefaultValue(m_iris, videoInputFilter, errorString, asAuto);
        ResetToDefaultValue(m_focus, videoInputFilter, errorString, asAuto);
        ResetToDefaultValue(m_powerlineFrequency, videoInputFilter, errorString, asAuto);
        ResetToDefaultValue(m_digitalZoomLevel, videoInputFilter, errorString, asAuto);

        m_isinitialized = true;
    }

    bool DirectShowCameraProperties::Refresh(IBaseFilter* videoInputFilter, std::string& errorString)
    {
        // Reset
        Reset();
        
        // Get properties

        // Get properties from IAMVideoProcAmp
        std::string amVideoProcAmpErrorString;
        const auto amVideoProcAmpSuccess = DirectShowCameraUtils::AmVideoProcAmpDecorator(
            videoInputFilter,
            [this](IAMVideoProcAmp* videoProcAmp)
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
            amVideoProcAmpErrorString
        );
        if (!amVideoProcAmpErrorString.empty())  errorString += amVideoProcAmpErrorString;

        // Get properties from IAMCameraControl
        std::string amCameraControlErrorString;
        const auto amCameraControlSuccess = DirectShowCameraUtils::AmCameraControlDecorator(
            videoInputFilter,
            [this](IAMCameraControl* cameraControl)
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
            amCameraControlErrorString
        );
        if (!amCameraControlErrorString.empty())  errorString += amCameraControlErrorString;

        // Get properties from IVideoProcAmp
        std::string videoProcAmpErrorString;
        const auto videoProcAmpSuccess = DirectShowCameraUtils::VideoProcAmpDecorator(
            videoInputFilter,
            [this, videoInputFilter](IVideoProcAmp* videoProcAmp)
            {
                // Import property
                m_powerlineFrequency->ImportProperty(videoProcAmp);
                m_digitalZoomLevel->ImportProperty(videoProcAmp, videoInputFilter);
            },
            videoProcAmpErrorString
        );
        if (!videoProcAmpErrorString.empty())  errorString += videoProcAmpErrorString;
        
        // Check success
        if (amVideoProcAmpSuccess && amCameraControlSuccess && videoProcAmpSuccess)
        {
            m_isinitialized = true;
            return true;
        }
        else
        {
            return false;
        }

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

    std::shared_ptr<DirectShowCameraProperty> DirectShowCameraProperties::getPowerlineFrequency() const
    {
        return m_powerlineFrequency;
    }

#pragma endregion Properties
}

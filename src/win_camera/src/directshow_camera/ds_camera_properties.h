#pragma once
#ifndef WIN_CAMERA__DIRECTSHOW_CAMERA__DIRECTSHOW_CAMERA_PROPERTIES_H
#define WIN_CAMERA__DIRECTSHOW_CAMERA__DIRECTSHOW_CAMERA_PROPERTIES_H

//************Content************

#include "directshow_camera/ds_camera_property.h"

namespace DirectShowCamera
{
    /**
     * @brief A class to store the DirectShow Camera Properties
     * 
     */
    class DirectShowCameraProperties
    {
    public:

        // Constructor
        DirectShowCameraProperties();
        DirectShowCameraProperties(IBaseFilter* videoInputFilter, std::string& errorString);
        ~DirectShowCameraProperties();

        void reset();
        void resetDefault(IBaseFilter* videoInputFilter, std::string& errorString, const bool asAuto);
        void refresh(IBaseFilter* videoInputFilter, std::string& errorString);
        void markAsInitialized();

        // Getter
        std::shared_ptr<DirectShowCameraProperty> getBrightness() const;
        std::shared_ptr<DirectShowCameraProperty> getContrast() const;
        std::shared_ptr<DirectShowCameraProperty> getHue() const;
        std::shared_ptr<DirectShowCameraProperty> getSaturation() const;
        std::shared_ptr<DirectShowCameraProperty> getSharpness() const;
        std::shared_ptr<DirectShowCameraProperty> getGamma() const;
        std::shared_ptr<DirectShowCameraProperty> getColorEnable() const;
        std::shared_ptr<DirectShowCameraProperty> getWhiteBalance() const;
        std::shared_ptr<DirectShowCameraProperty> getBacklightCompensation() const;
        std::shared_ptr<DirectShowCameraProperty> getGain() const;
        std::shared_ptr<DirectShowCameraProperty> getPan() const;
        std::shared_ptr<DirectShowCameraProperty> getTilt() const;
        std::shared_ptr<DirectShowCameraProperty> getRoll() const;
        std::shared_ptr<DirectShowCameraProperty> getZoom() const;
        std::shared_ptr<DirectShowCameraProperty> getExposure() const;
        std::shared_ptr<DirectShowCameraProperty> getIris() const;
        std::shared_ptr<DirectShowCameraProperty> getFocus() const;

        // Class to string

        operator std::string() const
        {
            std::string result;
            if (m_isinitialized)
            {
                result = "---Camera Properties---\n";
                result += (std::string)*m_brightness + "\n";
                result += (std::string)*m_contrast + "\n";
                result += (std::string)*m_hue + "\n";
                result += (std::string)*m_saturation + "\n";
                result += (std::string)*m_sharpness + "\n";
                result += (std::string)*m_gamma + "\n";
                result += (std::string)*m_colorEnable + "\n";
                result += (std::string)*m_whiteBalance + "\n";
                result += (std::string)*m_backlightCompensation + "\n";
                result += (std::string)*m_gain + "\n";
                result += (std::string)*m_pan + "\n";
                result += (std::string)*m_tilt + "\n";
                result += (std::string)*m_roll + "\n";
                result += (std::string)*m_zoom + "\n";
                result += (std::string)*m_exposure + "\n";
                result += (std::string)*m_iris + "\n";
                result += (std::string)*m_focus;
            }
            else
            {
                result = "Camera Properties(Not initialized)";
            }

            return result;
        }

        friend std::ostream& operator << (std::ostream& out, const DirectShowCameraProperties& obj) {
            return out << (std::string)obj;
        }

    private:
        void construct();
        bool PropertySetAsDefault(
            std::shared_ptr<DirectShowCameraProperty>& property,
            IBaseFilter* videoInputFilter,
            std::string& errorString,
            const bool asAuto
        );
    private:

        // Properties
        std::shared_ptr<DirectShowCameraProperty> m_brightness;
        std::shared_ptr<DirectShowCameraProperty> m_contrast;
        std::shared_ptr<DirectShowCameraProperty> m_hue;
        std::shared_ptr<DirectShowCameraProperty> m_saturation;
        std::shared_ptr<DirectShowCameraProperty> m_sharpness;
        std::shared_ptr<DirectShowCameraProperty> m_gamma;
        std::shared_ptr<DirectShowCameraProperty> m_colorEnable;
        std::shared_ptr<DirectShowCameraProperty> m_whiteBalance;
        std::shared_ptr<DirectShowCameraProperty> m_backlightCompensation;
        std::shared_ptr<DirectShowCameraProperty> m_gain;
        std::shared_ptr<DirectShowCameraProperty> m_pan;
        std::shared_ptr<DirectShowCameraProperty> m_tilt;
        std::shared_ptr<DirectShowCameraProperty> m_roll;
        std::shared_ptr<DirectShowCameraProperty> m_zoom;
        std::shared_ptr<DirectShowCameraProperty> m_exposure;
        std::shared_ptr<DirectShowCameraProperty> m_iris;
        std::shared_ptr<DirectShowCameraProperty> m_focus;

        bool m_isinitialized = false;

    };
}

//*******************************

#endif
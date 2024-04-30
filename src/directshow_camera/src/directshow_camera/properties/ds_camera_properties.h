/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef DIRECTSHOW_CAMERA__DIRECTSHOW_CAMERA__PROPERTIES__DIRECTSHOW_CAMERA_PROPERTIES_H
#define DIRECTSHOW_CAMERA__DIRECTSHOW_CAMERA__PROPERTIES__DIRECTSHOW_CAMERA_PROPERTIES_H

//************Content************

#include "directshow_camera/properties/ds_camera_property.h"
#include "directshow_camera/properties/ds_camera_camera_control_property.h"
#include "directshow_camera/properties/ds_camera_am_video_proc_amp_property.h"
#include "directshow_camera/properties/ds_camera_powerline_frequency_property.h"
#include "directshow_camera/properties/ds_camera_digital_zoom_level_property.h"

#include <memory>

namespace DirectShowCamera
{
    /**
     * @brief A class to store the DirectShow Camera Properties
     * 
     */
    class DirectShowCameraProperties
    {
    public:

#pragma region Constructor and Destructor
        /**
         * @brief Dummy constructor, use DirectShowCameraProperties(IBaseFilter*, std::string*) instead.
        */
        DirectShowCameraProperties();

        /**
         * @brief Constuct the DirectShow Camera Properties from IBaseFilter
         * @param[in] directShowFilter DirectShow Filter. Property will be load from this filter.
         * @param[out] errorString Error string
        */
        DirectShowCameraProperties(IBaseFilter* directShowFilter, std::string& errorString);

        /**
         * @brief Destructor
        */
        ~DirectShowCameraProperties();

#pragma endregion Constructor and Destructor

#pragma region Properties Update

        /**
         * @brief Reset variables
        */
        void Reset();

        /**
         * @brief Reset the DirectShow Camera Properties to the default value from IBaseFilter
         * @param[in] directShowFilter DirectShow Filter. Property will be load from this filter.
         * @param[out] errorString Error string
         * @param[in] asAuto (Optional) If this value is true, properties will try to set as auto. Default as true
        */
        void ResetToDefaultValue(IBaseFilter* directShowFilter, std::string& errorString, const bool asAuto = true);

        /**
         * @brief Refresh properties from DirectShow Filter.
         * @param[in] directShowFilter DirectShow Filter. Property will be load from this filter.
         * @param[out] errorString Error string
         * @return Return true if success.
        */
        bool Refresh(IBaseFilter* directShowFilter, std::string& errorString);

        /**
         * @brief Mark this object as initialized.
        */
        void MarkAsInitialized();

#pragma endregion Properties Update

#pragma region Properties

        /**
         * @brief Get brightness, from blanking(small value) to pure white(large value)
         * @return Return brightness
        */
        std::shared_ptr<DirectShowCameraProperty> getBrightness() const;

        /**
         * @brief Get contrast
         * @return Return contrast
        */
        std::shared_ptr<DirectShowCameraProperty> getContrast() const;

        /**
         * @brief Get hue (-180 to +180 degrees)
         *
         * @return Return hue
        */
        std::shared_ptr<DirectShowCameraProperty> getHue() const;

        /**
         * @brief Get saturation
         * @return Return saturation
        */
        std::shared_ptr<DirectShowCameraProperty> getSaturation() const;

        /**
         * @brief Get sharpness
         * @return Return sharpness
        */
        std::shared_ptr<DirectShowCameraProperty> getSharpness() const;

        /**
         * @brief Get gamma
         * @return Return gamma
        */
        std::shared_ptr<DirectShowCameraProperty> getGamma() const;

        /**
         * @brief Get color enable, 0(off) or 1(on)
         * @return
        */
        std::shared_ptr<DirectShowCameraProperty> getColorEnable() const;

        /**
         * @brief Get white balance
         * @return Return white balance
        */
        std::shared_ptr<DirectShowCameraProperty> getWhiteBalance() const;

        /**
         * @brief Get backlight compensation, 0(off) or 1(on)
         * @return Return backlight compensation
        */
        std::shared_ptr<DirectShowCameraProperty> getBacklightCompensation() const;

        /**
         * @brief Get gain, +ve are brighter and -ve are darker
         * @return Get gain
        */
        std::shared_ptr<DirectShowCameraProperty> getGain() const;

        /**
         * @brief Get pan, in degree?
         * @return Return pan
        */
        std::shared_ptr<DirectShowCameraProperty> getPan() const;

        /**
         * @brief Get tilt, in degree?
         * @return Return tilt
        */
        std::shared_ptr<DirectShowCameraProperty> getTilt() const;

        /**
         * @brief Get roll, in degree?
         * @return Return roll
        */
        std::shared_ptr<DirectShowCameraProperty> getRoll() const;

        /**
         * @brief Get zoom, in millimeters
         * @return Return zoom
        */
        std::shared_ptr<DirectShowCameraProperty> getZoom() const;

        /**
         * @brief Get exposure, value = log2(sec) which means sec = 0.5^value(value < 0) or 2^value(value > 0) , eg. value = -3 sec = 0.125s, value = 2 sec = 4
         * @return Return exposure
        */
        std::shared_ptr<DirectShowCameraProperty> getExposure() const;

        /**
         * @brief Get iris, fstop * 10
         * @return Return iris
        */
        std::shared_ptr<DirectShowCameraProperty> getIris() const;

        /**
         * @brief Get focus, in millimeters
         * @return Return focus
        */
        std::shared_ptr<DirectShowCameraProperty> getFocus() const;

        /**
        * @brief Get powerline frequency, 0 : Disable, 1 : 50 Hz, 2: 60 Hz
        * @return Return powerline frequency
        */
        std::shared_ptr<DirectShowCameraProperty> getPowerlineFrequency() const;

        /**
        * @brief Get digital zoom level
        * @return Return digital zoom level
        */
        std::shared_ptr<DirectShowCameraProperty> getDigitalZoomLevel() const;

#pragma endregion Properties

#pragma region operator

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
                result += (std::string)*m_focus + "\n";
                result += (std::string)*m_powerlineFrequency + "\n";
                result += (std::string)*m_digitalZoomLevel;
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

#pragma endregion operator

    private:
        /**
        * @brief Construct the DirectShow Camera Properties
        */
        void Construct();

        /**
        * @brief Set the DirectShow Camera Property to the default value from IBaseFilter
        *
        * @tparam PropertyType The property type, DirectShowCameraProperty, DirectShowCameraAMVideoProcAmpProperty ,DirectShowCameraCameraControlProperty, DirectShowCameraPowerlineFrequencyProperty or DirectShowCameraDigitalZoomLevelProperty
        * @param[in] property The property to set
        * @param[in] directShowFilter DirectShow Filter. Property will be load from this filter.
        * @param[out] errorString Error string
        * @param[in] asAuto (Optional) If this value is true, properties will try to set as auto. Default as true
        */
        template<class PropertyType>
        bool ResetToDefaultValue(
            std::shared_ptr<PropertyType>& property,
            IBaseFilter* directShowFilter,
            std::string& errorString,
            const bool asAuto
        )
        {
            // Check PropertyType type
            static_assert(
                std::is_same<PropertyType, DirectShowCameraProperty>::value ||
                std::is_same<PropertyType, DirectShowCameraAMVideoProcAmpProperty>::value ||
                std::is_same<PropertyType, DirectShowCameraCameraControlProperty>::value ||
                std::is_same<PropertyType, DirectShowCameraPowerlineFrequencyProperty>::value ||
                std::is_same<PropertyType, DirectShowCameraDigitalZoomLevelProperty>::value
                ,
                "PropertyType must be DirectShowCameraProperty, DirectShowCameraAMVideoProcAmpProperty, DirectShowCameraCameraControlProperty, DirectShowCameraPowerlineFrequencyProperty, DirectShowCameraDigitalZoomLevelProperty."
                );

            std::string errorStr;
            bool result = property->setToDefaultValue(directShowFilter, errorStr);

            // Amend error string
            if (!errorStr.empty())
            {
                errorString = errorString + '\n' + errorStr;
            }

            return result;
        }

    private:

        // Properties
        std::shared_ptr<DirectShowCameraAMVideoProcAmpProperty> m_brightness;
        std::shared_ptr<DirectShowCameraAMVideoProcAmpProperty> m_contrast;
        std::shared_ptr<DirectShowCameraAMVideoProcAmpProperty> m_hue;
        std::shared_ptr<DirectShowCameraAMVideoProcAmpProperty> m_saturation;
        std::shared_ptr<DirectShowCameraAMVideoProcAmpProperty> m_sharpness;
        std::shared_ptr<DirectShowCameraAMVideoProcAmpProperty> m_gamma;
        std::shared_ptr<DirectShowCameraAMVideoProcAmpProperty> m_colorEnable;
        std::shared_ptr<DirectShowCameraAMVideoProcAmpProperty> m_whiteBalance;
        std::shared_ptr<DirectShowCameraAMVideoProcAmpProperty> m_backlightCompensation;
        std::shared_ptr<DirectShowCameraAMVideoProcAmpProperty> m_gain;
        std::shared_ptr<DirectShowCameraCameraControlProperty> m_pan;
        std::shared_ptr<DirectShowCameraCameraControlProperty> m_tilt;
        std::shared_ptr<DirectShowCameraCameraControlProperty> m_roll;
        std::shared_ptr<DirectShowCameraCameraControlProperty> m_zoom;
        std::shared_ptr<DirectShowCameraCameraControlProperty> m_exposure;
        std::shared_ptr<DirectShowCameraCameraControlProperty> m_iris;
        std::shared_ptr<DirectShowCameraCameraControlProperty> m_focus;
        std::shared_ptr<DirectShowCameraPowerlineFrequencyProperty> m_powerlineFrequency;
        std::shared_ptr<DirectShowCameraDigitalZoomLevelProperty> m_digitalZoomLevel;

        bool m_isinitialized = false;

    };
}

//*******************************

#endif
#pragma once
#ifndef WIN_CAMERA__DIRECTSHOW_CAMERA__DIRECTSHOW_CAMERA_PROPERTY_H
#define WIN_CAMERA__DIRECTSHOW_CAMERA__DIRECTSHOW_CAMERA_PROPERTY_H

//************Content************

#include "directshow_camera/utils/ds_camera_utils.h"

namespace DirectShowCamera
{
    /**
     * @brief A class to store the DirectShow Camera Property
     * 
     */
    class DirectShowCameraProperty
    {
    // Const
    public: 
        static const int USE_AM_VIDEO_PROC_AMP = 0;
        static const int USE_AM_CAMERA_CONTROL = 1;

    public:

        // Constuctor
        DirectShowCameraProperty();
        DirectShowCameraProperty(std::string name, long propertyEnum, int queryInterface);

        void reset();

        // Support
        bool isSupported() const;
        bool supportAutoMode() const;
        bool supportManualMode() const;

        // Import

        void importProperty(bool supported, long min, long max, long step, long defaultValue, bool isAuto, long value, bool supportAuto, bool supportManual);

        /**
         * @brief Import property from IAMVideoProcAmp/IAMCameraControl
         * @tparam CameraControl IAMVideoProcAmp or IAMCameraControl
         * @param cameraControl IAMVideoProcAmp or IAMCameraControl
         * @return Return true if success.
        */
        template<class CameraControl>
        bool importProperty(CameraControl* cameraControl)
        {
            HRESULT hr = NO_ERROR;
            long min;
            long max;
            long step;
            long defaultValue;
            long availableCapsFlag;

            // get range
            hr = cameraControl->GetRange(getDSEnum(), &min, &max, &step, &defaultValue, &availableCapsFlag);

            if (SUCCEEDED(hr))
            {
                m_supportAuto = capsFlagIsAuto(availableCapsFlag);
                m_supportManual = capsFlagIsManual(availableCapsFlag);

                // Set range
                m_min = min;
                m_max = max;
                m_step = step;
                m_defaultValue = defaultValue;

                // Set current value
                importPropertyValue(cameraControl);

                // Set as initialzied
                m_supported = true;

                return true;
            }
            else
            {
                return false;
            }
        }

        /**
         * @brief Import current value of the property from IAMVideoProcAmp/IAMCameraControl
         * @tparam CameraControl IAMVideoProcAmp or IAMCameraControl
         * @param cameraControl IAMVideoProcAmp or IAMCameraControl
         * @return Return true if success.
        */
        template<class CameraControl>
        bool importPropertyValue(CameraControl* cameraControl)
        {
            HRESULT hr = NO_ERROR;

            // Get current value
            long value;
            long capsFlag;
            hr = cameraControl->Get(getDSEnum(), &value, &capsFlag);

            if (SUCCEEDED(hr))
            {
                // Set current value
                m_value = value;
                m_isAuto = capsFlagIsAuto(capsFlag);

                return true;
            }
            else
            {
                return false;
            }
        }

        /**
         * @brief Set the property to default value.
         * @param[in] videoInputFilter Video Input filter (Camera). If it is NULL, value will not set in through directshow and just update the object value.
         * @param[out] errorString Error string
         * @param[in] asAuto (Optional) The property will try to set as auto mode if this value is true. Default is true.
         * @return Return true if success
        */
        bool setAsDefault(IBaseFilter* videoInputFilter, std::string& errorString, const bool asAuto = true);

        /**
         * @brief Set property
         * @param[in] videoInputFilter Video Input filter (Camera). If it is NULL, value will not set in through directshow and just update the object value.
         * @param[in] value Value to be set
         * @param[in] isAutoMode Set as auto mode?
         * @param[out] errorString Error String
         * @return Return true if success.
        */
        bool setValue(IBaseFilter* videoInputFilter, const long value, const bool isAutoMode, std::string& errorString);

        // Getter
        std::string getName() const;
        long getDSEnum() const;
        std::pair<long, long> getRange() const;
        long getMin() const;
        long getMax() const;
        long getStep() const;
        long getDefault() const;
        long getValue() const;
        bool isAuto() const;

        // Class to string

        operator std::string() const
        {
            std::string result;
            if (m_supported)
            {
                std::string isAutoStr = m_isAuto ? "(Auto)" : "(Manual)";

                std::string supportMode;
                if (m_supportAuto)
                {
                    supportMode = "Auto";
                }
                if (m_supportManual)
                {
                    if (!supportMode.empty()) supportMode += "|";

                    supportMode += "Manual";
                }

                result = m_name + "(Value: " + std::to_string(m_value) + isAutoStr + ", Min: " + std::to_string(m_min) + ", Max: " + std::to_string(m_max) + ", Step: " + std::to_string(m_step) + ", Default: " + std::to_string(m_defaultValue) + ", Support mode: " + supportMode + ")";
            }
            else
            {
                result = m_name + "(Not supported)";
            }

            return result;
        }

        friend std::ostream& operator << (std::ostream& out, const DirectShowCameraProperty& obj) {
            return out << (std::string)obj;
        }
    private:

        /**
         * @brief Property Name, use for casting to string
        */
        std::string m_name;
        /**
         * @brief Property Enumeration in directshow
        */
        long m_enum = 0;
        /**
         * @brief USE_AM_VIDEO_PROC_AMP or USE_AM_CAMERA_CONTROL
        */
        int m_queryInterface = -1;

        long m_min;
        long m_max;
        long m_step;
        long m_defaultValue;
        bool m_isAuto;
        long m_value;

        bool m_supported;
        bool m_supportAuto;
        bool m_supportManual;

        /**
         * @brief Set value to IAMVideoProcAmp or IAMCameraControl
         * @tparam CameraControl IAMVideoProcAmp or IAMCameraControl
         * @param cameraControl
         * @param value
         * @param isAutoMode
         * @return
        */
        template<class CameraControl> bool setValueTemplate(CameraControl* cameraControl, long value, bool isAutoMode)
        {
            bool result = true;

            // Set
            HRESULT hr = cameraControl->Set(m_enum, value, isAutoToCapsFlag(isAutoMode));
            if (SUCCEEDED(hr))
            {
                // Success, update variable
                m_value = value;
                m_isAuto = isAutoMode;
                result = true;
            }
            else
            {
                // Fail
                result = false;
            }

            return result;
        }

        // Utils
        bool capsFlagIsAuto(long capsFlags) const;
        bool capsFlagIsManual(long capsFlags) const;
        long isAutoToCapsFlag(bool isAuto) const;
    };
}

//*******************************

#endif

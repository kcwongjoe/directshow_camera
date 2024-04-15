/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef WIN_CAMERA__DIRECTSHOW_CAMERA__PROPERTIES__DIRECTSHOW_CAMERA_PROPERTY_H
#define WIN_CAMERA__DIRECTSHOW_CAMERA__PROPERTIES__DIRECTSHOW_CAMERA_PROPERTY_H

//************Content************

#include "directshow_camera/ds_header.h"
#include <string>

namespace DirectShowCamera
{
    /**
     * @brief A class to store the DirectShow Camera Property. This is just a basic class. To enable the DirectShow feature, use DirectShowCameraAMVideoProcAmpProperty or DirectShowCameraCameraControlProperty instead.
     * 
     */
    class DirectShowCameraProperty
    {
    public:

#pragma region Constructor and Destructor

        /**
         * @brief Dummy constructor, use DirectShowCameraProperty(string) instead.
        */
        DirectShowCameraProperty();

        /**
         * @brief Constructor
         * @param[in] name Property Name, use for casting to string
        */
        DirectShowCameraProperty(
            const std::string name
        );

        /**
         * @brief Reset
        */
        virtual void Reset();

#pragma endregion Constructor and Destructor

#pragma region Support Mode

        /**
         * @brief Return true if camera supported this property
         * @return Return true if camera supported this property
        */
        bool isSupported() const;

        /**
         * @brief Return true if property support auto mode.
         * @return Return true if property support auto mode.
        */
        bool isSupportAutoMode() const;

        /**
         * @brief Return true if property support manual mode.
         * @return Return true if property support manual mode.
        */
        bool isSupportManualMode() const;

#pragma endregion Support Mode

#pragma region Import

        /**
         * @brief Import property
         *
         * @param[in] supported Is camera supported?
         * @param[in] min Min value
         * @param[in] max Max value
         * @param[in] step Step
         * @param[in] defaultValue Default value
         * @param[in] isAuto Is auto?
         * @param[in] value Current value
         * @param[in] supportAuto Support auto?
         * @param[in] supportManual Support manual?
        */
        void DirectShowCameraProperty::ImportProperty(
            const bool supported,
            const long min,
            const long max,
            const long step,
            const long defaultValue,
            const bool isAuto,
            const long value,
            const bool supportAuto,
            const bool supportManual,
            const long capsFlag = 0
        );

#pragma endregion Import

#pragma region Setter

        /**
         * @brief Set the property to default value.
         * @param[in] directShowFilter DirectShow Filter (Camera). If it is NULL, value will not set in through directshow and just update the object value.
         * @param[out] errorString Error string
         * @param[in] asAuto (Optional) The property will try to set as auto mode if this value is true. Default is true.
         * @return Return true if success
        */
        bool setToDefaultValue(IBaseFilter* directShowFilter, std::string& errorString, const bool asAuto = true);

        /**
         * @brief Set property
         * @param[in] directShowFilter DirectShow Filter (Camera). If it is NULL, value will not set in through directshow and just update the object value.
         * @param[in] value Value to be set
         * @param[in] isAutoMode Set as auto mode?
         * @param[out] errorString Error String
         * @return Return true if success.
        */
        bool setValue(IBaseFilter* directShowFilter, const long value, const bool isAutoMode, std::string& errorString);

#pragma endregion Setter

#pragma region Getter

        /**
         * @brief Get the property name
         * @return Return the property name
        */
        std::string getName() const;

        /**
         * @brief Get the range of the property
         * @return Return the range of the property
        */
        std::pair<long, long> getRange() const;

        /**
         * @brief Get the bottom limit of the property
         * @return Return the bottom limit of the property
        */
        long getMin() const;

        /**
         * @brief Get the upper limit of the property
         * @return Return the upper limit of the property
        */
        long getMax() const;

        /**
         * @brief Get the step value of the property
         * @return Return the step value of the property
        */
        long getStep() const;

        /**
         * @brief Get the default value of the property
         * @return Return the default value of the property
        */
        long getDefaultValue() const;

        /**
         * @brief Get the current value of the property
         * @return Return the current value of the property
        */
        long getValue() const;

        /**
         * @brief Return true if the current mode is auto, otherise manual mode is enabled.
         * @return Return true if the current mode is auto, otherise manual mode is enabled.
        */
        bool isAuto() const;

#pragma endregion Getter

#pragma region operator
        
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

#pragma endregion operator

    protected:

        bool CheckValue(
            const long value,
            const bool isAutoMode,
            std::string& errorString
        );

    protected:

        typedef std::function<bool(IBaseFilter* directShowFilter, const long value, const bool isAutoMode, std::string& errorString)> SetValueFunction;
        SetValueFunction m_setValueFunc = nullptr;

        /**
         * @brief Property Name, use for casting to string
        */
        std::string m_name;

        long m_min = 0;
        long m_max = 0;
        long m_step = 0;
        long m_defaultValue = 0;
        bool m_isAuto = true;
        long m_value = 0;
        long m_capsFlag = 0;

        bool m_supported = false;
        bool m_supportAuto = false;
        bool m_supportManual = false;

    };
}

//*******************************

#endif

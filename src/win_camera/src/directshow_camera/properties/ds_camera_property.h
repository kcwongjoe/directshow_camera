#pragma once
#ifndef WIN_CAMERA__DIRECTSHOW_CAMERA__PROPERTIES__DIRECTSHOW_CAMERA_PROPERTY_H
#define WIN_CAMERA__DIRECTSHOW_CAMERA__PROPERTIES__DIRECTSHOW_CAMERA_PROPERTY_H

//************Content************

#include "directshow_camera/ds_header.h"
#include <string>

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
        static const int USE_VIDEO_PROC_AMP = 2;

    public:

#pragma region Constructor and Destructor

        /**
         * @brief Dummy constructor, use DirectShowCameraProperty(string, long, int) instead.
        */
        DirectShowCameraProperty();

        /**
         * @brief Constructor
         * @param[in] name Property Name, use for casting to string
         * @param[in] propertyEnum Property Enumeration in directshow
         * @param[in] queryInterface Query interface, USE_AM_VIDEO_PROC_AMP or USE_AM_CAMERA_CONTROL
        */
        DirectShowCameraProperty(
            const std::string name,
            const long propertyEnum,
            const int queryInterface
        );

        /**
         * @brief Reset
        */
        void Reset();

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
        void ImportProperty(
            const bool supported,
            const long min,
            const long max,
            const long step,
            const long defaultValue,
            const bool isAuto,
            const long value,
            const bool supportAuto,
            const bool supportManual
        );

        /**
         * @brief Import property from IAMVideoProcAmp/IAMCameraControl
         * @tparam CameraControl IAMVideoProcAmp or IAMCameraControl
         * @param[in] cameraControl IAMVideoProcAmp or IAMCameraControl
         * @return Return true if success.
        */
        template<class CameraControl>
        bool ImportProperty(CameraControl* cameraControl)
        {
            // Check CameraControl type
            static_assert(
                std::is_same<CameraControl, IAMVideoProcAmp>::value ||
                std::is_same<CameraControl, IAMCameraControl>::value,
                "CameraControl must be IAMVideoProcAmp or IAMCameraControl."
                );

            HRESULT hr = NO_ERROR;
            long min;
            long max;
            long step;
            long defaultValue;
            long availableCapsFlag;

            // get range
            hr = cameraControl->GetRange(getDSEnum(), &min, &max, &step, &defaultValue, &availableCapsFlag);

            if (hr == S_OK)
            {
                m_supportAuto = isAutoCapsFlag(availableCapsFlag);
                m_supportManual = isManualCapsFlag(availableCapsFlag);

                // Set range
                m_min = min;
                m_max = max;
                m_step = step;
                m_defaultValue = defaultValue;

                // Set current value
                ImportPropertyValue(cameraControl);

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
         * @param[in] cameraControl IAMVideoProcAmp or IAMCameraControl
         * @return Return true if success.
        */
        template<class CameraControl>
        bool ImportPropertyValue(CameraControl* cameraControl)
        {
            // Check CameraControl type
            static_assert(
                std::is_same<CameraControl, IAMVideoProcAmp>::value ||
                std::is_same<CameraControl, IAMCameraControl>::value,
                "CameraControl must be IAMVideoProcAmp or IAMCameraControl."
                );

            HRESULT hr = NO_ERROR;

            // Get current value
            long value;
            long capsFlag;
            hr = cameraControl->Get(getDSEnum(), &value, &capsFlag);

            if (hr == S_OK)
            {
                // Set current value
                m_value = value;
                m_isAuto = isAutoCapsFlag(capsFlag);

                return true;
            }
            else
            {
                return false;
            }
        }

#pragma endregion Import

#pragma region Setter

        /**
         * @brief Set the property to default value.
         * @param[in] videoInputFilter Video Input filter (Camera). If it is NULL, value will not set in through directshow and just update the object value.
         * @param[out] errorString Error string
         * @param[in] asAuto (Optional) The property will try to set as auto mode if this value is true. Default is true.
         * @return Return true if success
        */
        bool setToDefaultValue(IBaseFilter* videoInputFilter, std::string& errorString, const bool asAuto = true);

        /**
         * @brief Set property
         * @param[in] videoInputFilter Video Input filter (Camera). If it is NULL, value will not set in through directshow and just update the object value.
         * @param[in] value Value to be set
         * @param[in] isAutoMode Set as auto mode?
         * @param[out] errorString Error String
         * @return Return true if success.
        */
        bool setValue(IBaseFilter* videoInputFilter, const long value, const bool isAutoMode, std::string& errorString);

#pragma endregion Setter

#pragma region Getter

        /**
         * @brief Get the property name
         * @return Return the property name
        */
        std::string getName() const;

        /**
         * @brief Get the property Enumeration in DirectShow
         * @return Return the property Enumeration in DirectShow
        */
        long getDSEnum() const;

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

    private:

    /**
     * @brief Set value to IAMVideoProcAmp or IAMCameraControl
     * @tparam CameraControl IAMVideoProcAmp or IAMCameraControl
     * @param[in] cameraControl
     * @param[in] value
     * @param[in] isAutoMode
     * @return
    */
    template<class CameraControl> bool setValueTemplate(
        CameraControl* cameraControl,
        const long value,
        const bool isAutoMode
    )
    {
        // Check CameraControl type
        static_assert(
            std::is_same<CameraControl, IAMVideoProcAmp>::value ||
            std::is_same<CameraControl, IAMCameraControl>::value,
            "CameraControl must be IAMVideoProcAmp or IAMCameraControl."
            );

        bool result = true;

        // Set
        HRESULT hr = cameraControl->Set(m_enum, value, getCapsFlag(isAutoMode));
        if (hr == S_OK)
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

#pragma region CapsFlag

    /**
     * @brief Check whether the Capsflags contains auto mode.
     * @param[in] capsFlags CapsFlag
     * @return Return true if the Capsflags contains auto mode.
    */
    bool isAutoCapsFlag(const long capsFlags) const;

    /**
     * @brief Check whether the Capsflags contains manual mode.
     * @param[in] capsFlags CapsFlag
     * @return Return true if the Capsflags contains manual mode.
    */
    bool isManualCapsFlag(const long capsFlags) const;

    /**
     * @brief Get CapsFlag based on the mode
     * @param[in] isAutoMode Is auto mode?
     * @return Return CapsFlag
    */
    long getCapsFlag(const bool isAuto) const;

#pragma endregion CapsFlag

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

        long m_min = 0;
        long m_max = 0;
        long m_step = 0;
        long m_defaultValue = 0;
        bool m_isAuto = true;
        long m_value = 0;

        bool m_supported = false;
        bool m_supportAuto = false;
        bool m_supportManual = false;

    };
}

//*******************************

#endif

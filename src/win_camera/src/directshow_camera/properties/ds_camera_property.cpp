#include "directshow_camera/properties/ds_camera_property.h"

#include "directshow_camera/utils/ds_camera_utils.h"

namespace DirectShowCamera
{
#pragma region constructor and destructor

    DirectShowCameraProperty::DirectShowCameraProperty()
    {
        Reset();
    }

    DirectShowCameraProperty::DirectShowCameraProperty(
        const std::string name,
        const long propertyEnum,
        const int queryInterface
    )
    {
        m_name = name;
        m_enum = propertyEnum;
        m_queryInterface = queryInterface;
        
        Reset();
    }

    void DirectShowCameraProperty::Reset()
    {
        m_min = 0;
        m_max = 0;
        m_step = 0;
        m_defaultValue = 0;
        m_isAuto = true;
        m_value = 0;

        m_supported = false;
        m_supportAuto = false;
        m_supportManual = false;
    }

#pragma endregion constructor and destructor

#pragma region Support Mode

    bool DirectShowCameraProperty::isSupported() const
    {
        return m_supported;
    }

    bool DirectShowCameraProperty::isSupportAutoMode() const
    {
        return m_supportAuto;
    }

    bool DirectShowCameraProperty::isSupportManualMode() const
    {
        return m_supportManual;
    }

#pragma endregion Support Mode

#pragma region Import

    void DirectShowCameraProperty::ImportProperty(
        const bool supported,
        const long min,
        const long max,
        const long step,
        const long defaultValue,
        const bool isAuto,
        const long value,
        const bool supportAuto,
        const bool supportManual
    )
    {
        m_min = min;
        m_max = max;
        m_step = step;
        m_defaultValue = defaultValue;
        m_isAuto = isAuto;
        m_value = value;

        m_supported = supported;
        m_supportAuto = supportAuto;
        m_supportManual = supportManual;
    }

#pragma endregion Import

#pragma region Setter

    bool DirectShowCameraProperty::setToDefaultValue(
        IBaseFilter* videoInputFilter,
        std::string& errorString,
        const bool asAuto
    )
    {
        // Get auto mode
        bool isAutoMode = false;
        if (asAuto && m_supportAuto) isAutoMode = true;

        bool result = setValue(videoInputFilter, m_defaultValue, isAutoMode, errorString);

        return result;
    }

    bool DirectShowCameraProperty::setValue(
        IBaseFilter* videoInputFilter,
        const long value,
        const bool isAutoMode,
        std::string& errorString
    )
    {
        bool result = true;
        
        // Check support
        if (m_supported)
        {
            // Check range
            if (value < m_min || value > m_max)
            {
                result = false;
                errorString = "Set " + m_name + " property error: Value(" + std::to_string(value) + ") is Out of range(" + std::to_string(m_min) + ", " + std::to_string(m_max) + ").";

            }

            // Check mode
            if ((isAutoMode && !m_supportAuto) ||
                (!isAutoMode && !m_supportManual))
            {
                // Append error on next line
                if (!result)
                {
                    errorString += "\n";
                }
                else
                {
                    errorString = "";
                }

                if (isAutoMode)
                {
                    errorString += "Set " + m_name + " property error: Auto mode is not supported.";
                }
                else
                {
                    errorString += "Set " + m_name + " property error: Manual mode is not supported.";
                }

                result = false;
            }

            // Set properties
            if (result)
            {
                bool success = true;
                if (videoInputFilter == NULL)
                {
                    // Only update object value
                    m_value = value;
                    m_isAuto = isAutoMode;
                }
                else
                {
                    // Set in directshow
                    if (m_queryInterface == USE_AM_VIDEO_PROC_AMP)
                    {
                        // Convert mode to VideoProcAmpFlags
                        long mode = 0;
                        if (isAutoMode)
                        {
                            mode = VideoProcAmp_Flags_Auto;
                        }
                        else
                        {
                            mode = VideoProcAmp_Flags_Manual;
                        }

                        // Am_VideoProcAmp
                        result = DirectShowCameraUtils::AmVideoProcAmpDecorator(videoInputFilter,
                            [this, &success, value, isAutoMode](IAMVideoProcAmp* videoProcAmp)
                            {
                                success = setValueTemplate(videoProcAmp, value, isAutoMode);
                            },
                            errorString
                                );
                    }
                    else if (m_queryInterface == USE_AM_CAMERA_CONTROL)
                    {
                        // Am_CameraControl
                        result = DirectShowCameraUtils::AmCameraControlDecorator(videoInputFilter,
                            [this, &success, value, isAutoMode](IAMCameraControl* cameraControl)
                            {
                                success = setValueTemplate(cameraControl, value, isAutoMode);
                            },
                            errorString
                                );
                    }
                }				

                // Combine result
                result = result && success;
            }
        }
        else
        {
            // Property not supported
            result = false;
            errorString = "Property " + m_name + " is not supported.";
        }	

        return result;
    }

#pragma endregion Setter

#pragma region Getter

    std::string DirectShowCameraProperty::getName() const
    {
        return m_name;
    }

    long DirectShowCameraProperty::getDSEnum() const
    {
        return m_enum;
    }

    std::pair<long, long> DirectShowCameraProperty::getRange() const
    {
        std::pair<long, long> result(m_min, m_max);
        return result;
    }

    long DirectShowCameraProperty::getMin() const
    {
        return m_min;
    }

    long DirectShowCameraProperty::getMax() const
    {
        return m_max;
    }

    long DirectShowCameraProperty::getStep() const
    {
        return m_step;
    }

    long DirectShowCameraProperty::getDefaultValue() const
    {
        return m_defaultValue;
    }

    long DirectShowCameraProperty::getValue() const
    {
        return m_value;
    }

    bool DirectShowCameraProperty::isAuto() const
    {
        return m_isAuto;
    }

#pragma endregion Getter

#pragma region CapsFlag

    bool DirectShowCameraProperty::isAutoCapsFlag(const long capsFlags) const
    {
        bool result = false;

        // Check
        if (m_queryInterface == USE_AM_VIDEO_PROC_AMP)
        {
            // Support auto mode
            if (capsFlags & VideoProcAmp_Flags_Auto)
            {
                result = true;
            }
        }
        else if (m_queryInterface == USE_AM_CAMERA_CONTROL)
        {
            // Support auto mode
            if (capsFlags & CameraControl_Flags_Auto)
            {
                result = true;
            }
        }

        return result;
    }

    bool DirectShowCameraProperty::isManualCapsFlag(const long capsFlags) const
    {
        bool result = false;

        // Check
        if (m_queryInterface == USE_AM_VIDEO_PROC_AMP)
        {
            // is manual mode
            if (capsFlags & VideoProcAmp_Flags_Manual)
            {
                result = true;
            }
        }
        else if (m_queryInterface == USE_AM_CAMERA_CONTROL)
        {
            // Support manual mode
            if (capsFlags & CameraControl_Flags_Manual)
            {
                result = true;
            }
        }

        return result;
    }

    long DirectShowCameraProperty::getCapsFlag(const bool isAuto) const
    {
        if (m_queryInterface == USE_AM_VIDEO_PROC_AMP)
        {
            // Am_Video_Proc_Amp
            if (isAuto)
            {
                // Auto
                return VideoProcAmp_Flags_Auto;
            }
            else
            {
                // Manual
                return VideoProcAmp_Flags_Manual;
            }
        }
        else if (m_queryInterface == USE_AM_CAMERA_CONTROL)
        {
            // Am_Camera_control
            if (isAuto)
            {
                // Auto
                return VideoProcAmp_Flags_Auto;
            }
            else
            {
                // Manual
                return VideoProcAmp_Flags_Manual;
            }
        }
        else
        {
            return false;
        }
    }

#pragma endregion CapsFlag
}


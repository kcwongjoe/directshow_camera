#include <ds_camera_property.h>

namespace DirectShowCamera
{
#pragma region constructor

    /**
     * @brief Dummy constructor, use DirectShowCameraProperty(string, long, int) instead.
    */
    DirectShowCameraProperty::DirectShowCameraProperty()
    {
        reset();
    }

    /**
     * @brief Constructor
     * @param name Property Name, use for casting to string
     * @param propertyEnum Property Enumeration in directshow
     * @param queryInterface Query interface, USE_AM_VIDEO_PROC_AMP or USE_AM_CAMERA_CONTROL
    */
    DirectShowCameraProperty::DirectShowCameraProperty(std::string name, long propertyEnum, int queryInterface)
    {
        m_name = name;
        m_enum = propertyEnum;
        m_queryInterface = queryInterface;
        
        reset();
    }

#pragma endregion constructor

    /**
     * @brief Reset internal variable
    */
    void DirectShowCameraProperty::reset()
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

#pragma region utils

    /**
     * @brief Check whether the Capsflags contains auto mode.
     * @param capsFlags CapsFlag
     * @return Return true if the Capsflags contains auto mode.
    */
    bool DirectShowCameraProperty::capsFlagIsAuto(long capsFlags) const
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

    /**
     * @brief Check whether the Capsflags contains manual mode.
     * @param capsFlags CapsFlag
     * @return Return true if the Capsflags contains manual mode.
    */
    bool DirectShowCameraProperty::capsFlagIsManual(long capsFlags) const
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

    long DirectShowCameraProperty::isAutoToCapsFlag(bool isAuto) const
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

#pragma endregion utils

#pragma region Getter

    /**
     * @brief Return true if camera supported this property
     * @return Return true if camera supported this property 
    */
    bool DirectShowCameraProperty::isSupported() const
    {
        return m_supported;
    }

    /**
     * @brief Return true if property support auto mode.
     * @return Return true if property support auto mode.
    */
    bool DirectShowCameraProperty::supportAutoMode() const
    {
        return m_supportAuto;
    }

    /**
     * @brief Return true if property support manual mode.
     * @return Return true if property support manual mode.
    */
    bool DirectShowCameraProperty::supportManualMode() const
    {
        return m_supportManual;
    }

    /**
     * @brief Get the property name
     * @return Return the property name
    */
    std::string DirectShowCameraProperty::getName() const
    {
        return m_name;
    }

    /**
     * @brief Get the property Enumeration in DirectShow 
     * @return Return the property Enumeration in DirectShow 
    */
    long DirectShowCameraProperty::getDSEnum() const
    {
        return m_enum;
    }

    /**
     * @brief Get the range of the property
     * @return Return the range of the property
    */
    std::pair<long, long> DirectShowCameraProperty::getRange() const
    {
        std::pair<long, long> result(m_min, m_max);
        return result;
    }

    /**
     * @brief Get the bottom limit of the property
     * @return Return the bottom limit of the property
    */
    long DirectShowCameraProperty::getMin() const
    {
        return m_min;
    }

    /**
     * @brief Get the upper limit of the property
     * @return Return the upper limit of the property
    */
    long DirectShowCameraProperty::getMax() const
    {
        return m_max;
    }

    /**
     * @brief Get the step value of the property
     * @return Return the step value of the property
    */
    long DirectShowCameraProperty::getStep() const
    {
        return m_step;
    }

    /**
     * @brief Get the default value of the property
     * @return Return the default value of the property
    */
    long DirectShowCameraProperty::getDefault() const
    {
        return m_defaultValue;
    }

    /**
     * @brief Get the current value of the property
     * @return Return the current value of the property
    */
    long DirectShowCameraProperty::getValue() const
    {
        return m_value;
    }

    /**
     * @brief Return true if the current mode is auto, otherise manual mode is enabled.
     * @return Return true if the current mode is auto, otherise manual mode is enabled.
    */
    bool DirectShowCameraProperty::isAuto() const
    {
        return m_isAuto;
    }

#pragma endregion Getter


    /**
     * @brief Import property
     * 
     * @param supported Is camera supported?
     * @param min Min value
     * @param max Max value
     * @param step Step
     * @param defaultValue Default value
     * @param isAuto Is auto?
     * @param value Current value
     * @param supportAuto Support auto?
     * @param supportManual Support manual?
    */
    void DirectShowCameraProperty::importProperty(bool supported, long min, long max, long step, long defaultValue, bool isAuto, long value, bool supportAuto, bool supportManual)
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

#pragma region Setter

    /**
     * @brief Set the property to default value.
     * @param videoInputFilter Video Input filter (Camera). If it is NULL, value will not set in through directshow and just update the object value.
     * @param asAuto The property will try to set as auto mode if this value is true.
     * @param errorString Error string
     * @return Return true if success
    */
    bool DirectShowCameraProperty::setAsDefault(IBaseFilter* videoInputFilter, bool asAuto, std::string* errorString)
    {
        // Get auto mode
        bool isAutoMode = false;
        if (asAuto && m_supportAuto) isAutoMode = true;

        bool result = setValue(videoInputFilter, m_defaultValue, isAutoMode, errorString);

        return result;
    }

    /**
     * @brief Set property
     * @param videoInputFilter Video Input filter (Camera). If it is NULL, value will not set in through directshow and just update the object value.
     * @param value Value to be set
     * @param isAutoMode Set as auto mode?
     * @param errorString Error String
     * @return Return true if success.
    */
    bool DirectShowCameraProperty::setValue(IBaseFilter* videoInputFilter, long value, bool isAutoMode, std::string* errorString)
    {
        bool result = true;
        
        // Check support
        if (m_supported)
        {
            // Check range
            if (value < m_min || value > m_max)
            {
                result = false;

                if (errorString)
                {
                    *errorString = "Set " + m_name + " property error: Value(" + std::to_string(value) + ") is Out of range(" + std::to_string(m_min) + ", " + std::to_string(m_max) + ").";
                }

            }

            // Check mode
            if ((isAutoMode && !m_supportAuto) ||
                (!isAutoMode && !m_supportManual))
            {
                // Append error on next line
                if (errorString)
                {
                    if (!result)
                    {
                        *errorString += "\n";
                    }
                    else
                    {
                        *errorString = "";
                    }

                    if (isAutoMode)
                    {
                        *errorString += "Set " + m_name + " property error: Auto mode is not supported.";
                    }
                    else
                    {
                        *errorString += "Set " + m_name + " property error: Manual mode is not supported.";
                    }
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
                        result = DirectShowCameraUtils::amVideoProcAmpDecorator(videoInputFilter,
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
                        result = DirectShowCameraUtils::amCameraControlDecorator(videoInputFilter,
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
            if (errorString)
            {
                *errorString = "Property " + m_name + " is not supported.";
            }
        }	

        return result;
    }

#pragma endregion Setter
}


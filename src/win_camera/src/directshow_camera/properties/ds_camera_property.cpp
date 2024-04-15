/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

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
        const std::string name
    )
    {
        m_name = name;
        
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
        const bool supportManual,
        const long capsFlag
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
        m_capsFlag = capsFlag;
    }

#pragma endregion Import

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

#pragma region Setter

    bool DirectShowCameraProperty::setToDefaultValue(
        IBaseFilter* directShowFilter,
        std::string& errorString,
        const bool asAuto
    )
    {
        // Get auto mode
        bool isAutoMode = false;
        if (asAuto && m_supportAuto) isAutoMode = true;

        bool result = setValue(directShowFilter, m_defaultValue, isAutoMode, errorString);

        return result;
    }

    bool DirectShowCameraProperty::setValue(IBaseFilter* directShowFilter, const long value, const bool isAutoMode, std::string& errorString)
    {
        // Check value
        bool result = CheckValue(value, isAutoMode, errorString);
        if (!result) return false;

        // Set value
        bool success = true;
        if (directShowFilter == NULL || m_setValueFunc == nullptr)
        {
            // Only update object value
            m_value = value;
            m_isAuto = isAutoMode;
        }
        else
        {
            // Set value via IBaseFilter
            m_setValueFunc(directShowFilter, value, isAutoMode, errorString);
        }

        return result;
    }

    bool DirectShowCameraProperty::CheckValue(const long value, const bool isAutoMode, std::string& errorString)
    {
        // Check Support
        if (!m_supported)
        {
            errorString = "Property " + m_name + " is not supported.";
            return false;
        }

        // Check range
        if (value < m_min || value > m_max)
        {
            errorString = "Set " + m_name + " property error: Value(" + std::to_string(value) + ") is Out of range(" + std::to_string(m_min) + ", " + std::to_string(m_max) + ").";
            return false;
        }

        // Check mode
        if ((isAutoMode && !m_supportAuto) ||
            (!isAutoMode && !m_supportManual))
        {
            if (isAutoMode)
            {
                errorString += "Set " + m_name + " property error: Auto mode is not supported.";
            }
            else
            {
                errorString += "Set " + m_name + " property error: Manual mode is not supported.";
            }

            return false;
        }

        return true;
    }

#pragma endregion Setter

#pragma region Getter

    std::string DirectShowCameraProperty::getName() const
    {
        return m_name;
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

}


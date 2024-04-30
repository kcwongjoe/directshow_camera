/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "directshow_camera/properties/ds_camera_powerline_frequency_property.h"

#include "directshow_camera/utils/ds_camera_utils.h"

namespace DirectShowCamera
{
#pragma region constructor and destructor

    DirectShowCameraPowerlineFrequencyProperty::DirectShowCameraPowerlineFrequencyProperty(
    ) : DirectShowCameraProperty("Powerline Frequency")
    {
        Constructor();
    }

    void DirectShowCameraPowerlineFrequencyProperty::Constructor()
    {
        // Set setValue Function for DirectShowCameraProperty::setValue
        m_setValueFunc = [this](
            IBaseFilter* directShowFilter,
            const long value,
            const bool isAutoMode,
            std::string& errorString
            )
        {
            // Check value
            if (value != 0 && value != 1 && value != 2)
            {
                errorString = "Set " + m_name + " property error: Value(" + std::to_string(value) + ") is invalid, it must be 0:Disable, 1:50Hz or 2:60Hz";
                return false;
            }

            const auto result = DirectShowCameraUtils::VideoProcAmpDecorator(
                directShowFilter,
                [this, value, isAutoMode, &errorString](IVideoProcAmp* videoProcAmp)
                {
                    // Set value from IVideoProcAmp

                    // Set value, 0 : Disable, 1 : 50 Hz, 2: 60 Hz
                    bool result = true;
                    const auto capFlag = getCapsFlag(isAutoMode);
                    HRESULT hr = videoProcAmp->put_PowerlineFrequency(value, capFlag);
                    if (hr == S_OK)
                    {
                        // Success, update variable
                        m_value = value;
                        m_capsFlag = capFlag;
                        m_isAuto = isAutoMode;
                        result = true;
                    }
                    else
                    {
                        // Fail
                        result = false;
                        errorString = "Set " + m_name + " property error: Set value(" + std::to_string(value) + ") failed, hr(" + std::to_string(hr) + ").";
                    }

                    return result;
                },
                errorString
            );

            return result;
        };
    }

#pragma endregion constructor and destructor

#pragma region Import

    bool DirectShowCameraPowerlineFrequencyProperty::ImportProperty(IVideoProcAmp* videoProcAmp)
    {
        HRESULT hr = NO_ERROR;
        long min;
        long max;
        long step;
        long defaultValue;
        long availableCapsFlag;

        // get range
        hr = videoProcAmp->getRange_PowerlineFrequency(&min, &max, &step, &defaultValue, &availableCapsFlag);

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
            ImportPropertyValue(videoProcAmp);

            // Set as initialzied
            m_supported = true;

            return true;
        }
        else
        {
            return false;
        }
    }

    bool DirectShowCameraPowerlineFrequencyProperty::ImportPropertyValue(IVideoProcAmp* videoProcAmp)
    {
        HRESULT hr = NO_ERROR;

        // Get current value
        long value;
        long capsFlag;
        hr = videoProcAmp->get_PowerlineFrequency(&value, &capsFlag);

        if (hr == S_OK)
        {
            // Set current value
            m_value = value;
            m_capsFlag = capsFlag;
            m_isAuto = isAutoCapsFlag(capsFlag);

            return true;
        }
        else
        {
            return false;
        }
    }

#pragma endregion Import

#pragma region CapsFlag

    bool DirectShowCameraPowerlineFrequencyProperty::isAutoCapsFlag(const long capsFlags) const
    {
        // Support auto mode
        if (capsFlags & VideoProcAmp_Flags_Auto)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool DirectShowCameraPowerlineFrequencyProperty::isManualCapsFlag(const long capsFlags) const
    {
        // Support manual mode
        if (capsFlags & VideoProcAmp_Flags_Manual)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    long DirectShowCameraPowerlineFrequencyProperty::getCapsFlag(const bool isAuto) const
    {
        // Return the last caps flag if both auto and manual are not supported.
        if (!m_supportAuto && !m_supportManual)
        {
            return m_capsFlag;
        }

        // Return the caps flag based on the mode.
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

#pragma endregion CapsFlag
}


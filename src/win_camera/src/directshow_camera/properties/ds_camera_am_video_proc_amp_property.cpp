/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#include "directshow_camera/properties/ds_camera_am_video_proc_amp_property.h"

#include "directshow_camera/utils/ds_camera_utils.h"

namespace DirectShowCamera
{
#pragma region constructor and destructor

    DirectShowCameraAMVideoProcAmpProperty::DirectShowCameraAMVideoProcAmpProperty() : DirectShowCameraProperty()
    {
        Constructor();
    }

    DirectShowCameraAMVideoProcAmpProperty::DirectShowCameraAMVideoProcAmpProperty(
        const std::string name,
        const long propertyTag
    ) : DirectShowCameraProperty(name)
    {
        m_propertyTag = propertyTag;
        
        Constructor();
    }

    void DirectShowCameraAMVideoProcAmpProperty::Constructor()
    {
        // Set setValue Function for DirectShowCameraProperty::setValue
        m_setValueFunc = [this](
            IBaseFilter* directShowFilter,
            const long value,
            const bool isAutoMode,
            std::string& errorString
            )
        {
            const auto result = DirectShowCameraUtils::AmVideoProcAmpDecorator(
                directShowFilter,
                [this, value, isAutoMode, &errorString](IAMVideoProcAmp* amVideoProcAmp)
                {
                    // Set value from IAMVideoProcAmp

                    // Set
                    bool result = true;
                    const auto capFlag = getCapsFlag(isAutoMode);
                    HRESULT hr = amVideoProcAmp->Set(m_propertyTag, value, capFlag);
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

    bool DirectShowCameraAMVideoProcAmpProperty::ImportProperty(IAMVideoProcAmp* amVideoProcAmp)
    {
        HRESULT hr = NO_ERROR;
        long min;
        long max;
        long step;
        long defaultValue;
        long availableCapsFlag;

        // get range
        hr = amVideoProcAmp->GetRange(m_propertyTag, &min, &max, &step, &defaultValue, &availableCapsFlag);

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
            ImportPropertyValue(amVideoProcAmp);

            // Set as initialzied
            m_supported = true;

            return true;
        }
        else
        {
            return false;
        }
    }

    bool DirectShowCameraAMVideoProcAmpProperty::ImportPropertyValue(IAMVideoProcAmp* amVideoProcAmp)
    {
        HRESULT hr = NO_ERROR;

        // Get current value
        long value;
        long capsFlag;
        hr = amVideoProcAmp->Get(m_propertyTag, &value, &capsFlag);

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

    bool DirectShowCameraAMVideoProcAmpProperty::isAutoCapsFlag(const long capsFlags) const
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

    bool DirectShowCameraAMVideoProcAmpProperty::isManualCapsFlag(const long capsFlags) const
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

    long DirectShowCameraAMVideoProcAmpProperty::getCapsFlag(const bool isAuto) const
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


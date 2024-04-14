#include "directshow_camera/properties/ds_camera_digital_zoom_level_property.h"

#include "directshow_camera/utils/ds_camera_utils.h"

namespace DirectShowCamera
{
#pragma region constructor and destructor

    DirectShowCameraDigitalZoomLevelProperty::DirectShowCameraDigitalZoomLevelProperty(
    ) : DirectShowCameraProperty("Digital Zoom Level")
    {
        Constructor();
    }

    void DirectShowCameraDigitalZoomLevelProperty::Reset()
    {
        DirectShowCameraProperty::Reset();
        m_maximumDigitalMagnification = -1;
    }

    void DirectShowCameraDigitalZoomLevelProperty::Constructor()
    {
        // Set setValue Function for DirectShowCameraProperty::setValue
        m_setValueFunc = [this](
            IBaseFilter* videoInputFilter,
            const long value,
            const bool isAutoMode,
            std::string& errorString
            )
        {
            const auto result = DirectShowCameraUtils::VideoProcAmpDecorator(
                videoInputFilter,
                [this, value, isAutoMode, &errorString](IVideoProcAmp* videoProcAmp)
                {
                    // Set value from IVideoProcAmp

                    // Set value
                    bool result = true;
                    const auto capFlag = getCapsFlag(isAutoMode);
                    HRESULT hr = videoProcAmp->put_DigitalMultiplier(value, capFlag);
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

    bool DirectShowCameraDigitalZoomLevelProperty::ImportProperty(IVideoProcAmp* videoProcAmp, IBaseFilter* videoInputFilter)
    {
        HRESULT hr = NO_ERROR;
        long min;
        long max;
        long step;
        long defaultValue;
        long availableCapsFlag;

        // get range
        hr = videoProcAmp->getRange_DigitalMultiplier(&min, &max, &step, &defaultValue, &availableCapsFlag);

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

            // Try to import maximum digital magnification
            if (videoInputFilter != NULL)
            {
                ImportMaximumDigitalMagnification(videoInputFilter);
            }

            return true;
        }
        else
        {
            return false;
        }
    }

    bool DirectShowCameraDigitalZoomLevelProperty::ImportPropertyValue(IVideoProcAmp* videoProcAmp)
    {
        HRESULT hr = NO_ERROR;

        // Get current value
        long value;
        long capsFlag;
        hr = videoProcAmp->get_DigitalMultiplier(&value, &capsFlag);

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

    bool DirectShowCameraDigitalZoomLevelProperty::ImportMaximumDigitalMagnification(IBaseFilter* videoInputFilter)
    {
        // Import maximum digital magnification if it is not imported yet.
        if (m_maximumDigitalMagnification != -1) return false;

        // m_maximumDigitalMagnification now is -1. Set to 0 so that it will not be imported again even if it fails.
        m_maximumDigitalMagnification = 0;

        // Check support
        if (!m_supported) return false;

        std::string ksPropertyErrorString;
        bool success = false;

        // Get KsPorpertySet
        DirectShowCameraUtils::KsPropertyDecorator(
            videoInputFilter,
            [this, &success](IKsPropertySet* ksPropertySet)
        {
            // Check KSPROPERTY_VIDEOPROCAMP_DIGITAL_MULTIPLIER_LIMIT support
            HRESULT hr;
            DWORD dwTypeSupport = 0;
            hr = ksPropertySet->QuerySupported(PROPSETID_VIDCAP_VIDEOPROCAMP, KSPROPERTY_VIDEOPROCAMP_DIGITAL_MULTIPLIER_LIMIT, &dwTypeSupport);
            // const auto result = DirectShowCamera::CheckHResultUtils::CheckIKsPropertySetQuerySupportedResult(hr, errorString, "Error on IKsPropertySet::QuerySupported(PROPSETID_VIDCAP_VIDEOPROCAMP, KSPROPERTY_VIDEOPROCAMP_DIGITAL_MULTIPLIER_LIMIT)");

            if (hr == S_OK)
            {
                // Get KSPROPERTY_VIDEOPROCAMP_DIGITAL_MULTIPLIER_LIMIT
                KSPROPERTY_VIDEOPROCAMP_S digitalMultiplierLimit;
                DWORD bytesReturn = 0;
                hr = ksPropertySet->Get(
                    PROPSETID_VIDCAP_VIDEOPROCAMP,
                    KSPROPERTY_VIDEOPROCAMP_DIGITAL_MULTIPLIER_LIMIT,
                    NULL,
                    0,
                    &digitalMultiplierLimit,
                    sizeof(KSPROPERTY_VIDEOPROCAMP_S),
                    &bytesReturn
                );

                if (hr == S_OK)
                {
                    // Set maximum digital magnification
                    m_maximumDigitalMagnification = digitalMultiplierLimit.Value;
                    success = true;
                }
            }
        },
            ksPropertyErrorString
        );

        return success;
    }

#pragma endregion Import

    long DirectShowCameraDigitalZoomLevelProperty::getMaximumDigitalMagnification() const
    {
        return m_maximumDigitalMagnification;
    }

#pragma region CapsFlag

    bool DirectShowCameraDigitalZoomLevelProperty::isAutoCapsFlag(const long capsFlags) const
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

    bool DirectShowCameraDigitalZoomLevelProperty::isManualCapsFlag(const long capsFlags) const
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

    long DirectShowCameraDigitalZoomLevelProperty::getCapsFlag(const bool isAuto) const
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


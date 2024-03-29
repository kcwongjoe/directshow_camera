#include "win_camera/properties/win_camera_property_exposure.h"

#include "win_camera/win_camera.h"
#include "win_camera/win_camera_utils.h"

namespace WinCamera
{
    WinCameraPropertyExposure::WinCameraPropertyExposure(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyExposure::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getExposure();
    }

    bool WinCameraPropertyExposure::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    std::pair<double, double> WinCameraPropertyExposure::getRange() const
    {
        const auto range = getRangeInternal();

        // Convert to second
        std::pair<double, double> result;
        result.first = WinCameraUtils::DSExposureValueToSec(range.first);
        result.second = WinCameraUtils::DSExposureValueToSec(range.second);

        return result;
    }

    double WinCameraPropertyExposure::getValue() const
    {
        const auto dsValue = getValueInternal();

        // Convert to second
        const auto result = WinCameraUtils::DSExposureValueToSec(dsValue);

        return result;
    }

    void WinCameraPropertyExposure::setValue(const double second)
    {
        // Convert to DirectShow value
        long dsValue = WinCameraUtils::DSExposureSecToValue(second);

        // Set value
        setValueInternal(dsValue);
    }

    bool WinCameraPropertyExposure::isAuto() const
    {
        return isAutoInternal();
    }

    void WinCameraPropertyExposure::setAuto(const bool setToAuto)
    {
        // Check mode support
        CheckModeSupport(setToAuto ? Mode::Auto : Mode::Manual);

        // Set value
        setValueInternal(getValueInternal(), setToAuto);
    }

    std::vector<double> WinCameraPropertyExposure::GetPossibleExposureValues() const
    {
        // Get range and step
        const auto range = getRangeInternal();
        const auto step = getStepInternal();

        // Add possible value to list
        std::vector<double> result;
        std::vector<long> exposureValues;
        long value = range.first;
        while (value < range.second)
        {
            // Convert to second
            exposureValues.push_back(value);

            // To next value
            value += step;
        }

        // Convert to second
        if (exposureValues.size() > 0)
        {
            // Sort and Unique
            std::sort(exposureValues.begin(), exposureValues.end());
            exposureValues.erase(std::unique(exposureValues.begin(), exposureValues.end()), exposureValues.end());

            // Convert to second
            for (int i = 0; i < exposureValues.size(); i++)
            {
                result.push_back(WinCameraUtils::DSExposureValueToSec(exposureValues[i]));
            }
        }

        return result;
    }

    int WinCameraPropertyExposure::GetExposureIndex() const
    {
        const auto exposureList = GetPossibleExposureValues();
        const auto exposureValue = getValue();
        int result = -1;

        if (exposureList.size() > 0)
        {
            // Find the closest value in the exposureList
            result = 0;
            double distance = abs(exposureList[0] - exposureValue);

            for (int i = 1; i < exposureList.size(); i++)
            {
                double currentDistance = abs(exposureList[i] - exposureValue);
                if (currentDistance < distance)
                {
                    result = i;
                    distance = currentDistance;
                }
            }
        }

        return result;
    }
}
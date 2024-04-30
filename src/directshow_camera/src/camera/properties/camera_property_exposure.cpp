/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "camera/properties/camera_property_exposure.h"

#include "camera/camera.h"
#include "camera/camera_utils.h"

namespace DirectShowCamera
{
    CameraPropertyExposure::CameraPropertyExposure(
        const Camera& camera,
        const std::shared_ptr<AbstractDirectShowCamera>& ds_camera
    ) : CameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCameraProperty> CameraPropertyExposure::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getExposure();
    }

    bool CameraPropertyExposure::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    std::pair<double, double> CameraPropertyExposure::getRange() const
    {
        const auto range = getRangeInternal();

        // Convert to second
        std::pair<double, double> result;
        result.first = CameraUtils::DSExposureValueToSec(range.first);
        result.second = CameraUtils::DSExposureValueToSec(range.second);

        return result;
    }

    double CameraPropertyExposure::getValue() const
    {
        const auto dsValue = getValueInternal();

        // Convert to second
        const auto result = CameraUtils::DSExposureValueToSec(dsValue);

        return result;
    }

    void CameraPropertyExposure::setValue(const double second)
    {
        // Convert to DirectShow value
        long dsValue = CameraUtils::DSExposureSecToValue(second);

        // Set value
        setValueInternal(dsValue);
    }

    bool CameraPropertyExposure::isAuto() const
    {
        return isAutoInternal();
    }

    void CameraPropertyExposure::setAuto(const bool setToAuto)
    {
        // Check mode support
        CheckModeSupport(setToAuto ? Mode::Auto : Mode::Manual);

        // Set value
        setValueInternal(getValueInternal(), setToAuto);
    }

    std::vector<double> CameraPropertyExposure::GetPossibleExposureValues() const
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
                result.push_back(CameraUtils::DSExposureValueToSec(exposureValues[i]));
            }
        }

        return result;
    }

    int CameraPropertyExposure::GetExposureIndex() const
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
/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "camera/properties/camera_property_digital_zoom_level.h"

#include "camera/camera.h"

namespace DirectShowCamera
{
    CameraPropertyDigitalZoomLevel::CameraPropertyDigitalZoomLevel(
        const Camera& camera,
        const std::shared_ptr<AbstractDirectShowCamera>& ds_camera
    ) : CameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCameraProperty> CameraPropertyDigitalZoomLevel::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getDigitalZoomLevel();
    }

    bool CameraPropertyDigitalZoomLevel::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    std::pair<long, long> CameraPropertyDigitalZoomLevel::getRange() const
    {
        return getRangeInternal();
    }

    long CameraPropertyDigitalZoomLevel::getStep() const
    {
        return getStepInternal();
    }

    long CameraPropertyDigitalZoomLevel::getValue() const
    {
        return getValueInternal();
    }

    void CameraPropertyDigitalZoomLevel::setValue(const long value)
    {
        setValueInternal(value);
    }

    double CameraPropertyDigitalZoomLevel::getFactorM() const
    {
        // Throw if camera is not opened or property is not supported
        CheckOpeningAndSupported();

        // Get value
        const auto& [min, max] = getDirectShowProperty()->getRange();
        const auto value = getDirectShowProperty()->getValue();
        const auto maxMagnification = ((DirectShowCameraDigitalZoomLevelProperty*)(getDirectShowProperty().get()))->getMaximumDigitalMagnification();

        // Calculate factor
        double m = (value - min)/(max - min);

        // maxMagnification can be 0 if the maximum digital magnification is failed to acquire from the camera
        // Todo: handle this case later
        if (maxMagnification > 0)
        {
            m = m * (maxMagnification - 1);
        }

        m = m + 1;

        return m;
    }
}
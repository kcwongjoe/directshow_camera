/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#include "win_camera/properties/win_camera_property_digital_zoom_level.h"

#include "win_camera/win_camera.h"

namespace WinCamera
{
    WinCameraPropertyDigitalZoomLevel::WinCameraPropertyDigitalZoomLevel(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyDigitalZoomLevel::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getDigitalZoomLevel();
    }

    bool WinCameraPropertyDigitalZoomLevel::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    std::pair<long, long> WinCameraPropertyDigitalZoomLevel::getRange() const
    {
        return getRangeInternal();
    }

    long WinCameraPropertyDigitalZoomLevel::getStep() const
    {
        return getStepInternal();
    }

    long WinCameraPropertyDigitalZoomLevel::getValue() const
    {
        return getValueInternal();
    }

    void WinCameraPropertyDigitalZoomLevel::setValue(const long value)
    {
        setValueInternal(value);
    }

    double WinCameraPropertyDigitalZoomLevel::getFactorM() const
    {
        // Throw if camera is not opened or property is not supported
        CheckOpeningAndSupported();

        // Get value
        const auto& [min, max] = getDirectShowProperty()->getRange();
        const auto value = getDirectShowProperty()->getValue();
        const auto maxMagnification = ((DirectShowCamera::DirectShowCameraDigitalZoomLevelProperty*)(getDirectShowProperty().get()))->getMaximumDigitalMagnification();

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
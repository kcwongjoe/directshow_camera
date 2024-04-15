/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "win_camera/properties/win_camera_property_hue.h"

#include "win_camera/win_camera.h"

#include "utils/math_utils.h"

namespace WinCamera
{
    WinCameraPropertyHue::WinCameraPropertyHue(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyHue::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getHue();
    }

    bool WinCameraPropertyHue::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    std::pair<long, long> WinCameraPropertyHue::getRange() const
    {
        return getRangeInternal();
    }

    long WinCameraPropertyHue::getStep() const
    {
        return getStepInternal();
    }

    long WinCameraPropertyHue::getValue() const
    {
        return getValueInternal();
    }

    void WinCameraPropertyHue::setValue(const long degree)
    {
        // Make sure the degree is in the range of -180 to 180
        const auto degreeIn180 = Utils::MathUtils::ConfirmDegreeIn180Range(degree);

        // Set value
        setValueInternal(degreeIn180);
    }
}
/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#include "win_camera/properties/win_camera_property_brightness.h"

#include "win_camera/win_camera.h"

namespace WinCamera
{
    WinCameraPropertyBrightness::WinCameraPropertyBrightness(
        const WinCamera& camera,
        const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
    ) : WinCameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> WinCameraPropertyBrightness::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getBrightness();
    }

    bool WinCameraPropertyBrightness::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    std::pair<long, long> WinCameraPropertyBrightness::getRange() const
    {
        return getRangeInternal();
    }

    long WinCameraPropertyBrightness::getStep() const
    {
        return getStepInternal();
    }

    long WinCameraPropertyBrightness::getValue() const
    {
        return getValueInternal();
    }

    void WinCameraPropertyBrightness::setValue(const long value)
    {
        setValueInternal(value);
    }
}
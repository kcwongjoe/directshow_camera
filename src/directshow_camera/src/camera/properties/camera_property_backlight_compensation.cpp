/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "camera/properties/camera_property_backlight_compensation.h"

#include "camera/camera.h"

namespace DirectShowCamera
{
    CameraPropertyBacklightCompensation::CameraPropertyBacklightCompensation(
        const Camera& camera,
        const std::shared_ptr<AbstractDirectShowCamera>& ds_camera
    ) : CameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCameraProperty> CameraPropertyBacklightCompensation::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getBacklightCompensation();
    }

    bool CameraPropertyBacklightCompensation::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    bool CameraPropertyBacklightCompensation::isBacklightCompensation() const
    {
        bool isBacklightCompensation = getValueInternal() == 0 ? false : true; // 0(off), 1(on)

        return isBacklightCompensation;
    }

    void CameraPropertyBacklightCompensation::setBacklightCompensation(const bool isOn)
    {
        long value = isOn ? 1 : 0; // 0(off), 1(on)

        setValueInternal(value);
    }
}
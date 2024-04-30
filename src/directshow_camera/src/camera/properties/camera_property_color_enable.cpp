/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "camera/properties/camera_property_color_enable.h"

#include "camera/camera.h"

namespace DirectShowCamera
{
    CameraPropertyColorEnable::CameraPropertyColorEnable(
        const Camera& camera,
        const std::shared_ptr<AbstractDirectShowCamera>& ds_camera
    ) : CameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCameraProperty> CameraPropertyColorEnable::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getColorEnable();
    }

    bool CameraPropertyColorEnable::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    bool CameraPropertyColorEnable::isColorEnable() const
    {
        bool isColorEnable = getValueInternal() == 0 ? false : true; // 0(off), 1(on)

        return isColorEnable;
    }

    void CameraPropertyColorEnable::setColorEnable(const bool isOn)
    {
        // Get value
        const auto range = getRangeInternal();
        long value = isOn ? range.second : range.first; // 0(off), 1(on)

        // Set
        setValueInternal(value);
    }
}
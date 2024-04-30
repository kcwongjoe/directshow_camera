/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "camera/properties/camera_property_sharpness.h"

#include "camera/camera.h"

namespace DirectShowCamera
{
    CameraPropertySharpness::CameraPropertySharpness(
        const Camera& camera,
        const std::shared_ptr<AbstractDirectShowCamera>& ds_camera
    ) : CameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCameraProperty> CameraPropertySharpness::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getSharpness();
    }

    bool CameraPropertySharpness::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    std::pair<long, long> CameraPropertySharpness::getRange() const
    {
        return getRangeInternal();
    }

    long CameraPropertySharpness::getStep() const
    {
        return getStepInternal();
    }

    long CameraPropertySharpness::getValue() const
    {
        return getValueInternal();
    }

    void CameraPropertySharpness::setValue(const long value)
    {
        setValueInternal(value);
    }
}
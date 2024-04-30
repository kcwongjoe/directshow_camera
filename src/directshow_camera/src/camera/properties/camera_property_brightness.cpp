/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "camera/properties/camera_property_brightness.h"

#include "camera/camera.h"

namespace DirectShowCamera
{
    CameraPropertyBrightness::CameraPropertyBrightness(
        const Camera& camera,
        const std::shared_ptr<AbstractDirectShowCamera>& ds_camera
    ) : CameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCameraProperty> CameraPropertyBrightness::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getBrightness();
    }

    bool CameraPropertyBrightness::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    std::pair<long, long> CameraPropertyBrightness::getRange() const
    {
        return getRangeInternal();
    }

    long CameraPropertyBrightness::getStep() const
    {
        return getStepInternal();
    }

    long CameraPropertyBrightness::getValue() const
    {
        return getValueInternal();
    }

    void CameraPropertyBrightness::setValue(const long value)
    {
        setValueInternal(value);
    }
}
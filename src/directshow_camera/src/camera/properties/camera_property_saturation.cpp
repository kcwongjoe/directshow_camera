/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "camera/properties/camera_property_saturation.h"

#include "camera/camera.h"

namespace DirectShowCamera
{
    CameraPropertySaturation::CameraPropertySaturation(
        const Camera& camera,
        const std::shared_ptr<AbstractDirectShowCamera>& ds_camera
    ) : CameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCameraProperty> CameraPropertySaturation::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getSaturation();
    }

    bool CameraPropertySaturation::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    std::pair<long, long> CameraPropertySaturation::getRange() const
    {
        return getRangeInternal();
    }

    long CameraPropertySaturation::getStep() const
    {
        return getStepInternal();
    }

    long CameraPropertySaturation::getValue() const
    {
        return getValueInternal();
    }

    void CameraPropertySaturation::setValue(const long value)
    {
        setValueInternal(value);
    }
}
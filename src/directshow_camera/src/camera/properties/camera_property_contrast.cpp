/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "camera/properties/camera_property_contrast.h"

#include "camera/camera.h"

namespace DirectShowCamera
{
    CameraPropertyContrast::CameraPropertyContrast(
        const Camera& camera,
        const std::shared_ptr<AbstractDirectShowCamera>& ds_camera
    ) : CameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCameraProperty> CameraPropertyContrast::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getContrast();
    }

    bool CameraPropertyContrast::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    std::pair<long, long> CameraPropertyContrast::getRange() const
    {
        return getRangeInternal();
    }

    long CameraPropertyContrast::getStep() const
    {
        return getStepInternal();
    }

    long CameraPropertyContrast::getValue() const
    {
        return getValueInternal();
    }

    void CameraPropertyContrast::setValue(const long value)
    {
        setValueInternal(value);
    }
}
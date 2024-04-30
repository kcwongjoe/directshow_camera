/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "camera/properties/camera_property_gamma.h"

#include "camera/camera.h"

namespace DirectShowCamera
{
    CameraPropertyGamma::CameraPropertyGamma(
        const Camera& camera,
        const std::shared_ptr<AbstractDirectShowCamera>& ds_camera
    ) : CameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCameraProperty> CameraPropertyGamma::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getGamma();
    }

    bool CameraPropertyGamma::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    std::pair<long, long> CameraPropertyGamma::getRange() const
    {
        return getRangeInternal();
    }

    long CameraPropertyGamma::getStep() const
    {
        return getStepInternal();
    }

    long CameraPropertyGamma::getValue() const
    {
        return getValueInternal();
    }

    void CameraPropertyGamma::setValue(const long value)
    {
        setValueInternal(value);
    }
}
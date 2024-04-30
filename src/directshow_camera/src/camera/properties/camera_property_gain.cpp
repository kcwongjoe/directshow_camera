/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "camera/properties/camera_property_gain.h"

#include "camera/camera.h"

namespace DirectShowCamera
{
    CameraPropertyGain::CameraPropertyGain(
        const Camera& camera,
        const std::shared_ptr<AbstractDirectShowCamera>& ds_camera
    ) : CameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCameraProperty> CameraPropertyGain::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getGain();
    }

    bool CameraPropertyGain::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    std::pair<long, long> CameraPropertyGain::getRange() const
    {
        return getRangeInternal();
    }

    long CameraPropertyGain::getStep() const
    {
        return getStepInternal();
    }

    long CameraPropertyGain::getValue() const
    {
        return getValueInternal();
    }

    void CameraPropertyGain::setValue(const long value)
    {
        setValueInternal(value);
    }
}
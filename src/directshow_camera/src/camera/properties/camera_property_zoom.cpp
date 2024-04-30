/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "camera/properties/camera_property_zoom.h"

#include "camera/camera.h"

namespace DirectShowCamera
{
    CameraPropertyZoom::CameraPropertyZoom(
        const Camera& camera,
        const std::shared_ptr<AbstractDirectShowCamera>& ds_camera
    ) : CameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCameraProperty> CameraPropertyZoom::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getZoom();
    }

    bool CameraPropertyZoom::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    std::pair<long, long> CameraPropertyZoom::getRange() const
    {
        return getRangeInternal();
    }

    long CameraPropertyZoom::getStep() const
    {
        return getStepInternal();
    }

    long CameraPropertyZoom::getValue() const
    {
        return getValueInternal();
    }

    void CameraPropertyZoom::setValue(const long millimeter)
    {
        setValueInternal(millimeter);
    }
}
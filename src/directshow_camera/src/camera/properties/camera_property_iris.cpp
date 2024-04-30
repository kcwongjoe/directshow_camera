/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "camera/properties/camera_property_iris.h"

#include "camera/camera.h"

namespace DirectShowCamera
{
    CameraPropertyIris::CameraPropertyIris(
        const Camera& camera,
        const std::shared_ptr<AbstractDirectShowCamera>& ds_camera
    ) : CameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCameraProperty> CameraPropertyIris::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getIris();
    }

    bool CameraPropertyIris::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    std::pair<long, long> CameraPropertyIris::getRange() const
    {
        return getRangeInternal();
    }

    long CameraPropertyIris::getStep() const
    {
        return getStepInternal();
    }

    long CameraPropertyIris::getValue() const
    {
        return getValueInternal();
    }

    void CameraPropertyIris::setValue(const long value)
    {
        setValueInternal(value);
    }

    bool CameraPropertyIris::isAuto() const
    {
        return isAutoInternal();
    }

    void CameraPropertyIris::setAuto(const bool setToAuto)
    {
        // Check mode support
        CheckModeSupport(setToAuto ? Mode::Auto : Mode::Manual);

        // Set value
        setValueInternal(getValueInternal(), setToAuto);
    }
}
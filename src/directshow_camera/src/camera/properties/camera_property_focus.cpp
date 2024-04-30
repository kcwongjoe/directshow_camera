/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "camera/properties/camera_property_focus.h"

#include "camera/camera.h"

namespace DirectShowCamera
{
    CameraPropertyFocus::CameraPropertyFocus(
        const Camera& camera,
        const std::shared_ptr<AbstractDirectShowCamera>& ds_camera
    ) : CameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCameraProperty> CameraPropertyFocus::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getFocus();
    }

    bool CameraPropertyFocus::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    std::pair<long, long> CameraPropertyFocus::getRange() const
    {
        return getRangeInternal();
    }

    long CameraPropertyFocus::getStep() const
    {
        return getStepInternal();
    }

    long CameraPropertyFocus::getValue() const
    {
        return getValueInternal();
    }

    void CameraPropertyFocus::setValue(const long millimeter)
    {
        setValueInternal(millimeter);
    }

    bool CameraPropertyFocus::isAuto() const
    {
        return isAutoInternal();
    }

    void CameraPropertyFocus::setAuto(const bool setToAuto)
    {
        // Check mode support
        CheckModeSupport(setToAuto ? Mode::Auto : Mode::Manual);

        // Set value
        setValueInternal(getValueInternal(), setToAuto);
    }
}
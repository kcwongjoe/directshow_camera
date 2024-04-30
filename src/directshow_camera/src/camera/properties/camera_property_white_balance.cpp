/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "camera/properties/camera_property_white_balance.h"

#include "camera/camera.h"

namespace DirectShowCamera
{
    CameraPropertyWhiteBalance::CameraPropertyWhiteBalance(
        const Camera& camera,
        const std::shared_ptr<AbstractDirectShowCamera>& ds_camera
    ) : CameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCameraProperty> CameraPropertyWhiteBalance::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getWhiteBalance();
    }

    bool CameraPropertyWhiteBalance::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    std::pair<long, long> CameraPropertyWhiteBalance::getRange() const
    {
        return getRangeInternal();
    }

    long CameraPropertyWhiteBalance::getStep() const
    {
        return getStepInternal();
    }

    long CameraPropertyWhiteBalance::getValue() const
    {
        return getValueInternal();
    }

    void CameraPropertyWhiteBalance::setValue(const long kelvin)
    {
        setValueInternal(kelvin);
    }

    bool CameraPropertyWhiteBalance::isAuto() const
    {
        return isAutoInternal();
    }

    void CameraPropertyWhiteBalance::setAuto(const bool setToAuto)
    {
        // Check mode support
        CheckModeSupport(setToAuto ? Mode::Auto : Mode::Manual);

        // Set value
        setValueInternal(getValueInternal(), setToAuto);
    }
}
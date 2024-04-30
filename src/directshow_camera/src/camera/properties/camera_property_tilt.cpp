/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "camera/properties/camera_property_tilt.h"

#include "camera/camera.h"

#include "utils/math_utils.h"

namespace DirectShowCamera
{
    CameraPropertyTilt::CameraPropertyTilt(
        const Camera& camera,
        const std::shared_ptr<AbstractDirectShowCamera>& ds_camera
    ) : CameraProperty(camera, ds_camera)
    {

    }

    std::shared_ptr<DirectShowCameraProperty> CameraPropertyTilt::getDirectShowProperty() const
    {
        return m_camera->getProperties()->getTilt();
    }

    bool CameraPropertyTilt::isSupported() const
    {
        return isPropertySupportedInternal();
    }

    std::pair<long, long> CameraPropertyTilt::getRange() const
    {
        return getRangeInternal();
    }

    long CameraPropertyTilt::getStep() const
    {
        return getStepInternal();
    }

    long CameraPropertyTilt::getValue() const
    {
        return getValueInternal();
    }

    void CameraPropertyTilt::setValue(const long degree)
    {
        // Make sure the degree is in the range of -180 to 180
        const auto degreeIn180 = Utils::MathUtils::ConfirmDegreeIn180Range(degree);

        // Set value
        setValueInternal(degreeIn180);
    }
}
#pragma once
#ifndef WIN_CAMERA_PROPERTY_H
#define WIN_CAMERA_PROPERTY_H

#include <functional>

#include "directshow_camera/ds_camera.h"
#include "directshow_camera/ds_camera_property.h"

namespace WinCamera { class WinCamera; }

namespace WinCamera
{
    /**
     * @brief A abstract class for WinCamera Property
     */
    class WinCameraProperty
    {
    public:
        WinCameraProperty(
            const WinCamera& camera,
            const std::shared_ptr<DirectShowCamera::DirectShowCamera>& ds_camera,
            const std::shared_ptr<DirectShowCamera::DirectShowCameraProperty>& property
        );

    protected:
        bool isPropertySupportedInternal() const;
        std::pair<long, long> getRangeInternal() const;
        long getStepInternal() const;
        long getValueInternal() const;
        void setValueInternal(const long value);

        void checkOpening() const;
        void checkOpeningAndSupported() const;

    private:
        typedef std::function<bool()> IsCameraOpenedFunc;
        IsCameraOpenedFunc m_isCameraOpenedFunc;
        
        const std::shared_ptr<DirectShowCamera::DirectShowCamera> m_camera;
        const std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> m_property;
    };
}

//*******************************

#endif
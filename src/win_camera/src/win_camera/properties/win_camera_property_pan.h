/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_PAN_H
#define WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_PAN_H

//************Content************

#include "win_camera/properties/win_camera_property.h"

namespace WinCamera { class WinCamera; }

namespace WinCamera
{
    class WinCameraPropertyPan : public WinCameraProperty
    {
    public:
        WinCameraPropertyPan(
            const WinCamera& camera,
            const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property pan is supported.
         * @return Retrun true if property pan is supported.
        */
        bool isSupported() const;

        /**
         * @brief Get the range of the property - pan
         * @return Return (min,max).
        */
        std::pair<long, long> getRange() const;

        /**
         * @brief Get the step of the property - pan.
         * @return Return the step of the pan.
        */
        long getStep() const;

        /**
         * @brief Get current pan
         * @return Return current pan.
        */
        long getValue() const;

        /**
         * @brief Set Pan
         * @param degree Value to be set in degree
         * @return Return true if success.
        */
        void setValue(const long degree);

    protected:
        std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> getDirectShowProperty() const override;
    };
}

//*******************************

#endif
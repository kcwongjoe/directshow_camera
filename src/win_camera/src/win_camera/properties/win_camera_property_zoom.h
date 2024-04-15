/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_ZOOM_H
#define WIN_CAMERA__WIN_CAMERA__PROPERTIES__WIN_CAMERA_PROPERTY_ZOOM_H

//************Content************

#include "win_camera/properties/win_camera_property.h"

namespace WinCamera { class WinCamera; }

namespace WinCamera
{
    class WinCameraPropertyZoom : public WinCameraProperty
    {
    public:
        WinCameraPropertyZoom(
            const WinCamera& camera,
            const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& ds_camera
        );

        /**
         * @brief Retrun true if property zoom is supported.
         * @return Retrun true if property zoom is supported.
        */
        bool isSupported() const;

        /**
         * @brief Get the range of the property - zoom
         * @return Return (min,max).
        */
        std::pair<long, long> getRange() const;

        /**
         * @brief Get the step of the property - zoom.
         * @return Return the step of the zoom.
        */
        long getStep() const;

        /**
         * @brief Get current zoom
         * @return Return current zoom.
        */
        long getValue() const;

        /**
         * @brief Set Zoom
         * @param millimeter Value to be set in millimeters
         * @return Return true if success.
        */
        void setValue(const long millimeter);

    protected:
        std::shared_ptr<DirectShowCamera::DirectShowCameraProperty> getDirectShowProperty() const override;
    };
}

//*******************************

#endif
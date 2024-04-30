/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef DIRECTSHOW_CAMERA__DIRECTSHOW_CAMERA__PROPERTIES__DIRECTSHOW_CAMERA_CAMERA_CONTROL_PROPERTY_H
#define DIRECTSHOW_CAMERA__DIRECTSHOW_CAMERA__PROPERTIES__DIRECTSHOW_CAMERA_CAMERA_CONTROL_PROPERTY_H

//************Content************

#include "directshow_camera/properties/ds_camera_property.h"
#include "directshow_camera/ds_header.h"
#include <string>

namespace DirectShowCamera
{
    /**
     * @brief A class to store the DirectShow Camera Property which is related to IAMCameraControl
     * 
     */
    class DirectShowCameraCameraControlProperty : public DirectShowCameraProperty
    {
    public:

#pragma region Constructor and Destructor

        /**
         * @brief Dummy constructor, use DirectShowCameraCameraControlProperty(string, long) instead.
        */
        DirectShowCameraCameraControlProperty();

        /**
         * @brief Constructor
         * @param[in] name Property Name, use for casting to string
         * @param[in] propertyTag Property tag in directshow
        */
        DirectShowCameraCameraControlProperty(
            const std::string name,
            const long propertyTag
        );

#pragma endregion Constructor and Destructor

#pragma region Import

        /**
         * @brief Import property from IAMCameraControl
         * @param[in] cameraControl IAMCameraControl
         * @return Return true if success.
        */
        bool ImportProperty(IAMCameraControl* cameraControl);

        /**
         * @brief Import current value of the property from IAMCameraControl
         * @param[in] cameraControl IAMCameraControl
         * @return Return true if success.
        */
        bool ImportPropertyValue(IAMCameraControl* cameraControl);

#pragma endregion Import

    private:

        /**
        * @brief Constructor
        */
        void Constructor();

#pragma region CapsFlag

    private:

        /**
         * @brief Check whether the Capsflags contains auto mode.
         * @param[in] capsFlags CapsFlag
         * @return Return true if the Capsflags contains auto mode.
        */
        bool isAutoCapsFlag(const long capsFlags) const;

        /**
         * @brief Check whether the Capsflags contains manual mode.
         * @param[in] capsFlags CapsFlag
         * @return Return true if the Capsflags contains manual mode.
        */
        bool isManualCapsFlag(const long capsFlags) const;

        /**
         * @brief Get CapsFlag based on the mode
         * @param[in] isAutoMode Is auto mode?
         * @return Return CapsFlag
        */
        long getCapsFlag(const bool isAuto) const;

#pragma endregion CapsFlag

    private:
        /**
         * @brief Property tag in directshow
        */
        long m_propertyTag = 0;
    };
}

//*******************************

#endif

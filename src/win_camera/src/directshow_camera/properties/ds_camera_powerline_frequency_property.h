/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef WIN_CAMERA__DIRECTSHOW_CAMERA__PROPERTIES__DIRECTSHOW_CAMERA_POWERLINE_FREQUENCY_PROPERTY_H
#define WIN_CAMERA__DIRECTSHOW_CAMERA__PROPERTIES__DIRECTSHOW_CAMERA_POWERLINE_FREQUENCY_PROPERTY_H

//************Content************

#include "directshow_camera/properties/ds_camera_property.h"
#include "directshow_camera/ds_header.h"
#include <string>

namespace DirectShowCamera
{
    /**
     * @brief A class to store the DirectShow Camera Powerline Frequency Property
     * 
     */
    class DirectShowCameraPowerlineFrequencyProperty : public DirectShowCameraProperty
    {
    public:

#pragma region Constructor and Destructor

        /**
         * @brief Constructor
        */
        DirectShowCameraPowerlineFrequencyProperty();

#pragma endregion Constructor and Destructor

#pragma region Import

        /**
         * @brief Import property from IVideoProcAmp
         * @param[in] videoProcAmp IVideoProcAmp
         * @return Return true if success.
        */
        bool ImportProperty(IVideoProcAmp* videoProcAmp);

        /**
         * @brief Import current value of the property from IVideoProcAmp
         * @param[in] videoProcAmp IVideoProcAmp
         * @return Return true if success.
        */
        bool ImportPropertyValue(IVideoProcAmp* videoProcAmp);

#pragma endregion Import

    private:

        /**
        * @brief Constructor
        */
        void Constructor();

#pragma region CapsFlag

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
    };
}

//*******************************

#endif

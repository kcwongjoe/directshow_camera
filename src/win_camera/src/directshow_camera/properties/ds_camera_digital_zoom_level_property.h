#pragma once
#ifndef WIN_CAMERA__DIRECTSHOW_CAMERA__PROPERTIES__DIRECTSHOW_CAMERA_DIGITAL_ZOOM_LEVEL_PROPERTY_H
#define WIN_CAMERA__DIRECTSHOW_CAMERA__PROPERTIES__DIRECTSHOW_CAMERA_DIGITAL_ZOOM_LEVEL_PROPERTY_H

//************Content************

#include "directshow_camera/properties/ds_camera_property.h"
#include "directshow_camera/ds_header.h"
#include <string>

namespace DirectShowCamera
{
    /**
     * @brief   A class to store the DirectShow Camera Digital Zoom Level Property
     *          Digital zoom is applied after the image is captured. The effect of
     *          digital zoom is to multiply the optical magnification by a factor m,
     *          which can be calculated as follows:
     *          m = ( (Z'cur - Z'min) * (m-max - 1) ) / (Z'max - Z'min) ) + 1
     *          where
     *          Z'cur is the current digital zoom level
     *          Z'min is the minimum digital zoom level
     *          Z'max is the maximum digital zoom level
     *          m-max is the Maximum digital magnification See KSPROPERTY_VIDEOPROCAMP_DIGITAL_MULTIPLIER_LIMIT
     * 
     */
    class DirectShowCameraDigitalZoomLevelProperty : public DirectShowCameraProperty
    {
    public:

#pragma region Constructor and Destructor

        /**
         * @brief Constructor
        */
        DirectShowCameraDigitalZoomLevelProperty();

        /**
         * @brief Reset
        */
        void Reset() override;

#pragma endregion Constructor and Destructor

#pragma region Import

        /**
         * @brief Import property from IVideoProcAmp
         * @param[in] videoProcAmp IVideoProcAmp
         * @param[in] videoInputFilter IBaseFilter
         * @return Return true if success.
        */
        bool ImportProperty(IVideoProcAmp* videoProcAmp, IBaseFilter* videoInputFilter = NULL);

        /**
         * @brief Import current value of the property from IVideoProcAmp
         * @param[in] videoProcAmp IVideoProcAmp
         * @return Return true if success.
        */
        bool ImportPropertyValue(IVideoProcAmp* videoProcAmp);

        /**
        * @brief Import Maximum Digital Magnification from IBaseFilter
        * @param[in] videoInputFilter IBaseFilter
        * @return Return true if success.
        */
        bool ImportMaximumDigitalMagnification(IBaseFilter* videoInputFilter);

#pragma endregion Import

        /**
        * @brief Get Maximum Digital Magnification
        * @return Return Maximum Digital Magnification. Return -1 if value is not imported, 0 if it is not supported or failed.
        **/
        long getMaximumDigitalMagnification() const;

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
    private:
        long m_maximumDigitalMagnification = -1;
    };
}

//*******************************

#endif

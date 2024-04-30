/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef DIRECTSHOW_CAMERA__CAMERA__PROPERTIES__CAMERA_PROPERTY_H
#define DIRECTSHOW_CAMERA__CAMERA__PROPERTIES__CAMERA_PROPERTY_H

//************Content************

#include "directshow_camera/camera/ds_camera.h"
#include "directshow_camera/properties/ds_camera_property.h"

#include <functional>
#include <memory>

// To prevent circular dependence, forward declaration Camera is used here and then pass Camera function in cpp file. For variable passing, use shared_ptr.
namespace DirectShowCamera { class Camera; }

namespace DirectShowCamera
{
    /**
     * @brief A abstract class for Camera Property
     */
    class CameraProperty
    {
    public:
        CameraProperty(
            const Camera& camera,
            const std::shared_ptr<AbstractDirectShowCamera>& ds_camera
        );

    protected:
        enum Mode
        {
            Auto,
            Manual
        };

    protected:

        /**
         * @brief Derived class must override this function to specific which property to use.
         * @return Return the property
        */
        virtual std::shared_ptr<DirectShowCameraProperty> getDirectShowProperty() const
        {
            throw std::runtime_error("getDirectShowProperty() must be overridden.");
        }

        /**
        * @brief Check if property being supported by the camera
        * @return Return true if property is supported
        **/
        bool isPropertySupportedInternal() const;

        /**
        * @brief Get the range of the property
        * @return Return the range of the property in (min, max)
        **/
        std::pair<long, long> getRangeInternal() const;

        /**
        * @brief Get the step of the property
        * @return Return the step of the property
        **/
        long getStepInternal() const;

        /**
        * @brief Get the value of the property
        * @return Return the value of the property
        **/
        long getValueInternal() const;

        /**
        * @brief Set the value of the property
        * @param value Value to be set
        * @param isAuto Set to true if the property is set to auto
        **/
        void setValueInternal(const long value, const bool isAuto = false);

        /**
        * @brief Check if the property is set to auto
        * 
        * @return Return true if the property is set to auto
        **/
        bool isAutoInternal() const;

        /**
        * @brief Check if the property support auto mode
        * 
        * @return Return true if the property support auto mode
        **/
        bool SupportAutoModeInternal() const;

        /**
        * @brief Check if the property support manual mode
        * 
        * @return Return true if the property support manual mode
        **/
        bool ySupportManualModeInternal() const;

        /**
        * @brief A method to check if camera is opened. If not, throw an exception.
        **/
        void CheckOpening() const;

        /**
        * @brief A method to check if camera is opened and property is supported. If not, throw an exception.
        **/
        void CheckOpeningAndSupported() const;

        /**
        * @brief A method to check if camera property support the mode. If not, throw an exception.
        **/
        void CheckModeSupport(Mode mode) const;

    protected:
        const std::shared_ptr<AbstractDirectShowCamera> m_camera;

    private:
        typedef std::function<bool()> IsCameraOpenedFunction;
        IsCameraOpenedFunction m_isCameraOpenedFunc;
    };
}

//*******************************

#endif
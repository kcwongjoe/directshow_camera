#pragma once
#ifndef WIN_CAMERA__DIRECTSHOW_CAMERA__CAMERA__DIRECTSHOW_CAMERA_ABSTRACT_H
#define WIN_CAMERA__DIRECTSHOW_CAMERA__CAMERA__DIRECTSHOW_CAMERA_ABSTRACT_H

//************Content************
#include "directshow_camera/properties/ds_camera_properties.h"

#include "directshow_camera/video_format/ds_video_format.h"

#include "directshow_camera/device/ds_camera_device.h"

#include <optional>

namespace DirectShowCamera
{
    /**
     * @brief Directshow Camera. This is the core of this project.
     *
     */
    class AbstractDirectShowCamera
    {
    public:
        virtual ~AbstractDirectShowCamera() { }

        virtual void Release() = 0;

        virtual bool Open(
            IBaseFilter** videoInputFilter,
            std::optional<const DirectShowVideoFormat> videoFormat = std::nullopt
        ) = 0;
        virtual void Close() = 0;
        virtual bool isOpening() const = 0;
        virtual bool isDisconnecting() = 0;
        virtual void setDisconnectionProcess(std::function<void()> func) = 0;

        virtual bool Start() = 0;
        virtual bool Stop() = 0;
        virtual bool isCapturing() const = 0;

        virtual bool getFrame
        (
            unsigned char* pixels,
            int& numOfBytes,
            unsigned long& frameIndex,
            const bool copyNewFrameOnly,
            const unsigned long previousFrameIndex
        ) = 0;
        virtual void setMinimumFPS(const double minimumFPS) = 0;
        virtual double getFPS() const = 0;
        virtual long getFrameTotalSize() const = 0;
        virtual GUID getFrameType() const = 0;

        // Video Format
        virtual std::vector<DirectShowVideoFormat> getVideoFormatList() const = 0;
        virtual int getCurrentVideoFormatIndex() const = 0;
        virtual DirectShowVideoFormat getCurrentVideoFormat() const = 0;
        virtual DirectShowVideoFormat getCurrentGrabberFormat() const = 0;

        virtual bool setVideoFormat(const DirectShowVideoFormat videoFormat) = 0;
        virtual bool setVideoFormat(const int videoFormatIndex) = 0;

        // Property
        virtual void RefreshProperties() = 0;
        virtual std::shared_ptr<DirectShowCameraProperties> getProperties() const = 0;

        virtual void ResetPropertiesToDefault(const bool asAuto = true) = 0;
        virtual bool setPropertyValue(
            std::shared_ptr<DirectShowCameraProperty>& property,
            const long value,
            const bool isAuto
        ) = 0;

        // Get camera
        virtual bool getCameras(std::vector<DirectShowCameraDevice>& cameraDevices) = 0;
        virtual bool getCamera(const int cameraIndex, IBaseFilter** videoInputFilter) = 0;
        virtual bool getCamera(const std::string devicePath, IBaseFilter** videoInputFilter) = 0;
        virtual bool getCamera(const DirectShowCameraDevice device, IBaseFilter** videoInputFilter) = 0;

        virtual std::string getLastError() const = 0;
    };
}


//*******************************

#endif
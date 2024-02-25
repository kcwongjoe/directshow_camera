#pragma once
#ifndef DIRECTSHOW_CAMERA_ABSTRACT_H
#define DIRECTSHOW_CAMERA_ABSTRACT_H

//************Content************
#include <ds_camera_properties.h>
#include <ds_video_format.h>

#include <camera_device.h>

#include <string>
#include <vector>
#include <functional>
#include <thread>

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

        virtual void release() = 0;

        virtual bool open(IBaseFilter** videoInputFilter, DirectShowVideoFormat* videoFormat = NULL) = 0;
        virtual void close() = 0;
        virtual bool isOpening() const = 0;
        virtual bool checkDisconnection() = 0;
        virtual void setDisconnectionProcess(std::function<void()> func) = 0;

        virtual bool start() = 0;
        virtual bool stop() = 0;
        virtual bool isCapturing() const = 0;

        virtual bool getFrame
        (
            unsigned char* pixels,
            unsigned long* frameIndex,
            int* numOfBytes,
            bool copyNewFrameOnly,
            unsigned long previousFrameIndex
        ) = 0;
        virtual void setMinimumPFS(double minimumFPS) = 0;
        virtual double getFPS() const = 0;
        virtual long getFrameTotalSize() const = 0;
        virtual GUID getFrameType() const = 0;

        // Video Format
        virtual std::vector<DirectShowVideoFormat> getVideoFormatList() const = 0;
        virtual int getCurrentVideoFormatIndex() const = 0;
        virtual DirectShowVideoFormat getCurrentVideoFormat() const = 0;
        virtual DirectShowVideoFormat getCurrentGrabberFormat() const = 0;

        virtual bool setVideoFormat(DirectShowVideoFormat* videoFormat) = 0;
        virtual bool setVideoFormat(int videoFormatIndex) = 0;

        // Property
        virtual void refreshProperties() = 0;
        virtual DirectShowCameraProperties* getProperties() const = 0;

        virtual void resetDefault(bool asAuto = true) = 0;
        virtual bool setValue(DirectShowCameraProperty* property, long value, bool isAuto) = 0;

        // Get camera
        virtual bool getCameras(std::vector<DirectShowCameraDevice>* cameraDevices) = 0;
        virtual bool getCamera(int cameraIndex, IBaseFilter** videoInputFilter) = 0;
        virtual bool getCamera(std::string devicePath, IBaseFilter** videoInputFilter) = 0;
        virtual bool getCamera(DirectShowCameraDevice device, IBaseFilter** videoInputFilter) = 0;

        virtual std::string getLastError() const = 0;
    };
}


//*******************************

#endif
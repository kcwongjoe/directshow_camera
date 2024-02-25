#pragma once
#ifndef DIRECTSHOW_CAMERA_H
#define DIRECTSHOW_CAMERA_H

//************Content************
#include <ds_camera_utils.h>

#include <abstract_ds_camera.h>
#include <ds_camera_properties.h>
#include <ds_video_format.h>
#include <ds_grabber_callback.h>

#include <camera_device.h>

#include <string>
#include <vector>
#include <functional>

namespace DirectShowCamera
{
    /**
     * @brief Directshow Camera. This is the core of this project.
     * 
     */
    class DirectShowCamera : public AbstractDirectShowCamera
    {
    public:

        DirectShowCamera();
        ~DirectShowCamera();
        void release();

        bool open(IBaseFilter** videoInputFilter, DirectShowVideoFormat* videoFormat = NULL);
        void close();
        bool isOpening() const;
        bool checkDisconnection();
        void setDisconnectionProcess(std::function<void()> func);

        bool start();
        bool stop();
        bool isCapturing() const;

        bool getFrame
        (
            unsigned char* pixels,
            unsigned long* frameIndex = NULL,
            int* numOfBytes = NULL,
            bool copyNewFrameOnly = false,
            unsigned long previousFrameIndex = 0
        );
        void setMinimumPFS(double minimumFPS);
        double getFPS() const;
        long getFrameTotalSize() const;
        GUID getFrameType() const;

        // Video Format
        std::vector<DirectShowVideoFormat> getVideoFormatList() const;
        int getCurrentVideoFormatIndex() const;
        DirectShowVideoFormat getCurrentVideoFormat() const;
        DirectShowVideoFormat getCurrentGrabberFormat() const;

        bool setVideoFormat(DirectShowVideoFormat* videoFormat);
        bool setVideoFormat(int videoFormatIndex);

        // Property
        void refreshProperties();
        DirectShowCameraProperties* getProperties() const;

        void resetDefault(bool asAuto = true);
        bool setValue(DirectShowCameraProperty* property, long value, bool isAuto);

        // Get camera
        bool getCameras(std::vector<DirectShowCameraDevice>* cameraDevices);
        bool getCamera(int cameraIndex, IBaseFilter** videoInputFilter);
        bool getCamera(std::string devicePath, IBaseFilter** videoInputFilter);
        bool getCamera(DirectShowCameraDevice device, IBaseFilter** videoInputFilter);

        std::string getLastError() const;

    private:

        // Graph and filter
        ICaptureGraphBuilder2* m_captureGraphBuilder = NULL;
        IGraphBuilder* m_filterGraphManager = NULL;
        IBaseFilter* m_videoInputFilter = NULL;
        IBaseFilter* m_grabberFilter = NULL;
        IBaseFilter* m_nullRendererFilter = NULL;

        // Config
        IAMStreamConfig* m_streamConfig = NULL;
        DirectShowCameraProperties* m_property = NULL;

        std::vector<DirectShowVideoFormat*>* m_videoFormats = NULL;
        int m_currentVideoFormatIndex = -1;

        // Callback
        ISampleGrabber* m_sampleGrabber = NULL;
        SampleGrabberCallback* m_sampleGrabberCallback = NULL;
        GUID m_grabberMediaSubType = MEDIASUBTYPE_None;
        DirectShowVideoFormat m_sampleGrabberVideoFormat;

        IMediaEventEx* m_mediaEvent = NULL;
        IMediaControl* m_mediaControl = NULL;

        bool m_isOpening = false;
        bool m_isCapturing = false;
        std::string m_errorString = "";

        std::thread m_checkConnectionThread;
        bool m_isRunningCheckConnectionThread = false;
        bool m_stopCheckConnectionThread = false;
        std::function<void()> m_disconnectionProcess = NULL;
        void startCheckConnectionThread();

        void updateGrabberFilterVideoFormat();
        bool updateVideoFormatList();
        void updateVideoFormatIndex();

        int getVideoFormatIndex(AM_MEDIA_TYPE* mediaType) const;
        int getVideoFormatIndex(DirectShowVideoFormat* videoFormat) const;

    };
}


//*******************************

#endif
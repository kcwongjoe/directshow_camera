#pragma once
#ifndef DIRECTSHOW_CAMERA_H
#define DIRECTSHOW_CAMERA_H

//************Content************
#include <ds_camera_utils.h>

#include <ds_camera.h>
#include <ds_camera_properties.h>
#include <ds_video_format.h>
#include <ds_grabber_callback.h>

#include <camera_device.h>
#include <camera_resolution.h>

#include <string>
#include <vector>
#include <functional>

namespace DirectShowCamera
{
    /**
     * @brief Directshow Camera. This is the core of this project.
     * 
     */
    class DirectShowCamera
    {
    private:

        static bool s_isInitializedCOMLib;

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
        SampleGrabberCallback* m_sampleGrabberCallback = new SampleGrabberCallback();
        GUID m_grabberMediaSubType = MEDIASUBTYPE_None;

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

        int getVideoFormatIndex(AM_MEDIA_TYPE* mediaType);
        int getVideoFormatIndex(DirectShowVideoFormat* videoFormat);

    public:

        DirectShowCamera();
        ~DirectShowCamera();
        void release();

        bool open(IBaseFilter** videoInputFilter, DirectShowVideoFormat* videoFormat = NULL);
        void close();
        bool isOpening();
        bool checkDisconnection();
        void setDisconnectionProcess(std::function<void()> func);

        bool start();
        bool stop();
        bool isCapturing();

        bool getFrame(unsigned char* pixels, unsigned long* frameIndex = NULL, int* numOfBytes = NULL, bool copyNewFrameOnly = false, unsigned long previousFrameIndex = 0);
        void setMinimumPFS(double minimumFPS);
        double getFPS();
        long getFrameTotalSize();
        GUID getFrameType();

        // Video Format
        std::vector<DirectShowVideoFormat> getVideoFormatList();
        int getCurrentVideoFormatIndex();
        DirectShowVideoFormat getCurrentVideoFormat();

        bool setVideoFormat(DirectShowVideoFormat* videoFormat);
        bool setVideoFormat(int videoFormatIndex);

        // Property
        void refreshProperties();
        DirectShowCameraProperties* getProperties();

        void resetDefault(bool asAuto = true);
        bool setValue(DirectShowCameraProperty* property, long value, bool isAuto);

        // Get camera
        bool getCameras(std::vector<DirectShowCameraDevice>* cameraDevices);
        bool getCamera(int cameraIndex, IBaseFilter** videoInputFilter);
        bool getCamera(std::string devicePath, IBaseFilter** videoInputFilter);
        bool getCamera(DirectShowCameraDevice device, IBaseFilter** videoInputFilter);

        std::string getLastError();


    };
}


//*******************************

#endif
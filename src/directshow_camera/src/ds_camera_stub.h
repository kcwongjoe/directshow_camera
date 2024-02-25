#pragma once
#ifndef DIRECTSHOW_CAMERA_STUB_H
#define DIRECTSHOW_CAMERA_STUB_H

//************Content************
#include <abstract_ds_camera.h>
#include <ds_camera_properties.h>
#include <ds_camera_stub_default.h>
#include <ds_video_format.h>

#include <camera_device.h>

#include <string>
#include <vector>
#include <functional>

namespace DirectShowCamera
{
    /**
     * @brief Directshow Camera Stub. Youc can use this stub to simulate a camera for testing.
     *
     */
    class DirectShowCameraStub : public AbstractDirectShowCamera
    {
    public:
        /**
         * @brief Get Frame function
        */
        typedef std::function<void(unsigned char* pixels, unsigned long* frameIndex, int* numOfBytes, bool copyNewFrameOnly, unsigned long previousFrameIndex)> GetFrameFunc;
    public:

        DirectShowCameraStub();
        ~DirectShowCameraStub();
        void release();

        bool open(IBaseFilter** videoInputFilter, DirectShowVideoFormat* videoFormat = NULL);
        void close();
        bool isOpening() const;
        bool checkDisconnection();
        void disconnetCamera();
        void setDisconnectionProcess(std::function<void()> func);

        bool start();
        bool stop();
        bool isCapturing() const;

        void setGetFrameFunction(GetFrameFunc func);
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
        // Config
        DirectShowCameraProperties* m_properties = NULL;

        std::vector<DirectShowVideoFormat*>* m_videoFormats = NULL;
        int m_currentVideoFormatIndex = -1;

        bool m_isOpening = false;
        bool m_isCapturing = false;
        std::string m_errorString = "";

        bool m_disconnectCamera = false;
        std::thread m_checkConnectionThread;
        bool m_isRunningCheckConnectionThread = false;
        bool m_stopCheckConnectionThread = false;
        std::function<void()> m_disconnectionProcess = NULL;
        void startCheckConnectionThread();

        double m_fps = 0.5;

        GetFrameFunc m_getFrameFunc = NULL;

        bool updateVideoFormatList();
        int getVideoFormatIndex(DirectShowVideoFormat* videoFormat) const;

    };
}


//*******************************

#endif
#pragma once
#ifndef DIRECTSHOW_CAMERA_H
#define DIRECTSHOW_CAMERA_H

//************Content************
#include "ds_camera/ds_camera_utils.h"

#include "ds_camera/abstract_ds_camera.h"
#include "ds_camera/ds_camera_properties.h"
#include "ds_camera/ds_video_format.h"
#include "ds_camera/ds_grabber_callback.h"

#include "camera_device.h"

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
        void release() override;

        bool open(IBaseFilter** videoInputFilter, DirectShowVideoFormat* videoFormat = NULL) override;
        void close() override;
        bool isOpening() const override;
        bool checkDisconnection() override;
        void setDisconnectionProcess(std::function<void()> func) override;

        bool start() override;
        bool stop() override;
        bool isCapturing() const override;

        /**
         * @brief Get current frame
         * @param[out] frame Frame bytes
         * @param[out] numOfBytes Number of bytes of the frames.
         * @param[out] frameIndex Index of frame, use to indicate whether a new frame.
         * @param[in] copyNewFrameOnly Set it as true if only want to collect new frame. Default is false
         * @param[in] previousFrameIndex The previous frame index, use to idendify whether a new frame. This variable work with copyNewFrameOnly. Default as 0.
         * @return Return true if success.
        */
        bool getFrame
        (
            unsigned char* pixels,
            int& numOfBytes,
            unsigned long& frameIndex,
            const bool copyNewFrameOnly = false,
            const unsigned long previousFrameIndex = 0
        ) override;

        void setMinimumPFS(double minimumFPS) override;
        double getFPS() const override;
        long getFrameTotalSize() const override;
        GUID getFrameType() const override;

        // Video Format
        std::vector<DirectShowVideoFormat> getVideoFormatList() const override;
        int getCurrentVideoFormatIndex() const override;
        DirectShowVideoFormat getCurrentVideoFormat() const override;
        DirectShowVideoFormat getCurrentGrabberFormat() const override;

        bool setVideoFormat(DirectShowVideoFormat* videoFormat) override;
        bool setVideoFormat(int videoFormatIndex) override;

        // Property
        void refreshProperties() override;
        std::shared_ptr<DirectShowCameraProperties> getProperties() const override;

        void resetDefault(bool asAuto = true) override;
        bool setValue(DirectShowCameraProperty* property, long value, bool isAuto) override;

        // Get camera
        bool getCameras(std::vector<DirectShowCameraDevice>* cameraDevices) override;
        bool getCamera(int cameraIndex, IBaseFilter** videoInputFilter) override;
        bool getCamera(std::string devicePath, IBaseFilter** videoInputFilter) override;
        bool getCamera(DirectShowCameraDevice device, IBaseFilter** videoInputFilter) override;

        std::string getLastError() const override;

    private:

        // Graph and filter
        ICaptureGraphBuilder2* m_captureGraphBuilder = NULL;
        IGraphBuilder* m_filterGraphManager = NULL;
        IBaseFilter* m_videoInputFilter = NULL;
        IBaseFilter* m_grabberFilter = NULL;
        IBaseFilter* m_nullRendererFilter = NULL;

        // Config
        IAMStreamConfig* m_streamConfig = NULL;
        std::shared_ptr<DirectShowCameraProperties> m_property = nullptr;

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
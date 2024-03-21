#pragma once
#ifndef DIRECTSHOW_CAMERA_STUB_H
#define DIRECTSHOW_CAMERA_STUB_H

//************Content************
#include "directshow_camera/abstract_ds_camera.h"
#include "directshow_camera/ds_camera_properties.h"
#include "directshow_camera/ds_camera_stub_default.h"
#include "directshow_camera/ds_video_format.h"
#include "directshow_camera/ds_camera_device.h"

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
        typedef std::function<void(unsigned char* pixels, int& numOfBytes, unsigned long& frameIndex,const bool copyNewFrameOnly,const unsigned long previousFrameIndex)> GetFrameFunc;
    public:

        DirectShowCameraStub();
        ~DirectShowCameraStub();
        void release() override;

        bool open(IBaseFilter** videoInputFilter, DirectShowVideoFormat* videoFormat = NULL) override;
        void close() override;
        bool isOpening() const override;
        bool checkDisconnection() override;
        void disconnetCamera();
        void setDisconnectionProcess(std::function<void()> func) override;

        bool start()override;
        bool stop()override;
        bool isCapturing() const override;

        void setGetFrameFunction(GetFrameFunc func);

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
        bool setValue(
            const std::shared_ptr<DirectShowCameraProperty>& property,
            const long value,
            const bool isAuto
        ) override;

        // Get camera
        bool getCameras(std::vector<DirectShowCameraDevice>* cameraDevices) override;
        bool getCamera(int cameraIndex, IBaseFilter** videoInputFilter) override;
        bool getCamera(std::string devicePath, IBaseFilter** videoInputFilter) override;
        bool getCamera(DirectShowCameraDevice device, IBaseFilter** videoInputFilter) override;

        std::string getLastError() const override;

    private:
        // Config
        std::shared_ptr<DirectShowCameraProperties> m_properties = nullptr;

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
#pragma once
#ifndef WIN_CAMERA__DIRECTSHOW_CAMERA__DIRECTSHOW_CAMERA_STUB_H
#define WIN_CAMERA__DIRECTSHOW_CAMERA__DIRECTSHOW_CAMERA_STUB_H

//************Content************
#include "directshow_camera/camera/abstract_ds_camera.h"
#include "directshow_camera/properties/ds_camera_properties.h"
#include "directshow_camera/ds_camera_stub_default.h"
#include "directshow_camera/video_format/ds_video_format_list.h"
#include "directshow_camera/device/ds_camera_device.h"

#include <thread>

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
        typedef std::function<
            void(
                unsigned char* pixels,
                int& numOfBytes,
                unsigned long& frameIndex,
                const bool copyNewFrameOnly,
                const unsigned long previousFrameIndex
            )> GetFrameFunc;
    public:

#pragma region Life cycle

        /**
         * @brief Constructor
        */
        DirectShowCameraStub();

        /**
         * @brief Desctructor
        */
        ~DirectShowCameraStub();

        /**
         * @brief Release
        */
        void Release() override;

#pragma endregion Life cycle

#pragma region Connection

        /**
         * @brief Build the directshow graph
         * @param[in] videoInputFilter Video input filter. Look up from DirectShowCamera::getCamera()
         * @param[in] videoFormat (Optional) Video Format to be set. Look up from DirectShowCameraDevice::getDirectShowVideoFormats()
         * @return Return true if success
        */
        bool Open(
            IBaseFilter** videoInputFilter,
            std::optional<const DirectShowVideoFormat> videoFormat = std::nullopt
        ) override;

        /**
         * @brief Close
        */
        void Close() override;

        /**
         * @brief Return true if camera was opened
         * @return Return true if camera was opened
        */
        bool isOpening() const override;

        /**
         * @brief It can be check camera whether disconnected accidently.
         * @return Return true if camera disconnected.
        */
        bool isDisconnecting() override;

        /**
         * @brief Disconnect camera. It is used to simulate a camera disconnection for testing DisconnectionProcess.
        */
        void DisconnetCamera();

        /**
         * @brief Set the disconnection process. When the process was set, a thread will start to keep check the connection. If camera is disconnected, this process will run and then run stop() internally.
         * @param[in] func void() Function to be run when camera is disconnected.
        */
        void setDisconnectionProcess(std::function<void()> func) override;

#pragma endregion Connection

#pragma region Start/Stop

        /**
         * @brief Start capture
         * @return Return true if success
        */
        bool Start()override;

        /**
         * @brief Stop Capture
         * @return Return true if success
        */
        bool Stop()override;

        /**
         * @brief Return true if camera is capturing
         * @return Return true if camera is capturing
        */
        bool isCapturing() const override;

#pragma endregion Start/Stop

#pragma region Frame

        void setGetFrameFunction(GetFrameFunc func);

        /**
         * @brief Get current frame
         * @param[out] frame Frame in bytes
         * @param[out] numOfBytes Number of bytes of the frames.
         * @param[out] frameIndex Index of frame, use to indicate whether a new frame.
         * @param[in] copyNewFrameOnly Set it as true if only want to collect new frame. Default is false
         * @param[in] previousFrameIndex The previous frame index, use to idendify whether a new frame. This variable work with copyNewFrameOnly. Default as 0.
         * @return Return true if success.
        */
        bool getFrame
        (
            unsigned char* frame,
            int& numOfBytes,
            unsigned long& frameIndex,
            const bool copyNewFrameOnly = false,
            const unsigned long previousFrameIndex = 0
        ) override;

        /**
         * @brief Set Minimum FPS. FPS below this value will be identified as 0. Default as 0.5
         * @param[in] minimumFPS.
        */
        void setMinimumFPS(const double minimumFPS) override;

        /**
         * @brief Get Frame per second.
         * @return Return fps. Return 0 if camera is not opened
        */
        double getFPS() const override;

        /**
         * @brief Get frame size in bytes.
         * @return Return frame size in bytes. Return 0 if camera is not opened.
        */
        long getFrameTotalSize() const override;

        /**
         * @brief Get current frame type. Such as MEDIASUBTYPE_RGB24
         * @return Return frame type
        */
        GUID getFrameType() const override;

#pragma endregion Frame

#pragma region Video Format

        /**
         * @brief Get current video format list of this opened camera.
         * @return
        */
        std::vector<DirectShowVideoFormat> getVideoFormatList() const override;

        /**
         * @brief Get current video format index.
         * @return
        */
        int getCurrentVideoFormatIndex() const override;

        /**
         * @brief Get current video format
         * @return
        */
        DirectShowVideoFormat getCurrentVideoFormat() const override;

        /**
         * @brief Get current grabber format
         * @return
        */
        DirectShowVideoFormat getCurrentGrabberFormat() const override;

        /**
         * @brief Set video format. It is suggested to set video format in the open(). It may not succes to change the video format after opened camera.
         * @param[in] videoFormat Video format to be set
         * @return Return true if success.
        */
        bool setVideoFormat(const DirectShowVideoFormat videoFormat) override;

        /**
         * @brief Set video format. It is suggested to set video format in the open(). It may not succes to change the video format after opening camera.
         * @param[in] videoFormatIndex Index of the video foramt list.
         * @return Return true if success.
        */
        bool setVideoFormat(const int videoFormatIndex) override;

#pragma endregion Video Format

#pragma region Properties

        /**
         * @brief Refresh properties
         */
        void RefreshProperties() override;

        /**
         * @brief Get properties
         * @return Return properties
        */
        std::shared_ptr<DirectShowCameraProperties> getProperties() const override;

        /**
         * @brief Reset properties to default
         *
         * @param[in] asAuto (Option) Set as true if you also want to set properties to auto. Default as true.
         */
        void ResetPropertiesToDefault(const bool asAuto = true) override;

        /**
         * @brief Set property value
         * @param[in] property Property
         * @param[in] value Value to be set
         * @param[in] isAuto Set as true for auto mode, false for manual mode
         * @return Return true if success
        */
        bool setPropertyValue(
            std::shared_ptr<DirectShowCameraProperty>& property,
            const long value,
            const bool isAuto
        ) override;

#pragma endregion Properties

#pragma region Camera

        /**
         * @brief Get the available camera list
         * @param[out] cameraDevices Camera Devices.
         * @return Return true if success
        */
        bool getCameras(std::vector<DirectShowCameraDevice>& cameraDevices) override;

        /**
         * @brief Get the video input filter based on the camera index
         * @param[in] cameraIndex Camera index
         * @param[out] videoInputFilter Output video input filter
         * @return Return true if success.
        */
        bool getCamera(const int cameraIndex, IBaseFilter** videoInputFilter) override;

        /**
         * @brief Get the video input filter based on the Camera device path
         * @param[in] devicePath Camera device path
         * @param[out] videoInputFilter Output video input filter
         * @return Return true if success.
        */
        bool getCamera(const std::string devicePath, IBaseFilter** videoInputFilter) override;

        /**
         * @brief Get the video input filter based on the Camera device object
         * @param[in] device Camera device
         * @param[out] videoInputFilter Output video input filter
         * @return Return true if success.
        */
        bool getCamera(const DirectShowCameraDevice device, IBaseFilter** videoInputFilter) override;

#pragma endregion Camera

        /**
         * @brief Get the last error
         * @return Return the last error
        */
        std::string getLastError() const override;

    private:
        // Config
        std::shared_ptr<DirectShowCameraProperties> m_properties = nullptr;

        DirectShowVideoFormatList m_videoFormats = DirectShowVideoFormatList();
        int m_currentVideoFormatIndex = -1;

        bool m_isOpening = false;
        bool m_isCapturing = false;
        std::string m_errorString = "";

        bool m_disconnectCamera = false;
        std::thread m_checkConnectionThread;
        bool m_isRunningCheckConnectionThread = false;
        bool m_stopCheckConnectionThread = false;
        std::function<void()> m_disconnectionProcess = NULL;

        /**
         * @brief Start a thread to check the device connection
        */
        void StartCheckConnectionThread();

        double m_fps = 0.5;

        GetFrameFunc m_getFrameFunc = NULL;

    private:
        /**
         * @brief Update video formats
        */
        bool UpdateVideoFormatList();

        /**
         * @brief Get index from the Video Format list
         * @param[in] videoFormat to be searched
         * @return Return -1 if not found
        */
        int getVideoFormatIndex(const DirectShowVideoFormat videoFormat) const;

    };
}


//*******************************

#endif
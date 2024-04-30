/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef DIRECTSHOW_CAMERA__DIRECTSHOW_CAMERA__CAMERA__DIRECTSHOW_CAMERA_H
#define DIRECTSHOW_CAMERA__DIRECTSHOW_CAMERA__CAMERA__DIRECTSHOW_CAMERA_H

//************Content************
#include "directshow_camera/camera/abstract_ds_camera.h"
#include "directshow_camera/camera/i_media_control_handler.h"

#include "directshow_camera/properties/ds_camera_properties.h"

#include "directshow_camera/grabber/ds_grabber_callback.h"

#include "directshow_camera/video_format/ds_video_format_list.h"
#include "directshow_camera/video_format/ds_video_format.h"


namespace DirectShowCamera
{
    /**
     * @brief Directshow Camera. This is the core of this project.
     * 
     */
    class DirectShowCamera : public AbstractDirectShowCamera
    {
    public:

#pragma region Life cycle

        /**
         * @brief Constructor
        */
        DirectShowCamera();

        /**
         * @brief Desctructor
        */
        ~DirectShowCamera();

        /**
         * @brief Release
        */
        void Release() override;

#pragma endregion Life cycle

#pragma region Connection

        /**
         * @brief Build the directshow graph
         * @param[in] directShowFilter DirectShow Filter. Look up from DirectShowCamera::getCamera()
         * @param[in] videoFormat (Optional)Video Format to be set. Look up from DirectShowCameraDevice::getDirectShowVideoFormats()
         * @return Return true if success
         *
         * @startuml {directshow_diagram.svg} "DirectShow Camera Diagram"
         *	skinparam linetype polyline
         *	skinparam linetype ortho
         *	"Capture Graph Builder" .d.>[Manage] "Filter Graph Manager"
         *	"Filter Graph Manager" .d.>[QueryInterface] "Media Control"
         *	note left
         *		Use to control the camera
         *	end note
         *	"Filter Graph Manager" .d.>[QueryInterface] "Media Event"
         *	note right
         * 		Check the camera disconnection state
         *	end note
         * 	"Filter Graph Manager" -r->[Contain] "Filters"
         *	partition "Filters" #00b9a0 {
         *		"DirectShow Filter" .d.> "Sample Grabber Filter"
         *		"Sample Grabber Filter" .d.> "Null Renderer Filter"
         *	}
         *	"Sample Grabber Filter" -r->[Set] "SampleGrabberCallback"
         * @enduml
        */
        bool Open(
            IBaseFilter** directShowFilter,
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
         * @brief Set the disconnection process. When the process was set, a thread will start to keep check the connection. If camera is disconnected, this process will run and then run stop() internally.
         * @param[in] func void() Function to be run when camera is disconnected.
        */
        void setDisconnectionProcess(std::function<void()> func) override;

#pragma endregion Connection
        
#pragma region start/stop

        /**
         * @brief Start capture
         * @return Return true if success
        */
        bool Start() override;

        /**
         * @brief Stop Capture
         * @return Return true if success
        */
        bool Stop() override;

        /**
         * @brief Return true if camera is capturing
         * @return Return true if camera is capturing
        */
        bool isCapturing() const override;

#pragma endregion start/stop

#pragma region Frame

        /**
         * @brief Get current frame
         * @param[out] frame Frame bytes
         * @param[out] numOfBytes Number of bytes of the frames.
         * @param[out] frameIndex Index of frame, use to indicate whether a new frame.
         * @return Return true if success.
        */
        bool getFrame
        (
            unsigned char* pixels,
            int& numOfBytes,
            unsigned long& frameIndex
        ) override;

        /**
        * @brief Get the last frame index. It use to identify whether a new frame. Index will only be updated when you call getFrame() or gatMat();
        * @return Return the last frame index.
        */
        unsigned long getLastFrameIndex() const override;

        /**
         * @brief Set Minimum FPS. FPS below this value will be identified as 0. Default as 0.5
         * @param[in] minimumFPS
        */
        void setMinimumFPS(const double minimumFPS) override;

        /**
         * @brief Get current Frame per second. Return 0 if camera is not opened.
         * @return Return fps.
        */
        double getFPS() const override;

        /**
         * @brief Get current frame size in bytes. Return 0 if camera is not opened.
         * @return Return frame size in bytes.
        */
        long getFrameTotalSize() const override;

        /**
         * @brief Get current frame type. Such as MEDIASUBTYPE_RGB24
         * @return Return current frame type.
        */
        GUID getFrameType() const override;

#pragma endregion Frame

#pragma region Video Format

        /**
         * @brief Get current video format list of the current camera.
         * @return Return the video format list.
        */
        std::vector<DirectShowVideoFormat> getVideoFormatList() const override;

        /**
         * @brief Get current video format index.
         * @return Return the current video format index.
        */
        int getCurrentVideoFormatIndex() const override;

        /**
         * @brief Get current video format
         * @return Return the current video format
        */
        DirectShowVideoFormat getCurrentVideoFormat() const override;

        /**
        * @brief Get current grabber format
        * @return Return the current grabber format
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
         * @param[in] asAuto (Optional) Set as true if you also want to set properties to auto. Default as true.
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

#pragma region getCamera

        /**
         * @brief Get the available camera list
         * @param[out] cameraDevices Camera Devices.
         * @return Return true if success
        */
        bool getCameras(std::vector<DirectShowCameraDevice>& cameraDevices) override;

        /**
         * @brief Get the DirectShow Filter based on the camera index
         * @param[in] cameraIndex Camera index
         * @param[out] directShowFilter Output DirectShow Filter
         * @return Return true if success.
        */
        bool getCamera(const int cameraIndex, IBaseFilter** directShowFilter) override;

        /**
         * @brief Get the DirectShow Filter based on the Camera device path
         * @param[in] devicePath Camera device path
         * @param[out] directShowFilter Output DirectShow Filter
         * @return Return true if success.
        */
        bool getCamera(const std::string devicePath, IBaseFilter** directShowFilter) override;

        /**
         * @brief Get the DirectShow Filter based on the Camera device object
         * @param[in] device Camera device
         * @param[out] directShowFilter Output DirectShow Filter
         * @return Return true if success.
        */
        bool getCamera(const DirectShowCameraDevice device, IBaseFilter** directShowFilter) override;

#pragma endregion getCamera


#pragma region Error

        /**
        * @brief Reset last error
        */
        void ResetLastError() override;

        /**
         * @brief Get the last error
         * @return Return the last error
        */
        std::string getLastError() const override;

#pragma endregion Error

    private:
        /**
         * @brief Start a thread to check the device connection
        */
        void StartCheckConnectionThread();

        /**
         * @brief Update the grabber filter buffer size and the media type.
        */
        void UpdateGrabberFilterVideoFormat();

        /**
         * @brief Update video formats
        */
        bool UpdateVideoFormatList();

        /**
         * @brief Update the current video foramt index from Steam Config
        */
        void UpdateVideoFormatIndex();

        /**
         * @brief Get index from the Video Format list
         * @param[in] mediaType
         * @return Return -1 if not found
        */
        int getVideoFormatIndex(const AM_MEDIA_TYPE* mediaType) const;

        /**
         * @brief Get index from the Video Format list
         * @param[in] videoFormat
         * @return Return -1 if not found
        */
        int getVideoFormatIndex(const DirectShowVideoFormat videoFormat) const;

    private:

        // Graph and filter
        ICaptureGraphBuilder2* m_captureGraphBuilder = NULL;
        IGraphBuilder* m_filterGraphManager = NULL;
        IBaseFilter* m_directShowFilter = NULL;
        IBaseFilter* m_grabberFilter = NULL;
        IBaseFilter* m_nullRendererFilter = NULL;

        // Config
        IAMStreamConfig* m_amStreamConfig = NULL;
        std::shared_ptr<DirectShowCameraProperties> m_property = nullptr;

        DirectShowVideoFormatList m_videoFormats = DirectShowVideoFormatList();
        int m_currentVideoFormatIndex = -1;

        // Callback
        ISampleGrabber* m_sampleGrabber = NULL;
        SampleGrabberCallback* m_sampleGrabberCallback = NULL;
        GUID m_grabberMediaSubType = MEDIASUBTYPE_None;
        DirectShowVideoFormat m_sampleGrabberVideoFormat;

        IMediaEventEx* m_mediaEvent = NULL;
        IMediaControlHandler m_mediaControlHandler;

        bool m_isOpening = false;
        bool m_isCapturing = false;
        std::string m_errorString = "";

        std::thread m_checkConnectionThread;
        bool m_isRunningCheckConnectionThread = false;
        bool m_stopCheckConnectionThread = false;
        std::function<void()> m_disconnectionProcess = NULL;
    };
}


//*******************************

#endif
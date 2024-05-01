/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef DIRECTSHOW_CAMERA__CAMERA__CAMERA_H
#define DIRECTSHOW_CAMERA__CAMERA__CAMERA_H

//************Content************

// Include

#include "camera/camera_device.h"

#include "frame/frame.h"
#include "frame/frame_settings.h"

#include "camera/properties/camera_property_brightness.h"
#include "camera/properties/camera_property_contrast.h"
#include "camera/properties/camera_property_hue.h"
#include "camera/properties/camera_property_saturation.h"
#include "camera/properties/camera_property_sharpness.h"
#include "camera/properties/camera_property_gamma.h"
#include "camera/properties/camera_property_color_enable.h"
#include "camera/properties/camera_property_white_balance.h"
#include "camera/properties/camera_property_backlight_compensation.h"
#include "camera/properties/camera_property_gain.h"
#include "camera/properties/camera_property_pan.h"
#include "camera/properties/camera_property_tilt.h"
#include "camera/properties/camera_property_roll.h"
#include "camera/properties/camera_property_zoom.h"
#include "camera/properties/camera_property_exposure.h"
#include "camera/properties/camera_property_iris.h"
#include "camera/properties/camera_property_focus.h"
#include "camera/properties/camera_property_powerline_frequency.h"
#include "camera/properties/camera_property_digital_zoom_level.h"

#include "directshow_camera/camera/ds_camera.h"

#include <functional>
#include <optional>
#include <memory>
#include <vector>

// Include Opencv
#ifdef WITH_OPENCV2
#include <opencv2/opencv.hpp>
#endif

namespace DirectShowCamera
{
    /**
     * @brief Camera Class
     * 
     * - Only one Camera object can be created at a time. 
     * - Once created, GDI+ and COMLib will be initialized. If you have initialized GDI+ before creating Camera,
     *   add "#define DONT_INIT_GDIPLUS_IN_Camera" into your code to prevent re-initialization in the Camera.
     */
    class Camera
    {

    /**********************Public********************************/
    public:
#ifdef WITH_OPENCV2
        typedef std::function<void(cv::Mat image)> ExposureFusionAsyncResult;
#endif // def WITH_OPENCV2

#pragma region Constructor and Destructor

        /**
         * @brief Constructor
        */
        Camera();

        /**
         * @brief Constructor. Construct a Camera with the DirectShowCamera or DirectShowCameraStub.
         * @param[in] abstractDirectShowCamera DirectShowCamera or DirectShowCameraStub.
        */
        Camera(const std::shared_ptr<AbstractDirectShowCamera>& abstractDirectShowCamera);

        /**
         * @brief Constructor
        */
        void Constructor();

        /**
         * @brief Destructor
        */
        ~Camera();

#pragma endregion Constructor and Destructor

#pragma region Connection

        /**
         * @brief Open the first camera in the camera list. The support resolution can be retrieved from CameraDevice. If the frame width and height is not specified, the default resolution will be used.
         * @param[in] width (Optional) The frame width in pixel.
         * @param[in] height (Optional) The frame height in pixel.
         * @param[in] rgb (Optional) Set as true to capture RGB image. Otherwise it capture MonoChrome image. Default as true.
         * @return Return true if success.
        */
        bool Open(const int width = -1, const int height = -1, const bool rgb = true);

        /**
         * @brief Open camera with the specific resolution. The support resolution can be retrieved from CameraDevice. If the frame width and height is not specified, the default resolution will be used.
         * @param[in] device The camera to be opened. You can use getCameras() to extract the available cameras.
         * @param[in] width (Optional) The frame width in pixel.
         * @param[in] height (Optional) The frame height in pixel.
         * @param[in] rgb (Optional) Set as true to capture RGB image. Otherwise it capture MonoChrome image. Default as true.
         * @return Return true if success.
        */
        bool Open(const CameraDevice& device, const int width = -1, const int height = -1, const bool rgb = true);

        /**
         * @brief Open camera with the specific DirectShowVideoFormat.
         * @param[in] device The camera to be opened.
         * @param[in] videoFormat (Optional) Video format to be set. Default as nullopt which use the default video format.
         * @return Return true if success.
        */
        bool Open(
            const DirectShowCameraDevice& device,
            std::optional<const DirectShowVideoFormat> videoFormat = std::nullopt
        );

        /**
         * @brief Return true if the camera is opened.
         * @return Return true if the camera is opened.
        */
        bool isOpened() const;

        /**
         * @brief Close
         * @return Return true if success
        */
        bool Close();

        /**
         * @brief Return true if camera is disconnected.
         * @return Return true if camera is disconnected.
         */
        bool CheckDisconnection();

        /**
         * @brief Set the disconnection process. When the process was set, a thread will start to keep check the connection. If camera is disconnected, this process will run.
         * @param[in] func void()
         */
        void setDisconnectionProcess(std::function<void()> func);

#pragma endregion Connection

#pragma region Capture

        /**
         * @brief Start capture
         * @return Return true if success
        */
        bool StartCapture();

        /**
         * @brief Stop capture
         * @return Return true if success
        */
        bool StopCapture();

        /**
         * @brief Return true if camera is capturing.
         * @return Return true if camera is capturing.
        */
        bool isCapturing() const;

#pragma endregion Capture

#pragma region DirectShow Video Format

        // Todo: Move to protected and change the test object
        /**
         * @brief Get support DirectShowVideoFormat list. It is a advance option. Suggest to acquire the resolution from CameraDevice (Use getCameras() to get CameraDevice).
         * @return Return the support DirectShowVideoFormat list.
        */
        std::vector<DirectShowVideoFormat> getSupportDirectShowVideoFormats() const;

        /**
         * @brief Get the current DirectShowVideoFormat
         * @return Return the current DirectShowVideoFormat
        */
        DirectShowVideoFormat getDirectShowVideoFormat() const;

        // Todo: Fix changing video format problem when camera is opened
        /**
         * @brief Set DirectShowVideoFormat to the camera. Problam may caused when camera was opened. It is suggest to set DirectShowVideoFormat by open() fucntion.
         * @param[in] videoFormat DirectShowVideoFormat
         * @return Return true if success.
        */
        bool setDirectShowVideoFormat(const DirectShowVideoFormat videoFormat);

#pragma endregion DirectShow Video Format

#pragma region Frame

        /**
         * @brief Get frame
         * @param[out] frame Frame
         * @param[out] numOfBytes Number of bytes of the frame.
         * @param[in] onlyGetNewFrame (Optional) Set it as true if you only want to get the new frame which has not been get by getFrame. Default as false
         * @return Return true if success. If the frame is a old frame and onlyGetNewFrame is true, it will return false.
        */
        bool getFrame(Frame& frame, const bool onlyGetNewFrame = false);

        /**
         * @brief   Try to get a new Frame in sync mode. It will return the new frame if existed.
         *          Otherwise, it will wait for the new frame and then return. If timeout, it will return false
         * @param[out] frame Frame
         * @param[in] step (Optional) Step for checking new frame in ms. Default as 50ms
         * @param[in] timeout (Optional) Timeout in ms. Default as 3000ms
         * @param[in] skip (Optional) Number of new Frame to be skipped. For example, if skip = 3, the fourth new frame will be returned. Default as 0.
         * @return Return true if success. If timeout, it will return false.
        */
        bool getNewFrame(Frame& frame, const int step = 50, const int timeout = 3000, const int skip = 0);

        /**
         * @brief Return the last frame index. It use to identify whether a new frame. Index will only be updated when you call getFrame() or gatMat();
         * @return Return the last frame index.
        */
        long getLastFrameIndex() const;

        /**
         * @brief Get frame per second
         * @return
        */
        double getFPS() const;

        /**
         * @brief Get the frame width in pixel
         * @return Return the frame width
        */
        int getWidth() const;

        /**
         * @brief Get the frame height in pixel
         * @return Return the frame height
        */
        int getHeight() const;

        /**
         * @brief Get frame size in bytes
         * @return Return the the frame size in bytes
        */
        int getFrameSize() const;

        /**
         * @brief Get the number of bytes per pixel
         * @return Return the number of bytes per pixel
        */
        int getNumOfBytePerPixel() const;

        /**
         * @brief Get the number of pixel per frame.
         * @return Return the number of pixel per frame.
        */
        int getNumOfPixel() const;

        /**
        * @brief Get the frame settings
        * @return Return the frame settings by reference so that you can change the settings.
        */
        FrameSettings& getFrameSettings();

#ifdef WITH_OPENCV2

        /**
         * @brief Exposure fusion
         * @param[in] exposureFusionAsyncResult (Optional) void(cv::Mat) async processing the exposure fusion and return the result.  Default is nullptr. If this funciton is set, exposureFusion() wil return a empty cv::Mat.
         * @param[out] exposureImages (Optional) Images captured in different exposures.
         * @param[in] minSetExposureDelay (Optional) Minimum time delay in the unit of ms between setting exposures. For example, if the exposure time is 1s and minSetExposureDelay = 200, frame will capture between 1.2s. Default is 200ms
         * @return Return the fusion result. Empty will return if fail or exposureFusionAsyncResult was set.
        */
        cv::Mat ExposureFusion(
            const ExposureFusionAsyncResult exposureFusionAsyncResult = nullptr,
            std::vector<cv::Mat>* exposureImages = nullptr,
            const int minSetExposureDelay = 200
        );

        /**
         * @brief Exposure fusion. Reference: [Tom Mertens, Jan Kautz, and Frank Van Reeth. Exposure fusion. In Computer Graphics and Applications, 2007. PG'07. 15th Pacific Conference on, pages 382-390. IEEE, 2007.](https://mericam.github.io/exposure_fusion/index.html)
         * @param[in] exposures Exposures time to be captured
         * @param[in] exposureFusionAsyncResult (Optional) void(cv::Mat) async processing the exposure fusion and return the result.  Default is nullptr. If this funciton is set, exposureFusion() wil return a empty cv::Mat.
         * @param[out] exposureImages (Optional) Images captured in different exposures.
         * @param[in] minSetExposureDelay (Optional) Minimum time delay in the unit of ms between setting exposures. For example, if the exposure time is 1s and minSetExposureDelay = 200, frame will capture between 1.2s. Default is 200ms
         * @return Return the fusion result. Empty will return if fail or exposureFusionAsyncResult was set.
        */
        cv::Mat ExposureFusion(
            const std::vector<double> exposures,
            const ExposureFusionAsyncResult exposureFusionAsyncResult = nullptr,
            std::vector<cv::Mat>* exposureImages = nullptr,
            const int minSetExposureDelay = 200
        );
#endif // def WITH_OPENCV2

#pragma endregion Frame

#pragma region Camera

        // Todo: Move to protected and change the test object
        /**
         * @brief Get the available DirectShowCameraDevice list. It is a advance option. Suggest to use getCameras().
         * @return Return the available DirectShowCameraDevice list
        */
        std::vector<DirectShowCameraDevice> getDirectShowCameras();

        /**
         * @brief Get the available camera.
         * @return Return the available camera.
        */
        std::vector<CameraDevice> getCameras();

#pragma endregion Camera

#pragma region Properties

        /**
         * @brief Reset camera properties as default value.
         * @param[in] asAuto Set as true if you want to use auto mode as default value.
        */
        void ResetProperties(const bool asAuto = true);

        // Todo: Move to protected and change the test object
        /**
         * @brief Get directshow properties pointer. It is a advance option.
         * @return Return the directshow properties pointer
        */
        std::shared_ptr<DirectShowCameraProperties> getDirectShowProperties() const;

        void InitProperties();

        std::shared_ptr<CameraPropertyBrightness> Brightness();
        std::shared_ptr<CameraPropertyContrast> Contrast();
        std::shared_ptr<CameraPropertyHue> Hue();
        std::shared_ptr<CameraPropertySaturation> Saturation();
        std::shared_ptr<CameraPropertySharpness> Sharpness();
        std::shared_ptr<CameraPropertyGamma> Gamma();
        std::shared_ptr<CameraPropertyColorEnable> ColorEnable();
        std::shared_ptr<CameraPropertyWhiteBalance> WhiteBalance();
        std::shared_ptr<CameraPropertyBacklightCompensation> BacklightCompensation();
        std::shared_ptr<CameraPropertyGain> Gain();
        std::shared_ptr<CameraPropertyPan> Pan();
        std::shared_ptr<CameraPropertyTilt> Tilt();
        std::shared_ptr<CameraPropertyRoll> Roll();
        std::shared_ptr<CameraPropertyZoom> Zoom();
        std::shared_ptr<CameraPropertyExposure> Exposure();
        std::shared_ptr<CameraPropertyIris> Iris();
        std::shared_ptr<CameraPropertyFocus> Focus();
        std::shared_ptr<CameraPropertyPowerlineFrequency> PowerlineFrequency();
        std::shared_ptr<CameraPropertyDigitalZoomLevel> DigitalZoomLevel();

#pragma endregion Properties

    private:

        /**
         * @brief Open the camera
         * @param[in] directShowFilter Video filter
         * @param[in] videoFormat (Optional) Video format to be set. Default as nullopt.
         * @return Return true if success.
        */
        bool Open(
            IBaseFilter** directShowFilter,
            std::optional<const DirectShowVideoFormat> videoFormat = std::nullopt
        );

        /**
         * @brief Throw DirectShow exception if existed
        */
        void ThrowDirectShowException();

    private:
        std::shared_ptr<AbstractDirectShowCamera> m_directShowCamera;
        bool m_isInitialized = false;

        unsigned long m_lastFrameIndex = 0;

        std::shared_ptr<CameraPropertyBrightness> m_brightness;
        std::shared_ptr<CameraPropertyContrast> m_contrast;
        std::shared_ptr<CameraPropertyHue> m_hue;
        std::shared_ptr<CameraPropertySaturation> m_saturation;
        std::shared_ptr<CameraPropertySharpness> m_sharpness;
        std::shared_ptr<CameraPropertyGamma> m_gamma;
        std::shared_ptr<CameraPropertyColorEnable> m_color_enable;
        std::shared_ptr<CameraPropertyWhiteBalance> m_white_balance;
        std::shared_ptr<CameraPropertyBacklightCompensation> m_backlight_compensation;
        std::shared_ptr<CameraPropertyGain> m_gain;
        std::shared_ptr<CameraPropertyPan> m_pan;
        std::shared_ptr<CameraPropertyTilt> m_tilt;
        std::shared_ptr<CameraPropertyRoll> m_roll;
        std::shared_ptr<CameraPropertyZoom> m_zoom;
        std::shared_ptr<CameraPropertyExposure> m_exposure;
        std::shared_ptr<CameraPropertyIris> m_iris;
        std::shared_ptr<CameraPropertyFocus> m_focus;
        std::shared_ptr<CameraPropertyPowerlineFrequency> m_powerline_frequency;
        std::shared_ptr<CameraPropertyDigitalZoomLevel> m_digital_zoom_level;

        FrameSettings m_frameSettings;
    };
}

//*******************************

#endif //ndef DIRECTSHOW_CAMERA__CAMERA__directshow_camera_H
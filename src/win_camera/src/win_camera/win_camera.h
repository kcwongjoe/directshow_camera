/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__WIN_CAMERA_H
#define WIN_CAMERA__WIN_CAMERA__WIN_CAMERA_H

//************Content************

// Include

#include "win_camera/win_camera_device.h"

#include "win_camera/properties/win_camera_property_brightness.h"
#include "win_camera/properties/win_camera_property_contrast.h"
#include "win_camera/properties/win_camera_property_hue.h"
#include "win_camera/properties/win_camera_property_saturation.h"
#include "win_camera/properties/win_camera_property_sharpness.h"
#include "win_camera/properties/win_camera_property_gamma.h"
#include "win_camera/properties/win_camera_property_color_enable.h"
#include "win_camera/properties/win_camera_property_white_balance.h"
#include "win_camera/properties/win_camera_property_backlight_compensation.h"
#include "win_camera/properties/win_camera_property_gain.h"
#include "win_camera/properties/win_camera_property_pan.h"
#include "win_camera/properties/win_camera_property_tilt.h"
#include "win_camera/properties/win_camera_property_roll.h"
#include "win_camera/properties/win_camera_property_zoom.h"
#include "win_camera/properties/win_camera_property_exposure.h"
#include "win_camera/properties/win_camera_property_iris.h"
#include "win_camera/properties/win_camera_property_focus.h"
#include "win_camera/properties/win_camera_property_powerline_frequency.h"
#include "win_camera/properties/win_camera_property_digital_zoom_level.h"

#include "directshow_camera/camera/ds_camera.h"

#include <functional>
#include <optional>
#include <memory>
#include <vector>

// Include Opencv
#ifdef WITH_OPENCV2
#include "opencv_utils/cv_mat_convertor.h"
#endif

namespace WinCamera
{
    /**
     * @brief WinCamera Class
     * 
     */
    class WinCamera
    {

    /**********************Public********************************/
    public:
#ifdef WITH_OPENCV2
        typedef std::function<void(cv::Mat image)> ExposureFusionAsyncResult;
#endif

#pragma region Constructor and Destructor

        /**
         * @brief Constructor
        */
        WinCamera();

        /**
         * @brief Constructor. Construct a WinCamera with the DirectShowCamera or DirectShowCameraStub.
         * @param[in] abstractDirectShowCamera DirectShowCamera or DirectShowCameraStub.
        */
        WinCamera(const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& abstractDirectShowCamera);

        /**
         * @brief Constructor
        */
        void Constructor();

        /**
         * @brief Destructor
        */
        ~WinCamera();

#pragma endregion Constructor and Destructor

#pragma region Connection

        /**
         * @brief Open camera with the specific resolution. The support resolution can be retrieved from CameraDevice. If the frame width and height is not specified, the default resolution will be used.
         * @param[in] device The camera to be opened. You can use getCameras() to extract the available cameras.
         * @param[in] width (Optional) The frame width in pixel.
         * @param[in] height (Optional) The frame height in pixel.
         * @param[in] rgb (Optional) Set as true to capture RGB image. Otherwise it capture MonoChrome image. Default as true.
         * @return Return true if success.
        */
        bool Open(const WinCameraDevice& device, const int width = -1, const int height = -1, const bool rgb = true);

        /**
         * @brief Open camera with the specific DirectShowVideoFormat.
         * @param[in] device The camera to be opened.
         * @param[in] videoFormat (Optional) Video format to be set. Default as nullopt which use the default video format.
         * @return Return true if success.
        */
        bool Open(
            const DirectShowCamera::DirectShowCameraDevice& device,
            std::optional<const DirectShowCamera::DirectShowVideoFormat> videoFormat = std::nullopt
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
        std::vector<DirectShowCamera::DirectShowVideoFormat> getSupportDirectShowVideoFormats() const;

        /**
         * @brief Get the current DirectShowVideoFormat
         * @return Return the current DirectShowVideoFormat
        */
        DirectShowCamera::DirectShowVideoFormat getDirectShowVideoFormat() const;

        // Todo: Fix changing video format problem when camera is opened
        /**
         * @brief Set DirectShowVideoFormat to the camera. Problam may caused when camera was opened. It is suggest to set DirectShowVideoFormat by open() fucntion.
         * @param[in] videoFormat DirectShowVideoFormat
         * @return Return true if success.
        */
        bool setDirectShowVideoFormat(const DirectShowCamera::DirectShowVideoFormat videoFormat);

#pragma endregion DirectShow Video Format

#pragma region Frame

        /**
         * @brief Get frame
         * @param[out] frame Frame bytes (BGR)
         * @param[out] numOfBytes Number of bytes of the frame.
         * @param[in] onlyGetNewFrame (Option) Set it as true if you only want to get the new frame. Default as false
         * @return Return true if success
        */
        bool getFrame(unsigned char* frame, int& numOfBytes, const bool onlyGetNewFrame = false);

        /**
         * @brief Return the last frame index. It use to identify whether a new frame. Index will only be updated when you call getFrame() or gatMat();
         * @return Return the last frame index.
        */
        long getFrameIndex() const;

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

    #ifdef WITH_OPENCV2

        /**
         * @brief Set as true to return a vertical flip cv::Mat. Default as true.
         * @param[in] verticalFlip Set it as true to return a vertical flip cv::Mat.
        */
        void VerticalFlipMat(const bool verticalFlip);

        /**
         * @brief Set as true to return a BGR cv::Mat. Default as false which return a RGB Mat.
         * @param[in] asBGR Set as true to return a BGR cv::Mat.
        */
        void setMatAsBGR(const bool asBGR);

        /**
         * @brief Allocate frame buffer
         * @return Return true if success
        */
        bool AllocateMatBuffer();

        /**
         * @brief Get cv::Mat of the current frame
         *
         * @param[in] onlyGetNewMat (Option) Set it as true if you only want to get the new Mat. Default as false
         * @return Return cv::Mat
        */
        cv::Mat getMat(const bool onlyGetNewMat = false);

        /**
         * @brief Get cv::Mat from the Mat buffer. Buffer will not be updated if you have not to call getMat();
         * @return Return the cv::Mat
        */
        cv::Mat getLastMat();

        /**
         * @brief Get new cv::Mat in sync mode.
         * @param[in] step (Option) Step for checking new frame in ms. Default as 50ms
         * @param[in] timeout (Option) Timeout in ms. Default as 3000ms
         * @param[in] skip (Option) Number of new Mat to be skipped. For example, if skip = 3, the fourth new mat will be returned. Default as 0.
         * @return Return the cv::Mat. Empty mat return if timeout.
        */
        cv::Mat getNewMat(const int step = 50, const int timeout = 3000, const int skip = 0);

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
    #endif

#pragma endregion Frame

#pragma region Camera

        // Todo: Move to protected and change the test object
        /**
         * @brief Get the available DirectShowCameraDevice list. It is a advance option. Suggest to use getCameras().
         * @return Return the available DirectShowCameraDevice list
        */
        std::vector<DirectShowCamera::DirectShowCameraDevice> getDirectShowCameras();

        /**
         * @brief Get the available camera.
         * @return Return the available camera.
        */
        std::vector<WinCameraDevice> getCameras();

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
        std::shared_ptr<DirectShowCamera::DirectShowCameraProperties> getDirectShowProperties() const;

        void InitProperties();

        std::shared_ptr<WinCameraPropertyBrightness> Brightness();
        std::shared_ptr<WinCameraPropertyContrast> Contrast();
        std::shared_ptr<WinCameraPropertyHue> Hue();
        std::shared_ptr<WinCameraPropertySaturation> Saturation();
        std::shared_ptr<WinCameraPropertySharpness> Sharpness();
        std::shared_ptr<WinCameraPropertyGamma> Gamma();
        std::shared_ptr<WinCameraPropertyColorEnable> ColorEnable();
        std::shared_ptr<WinCameraPropertyWhiteBalance> WhiteBalance();
        std::shared_ptr<WinCameraPropertyBacklightCompensation> BacklightCompensation();
        std::shared_ptr<WinCameraPropertyGain> Gain();
        std::shared_ptr<WinCameraPropertyPan> Pan();
        std::shared_ptr<WinCameraPropertyTilt> Tilt();
        std::shared_ptr<WinCameraPropertyRoll> Roll();
        std::shared_ptr<WinCameraPropertyZoom> Zoom();
        std::shared_ptr<WinCameraPropertyExposure> Exposure();
        std::shared_ptr<WinCameraPropertyIris> Iris();
        std::shared_ptr<WinCameraPropertyFocus> Focus();
        std::shared_ptr<WinCameraPropertyPowerlineFrequency> PowerlineFrequency();
        std::shared_ptr<WinCameraPropertyDigitalZoomLevel> DigitalZoomLevel();

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
            std::optional<const DirectShowCamera::DirectShowVideoFormat> videoFormat = std::nullopt
        );

        /**
         * @brief Throw DirectShow exception if existed
        */
        void ThrowDirectShowException();

    private:
        std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera> m_directShowCamera;
        bool m_isInitialized = false;

        unsigned long m_lastFrameIndex = 0;

        std::shared_ptr<WinCameraPropertyBrightness> m_brightness;
        std::shared_ptr<WinCameraPropertyContrast> m_contrast;
        std::shared_ptr<WinCameraPropertyHue> m_hue;
        std::shared_ptr<WinCameraPropertySaturation> m_saturation;
        std::shared_ptr<WinCameraPropertySharpness> m_sharpness;
        std::shared_ptr<WinCameraPropertyGamma> m_gamma;
        std::shared_ptr<WinCameraPropertyColorEnable> m_color_enable;
        std::shared_ptr<WinCameraPropertyWhiteBalance> m_white_balance;
        std::shared_ptr<WinCameraPropertyBacklightCompensation> m_backlight_compensation;
        std::shared_ptr<WinCameraPropertyGain> m_gain;
        std::shared_ptr<WinCameraPropertyPan> m_pan;
        std::shared_ptr<WinCameraPropertyTilt> m_tilt;
        std::shared_ptr<WinCameraPropertyRoll> m_roll;
        std::shared_ptr<WinCameraPropertyZoom> m_zoom;
        std::shared_ptr<WinCameraPropertyExposure> m_exposure;
        std::shared_ptr<WinCameraPropertyIris> m_iris;
        std::shared_ptr<WinCameraPropertyFocus> m_focus;
        std::shared_ptr<WinCameraPropertyPowerlineFrequency> m_powerline_frequency;
        std::shared_ptr<WinCameraPropertyDigitalZoomLevel> m_digital_zoom_level;

    #ifdef WITH_OPENCV2
        unsigned char* m_matBuffer = NULL;
        int m_matBufferSize = 0;
        OpenCVMatConverter m_matConvertor;
    #endif
    };
}

//*******************************

#endif
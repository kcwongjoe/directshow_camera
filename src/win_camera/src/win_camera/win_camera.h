#pragma once
#ifndef WIN_CAMERA_H
#define WIN_CAMERA_H

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

#include "directshow_camera/ds_camera_utils.h"
#include "directshow_camera/ds_camera.h"
#include "directshow_camera/abstract_ds_camera.h"
#include "directshow_camera/ds_libs_setting.h"

#include "exceptions/camera_not_opened_exception.h"
#include "exceptions/property_not_support_exception.h"



// Include Opencv
#ifdef HAS_OPENCV
#include "opencv_utils/cv_mat_convertor.h"
#endif

#include <string>
#include <math.h>
#include <vector>

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
#ifdef HAS_OPENCV
        typedef std::function<void(cv::Mat image)> ExposureFusionAsyncResult;
#endif

        //-----------------Constructor-----------------

        /**
         * @brief Constructor
        */
        WinCamera();

        /**
         * @brief Constructor
         * @param abstractDirectShowCamera DirectShowCamera or DirectShowCameraStub.
        */
        WinCamera(const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera>& abstractDirectShowCamera);

        /**
         * @brief Constructor
         * @param abstractDirectShowCamera Abstract DirectShow Camera
        */
        void constructor();

        /**
         * @brief Destructor
        */
        ~WinCamera();

        //-----------------Connection-----------------

        /**
         * @brief Open camera with the specific resolution. The support resolution can be retrieved from CameraDevice. If the frame width and height is not specified, the default resolution will be used.
         * @param device The camera to be opened. You can use getCameras() to extract the available cameras.
         * @param width (Option) The frame width in pixel.
         * @param height (Option) The frame height in pixel.
         * @param rgb (Option) Set as true to capture RGB image. Otherwise it capture MonoChrome image. Default as true.
         * @return Return true if success.
        */
        bool open(const WinCameraDevice& device, const int width = -1, const int height = -1, const bool rgb = true);

        /**
         * @brief Open camera with the specific DirectShowVideoFormat.
         * @param device The camera to be opened.
         * @param videoFormat (Option) Video format. Default as NULL which use the default video format.
         * @return Return true if success.
        */
        bool open(const DirectShowCamera::DirectShowCameraDevice& device, DirectShowCamera::DirectShowVideoFormat* videoFormat = NULL);

        /**
         * @brief Return true if the camera is opened.
         * @return Return true if the camera is opened.
        */
        bool isOpened() const;

        /**
         * @brief Close
        */
        bool close();

        /**
         * @brief Return true if camera is disconnected.
         * @return Return true if camera is disconnected.
         */
        bool checkDisconnection();

        /**
         * @brief Set the disconnection process. When the process was set, a thread will start to keep check the connection. If camera is disconnected, this process will run.
         * @param func void()
         */
        void setDisconnectionProcess(std::function<void()> func);

        //-----------------Capture-----------------

        /**
         * @brief Start capture
         * @return Return true if success
        */
        bool startCapture();

        /**
         * @brief Stop capture
         * @return Return true if success
        */
        bool stopCapture();

        /**
         * @brief Return true if camera is capturing.
         * @return Return true if camera is capturing.
        */
        bool isCapturing() const;

        /**
         * @brief Get the last error message.
         * @return Return the last error message.
        */
        std::string getLastError() const;

        // ------DirectShow Video Format------

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

        /**
         * @brief Set DirectShowVideoFormat to the camera. Problam may caused when camera was opened. It is suggest to set DirectShowVideoFormat by open() fucntion.
         * @param videoFormat DirectShowVideoFormat
         * @return Return true if success.
        */
        bool setDirectShowVideoFormat(DirectShowCamera::DirectShowVideoFormat* videoFormat);

        // ------Frame------

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

    #ifdef HAS_OPENCV

        /**
         * @brief Set as true to return a vertical flip cv::Mat. Default as true.
         * @param verticalFlip Set it as true to return a vertical flip cv::Mat.
        */
        void vecticalFlipMat(const bool verticalFlip);

        /**
         * @brief Set as true to return a BGR cv::Mat. Default as false which return a RGB Mat.
         * @param asBGR Set as true to return a BGR cv::Mat.
        */
        void setMatAsBGR(const bool asBGR);

        /**
         * @brief Allocate frame buffer
         * @return Return true if success
        */
        bool allocateMatBuffer();

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
         * @param[out] exposureImages Images captured in different exposures.
         * @param[in] exposureFusionAsyncResult void(cv::Mat) async processing the exposure fusion and return the result.  Default is NULL. If this funciton is set, exposureFusion() wil return a empty cv::Mat.
         * @param[in] minSetExposureDelay Minimum time delay in between setting exposures.
         * @return Return the fusion result. Empty will return if fail or exposureFusionAsyncResult was set.
        */
        cv::Mat exposureFusion(
            const ExposureFusionAsyncResult exposureFusionAsyncResult = nullptr,
            std::vector<cv::Mat>* exposureImages = NULL,
            const int minSetExposureDelay = 200
        );

        /**
         * @brief Exposure fusion
         * @param[in] exposures Exposures time to be captured
         * @param[out] exposureImages Images captured in different exposures.
         * @param[in] exposureFusionAsyncResult void(cv::Mat) async processing the exposure fusion and return the result.  Default is NULL. If this funciton is set, exposureFusion() wil return a empty cv::Mat.
         * @param[in] minSetExposureDelay Minimum time delay in between setting exposures.
         * @return Return the fusion result. Empty will return if fail or exposureFusionAsyncResult was set.
        */
        cv::Mat exposureFusion(
            const std::vector<double> exposures,
            const ExposureFusionAsyncResult exposureFusionAsyncResult = nullptr,
            std::vector<cv::Mat>* exposureImages = NULL,
            const int minSetExposureDelay = 200
        );
    #endif

        // ------Camera------

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

        //------Properties------

        /**
         * @brief Reset camera properties as default value.
         * @param asAuto Set as true if you want to use auto mode as default value.
        */
        void resetProperties(const bool asAuto = true);

        /**
         * @brief Get directshow properties pointer. It is a advance option.
         * @return Return the directshow properties pointer
        */
        std::shared_ptr<DirectShowCamera::DirectShowCameraProperties> getDirectShowProperties() const;

        void initProperties();

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

    /**********************Private********************************/
    private:
        std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera> m_directShowCamera;
        bool m_isInitialized = false;

        unsigned long m_lastFrameIndex = 0;
        std::string m_errorString;

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

    #ifdef HAS_OPENCV
        unsigned char* m_matBuffer = NULL;
        int m_matBufferSize = 0;
        OpenCVMatConverter m_matConvertor;
    #endif

        /**
         * @brief Open the camera
         * @param videoInputFilter Video filter
         * @param videoFormat Video format
         * @return Return true if success.
        */
        bool open(IBaseFilter** videoInputFilter, DirectShowCamera::DirectShowVideoFormat* videoFormat = NULL);

        // Utils
        void copyError(bool success);
    };
}

//*******************************

#endif
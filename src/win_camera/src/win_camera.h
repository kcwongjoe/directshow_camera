#pragma once
#ifndef WIN_CAMERA_H
#define WIN_CAMERA_H

//************Content************

// Include
#include "directshow_camera/ds_camera_utils.h"
#include "win_camera_device.h"
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

        // ---Brightness---

        /**
         * @brief Retrun true if property brightness is supported.
         * @return Retrun true if property brightness is supported.
        */
        bool supportBrightness() const;

        /**
         * @brief Get the range of the property - brightness
         * @return Return (min,max). (0,0) return if error occurred.
        */
        std::pair<long, long> getBrightnessRange() const;

        /**
         * @brief Get the step of the property - Brightness.
         * @return Return the step of the Brightness. -1 return if error occurred.
        */
        long getBrightnessStep() const;

        /**
         * @brief Get current brightness, from blanking(small value) to pure white(large value)
         * @return Return current brightness. -1 return if error occurred.
        */
        long getBrightness() const;

        /**
         * @brief Set brightness
         * @param value Value to be set
         * @return Return true if success.
        */
        bool setBrightness(const long value);

        // Contrast
        bool supportContrast() const;
        std::pair<long, long> getContrastRange() const;
        long getContrastStep() const;
        long getContrast() const;
        bool setContrast(const long value);

        // Hue
        bool supportHue() const;
        std::pair<long, long> getHueRange() const;
        long getHueStep() const;
        long getHue() const;
        bool setHue(const long degree);

        // Saturation
        bool supportSaturation() const;
        std::pair<long, long> getSaturationRange() const;
        long getSaturationStep() const;
        long getSaturation() const;
        bool setSaturation(const long value);

        // Sharpness
        bool supportSharpness() const;
        std::pair<long, long> getSharpnessRange() const;
        long getSharpnessStep() const;
        long getSharpness() const;
        bool setSharpness(const long value);

        // Gamma
        bool supportGamma() const;
        std::pair<long, long> getGammaRange() const;
        long getGammaStep() const;
        long getGamma() const;
        bool setGamma(const long value);

        // Color Enable
        bool supportColorEnable() const;
        bool isColorEnable() const;
        bool setColorEnable(const bool isOn);

        // WhiteBalance
        bool supportWhiteBalance() const;
        std::pair<long, long> getWhiteBalanceRange() const;
        long getWhiteBalanceStep() const;
        long getWhiteBalance() const;
        bool setWhiteBalance(const long kelvin);
        bool isAutoWhiteBalance() const;
        bool setAutoWhiteBalance(const bool setToAuto);

        // Backlight Compensation
        bool supportBacklightCompensation() const;
        bool isBacklightCompensation() const;
        bool setBacklightCompensation(const bool isOn);

        // Gain
        bool supportGain() const;
        std::pair<long, long> getGainRange() const;
        long getGainStep() const;
        long getGain() const;
        bool setGain(const long value);

        // Pan
        bool supportPan() const;
        std::pair<long, long> getPanRange() const;
        long getPanStep() const;
        long getPan() const;
        bool setPan(const long value);

        // Tilt
        bool supportTilt() const;
        std::pair<long, long> getTiltRange() const;
        long getTiltStep() const;
        long getTilt() const;
        bool setTilt(const long degree);

        // Roll
        bool supportRoll() const;
        std::pair<long, long> getRollRange() const;
        long getRollStep() const;
        long getRoll() const;
        bool setRoll(const long value);

        // Zoom
        bool supportZoom() const;
        std::pair<long, long> getZoomRange() const;
        long getZoomStep() const;
        long getZoom() const;
        bool setZoom(const long millimeter);

        // Exposure
        bool supportExposure() const;
        std::pair<double, double> getExposureRange() const;
        double getExposure() const;
        bool setExposure(const double value);
        std::vector<double> getPossibleExposureValues() const;
        int getExposureIndex() const;
        bool isAutoExposure() const;
        bool setAutoExposure(const bool setToAuto);

        // Iris
        bool supportIris() const;
        std::pair<long, long> getIrisRange() const;
        long getIrisStep() const;
        long getIris() const;
        bool setIris(const long value);
        bool isAutoIris() const;
        bool setAutoIris(const bool setToAuto);

        // Focus
        bool supportFocus() const;
        std::pair<long, long> getFocusRange() const;
        long getFocusStep() const;
        long getFocus() const;
        bool setFocus(const long value);
        bool isAutoFocus() const;
        bool setAutoFocus(const bool setToAuto);

    /**********************Private********************************/
    private:
        std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera> m_directShowCamera;

        unsigned long m_lastFrameIndex = 0;
        std::string m_errorString;

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
        int confirmDegreeRange(int degree);
        double exposureConvertion(const long dsValue) const;
        long exposureConvertion(const double second) const;

        /**
         * @brief A template to return property whether support.
         * @tparam Func bool()
         * @param func[in] Lambda function to return the property whether support.
         * @param errorDescription[in] Error description, e.g. "Error on supportExposure()"
         * @return Return true if property support.
        */
        bool supportPropertyTemplate(
            const DirectShowCamera::DirectShowCameraProperty& property
        ) const
        {
            // Throw if camera is not opened
            if (!isOpened()) throw CameraNotOpenedException();

            // Get property support
            return property.isSupported();
        }

        /**
         * @brief A template for getPropertyRange()
         * @tparam Func bool()
         * @param func[in] Lambda function to return the property or property range or state. Note that ds_error will not be copied if return false, so do copyError() in the lambda function.
         * @param property[in] property
         * @param propertyName[in] (Option) Property name for error description. Default as "".
         * @return Return false if error occurred.
        */
        template <typename Func> bool getPropertyTemplate(
            const Func func,
            const DirectShowCamera::DirectShowCameraProperty& property,
            const std::string propertyName= ""
        ) const
        {
            // Throw if camera is not opened
            if (!isOpened()) throw CameraNotOpenedException();

            // Throw if property is not support
            if (!property.isSupported())
            {
                if (propertyName.empty())
                {
                    throw PropertyNotSupportException(property.getName());
                }
                else
                {
                    throw PropertyNotSupportException(propertyName);
                }

            }

            // Get property
            func();

            return true;
        }
    };
}

//*******************************

#endif
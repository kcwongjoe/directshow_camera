#pragma once
#ifndef UVC_CAMERA_H
#define UVC_CAMERA_H

//************Content************

// Include
#include <ds_camera_utils.h>
#include <camera_device.h>
#include <ds_camera.h>
#include<abstract_ds_camera.h>
#include <ds_libs_setting.h>

// Include Opencv
#ifdef HAS_OPENCV
#include <cv_mat_convertor.h>
#endif

#include <string>
#include <math.h>
#include <vector>

/**
 * @example eg1_capture.cpp
 * @example eg2_properties.cpp
 * @example eg3_camera_looper.cpp
 * @example eg4_exposure_fusion.cpp
 * @example eg5_disconnect_process.cpp
 * @example eg6_stub.cpp
 */
namespace DirectShowCamera
{
    /**
     * @brief UVC Camera Class
     * 
     */
    class UVCCamera
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
        UVCCamera();

        /**
         * @brief Constructor
         * @param abstractDirectShowCamera DirectShowCamera or DirectShowCameraStub.
        */
        UVCCamera(const std::shared_ptr<AbstractDirectShowCamera>& abstractDirectShowCamera);

        /**
         * @brief Constructor
         * @param abstractDirectShowCamera Abstract DirectShow Camera
        */
        void constructor();

        /**
         * @brief Destructor
        */
        ~UVCCamera();

        //-----------------Connection-----------------

        /**
         * @brief Open camera with the specific resolution. The support resolution can be retrieved from CameraDevice. If the frame width and height is not specified, the default resolution will be used.
         * @param device The camera to be opened. You can use getCameras() to extract the available cameras.
         * @param width (Option) The frame width in pixel.
         * @param height (Option) The frame height in pixel.
         * @param rgb (Option) Set as true to capture RGB image. Otherwise it capture MonoChrome image. Default as true.
         * @return Return true if success.
        */
        bool open(const CameraDevice& device, const int width = -1, const int height = -1, const bool rgb = true);

        /**
         * @brief Open camera with the specific DirectShowVideoFormat.
         * @param device The camera to be opened.
         * @param videoFormat (Option) Video format. Default as NULL which use the default video format.
         * @return Return true if success.
        */
        bool open(const DirectShowCameraDevice& device, DirectShowVideoFormat* videoFormat = NULL);

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
        std::vector<DirectShowVideoFormat> getSupportDirectShowVideoFormats() const;

        /**
         * @brief Get the current DirectShowVideoFormat
         * @return Return the current DirectShowVideoFormat
        */
        DirectShowVideoFormat getDirectShowVideoFormat() const;

        /**
         * @brief Set DirectShowVideoFormat to the camera. Problam may caused when camera was opened. It is suggest to set DirectShowVideoFormat by open() fucntion.
         * @param videoFormat DirectShowVideoFormat
         * @return Return true if success.
        */
        bool setDirectShowVideoFormat(DirectShowVideoFormat* videoFormat);

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
        std::vector<DirectShowCameraDevice> getDirectShowCameras();

        /**
         * @brief Get the available camera.
         * @return Return the available camera.
        */
        std::vector<CameraDevice> getCameras();

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
        DirectShowCameraProperties* getDirectShowProperties() const;

        // Brightness
        bool supportBrightness();
        std::pair<long, long> getBrightnessRange();
        long getBrightnessStep();
        long getBrightness();
        bool setBrightness(long value);

        // Contrast
        bool supportContrast();
        std::pair<long, long> getContrastRange();
        long getContrastStep();
        long getContrast();
        bool setContrast(long value);

        // Hue
        bool supportHue();
        std::pair<long, long> getHueRange();
        long getHueStep();
        long getHue();
        bool setHue(long degree);

        // Saturation
        bool supportSaturation();
        std::pair<long, long> getSaturationRange();
        long getSaturationStep();
        long getSaturation();
        bool setSaturation(long value);

        // Sharpness
        bool supportSharpness();
        std::pair<long, long> getSharpnessRange();
        long getSharpnessStep();
        long getSharpness();
        bool setSharpness(long value);

        // Gamma
        bool supportGamma();
        std::pair<long, long> getGammaRange();
        long getGammaStep();
        long getGamma();
        bool setGamma(long value);

        // Color Enable
        bool supportColorEnable();
        bool isColorEnable();
        bool setColorEnable(bool isOn);

        // WhiteBalance
        bool supportWhiteBalance();
        std::pair<long, long> getWhiteBalanceRange();
        long getWhiteBalanceStep();
        long getWhiteBalance();
        bool setWhiteBalance(long kelvin);
        bool isAutoWhiteBalance();
        bool setAutoWhiteBalance(bool setToAuto);

        // Backlight Compensation
        bool supportBacklightCompensation();
        bool isBacklightCompensation();
        bool setBacklightCompensation(bool isOn);

        // Gain
        bool supportGain();
        std::pair<long, long> getGainRange();
        long getGainStep();
        long getGain();
        bool setGain(long value);

        // Pan
        bool supportPan();
        std::pair<long, long> getPanRange();
        long getPanStep();
        long getPan();
        bool setPan(long value);

        // Tilt
        bool supportTilt();
        std::pair<long, long> getTiltRange();
        long getTiltStep();
        long getTilt();
        bool setTilt(long degree);

        // Roll
        bool supportRoll();
        std::pair<long, long> getRollRange();
        long getRollStep();
        long getRoll();
        bool setRoll(long value);

        // Zoom
        bool supportZoom();
        std::pair<long, long> getZoomRange();
        long getZoomStep();
        long getZoom();
        bool setZoom(long millimeter);

        // Exposure
        bool supportExposure();
        std::pair<double, double> getExposureRange();
        double getExposure();
        bool setExposure(double value);
        std::vector<double> getPossibleExposureValues();
        int getExposureIndex();
        bool isAutoExposure();
        bool setAutoExposure(bool setToAuto);

        // Iris
        bool supportIris();
        std::pair<long, long> getIrisRange();
        long getIrisStep();
        long getIris();
        bool setIris(long value);
        bool isAutoIris();
        bool setAutoIris(bool setToAuto);

        // Focus
        bool supportFocus();
        std::pair<long, long> getFocusRange();
        long getFocusStep();
        long getFocus();
        bool setFocus(long value);
        bool isAutoFocus();
        bool setAutoFocus(bool setToAuto);

    /**********************Private********************************/
    private:
        std::shared_ptr<AbstractDirectShowCamera> m_directShowCamera;

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
        bool open(IBaseFilter** videoInputFilter, DirectShowVideoFormat* videoFormat = NULL);

        // Utils
        void copyError(bool success);
        int confirmDegreeRange(int degree);
        double exposureConvertion(long dsValue);
        long exposureConvertion(double second);

        /**
         * @brief A template to return property whether support.
         * @tparam Func bool()
         * @param func Lambda function to return the property whether support.
         * @param errorDescription Error description, e.g. "Error on supportExposure()"
         * @return Return true if property support.
        */
        template <typename Func> bool supportPropertyTemplate(Func func, std::string errorDescription)
        {
            bool result = checkOpenDecorator(
                [func]()
                {
                    return func();				
                },
                errorDescription
            );

            return result;
        }

        /**
         * @brief A template for getPropertyRange()
         * @tparam Func bool()
         * @param func Lambda function to return the property or property range or state. Note that ds_error will not be copied if return false, so do copyError() in the lambda function.
         * @param supportProperty Support property?
         * @param errorDescription Error description, e.g. "Error on getExposure()"
         * @param notSupportErrorString Not support error string, e.g. "Exposure is not support."
         * @return Return (min, max). (0,0) return if error occurred.
        */
        template <typename Func> bool handlePropertyTemplate(Func func, bool supportProperty, std::string errorDescription, std::string notSupportErrorString)
        {
            bool success = checkOpenDecorator(
                [this, func, supportProperty, notSupportErrorString]()
                {
                    if (supportProperty)
                    {
                        func();
                        return true;					
                    }
                    else
                    {
                        m_errorString = notSupportErrorString;
                        return false;
                    }
                },
                errorDescription
            );

            return success;
        }

        /**
         * @brief A decorator to check whether opening
         * @tparam Func bool()
         * @param func Lambda function to be processed if opened. Note that ds_error will not be copied if return false, so do copyError() in the lambda function.
         * @param errorDescription Error description, e.g. "Error on getExposure()"
         * @return Return true if success
        */
        template <typename Func> bool checkOpenDecorator(Func func, std::string errorDescription = "")
        {
            if (isOpened())
            {
                bool success = func();
                return success;
            }
            else
            {
                if (!errorDescription.empty())
                {
                    m_errorString = errorDescription + " : ";
                }
                else
                {
                    m_errorString = "";
                }
                m_errorString += "Camera is not opened.";
                return false;
            }
        }


    };
}

//*******************************

#endif
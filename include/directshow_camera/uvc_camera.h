#pragma once
#ifndef UVC_CAMERA_H
#define UVC_CAMERA_H

//************Content************

// Include
#include <ds_camera_utils.h>
#include <camera_device.h>
#include <ds_camera.h>
#include <ds_libs_setting.h>

// Include Opencv
#ifdef HAS_OPENCV
#include <cv_mat_convertor.h>
#endif

#include <string>
#include <math.h>
#include <vector>

namespace DirectShowCamera
{
	class UVCCamera
	{

	/**********************Public********************************/
	public:

		typedef std::function<void(cv::Mat image)> ExposureFusionAsyncResult;

		// Constructor
		UVCCamera();

		~UVCCamera();

		// Connection
		bool open(CameraDevice device, int width = -1, int height = -1, bool rgb = true);
		bool open(DirectShowCameraDevice device, DirectShowVideoFormat* videoFormat = NULL);
		bool isOpened();
		bool close();
		bool checkDisconnection();
		void setDisconnectionProcess(std::function<void()> func);

		// Capture
		bool startCapture();
		bool stopCapture();
		bool isCapturing();

		std::string getLastError();

		// ------DirectShow Video Format------
		std::vector<DirectShowVideoFormat> getSupportDirectShowVideoFormats();
		DirectShowVideoFormat getDirectShowVideoFormat();

		bool setDirectShowVideoFormat(DirectShowVideoFormat* videoFormat);

		// ------Frame------

		bool onlyGetNewFrame = true;
		bool getFrame(unsigned char* frame, int* numOfBytes = NULL);
		long getFrameIndex();

		double getFPS();
		int getWidth();
		int getHeight();
		int getFrameSize();
		int getNumOfBytePerPixel();
		int getNumOfPixel();

	#ifdef HAS_OPENCV
		void vecticalFlipMat(bool verticalFlip);
		void setMatAsBGR(bool asBGR);
		bool allocateMatBuffer();
		cv::Mat getMat();
		cv::Mat getLastMat();
		cv::Mat getNewMat(int step = 50, int timeout = 3000, int skip = 0);

		cv::Mat exposureFusion(ExposureFusionAsyncResult exposureFusionAsyncResult = NULL,std::vector<cv::Mat>* exposureImages = NULL, int minSetExposureDelay = 200);
		cv::Mat exposureFusion(std::vector<double> exposures, ExposureFusionAsyncResult exposureFusionAsyncResult = NULL, std::vector<cv::Mat>* exposureImages = NULL, int minSetExposureDelay = 200);
	#endif

		// ------Camera------

		std::vector<DirectShowCameraDevice> getDirectShowCameras();
		std::vector<CameraDevice> getCameras();

		//------Properties------

		void resetProperties(bool asAuto = true);
		DirectShowCameraProperties* getDirectShowProperties();

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
		DirectShowCamera* m_directShowCamera = NULL;

		unsigned long m_lastFrameIndex = 0;
		std::string m_errorString;

	#ifdef HAS_OPENCV
		unsigned char* m_matBuffer = NULL;
		int m_matBufferSize = 0;
		OpenCVMatConvter m_matConvertor;
	#endif
		bool open(IBaseFilter** videoInputFilter, DirectShowVideoFormat* videoFormat = NULL);

		// Utils
		void copyError(bool success);
		int confirmDegreeRange(int degree);
		double UVCCamera::exposureConvertion(long dsValue);
		long UVCCamera::exposureConvertion(double second);

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
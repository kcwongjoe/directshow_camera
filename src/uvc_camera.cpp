#include <uvc_camera.h>

namespace DirectShowCamera
{

#pragma region Constructor and Destructor

	/**
	 * @brief Constructor
	*/
	UVCCamera::UVCCamera()
	{
		DirectShowCameraUtils::initCOMLib();

		m_directShowCamera = new DirectShowCamera();

	#ifdef HAS_OPENCV
		m_matConvertor = OpenCVMatConverter();
		m_matConvertor.isBGR = false;
		m_matConvertor.isVerticalFlip = true;
	#endif
	}

	/**
	 * @brief Destructor
	*/
	UVCCamera::~UVCCamera()
	{
		close();

		if (this && m_directShowCamera)
		{
			m_directShowCamera->release();
			delete m_directShowCamera;
		}
		
		// Uninitialize COM Library
		DirectShowCameraUtils::uninitCOMLib();
	}

#pragma endregion Constructor and Destructor

#pragma region Connection

	/**
	 * @brief Open camera with the specific resolution. The support resolution can be retrieved from CameraDevice. If the frame width and height is not specified, the default resolution will be used.
	 * @param device The camera to be opened. You can use getCameras() to extract the available cameras.
	 * @param width (Option) The frame width in pixel. 
	 * @param height (Option) The frame height in pixel.
	 * @param rgb (Option) Set as true to capture RGB image. Otherwise it capture MonoChrome image. Default as true.
	 * @return Return true if success.
	*/
	bool UVCCamera::open(CameraDevice device, int width, int height, bool rgb)
	{
		bool result = false;

		// Get DirectShowCameraDevice
		std::vector<DirectShowCameraDevice> possibleCamera = getDirectShowCameras();
		int cameraIndex = -1;
		for (int i = 0; i < possibleCamera.size(); i++)
		{
			if (possibleCamera[i].getDevicePath() == device.getDevicePath())
			{
				// Found
				cameraIndex = i;
				result = true;
				break;
			}
		}

		// Get DirectShowVideoFormat
		if (result)
		{
			if (width <= 0 || height <= 0)
			{
				// No specific format
				result = open(possibleCamera[cameraIndex], NULL);
			}
			else
			{
				// Lookup format
				std::vector<DirectShowVideoFormat> videoFormats = possibleCamera[cameraIndex].getDirectShowVideoFormats();

				// Lookup size
				std::vector<DirectShowVideoFormat> possibleVideoFormat;
				for (int i = 0; i < videoFormats.size(); i++)
				{
					if (videoFormats[i].getWidth() == width && videoFormats[i].getHeight() == height)
					{
						possibleVideoFormat.push_back(videoFormats[i]);
					}
				}

				if (possibleVideoFormat.size() > 0)
				{
					// Get media type list
					int videoFormatIndex = -1;
					std::vector<GUID> mediaType;
					if (rgb)
					{
						// RGB
						mediaType = DirectShowVideoFormat::getSupportRGBSubType();
					}
					else
					{
						// MonoChrome
						mediaType = DirectShowVideoFormat::getMonochromeSubType();
					}

					// Match with list
					for (int i = 0; i < mediaType.size(); i++)
					{
						for (int j = 0; j < possibleVideoFormat.size(); j++)
						{
							if (possibleVideoFormat[j].getVideoType() == mediaType[i])
							{
								// Found
								videoFormatIndex = j;
								break;
							}
						}

						// Exit if found
						if (videoFormatIndex >= 0) break;
					}

					// If success
					if (videoFormatIndex >= 0)
					{
						// Open
						result = open(possibleCamera[cameraIndex], &possibleVideoFormat[videoFormatIndex]);
					}
					else
					{
						// Not Found the size
						result = false;
						if (rgb)
						{
							m_errorString == "Camera" + device.getFriendlyName() + " does not supported size (" + std::to_string(width) + " x " + std::to_string(height) + " x 3).";
						}
						else
						{
							m_errorString == "Camera" + device.getFriendlyName() + " does not supported size (" + std::to_string(width) + " x " + std::to_string(height) + " x 1).";
						}

					}
				}
				else
				{
					// Not Found the size
					result = false;
					m_errorString == "Camera" + device.getFriendlyName() + " does not supported size (" + std::to_string(width) + " x " + std::to_string(height) + ").";
				}
			}
		}

		return result;
	}

	/**
	 * @brief Open camera with the specific DirectShowVideoFormat.
	 * @param device The camera to be opened.
	 * @param videoFormat (Option) Video format. Default as NULL which use the default video format.
	 * @return Return true if success.
	*/
	bool UVCCamera::open(DirectShowCameraDevice device, DirectShowVideoFormat* videoFormat)
	{
		bool result = false;

		// Get device input filter
		IBaseFilter* videoInputFilter;
		result = m_directShowCamera->getCamera(device, &videoInputFilter);

		// Open
		if (result)
		{
			result = open(&videoInputFilter, videoFormat);
		}
		else
		{
			// Release device input filter
			videoInputFilter->Release();
			delete videoInputFilter;

			copyError(result);
		}

		return result;
	}

	/**
	 * @brief Open the camera
	 * @param videoInputFilter Video filter
	 * @param videoFormat Video format
	 * @return Return true if success.
	*/
	bool UVCCamera::open(IBaseFilter** videoInputFilter, DirectShowVideoFormat* videoFormat)
	{
		bool result = false;

		// Initialize camera
		result = m_directShowCamera->open(videoInputFilter, videoFormat);

	#ifdef HAS_OPENCV
		if (result)
		{
			allocateMatBuffer();
		}
	#endif

		copyError(result);

		return result;
	}

	/**
	 * @brief Return true if the camera is opened.
	 * @return Return true if the camera is opened.
	*/
	bool UVCCamera::isOpened()
	{
		if (m_directShowCamera)
			return m_directShowCamera->isOpening();
		else
			return false;
	}

	/**
	 * @brief Close
	*/
	bool UVCCamera::close()
	{
		bool result = true;
		if (this && result && isCapturing())
		{
			result = stopCapture();
		}

		if (this && result && isOpened())
		{
			m_directShowCamera->close();
		}

		return result;
	}

	/**
	 * @brief Return true if camera is disconnected.
	 * @return Return true if camera is disconnected.
	 */
	bool UVCCamera::checkDisconnection()
	{
		return m_directShowCamera->checkDisconnection();
	}

	/**
	 * @brief Set the disconnection process. When the process was set, a thread will start to keep check the connection. If camera is disconnected, this process will run.
	 * @param func void()
	 */
	void UVCCamera::setDisconnectionProcess(std::function<void()> func)
	{
		m_directShowCamera->setDisconnectionProcess(func);
	}


#pragma endregion Connection

#pragma region Capture

	/**
	 * @brief Start capture
	 * @return Return true if success
	*/
	bool UVCCamera::startCapture()
	{
		if (m_directShowCamera->isOpening())
		{
			bool success = m_directShowCamera->start();

			copyError(success);

			return success;
		}
		else
		{
			m_errorString = "Error on startCapture() : Camera is not opened.";
			return false;
		}
	}

	/**
	 * @brief Stop capture
	 * @return Return true if success
	*/
	bool UVCCamera::stopCapture()
	{
		if (m_directShowCamera->isOpening())
		{
			bool success = m_directShowCamera->stop();

			copyError(success);

			return success;
		}
		else
		{
			m_errorString = "Error on stopCapture() : Camera is not opened.";
			return false;
		}
	}

	/**
	 * @brief Return true if camera is capturing.
	 * @return Return true if camera is capturing.
	*/
	bool UVCCamera::isCapturing()
	{
		return m_directShowCamera->isCapturing();
	}

#pragma endregion Capture

#pragma region Properties

	/**
	 * @brief Reset camera properties as default value.
	 * @param asAuto Set as true if you want to use auto mode as default value.
	*/
	void UVCCamera::resetProperties(bool asAuto)
	{
		m_directShowCamera->resetDefault(asAuto);
	}

	/**
	 * @brief Get directshow properties pointer. It is a advance option.
	 * @return Return the directshow properties pointer
	*/
	DirectShowCameraProperties* UVCCamera::getDirectShowProperties()
	{
		return m_directShowCamera->getProperties();
	}

#pragma region Brightness

	/**
	 * @brief Retrun true if property brightness is supported.
	 * @return Retrun true if property brightness is supported.
	*/
	bool UVCCamera::supportBrightness()
	{
		return supportPropertyTemplate(
			[this]()
			{
				return m_directShowCamera->getProperties()->getBrightness()->isSupported();
			},
			"Error on supportBrightness()"
				);
	}

	/**
	 * @brief Get the range of the property - brightness
	 * @return Return (min,max). (0,0) return if error occurred.
	*/
	std::pair<long, long> UVCCamera::getBrightnessRange()
	{
		std::pair<long, long> result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getBrightness()->getRange();
			},
			supportBrightness(),
				"Error on getBrightnessRange()",
				"Brightness is not supported."
				);

		return result;
	}

	/**
	 * @brief Get the step of the property - Brightness.
	 * @return Return the step of the Brightness. -1 return if error occurred.
	*/
	long UVCCamera::getBrightnessStep()
	{
		long result = -1;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getBrightness()->getStep();
			},
			supportBrightness(),
				"Error on getBrightnessStep()",
				"Brightness is not supported."
				);

		return result;
	}

	/**
	 * @brief Get current brightness, from blanking(small value) to pure white(large value)
	 * @return Return current brightness. -1 return if error occurred.
	*/
	long UVCCamera::getBrightness()
	{
		long result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getBrightness()->getValue();
			},
			supportBrightness(),
				"Error on getBrightness()",
				"Brightness is not supported."
				);

		if (!success) result = -1;

		return result;
	}

	/**
	 * @brief Set brightness
	 * @param value Value to be set
	 * @return Return true if success.
	*/
	bool UVCCamera::setBrightness(long value)
	{
		bool result = false;
		bool success = handlePropertyTemplate(
			[this, value, &result]()
			{
				result = m_directShowCamera->setValue(m_directShowCamera->getProperties()->getBrightness(), value, false);

				copyError(result);
			},
			supportBrightness(),
				"Error on setBrightness()",
				"Brightness is not supported."
				);
		success = success && result;

		return success;
	}

#pragma endregion Brightness

#pragma region Contrast

	/**
	 * @brief Retrun true if property contrast is supported.
	 * @return Retrun true if property contrast is supported.
	*/
	bool UVCCamera::supportContrast()
	{
		return supportPropertyTemplate(
			[this]()
			{
				return m_directShowCamera->getProperties()->getContrast()->isSupported();
			},
			"Error on supportContrast()"
				);
	}

	/**
	 * @brief Get the range of the property - contrast
	 * @return Return (min,max). (0,0) return if error occurred.
	*/
	std::pair<long, long> UVCCamera::getContrastRange()
	{
		std::pair<long, long> result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getContrast()->getRange();
			},
			supportContrast(),
				"Error on getContrastRange()",
				"Contrast is not supported."
				);

		return result;
	}

	/**
	 * @brief Get the step of the property - Contrast.
	 * @return Return the step of the Contrast. -1 return if error occurred.
	*/
	long UVCCamera::getContrastStep()
	{
		long result = -1;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getContrast()->getStep();
			},
			supportContrast(),
				"Error on getContrastStep()",
				"Contrast is not supported."
				);

		return result;
	}

	/**
	 * @brief Get current contrast
	 * @return Return current contrast. -1 return if error occurred.
	*/
	long UVCCamera::getContrast()
	{
		long result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getContrast()->getValue();
			},
			supportContrast(),
				"Error on getContrast()",
				"Contrast is not supported."
				);

		if (!success) result = -1;

		return result;
	}

	/**
	 * @brief Set Contrast
	 * @param value Value to be set
	 * @return Return true if success.
	*/
	bool UVCCamera::setContrast(long value)
	{
		bool result = false;
		bool success = handlePropertyTemplate(
			[this, value, &result]()
			{
				result = m_directShowCamera->setValue(m_directShowCamera->getProperties()->getContrast(), value, false);

				copyError(result);
			},
			supportContrast(),
				"Error on setContrast()",
				"Contrast is not supported."
				);
		success = success && result;

		return success;
	}

#pragma endregion Contrast

#pragma region Hue

	/**
	 * @brief Retrun true if property hue is supported.
	 * @return Retrun true if property hue is supported.
	*/
	bool UVCCamera::supportHue()
	{
		return supportPropertyTemplate(
			[this]()
			{
				return m_directShowCamera->getProperties()->getHue()->isSupported();
			},
			"Error on supportHue()"
				);
	}

	/**
	 * @brief Get the range of the property - hue
	 * @return Return (min,max). (0,0) return if error occurred.
	*/
	std::pair<long, long> UVCCamera::getHueRange()
	{
		std::pair<long, long> result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getHue()->getRange();
			},
			supportHue(),
				"Error on getHueRange()",
				"Hue is not supported."
				);

		return result;
	}

	/**
	 * @brief Get the step of the property - Hue.
	 * @return Return the step of the Hue. -1 return if error occurred.
	*/
	long UVCCamera::getHueStep()
	{
		long result = -1;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getHue()->getStep();
			},
			supportHue(),
				"Error on getHueStep()",
				"Hue is not supported."
				);

		return result;
	}

	/**
	 * @brief Get current hue
	 * @return Return current hue.
	*/
	long UVCCamera::getHue()
	{
		long result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getHue()->getValue();
			},
			supportHue(),
				"Error on getHue()",
				"Hue is not supported."
				);

		return result;
	}

	/**
	 * @brief Set Hue
	 * @param degree Value to be set in degree
	 * @return Return true if success.
	*/
	bool UVCCamera::setHue(long degree)
	{
		bool result = false;
		bool success = handlePropertyTemplate(
			[this, degree, &result]()
			{
				long value = confirmDegreeRange(degree);
				result = m_directShowCamera->setValue(m_directShowCamera->getProperties()->getHue(), value, false);

				copyError(result);
			},
			supportHue(),
				"Error on setHue()",
				"Hue is not supported."
				);
		success = success && result;

		return success;
	}

#pragma endregion Hue

#pragma region Saturation

	/**
	 * @brief Retrun true if property saturation is supported.
	 * @return Retrun true if property saturation is supported.
	*/
	bool UVCCamera::supportSaturation()
	{
		return supportPropertyTemplate(
			[this]()
			{
				return m_directShowCamera->getProperties()->getSaturation()->isSupported();
			},
			"Error on supportSaturation()"
				);
	}

	/**
	 * @brief Get the range of the property - saturation
	 * @return Return (min,max). (0,0) return if error occurred.
	*/
	std::pair<long, long> UVCCamera::getSaturationRange()
	{
		std::pair<long, long> result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getSaturation()->getRange();
			},
			supportSaturation(),
				"Error on getSaturationRange()",
				"Saturation is not supported."
				);

		return result;
	}

	/**
	 * @brief Get the step of the property - Saturation.
	 * @return Return the step of the Saturation. -1 return if error occurred.
	*/
	long UVCCamera::getSaturationStep()
	{
		long result = -1;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getSaturation()->getStep();
			},
			supportSaturation(),
				"Error on getSaturationStep()",
				"Saturation is not supported."
				);

		return result;
	}

	/**
	 * @brief Get current saturation
	 * @return Return current saturation. -1 return if error occurred.
	*/
	long UVCCamera::getSaturation()
	{
		long result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getSaturation()->getValue();
			},
			supportSaturation(),
				"Error on getSaturation()",
				"Saturation is not supported."
				);

		if (!success) result = -1;

		return result;
	}

	/**
	 * @brief Set Saturation
	 * @param value Value to be set
	 * @return Return true if success.
	*/
	bool UVCCamera::setSaturation(long value)
	{
		bool result = false;
		bool success = handlePropertyTemplate(
			[this, value, &result]()
			{
				result = m_directShowCamera->setValue(m_directShowCamera->getProperties()->getSaturation(), value, false);

				copyError(result);
			},
			supportSaturation(),
				"Error on setSaturation()",
				"Saturation is not supported."
				);
		success = success && result;

		return success;
	}

#pragma endregion Saturation

#pragma region Sharpness

	/**
	 * @brief Retrun true if property sharpness is supported.
	 * @return Retrun true if property sharpness is supported.
	*/
	bool UVCCamera::supportSharpness()
	{
		return supportPropertyTemplate(
			[this]()
			{
				return m_directShowCamera->getProperties()->getSharpness()->isSupported();
			},
			"Error on supportSharpness()"
				);
	}

	/**
	 * @brief Get the range of the property - sharpness
	 * @return Return (min,max). (0,0) return if error occurred.
	*/
	std::pair<long, long> UVCCamera::getSharpnessRange()
	{
		std::pair<long, long> result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getSharpness()->getRange();
			},
			supportSharpness(),
				"Error on getSharpnessRange()",
				"Sharpness is not supported."
				);

		return result;
	}

	/**
	 * @brief Get the step of the property - Sharpness.
	 * @return Return the step of the Sharpness. -1 return if error occurred.
	*/
	long UVCCamera::getSharpnessStep()
	{
		long result = -1;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getSharpness()->getStep();
			},
			supportSharpness(),
				"Error on getSharpnessStep()",
				"Sharpness is not supported."
				);

		return result;
	}

	/**
	 * @brief Get current sharpness
	 * @return Return current sharpness. -1 return if error occurred.
	*/
	long UVCCamera::getSharpness()
	{
		long result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getSharpness()->getValue();
			},
			supportSharpness(),
				"Error on getSharpness()",
				"Sharpness is not supported."
				);

		if (!success) result = -1;

		return result;
	}

	/**
	 * @brief Set Sharpness
	 * @param value Value to be set
	 * @return Return true if success.
	*/
	bool UVCCamera::setSharpness(long value)
	{
		bool result = false;
		bool success = handlePropertyTemplate(
			[this, value, &result]()
			{
				result = m_directShowCamera->setValue(m_directShowCamera->getProperties()->getSharpness(), value, false);

				copyError(result);
			},
			supportSharpness(),
				"Error on setSharpness()",
				"Sharpness is not supported."
				);
		success = success && result;

		return success;
	}

#pragma endregion Sharpness

#pragma region Gamma

	/**
	 * @brief Retrun true if property gamma is supported.
	 * @return Retrun true if property gamma is supported.
	*/
	bool UVCCamera::supportGamma()
	{
		return supportPropertyTemplate(
			[this]()
			{
				return m_directShowCamera->getProperties()->getGamma()->isSupported();
			},
			"Error on supportGamma()"
				);
	}

	/**
	 * @brief Get the range of the property - gamma
	 * @return Return (min,max). (0,0) return if error occurred.
	*/
	std::pair<long, long> UVCCamera::getGammaRange()
	{
		std::pair<long, long> result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getGamma()->getRange();
			},
			supportGamma(),
				"Error on getGammaRange()",
				"Gamma is not supported."
				);

		return result;
	}

	/**
	 * @brief Get the step of the property - Gamma.
	 * @return Return the step of the Gamma. -1 return if error occurred.
	*/
	long UVCCamera::getGammaStep()
	{
		long result = -1;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getGamma()->getStep();
			},
			supportGamma(),
				"Error on getGammaStep()",
				"Gamma is not supported."
				);

		return result;
	}

	/**
	 * @brief Get current gamma
	 * @return Return current gamma. -1 return if error occurred.
	*/
	long UVCCamera::getGamma()
	{
		long result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getGamma()->getValue();
			},
			supportGamma(),
				"Error on getGamma()",
				"Gamma is not supported."
				);

		if (!success) result = -1;

		return result;
	}

	/**
	 * @brief Set Gamma
	 * @param value Value to be set
	 * @return Return true if success.
	*/
	bool UVCCamera::setGamma(long value)
	{
		bool result = false;
		bool success = handlePropertyTemplate(
			[this, value, &result]()
			{
				result = m_directShowCamera->setValue(m_directShowCamera->getProperties()->getGamma(), value, false);

				copyError(result);

				return result;
			},
			supportGamma(),
				"Error on setGamma()",
				"Gamma is not supported."
				);
		success = success && result;

		return success;
	}

#pragma endregion Gamma

#pragma region ColorEnable

	/**
	 * @brief Retrun true if property Color Enable is supported.
	 * @return Retrun true if property Color Enable is supported.
	*/
	bool UVCCamera::supportColorEnable()
	{
		return supportPropertyTemplate(
			[this]()
			{
				return m_directShowCamera->getProperties()->getColorEnable()->isSupported();
			},
			"Error on supportColorEnable()"
				);
	}

	/**
	 * @brief Get current color enable
	 * @return Return current color enable. -1 return if error occurred.
	*/
	bool UVCCamera::isColorEnable()
	{
		long result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getColorEnable()->getValue();
			},
			supportColorEnable(),
				"Error on isColorEnable()",
				"ColorEnable is not supported."
				);

		bool isColorEnable = result == 0 ? false : true; // 0(off), 1(on)
		if (!success) isColorEnable = false;

		return isColorEnable;
	}

	/**
	 * @brief Set Color Enable
	 * @param isOn Color Enable On = true
	 * @return Return true if success.
	*/
	bool UVCCamera::setColorEnable(bool isOn)
	{
		bool result = false;
		bool success = handlePropertyTemplate(
			[this, isOn, &result]()
			{
				// Get value
				std::pair<long, long> range = m_directShowCamera->getProperties()->getContrast()->getRange();
				long value = isOn ? range.second : range.first; // 0(off), 1(on)

				// Set
				result = m_directShowCamera->setValue(m_directShowCamera->getProperties()->getContrast(), value, false);

				copyError(result);
			},
			supportColorEnable(),
				"Error on setColorEnable()",
				"Color Enable is not supported."
				);
		success = success && result;

		return success;
	}

#pragma endregion Color Enable

#pragma region WhiteBalance

	/**
	 * @brief Retrun true if property white balance is supported.
	 * @return Retrun true if property white balance is supported.
	*/
	bool UVCCamera::supportWhiteBalance()
	{
		return supportPropertyTemplate(
			[this]()
			{
				return m_directShowCamera->getProperties()->getWhiteBalance()->isSupported();
			},
			"Error on supportWhiteBalance()"
				);
	}

	/**
	 * @brief Get the range of the property - white balance in degree kelvin
	 * @return Return (min,max). (0,0) return if error occurred.
	*/
	std::pair<long, long> UVCCamera::getWhiteBalanceRange()
	{
		std::pair<long, long> result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getWhiteBalance()->getRange();
			},
			supportWhiteBalance(),
				"Error on getWhiteBalanceRange()",
				"WhiteBalance is not supported."
				);

		return result;
	}

	/**
	 * @brief Get the step of the property - White Balance.
	 * @return Return the step of the White Balance. -1 return if error occurred.
	*/
	long UVCCamera::getWhiteBalanceStep()
	{
		long result = -1;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getWhiteBalance()->getStep();
			},
			supportWhiteBalance(),
				"Error on getWhiteBalanceStep()",
				"WhiteBalance is not supported."
				);

		return result;
	}

	/**
	 * @brief Get current white balance in degree kelvin
	 * @return Return current white balance. -1 return if error occurred.
	*/
	long UVCCamera::getWhiteBalance()
	{
		long result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getWhiteBalance()->getValue();
			},
			supportWhiteBalance(),
				"Error on getWhiteBalance()",
				"WhiteBalance is not supported."
				);

		if (!success) result = -1;

		return result;
	}

	/**
	 * @brief Set WhiteBalance
	 * @param kelvin Value to be set in degree kelvin
	 * @return Return true if success.
	*/
	bool UVCCamera::setWhiteBalance(long kelvin)
	{
		bool result = false;
		bool success = handlePropertyTemplate(
			[this, kelvin, &result]()
			{
				result = m_directShowCamera->setValue(m_directShowCamera->getProperties()->getWhiteBalance(), kelvin, false);

				copyError(result);
			},
			supportWhiteBalance(),
				"Error on setWhiteBalance()",
				"WhiteBalance is not supported."
				);
		success = success && result;

		return success;
	}

	/**
	 * @brief Retrun true if white balance is auto mode, return false if it is manual mode.
	 * @return Retrun true if white balance is auto mode, return false if it is manual mode or error occurred.
	*/
	bool UVCCamera::isAutoWhiteBalance()
	{
		bool isAuto = false;
		bool success = handlePropertyTemplate(
			[this, &isAuto]()
			{
				isAuto = m_directShowCamera->getProperties()->getWhiteBalance()->isAuto();
			},
			supportWhiteBalance(),
				"Error on isAutoWhiteBalance()",
				"WhiteBalance is not supported."
				);

		if (!success) isAuto = false;

		return isAuto;
	}

	/**
	 * @brief Set white balance to auto or manual mode
	 * @param setToAuto Set it as true if you want to set as auto mode. Manual mode as false.
	 * @return Return true if success.
	*/
	bool UVCCamera::setAutoWhiteBalance(bool setToAuto)
	{
		bool result = false;
		bool success = handlePropertyTemplate(
			[this, setToAuto, &result]()
			{
				if (setToAuto && !(m_directShowCamera->getProperties()->getWhiteBalance()->supportAutoMode()))
				{
					// Not support auto mode
					result = false;
					m_errorString = "White Balance does not support auto mode.";
				}
				else if (!setToAuto && !(m_directShowCamera->getProperties()->getWhiteBalance()->supportManualMode()))
				{
					// Not support manual mode
					result = false;
					m_errorString = "White Balance does not support manual mode.";
				}
				else
				{
					// Set
					result = m_directShowCamera->setValue(m_directShowCamera->getProperties()->getWhiteBalance(),
						m_directShowCamera->getProperties()->getWhiteBalance()->getValue(),
						setToAuto);

					copyError(result);
				}
			},
			supportWhiteBalance(),
				"Error on setAutoWhiteBalance()",
				"WhiteBalance is not supported."
				);
		success = success && result;

		return success;
	}

#pragma endregion WhiteBalance

#pragma region BacklightCompensation

	/**
	 * @brief Retrun true if property backlight compensation is supported.
	 * @return Retrun true if property backlight compensation is supported.
	*/
	bool UVCCamera::supportBacklightCompensation()
	{
		return supportPropertyTemplate(
			[this]()
			{
				return m_directShowCamera->getProperties()->getBacklightCompensation()->isSupported();
			},
			"Error on supportBacklightCompensation()"
				);
	}


	/**
	 * @brief Get current backlight compensation
	 * @return Return current backlight compensation. -1 return if error occurred.
	*/
	bool UVCCamera::isBacklightCompensation()
	{
		long result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getBacklightCompensation()->getValue();
			},
			supportBacklightCompensation(),
				"Error on isBacklightCompensation()",
				"BacklightCompensation is not supported."
				);

		bool isBacklightCompensation = result == 0 ? false : true; // 0(off), 1(on)
		if (!success) isBacklightCompensation = false;

		return isBacklightCompensation;
	}

	/**
	 * @brief Set Contrast
	 * @param isOn Backlight Compensation On = true
	 * @return Return true if success.
	*/
	bool UVCCamera::setBacklightCompensation(bool isOn)
	{
		bool result = false;
		bool success = handlePropertyTemplate(
			[this, isOn, &result]()
			{
				// Get value
				long value = isOn ? 1 : 0; // 0(off), 1(on)

				result = m_directShowCamera->setValue(m_directShowCamera->getProperties()->getBacklightCompensation(), value, false);

				copyError(result);
			},
			supportBacklightCompensation(),
				"Error on setBacklightCompensation()",
				"BacklightCompensation is not supported."
				);
		success = success && result;

		return success;
	}

#pragma endregion BacklightCompensation

#pragma region Gain

	/**
	 * @brief Retrun true if property gain is supported.
	 * @return Retrun true if property gain is supported.
	*/
	bool UVCCamera::supportGain()
	{
		return supportPropertyTemplate(
			[this]()
			{
				return m_directShowCamera->getProperties()->getGain()->isSupported();
			},
			"Error on supportGain()"
				);
	}

	/**
	 * @brief Get the range of the property - gain
	 * @return Return (min,max). (0,0) return if error occurred.
	*/
	std::pair<long, long> UVCCamera::getGainRange()
	{
		std::pair<long, long> result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getGain()->getRange();
			},
			supportGain(),
				"Error on getGainRange()",
				"Gain is not supported."
				);

		return result;
	}

	/**
	 * @brief Get the step of the property - gain.
	 * @return Return the step of the gain. -1 return if error occurred.
	*/
	long UVCCamera::getGainStep()
	{
		long result = -1;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getGain()->getStep();
			},
			supportGain(),
				"Error on getGainStep()",
				"Gain is not supported."
				);

		return result;
	}

	/**
	 * @brief Get current gain
	 * @return Return current gain. -1 return if error occurred.
	*/
	long UVCCamera::getGain()
	{
		long result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getGain()->getValue();
			},
			supportGain(),
				"Error on getGain()",
				"Gain is not supported."
				);

		if (!success) result = -1;

		return result;
	}

	/**
	 * @brief Set Gain
	 * @param value Value to be set
	 * @return Return true if success.
	*/
	bool UVCCamera::setGain(long value)
	{
		bool result = false;
		bool success = handlePropertyTemplate(
			[this, value, &result]()
			{
				result = m_directShowCamera->setValue(m_directShowCamera->getProperties()->getGain(), value, false);

				copyError(result);
			},
			supportGain(),
				"Error on setGain()",
				"Gain is not supported."
				);
		success = success && result;

		return success;
	}

#pragma endregion Gain

#pragma region Pan

	/**
	 * @brief Retrun true if property pan is supported.
	 * @return Retrun true if property pan is supported.
	*/
	bool UVCCamera::supportPan()
	{
		return supportPropertyTemplate(
			[this]()
			{
				return m_directShowCamera->getProperties()->getPan()->isSupported();
			},
			"Error on supportPan()"
				);
	}

	/**
	 * @brief Get the range of the property - pan
	 * @return Return (min,max). (0,0) return if error occurred.
	*/
	std::pair<long, long> UVCCamera::getPanRange()
	{
		std::pair<long, long> result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getPan()->getRange();
			},
			supportPan(),
				"Error on getPanRange()",
				"Pan is not supported."
				);

		return result;
	}

	/**
	 * @brief Get the step of the property - pan.
	 * @return Return the step of the pan. -1 return if error occurred.
	*/
	long UVCCamera::getPanStep()
	{
		long result = -1;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getPan()->getStep();
			},
			supportPan(),
				"Error on getPanStep()",
				"Pan is not supported."
				);

		return result;
	}

	/**
	 * @brief Get current pan
	 * @return Return current pan. 0 return if error occurred.
	*/
	long UVCCamera::getPan()
	{
		long result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getPan()->getValue();
			},
			supportPan(),
				"Error on getPan()",
				"Pan is not supported."
				);

		if (!success) result = 0;

		return result;
	}

	/**
	 * @brief Set Pan
	 * @param degree Value to be set in degree
	 * @return Return true if success.
	*/
	bool UVCCamera::setPan(long degree)
	{
		bool result = false;
		bool success = handlePropertyTemplate(
			[this, degree, &result]()
			{
				long value = confirmDegreeRange(degree);
				result = m_directShowCamera->setValue(m_directShowCamera->getProperties()->getPan(), value, false);

				copyError(result);
			},
			supportPan(),
				"Error on setPan()",
				"Pan is not supported."
				);
		success = success && result;

		return success;
	}

#pragma endregion Pan

#pragma region Tilt

	/**
	 * @brief Retrun true if property tilt is supported.
	 * @return Retrun true if property tilt is supported.
	*/
	bool UVCCamera::supportTilt()
	{
		return supportPropertyTemplate(
			[this]()
			{
				return m_directShowCamera->getProperties()->getTilt()->isSupported();
			},
			"Error on supportTilt()"
				);
	}

	/**
	 * @brief Get the range of the property - tilt
	 * @return Return (min,max). (0,0) return if error occurred.
	*/
	std::pair<long, long> UVCCamera::getTiltRange()
	{
		std::pair<long, long> result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getTilt()->getRange();
			},
			supportTilt(),
				"Error on getTiltRange()",
				"Tilt is not supported."
				);

		return result;
	}

	/**
	 * @brief Get the step of the property - tilt.
	 * @return Return the step of the tilt. -1 return if error occurred.
	*/
	long UVCCamera::getTiltStep()
	{
		long result = -1;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getTilt()->getStep();
			},
			supportTilt(),
				"Error on getTiltStep()",
				"Tilt is not supported."
				);

		return result;
	}

	/**
	 * @brief Get current tilt
	 * @return Return current tilt. 0 return if error occurred.
	*/
	long UVCCamera::getTilt()
	{
		long result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getTilt()->getValue();
			},
			supportTilt(),
				"Error on getTilt()",
				"Tilt is not supported."
				);

		if (!success) result = 0;

		return result;
	}

	/**
	 * @brief Set Tilt
	 * @param degree Value to be set in degree
	 * @return Return true if success.
	*/
	bool UVCCamera::setTilt(long degree)
	{
		bool result = false;
		bool success = handlePropertyTemplate(
			[this, degree, &result]()
			{
				long value = confirmDegreeRange(degree);
				result = m_directShowCamera->setValue(m_directShowCamera->getProperties()->getTilt(), value, false);

				copyError(result);
			},
			supportTilt(),
				"Error on setTilt()",
				"Tilt is not supported."
				);
		success = success && result;

		return success;
	}

#pragma endregion Tilt

#pragma region Roll

	/**
	 * @brief Retrun true if property roll is supported.
	 * @return Retrun true if property roll is supported.
	*/
	bool UVCCamera::supportRoll()
	{
		return supportPropertyTemplate(
			[this]()
			{
				return m_directShowCamera->getProperties()->getRoll()->isSupported();
			},
			"Error on supportRoll()"
				);
	}

	/**
	 * @brief Get the range of the property - roll
	 * @return Return (min,max). (0,0) return if error occurred.
	*/
	std::pair<long, long> UVCCamera::getRollRange()
	{
		std::pair<long, long> result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getRoll()->getRange();
			},
			supportRoll(),
				"Error on getRollRange()",
				"Roll is not supported."
				);

		return result;
	}

	/**
	 * @brief Get the step of the property - roll.
	 * @return Return the step of the roll. -1 return if error occurred.
	*/
	long UVCCamera::getRollStep()
	{
		long result = -1;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getRoll()->getStep();
			},
			supportRoll(),
				"Error on getRollStep()",
				"Roll is not supported."
				);

		return result;
	}

	/**
	 * @brief Get current Roll
	 * @return Return current Roll. 0 return if error occurred.
	*/
	long UVCCamera::getRoll()
	{
		long result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getRoll()->getValue();
			},
			supportRoll(),
				"Error on Roll()",
				"Roll is not supported."
				);

		if (!success) result = 0;

		return result;
	}

	/**
	 * @brief Set Roll
	 * @param degree Value to be set in degree
	 * @return Return true if success.
	*/
	bool UVCCamera::setRoll(long degree)
	{
		bool result = false;
		bool success = handlePropertyTemplate(
			[this, degree, &result]()
			{
				long value = confirmDegreeRange(degree);
				result = m_directShowCamera->setValue(m_directShowCamera->getProperties()->getRoll(), value, false);

				copyError(result);
			},
			supportRoll(),
				"Error on setRoll()",
				"Roll is not supported."
				);
		success = success && result;

		return success;
	}

#pragma endregion Roll

#pragma region Zoom

	/**
	 * @brief Retrun true if property zoom is supported.
	 * @return Retrun true if property zoom is supported.
	*/
	bool UVCCamera::supportZoom()
	{
		return supportPropertyTemplate(
			[this]()
			{
				return m_directShowCamera->getProperties()->getZoom()->isSupported();
			},
			"Error on supportZoom()"
				);
	}

	/**
	 * @brief Get the range of the property - zoom
	 * @return Return (min,max). (0,0) return if error occurred.
	*/
	std::pair<long, long> UVCCamera::getZoomRange()
	{
		std::pair<long, long> result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getZoom()->getRange();
			},
			supportZoom(),
				"Error on getZoomRange()",
				"Zoom is not supported."
				);

		return result;
	}

	/**
	 * @brief Get the step of the property - zoom.
	 * @return Return the step of the zoom. -1 return if error occurred.
	*/
	long UVCCamera::getZoomStep()
	{
		long result = -1;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getZoom()->getStep();
			},
			supportZoom(),
				"Error on getZoomStep()",
				"Zoom is not supported."
				);

		return result;
	}

	/**
	 * @brief Get current zoom
	 * @return Return current zoom. -1 return if error occurred.
	*/
	long UVCCamera::getZoom()
	{
		long result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getZoom()->getValue();
			},
			supportZoom(),
				"Error on getZoom()",
				"Zoom is not supported."
				);

		if (!success) result = -1;

		return result;
	}

	/**
	 * @brief Set Zoom
	 * @param millimeter Value to be set in millimeters
	 * @return Return true if success.
	*/
	bool UVCCamera::setZoom(long millimeter)
	{
		bool result = false;
		bool success = handlePropertyTemplate(
			[this, millimeter, &result]()
			{
				result = m_directShowCamera->setValue(m_directShowCamera->getProperties()->getZoom(), millimeter, false);

				copyError(result);
			},
			supportZoom(),
				"Error on setZoom()",
				"Zoom is not supported."
				);
		success = success && result;

		return success;
	}

#pragma endregion Zoom

#pragma region Exposure

	/**
	 * @brief Retrun true if property exposure is supported.
	 * @return Retrun true if property exposure is supported.
	*/
	bool UVCCamera::supportExposure()
	{
		return supportPropertyTemplate(
			[this]()
			{
				return m_directShowCamera->getProperties()->getExposure()->isSupported();
			},
			"Error on supportExposure()"
				);
	}

	/**
	 * @brief Get the range of the property - exposure in second
	 * @return Return (min,max). (0,0) return if error occurred.
	*/
	std::pair<double, double> UVCCamera::getExposureRange()
	{
		std::pair<double, double> result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				//Get
				std::pair<long, long> range = m_directShowCamera->getProperties()->getExposure()->getRange();

				// Convert to second
				result.first = exposureConvertion(range.first);
				result.second = exposureConvertion(range.second);
			},
			supportExposure(),
				"Error on getExposureRange()",
				"Exposure is not supported."
				);

		return result;
	}

	/**
	 * @brief Get current exposure in second
	 * @return Return current exposure. -1 return if error occurred.
	*/
	double UVCCamera::getExposure()
	{
		double result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				// Get value
				long value = m_directShowCamera->getProperties()->getExposure()->getValue();

				// Convert to second
				result = exposureConvertion(value);
			},
			supportExposure(),
				"Error on getExposure()",
				"Exposure is not supported."
				);

		if (!success) result = -1;

		return result;
	}

	/**
	 * @brief Set Exposure
	 * @param second Value to be set in second
	 * @return Return true if success.
	*/
	bool UVCCamera::setExposure(double second)
	{
		bool result = false;
		bool success = handlePropertyTemplate(
			[this, second, &result]()
			{
				// Convert to DirectShow value
				long dsValue = exposureConvertion(second);

				// Set
				result = m_directShowCamera->setValue(m_directShowCamera->getProperties()->getExposure(), dsValue, false);

				copyError(result);
			},
			supportExposure(),
				"Error on setExposure()",
				"Exposure is not supported."
				);
		success = success && result;

		return success;
	}

	/**
	 * @brief Get all possible exposure values in second
	 * @return Return the possible exposure values. Return empty vector if error occurred.
	*/
	std::vector<double> UVCCamera::getPossibleExposureValues()
	{
		std::vector<double> result;
		std::vector<long> exposureValues;
		bool success = handlePropertyTemplate(
			[this, &exposureValues]()
			{
				// Get range and step
				std::pair<long, long> range = m_directShowCamera->getProperties()->getExposure()->getRange();
				long step = m_directShowCamera->getProperties()->getExposure()->getStep();

				// Add possible value
				long value = range.first;
				while (value < range.second)
				{
					// Convert to second
					exposureValues.push_back(value);

					// To next value
					value += step;
				}
			},
			supportExposure(),
				"Error on getPossibleExposureValue()",
				"Exposure is not supported."
				);

		if (success && exposureValues.size() > 0)
		{
			// Sort and Unique
			std::sort(exposureValues.begin(), exposureValues.end());
			exposureValues.erase(std::unique(exposureValues.begin(), exposureValues.end()), exposureValues.end());

			// Convert to second
			for (int i = 0; i < exposureValues.size(); i++)
			{
				result.push_back(exposureConvertion(exposureValues[i]));
			}

		}

		return result;
	}

	/**
	 * @brief Get current exposure index of the getPossibleExposureValues()
	 * @return Return current exposure index of the getPossibleExposureValues(). Return -1 if error occurred.
	*/
	int UVCCamera::getExposureIndex()
	{
		std::vector<double> exposureList = getPossibleExposureValues();
		double exposureValue = getExposure();
		int result = -1;

		if (exposureList.size() > 0)
		{
			// Find the closest value in the exposureList
			result = 0;
			double distance = abs(exposureList[0] - exposureValue);

			for (int i = 1;i< exposureList.size();i++)
			{
				double currentDistance = abs(exposureList[i] - exposureValue);
				if (currentDistance < distance)
				{
					result = i;
					distance = currentDistance;
				}
			}
		}
		
		return result;
	}

	/**
	 * @brief Retrun true if exposure is auto mode, return false if it is manual mode.
	 * @return Retrun true if exposure is auto mode, return false if it is manual mode or error occurred.
	*/
	bool UVCCamera::isAutoExposure()
	{
		bool isAuto = false;
		bool success = handlePropertyTemplate(
			[this, &isAuto]()
			{
				isAuto = m_directShowCamera->getProperties()->getExposure()->isAuto();
			},
			supportExposure(),
				"Error on isAutoExposure()",
				"Exposure is not supported."
				);

		if (!success) isAuto = false;

		return isAuto;
	}

	/**
	 * @brief Set exposure to auto or manual mode
	 * @param setToAuto Set it as true if you want to set as auto mode. Manual mode as false.
	 * @return Return true if success.
	*/
	bool UVCCamera::setAutoExposure(bool setToAuto)
	{
		bool result = false;
		bool success = handlePropertyTemplate(
			[this, setToAuto, &result]()
			{
				if (setToAuto && !(m_directShowCamera->getProperties()->getExposure()->supportAutoMode()))
				{
					// Not support auto mode
					result = false;
					m_errorString = "Exposure does not support auto mode.";
				}
				else if (!setToAuto && !(m_directShowCamera->getProperties()->getExposure()->supportManualMode()))
				{
					// Not support manual mode
					result = false;
					m_errorString = "Exposure does not support manual mode.";
				}
				else
				{
					// Set
					result = m_directShowCamera->setValue(m_directShowCamera->getProperties()->getExposure(),
						m_directShowCamera->getProperties()->getExposure()->getValue(),
						setToAuto);

					copyError(result);
				}
			},
			supportExposure(),
				"Error on setAutoExposure()",
				"Exposure is not supported."
				);
		success = success && result;

		return success;
	}

#pragma endregion Exposure

#pragma region Iris

	/**
	 * @brief Retrun true if property iris is supported.
	 * @return Retrun true if property iris is supported.
	*/
	bool UVCCamera::supportIris()
	{
		return supportPropertyTemplate(
			[this]()
			{
				return m_directShowCamera->getProperties()->getIris()->isSupported();
			},
			"Error on supportIris()"
				);
	}

	/**
	 * @brief Get the range of the property - iris in units of f_stop * 10.
	 * @return Return (min,max). (0,0) return if error occurred.
	*/
	std::pair<long, long> UVCCamera::getIrisRange()
	{
		std::pair<long, long> result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getIris()->getRange();
			},
			supportIris(),
				"Error on getIrisRange()",
				"Iris is not supported."
				);

		return result;
	}

	/**
	 * @brief Get the step of the property - iris in units of f_stop * 10.
	 * @return Return the step of the iris. -1 return if error occurred.
	*/
	long UVCCamera::getIrisStep()
	{
		long result = -1;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getIris()->getStep();
			},
			supportIris(),
				"Error on getIrisStep()",
				"Iris is not supported."
				);

		return result;
	}

	/**
	 * @brief Get current iris in units of f_stop * 10.
	 * @return Return current iris. -1 return if error occurred.
	*/
	long UVCCamera::getIris()
	{
		long result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getIris()->getValue();
			},
			supportIris(),
				"Error on getIris()",
				"Iris is not supported."
				);

		if (!success) result = -1;

		return result;
	}

	/**
	 * @brief Set Iris
	 * @param value Value to be set in units of f_stop * 10.
	 * @return Return true if success.
	*/
	bool UVCCamera::setIris(long value)
	{
		bool result = false;
		bool success = handlePropertyTemplate(
			[this, value, &result]()
			{
				result = m_directShowCamera->setValue(m_directShowCamera->getProperties()->getIris(), value, false);

				copyError(result);
			},
			supportIris(),
				"Error on setIris()",
				"Iris is not supported."
				);
		success = success && result;

		return success;
	}

	/**
	 * @brief Retrun true if iris is auto mode, return false if it is manual mode.
	 * @return Retrun true if iris is auto mode, return false if it is manual mode or error occurred.
	*/
	bool UVCCamera::isAutoIris()
	{
		bool isAuto = false;
		bool success = handlePropertyTemplate(
			[this, &isAuto]()
			{
				isAuto = m_directShowCamera->getProperties()->getIris()->isAuto();
			},
			supportIris(),
				"Error on isAutoIris()",
				"Iris is not supported."
				);

		if (!success) isAuto = false;

		return isAuto;
	}

	/**
	 * @brief Set iris to auto or manual mode
	 * @param setToAuto Set it as true if you want to set as auto mode. Manual mode as false.
	 * @return Return true if success.
	*/
	bool UVCCamera::setAutoIris(bool setToAuto)
	{
		bool result = false;
		bool success = handlePropertyTemplate(
			[this, setToAuto, &result]()
			{
				if (setToAuto && !(m_directShowCamera->getProperties()->getIris()->supportAutoMode()))
				{
					// Not support auto mode
					result = false;
					m_errorString = "Iris does not support auto mode.";
				}
				else if (!setToAuto && !(m_directShowCamera->getProperties()->getIris()->supportManualMode()))
				{
					// Not support manual mode
					result = false;
					m_errorString = "Iris does not support manual mode.";
				}
				else
				{
					// Set
					result = m_directShowCamera->setValue(m_directShowCamera->getProperties()->getIris(),
						m_directShowCamera->getProperties()->getIris()->getValue(),
						setToAuto);

					copyError(result);
				}
			},
			supportIris(),
				"Error on setAutoIris()",
				"Iris is not supported."
				);
		success = success && result;

		return success;
	}

#pragma endregion Iris

#pragma region Focus

	/**
	 * @brief Retrun true if property focus is supported.
	 * @return Retrun true if property focus is supported.
	*/
	bool UVCCamera::supportFocus()
	{
		return supportPropertyTemplate(
			[this]()
			{
				return m_directShowCamera->getProperties()->getFocus()->isSupported();
			},
			"Error on supportFocus()"
				);
	}

	/**
	 * @brief Get the range of the property - focus in millimeters.
	 * @return Return (min,max). (0,0) return if error occurred.
	*/
	std::pair<long, long> UVCCamera::getFocusRange()
	{
		std::pair<long, long> result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getFocus()->getRange();
			},
			supportFocus(),
				"Error on getFocusRange()",
				"Focus is not supported."
				);

		return result;
	}

	/**
	 * @brief Get the step of the property - focus in millimeters.
	 * @return Return the step of the property - focus in millimeters. -1 return if error occurred.
	*/
	long UVCCamera::getFocusStep()
	{
		long result = -1;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getFocus()->getStep();
			},
			supportFocus(),
				"Error on getFocusStep()",
				"Focus is not supported."
				);

		return result;
	}

	/**
	 * @brief Get current focus in millimeters.
	 * @return Return current focus. -1 return if error occurred.
	*/
	long UVCCamera::getFocus()
	{
		long result;
		bool success = handlePropertyTemplate(
			[this, &result]()
			{
				result = m_directShowCamera->getProperties()->getFocus()->getValue();
			},
			supportFocus(),
				"Error on getFocus()",
				"Focus is not supported."
				);

		if (!success) result = -1;

		return result;
	}

	/**
	 * @brief Set Focus
	 * @param millimeter Value to be set in millimeters.
	 * @return Return true if success.
	*/
	bool UVCCamera::setFocus(long millimeter)
	{
		bool result = false;
		bool success = handlePropertyTemplate(
			[this, millimeter, &result]()
			{
				result = m_directShowCamera->setValue(m_directShowCamera->getProperties()->getFocus(), millimeter, false);

				copyError(result);
			},
			supportFocus(),
				"Error on setFocus()",
				"Focus is not supported."
				);
		success = success && result;

		return success;
	}

	/**
	 * @brief Retrun true if focus is auto mode, return false if it is manual mode.
	 * @return Retrun true if focus is auto mode, return false if it is manual mode or error occurred.
	*/
	bool UVCCamera::isAutoFocus()
	{
		bool isAuto = false;
		bool success = handlePropertyTemplate(
			[this, &isAuto]()
			{
				isAuto = m_directShowCamera->getProperties()->getFocus()->isAuto();
			},
			supportFocus(),
				"Error on isAutoFocus()",
				"Focus is not supported."
				);

		if (!success) isAuto = false;

		return isAuto;
	}

	/**
	 * @brief Set focus to auto or manual mode
	 * @param setToAuto Set it as true if you want to set as auto mode. Manual mode as false.
	 * @return Return true if success.
	*/
	bool UVCCamera::setAutoFocus(bool setToAuto)
	{
		bool result = false;
		bool success = handlePropertyTemplate(
			[this, setToAuto, &result]()
			{
				if (setToAuto && !(m_directShowCamera->getProperties()->getFocus()->supportAutoMode()))
				{
					// Not support auto mode
					result = false;
					m_errorString = "Focus does not support auto mode.";
				}
				else if (!setToAuto && !(m_directShowCamera->getProperties()->getFocus()->supportManualMode()))
				{
					// Not support manual mode
					result = false;
					m_errorString = "Focus does not support manual mode.";
				}
				else
				{
					// Set
					result = m_directShowCamera->setValue(m_directShowCamera->getProperties()->getFocus(),
						m_directShowCamera->getProperties()->getFocus()->getValue(),
						setToAuto);

					copyError(result);
				}
			},
			supportFocus(),
				"Error on setAutoFocus()",
				"Focus is not supported."
				);
		success = success && result;

		return success;
	}

#pragma endregion Focus

#pragma endregion Properties

	/**
	 * @brief Get the last error message.
	 * @return Return the last error message.
	*/
	std::string UVCCamera::getLastError()
	{
		return m_errorString;
	}

#pragma region Cameras

	/**
	 * @brief Get the available DirectShowCameraDevice list. It is a advance option. Suggest to use getCameras().
	 * @return Return the available DirectShowCameraDevice list
	*/
	std::vector<DirectShowCameraDevice> UVCCamera::getDirectShowCameras()
	{
		std::vector<DirectShowCameraDevice> result;
		bool success = m_directShowCamera->getCameras(&result);

		copyError(success);

		return result;
	}

	/**
	 * @brief Get the available camera.
	 * @return Return the available camera.
	*/
	std::vector<CameraDevice> UVCCamera::getCameras()
	{
		// Get DirectShowCameraDevice
		std::vector<DirectShowCameraDevice> directShowCameras = getDirectShowCameras();

		// Convert to CameraDevice
		std::vector<CameraDevice> result;
		for (int i = 0; i < directShowCameras.size(); i++)
		{
			result.push_back(CameraDevice(&directShowCameras[i]));
		}

		return result;
	}

#pragma endregion Cameras

#pragma region DirectShow Video Format

	/**
	 * @brief Get support DirectShowVideoFormat list. It is a advance option. Suggest to acquire the resolution from CameraDevice (Use getCameras() to get CameraDevice).
	 * @return Return the support DirectShowVideoFormat list.
	*/
	std::vector<DirectShowVideoFormat> UVCCamera::getSupportDirectShowVideoFormats()
	{
		return m_directShowCamera->getVideoFormatList();
	}

	/**
	 * @brief Set DirectShowVideoFormat to the camera. Problam may caused when camera was opened. It is suggest to set DirectShowVideoFormat by open() fucntion.
	 * @param videoFormat DirectShowVideoFormat
	 * @return Return true if success.
	*/
	bool UVCCamera::setDirectShowVideoFormat(DirectShowVideoFormat* videoFormat)
	{
		if (m_directShowCamera->isOpening())
		{
			bool requireStart = m_directShowCamera->isCapturing();

			// Stop capturing
			m_directShowCamera->stop();

			// Set resolution
			bool result = m_directShowCamera->setVideoFormat(videoFormat);

			// Copy error
			copyError(result);

#ifdef HAS_OPENCV
			allocateMatBuffer();
#endif

			// Restart capturing
			if (requireStart)
			{
				m_directShowCamera->start();
			}

			return result;
		}
		else
		{
			// Copy error
			copyError(false);

			return false;
		}
	}

	/**
	 * @brief Get the current DirectShowVideoFormat
	 * @return Return the current DirectShowVideoFormat
	*/
	DirectShowVideoFormat UVCCamera::getDirectShowVideoFormat()
	{
		return m_directShowCamera->getCurrentVideoFormat();
	}

#pragma endregion DirectShow Video Format

#pragma region Frame


	/**
	 * @brief Get frame
	 * @param[out] frame Frame bytes
	 * @param[out] numOfBytes Number of bytes of the frame
	 * @return Return true if success
	*/
	bool UVCCamera::getFrame(unsigned char* frame, int* numOfBytes)
	{
		bool result = false;

		if (m_directShowCamera->isCapturing())
		{
			unsigned long frameIndex;
			bool success = m_directShowCamera->getFrame(frame, &frameIndex, numOfBytes, onlyGetNewFrame, m_lastFrameIndex);


			if (onlyGetNewFrame && frameIndex == m_lastFrameIndex)
			{
				// Get old frame but we want new frame only.
				result = false;
			}
			else if (!success)
			{
				result = false;
			}
			else
			{
				result = true;
			}

			// Update frame index
			m_lastFrameIndex = frameIndex;
		}
		else
		{
			result = false;
		}

		return result;
	}

	/**
	 * @brief Return the last frame index. It use to identify whether a new frame. Index will only be updated when you call getFrame() or gatMat();
	 * @return Return the last frame index.
	*/
	long UVCCamera::getFrameIndex()
	{
		return m_lastFrameIndex;
	}

	/**
	 * @brief Get frame per second
	 * @return
	*/
	double UVCCamera::getFPS()
	{
		return m_directShowCamera->getFPS();
	}

#pragma region Opencv Function

#ifdef HAS_OPENCV

	/**
	 * @brief Set as true to return a BGR cv::Mat. Default as false which return a RGB Mat.
	 * @param asBGR Set as true to return a BGR cv::Mat.
	*/
	void UVCCamera::setMatAsBGR(bool asBGR)
	{
		m_matConvertor.isBGR = asBGR;
	}

	/**
	 * @brief Set as true to return a vertical flip cv::Mat. Default as true.
	 * @param verticalFlip Set as true to return a vertical flip cv::Mat.
	*/
	void UVCCamera::vecticalFlipMat(bool verticalFlip)
	{
		m_matConvertor.isVerticalFlip = verticalFlip;
	}

	/**
	 * @brief Allocate frame buffer
	 * @return Return true if success
	*/
	bool UVCCamera::allocateMatBuffer()
	{
		bool result = false;

		if (isOpened())
		{
			// Get frame size
			const long bufferSize = m_directShowCamera->getFrameTotalSize();

			if (m_matBufferSize != bufferSize)
			{
				// Allocate buffer
				if (m_matBuffer != NULL)
				{
					delete m_matBuffer;
					m_matBuffer = NULL;
				}

				m_matBuffer = new unsigned char[bufferSize];
				m_matBufferSize = bufferSize;

				result = true;
			}

			// Update convertor media type
			m_matConvertor.videoType = m_directShowCamera->getFrameType();
		}

		return result;
	}

	/**
	 * @brief Get cv::Mat of the current frame
	 * @return Return cv::Mat
	*/
	cv::Mat UVCCamera::getMat()
	{
		// Reallocate frame buffer size if changed
		if (m_matBufferSize != m_directShowCamera->getFrameTotalSize())
		{
			allocateMatBuffer();
		}

		// Get frame
		bool success = getFrame(m_matBuffer);

		if (success)
		{
			return m_matConvertor.convert(m_matBuffer, getWidth(), getHeight());
		}
		else
		{
			cv::Mat result;
			return result;
		}
	}

	/**
	 * @brief Get cv::Mat from the Mat buffer. Buffer will not be updated if you have not to call getMat();
	 * @return Return the cv::Mat
	*/
	cv::Mat UVCCamera::getLastMat()
	{
		return m_matConvertor.convert(m_matBuffer, getWidth(), getHeight());
	}

	/**
	 * @brief Get new cv::Mat in sync mode.
	 * @param step (Option) Step for checking new frame in ms. Default as 50ms
	 * @param timeout (Option) Timeout in ms. Default as 3000ms
	 * @param skip (Option) Number of new Mat to be skipped. For example, if skip = 3, the fourth new mat will be returned. Default as 0.
	 * @return Return the cv::Mat. Empty mat return if timeout.
	*/
	cv::Mat UVCCamera::getNewMat(int step, int timeout, int skip)
	{
		auto lastFrameIndex = m_lastFrameIndex;
		int pastTime = 0;
		cv::Mat result;

		// Wait for new frame
		while (m_lastFrameIndex < lastFrameIndex + 1 + skip && pastTime <= timeout)
		{
			// Sleep
			std::this_thread::sleep_for(std::chrono::milliseconds(step));

			// Get mat and update m_lastFrameIndex
			result = getMat();

			// Update past time
			pastTime += step;
		}

		// Output
		if (pastTime > timeout)
		{
			// Timeout
			return cv::Mat();
		}
		else
		{
			// New frame collected
			if (result.empty())
			{
				return getLastMat();
			}
			else
			{
				return result;
			}
			
		}
	}

	/**
	 * @brief Exposure fusion
	 * @param[in] exposureFusionAsyncResult void(cv::Mat) async processing the exposure fusion and return the result.  Default is NULL. If this funciton is set, exposureFusion() wil return a empty cv::Mat.
	 * @param[out] exposureImages Images captured in different exposures. Default as NULL
	 * @param[in] minSetExposureDelay Minimum time delay in between setting exposures.
	 * @return Return the fusion result. Empty will return if fail or exposureFusionAsyncResult was set.
	*/
	cv::Mat UVCCamera::exposureFusion(ExposureFusionAsyncResult exposureFusionAsyncResult, std::vector<cv::Mat>* exposureImages, int minSetExposureDelay)
	{
		std::vector<double> exposures = getPossibleExposureValues();
		
		return exposureFusion(exposures, exposureFusionAsyncResult, exposureImages, minSetExposureDelay);
	}

	/**
	 * @brief Exposure fusion
	 * @param[in] exposures Exposures time to be captured
	 * @param[in] exposureFusionAsyncResult void(cv::Mat) async processing the exposure fusion and return the result.  Default is NULL. If this funciton is set, exposureFusion() wil return a empty cv::Mat.
	 * @param[out] exposureImages Images captured in different exposures. Default as NULL
	 * @param[in] minSetExposureDelay Minimum time delay in between setting exposures.
	 * @return Return the fusion result. Empty will return if fail or exposureFusionAsyncResult was set.
	*/
	cv::Mat UVCCamera::exposureFusion(std::vector<double> exposures, ExposureFusionAsyncResult exposureFusionAsyncResult, std::vector<cv::Mat>* exposureImages, int minSetExposureDelay)
	{
		// Save current exposure
		double currentExposure = getExposure();
		bool isAutoExposureNow = isAutoExposure();

		bool releaseExposureImages = false;
		if (!exposureImages)
		{
			exposureImages = new std::vector<cv::Mat>();
			releaseExposureImages = true;
		}

		// Get images in different exposure
		for (int i = 0; i < exposures.size(); i++)
		{
			setExposure(exposures[i]);

			std::this_thread::sleep_for(std::chrono::milliseconds(minSetExposureDelay + (int)(exposures[i] * 1000)));

			cv::Mat newMat = getNewMat();
			if (!newMat.empty())
			{
				exposureImages->push_back(newMat);
			}
		}

		// Recovery exposure
		if (isAutoExposureNow)
		{
			setAutoExposure(true);
		}
		else
		{
			setExposure(currentExposure);
		}

		// Run exposure fusion	
		cv::Mat fusion;
		if (exposureImages)
		{
			// Exposure fusion process
			auto exposureFusionProcess = [exposureImages, releaseExposureImages]()
			{
				cv::Mat fusion;

				// Fusion
				cv::Ptr<cv::MergeMertens> merge_mertens = cv::createMergeMertens();
				merge_mertens->process(*exposureImages, fusion);

				// Convert to 8UC3
				cv::Mat fusion8UC3;
				fusion.convertTo(fusion8UC3, CV_8U, 255, 0);
				fusion = fusion8UC3;

				// Release
				if (releaseExposureImages)
				{
					exposureImages->clear();
					delete exposureImages;
				}

				return fusion;
			};

			// Run in Sync or Async mode
			if (exposureFusionAsyncResult == nullptr)
			{
				// Run in sync mode
				fusion = exposureFusionProcess();
			}
			else
			{
				// Run in async mode
				std::thread([exposureFusionProcess, exposureFusionAsyncResult]()
					{
						cv::Mat fusion = exposureFusionProcess();
						exposureFusionAsyncResult(fusion);
					}
				).detach();
			}
		}

		return fusion;
	}

	#endif

#pragma endregion Opencv Function

	/**
	 * @brief Get the frame width in pixel
	 * @return Return the frame width
	*/
	int UVCCamera::getWidth()
	{
		return getDirectShowVideoFormat().getWidth();
	}

	/**
	 * @brief Get the frame height in pixel
	 * @return Return the frame height
	*/
	int UVCCamera::getHeight()
	{
		return getDirectShowVideoFormat().getHeight();
	}

	/**
	 * @brief Get frame size in bytes
	 * @return Return the the frame size in bytes
	*/
	int UVCCamera::getFrameSize()
	{
		return getDirectShowVideoFormat().getTotalSize();
	}

	/**
	 * @brief Get the number of bytes per pixel
	 * @return Return the number of bytes per pixel
	*/
	int UVCCamera::getNumOfBytePerPixel()
	{
		return getDirectShowVideoFormat().getBitPerPixel() / 8;
	}

	/**
	 * @brief Get the number of pixel per frame.
	 * @return Return the number of pixel per frame.
	*/
	int UVCCamera::getNumOfPixel()
	{
		return getDirectShowVideoFormat().getWidth() * getDirectShowVideoFormat().getHeight();
	}

#pragma endregion Frame

#pragma region Utils

	/**
	 * @brief Copy error from DirectShowCamera to this
	 * @param success Only copy error if success = false
	*/
	void UVCCamera::copyError(bool success)
	{
		if (!success) m_errorString = m_directShowCamera->getLastError();
	}

	/**
	 * @brief A utils to convert degree to -180 to 180
	 * @param degree Degree to be converted
	 * @return Return a degree within -180 and 180
	*/
	int UVCCamera::confirmDegreeRange(int degree)
	{
		int result = 0;
		if (degree % 360 != 0)
		{
			bool positiveRegion = sin(degree * 0.01745329251994329576923690722222) > 0.0 ? true : false;

			if (degree > 0)
			{
				if (positiveRegion)
				{
					// 0 to 180
					result = degree % 180;
				}
				else
				{
					// 0 to -180
					result = degree % 180 - 180;
				}
			}
			else
			{
				if (positiveRegion)
				{
					// 0 to 180
					result = 180 + degree % 180;
				}
				else
				{
					// 0 to -180
					result = degree % 180;
				}
			}
		}

		return result;
	}

	/**
	 * @brief Convert DirectShow exposure value to seconds
	 * @param dsValue DirectShow exposure value
	 * @return Return exposure in second
	*/
	double UVCCamera::exposureConvertion(long dsValue)
	{
		if (dsValue < 0)
		{
			return pow(0.5, (double)-dsValue);
		}
		else
		{
			return pow(2, (double)dsValue);
		}
	}

	/**
	 * @brief Convert exposure value in second to DirectShow exposure value
	 * @param second Exposure value in second
	 * @return Return DirectShow exposure value
	*/
	long UVCCamera::exposureConvertion(double second)
	{
		if (second <= 0)
		{
			return 0;
		}
		else
		{
			return (long)lround(log2(second));
		}

	}

#pragma endregion Utils
}
#include <ds_camera_properties.h>


namespace DirectShowCamera
{
#pragma region Constructor

	/**
	 * @brief Dummy constructor, use DirectShowCameraProperties(IBaseFilter*, std::string*) instead.
	*/
	DirectShowCameraProperties::DirectShowCameraProperties()
	{
		m_isinitialized = false;
	}

	/**
	 * @brief Constuctor
	 * @param videoInputFilter Video input filter. Property will be load from this filter.
	 * @param errorString Error string
	*/
	DirectShowCameraProperties::DirectShowCameraProperties(IBaseFilter* videoInputFilter, std::string* errorString)
	{
		refresh(videoInputFilter, errorString);
	}

	/**
	 * @brief Destructor
	*/
	DirectShowCameraProperties::~DirectShowCameraProperties()
	{
		delete m_brightness;
		delete m_contrast;
		delete m_hue;
		delete m_saturation;
		delete m_sharpness;
		delete m_gamma;
		delete m_colorEnable;
		delete m_whiteBalance;
		delete m_backlightCompensation;
		delete m_gain;
		delete m_pan;
		delete m_tilt;
		delete m_roll;
		delete m_zoom;
		delete m_exposure;
		delete m_iris;
		delete m_focus;
	}

	/**
	 * @brief Reset variables
	*/
	void DirectShowCameraProperties::reset()
	{
		m_brightness->reset();
		m_contrast->reset();
		m_hue->reset();
		m_saturation->reset();
		m_sharpness->reset();
		m_gamma->reset();
		m_colorEnable->reset();
		m_whiteBalance->reset();
		m_backlightCompensation->reset();
		m_gain->reset();
		m_pan->reset();
		m_tilt->reset();
		m_roll->reset();
		m_zoom->reset();
		m_exposure->reset();
		m_iris->reset();
		m_focus->reset();

		m_isinitialized = false;
	}

	void DirectShowCameraProperties::resetDefault(IBaseFilter* videoInputFilter, bool asAuto)
	{
		m_brightness->setAsDefault(videoInputFilter);
		m_contrast->setAsDefault(videoInputFilter);
		m_hue->setAsDefault(videoInputFilter);
		m_saturation->setAsDefault(videoInputFilter);
		m_sharpness->setAsDefault(videoInputFilter);
		m_gamma->setAsDefault(videoInputFilter);
		m_colorEnable->setAsDefault(videoInputFilter);
		m_whiteBalance->setAsDefault(videoInputFilter);
		m_backlightCompensation->setAsDefault(videoInputFilter);
		m_gain->setAsDefault(videoInputFilter);
		m_pan->setAsDefault(videoInputFilter);
		m_tilt->setAsDefault(videoInputFilter);
		m_roll->setAsDefault(videoInputFilter);
		m_zoom->setAsDefault(videoInputFilter);
		m_exposure->setAsDefault(videoInputFilter);
		m_iris->setAsDefault(videoInputFilter);
		m_focus->setAsDefault(videoInputFilter);

		m_isinitialized = true;
	}

	/**
	 * @brief Refresh properties from video input filter.
	 * @param videoInputFilter Video input filter. Property will be load from this filter.
	 * @param errorString Error string
	*/
	void DirectShowCameraProperties::refresh(IBaseFilter* videoInputFilter, std::string* errorString)
	{
		reset();
		bool success = true;

		// Get properties
		success = DirectShowCameraUtils::amVideoProcAmpDecorator(videoInputFilter,
			[this, errorString](IAMVideoProcAmp* videoProcAmp)
			{
				// Import property
				m_brightness->importProperty(videoProcAmp);
				m_contrast->importProperty(videoProcAmp);
				m_hue->importProperty(videoProcAmp);
				m_saturation->importProperty(videoProcAmp);
				m_sharpness->importProperty(videoProcAmp);
				m_gamma->importProperty(videoProcAmp);
				m_colorEnable->importProperty(videoProcAmp);
				m_whiteBalance->importProperty(videoProcAmp);
				m_backlightCompensation->importProperty(videoProcAmp);
				m_gain->importProperty(videoProcAmp);
			},
			errorString
		);

		success = DirectShowCameraUtils::amCameraControlDecorator(videoInputFilter,
			[this, errorString](IAMCameraControl* cameraControl)
			{
				// Import property
				m_pan->importProperty(cameraControl);
				m_tilt->importProperty(cameraControl);
				m_roll->importProperty(cameraControl);
				m_zoom->importProperty(cameraControl);
				m_exposure->importProperty(cameraControl);
				m_iris->importProperty(cameraControl);
				m_focus->importProperty(cameraControl);
			},
			errorString
		);

		m_isinitialized = true;
	}

	/**
	 * @brief Mark this object as initialized.
	*/
	void DirectShowCameraProperties::markAsInitialized()
	{
		m_isinitialized = true;
	}

#pragma endregion Constructor

#pragma region Getter

	/**
	 * @brief Get brightness, from blanking(small value) to pure white(large value)
	 * @return Return brightness
	*/
	DirectShowCameraProperty* DirectShowCameraProperties::getBrightness()
	{
		return m_brightness;
	}

	/**
	 * @brief Get contrast
	 * @return Return contrast
	*/
	DirectShowCameraProperty* DirectShowCameraProperties::getContrast()
	{
		return m_contrast;
	}

	/**
	 * @brief Get hue (-180 to +180 degrees)
	 * 
	 * @return Return hue
	*/
	DirectShowCameraProperty* DirectShowCameraProperties::getHue()
	{
		return m_hue;
	}

	/**
	 * @brief Get saturation
	 * @return Return saturation
	*/
	DirectShowCameraProperty* DirectShowCameraProperties::getSaturation()
	{
		return m_saturation;
	}

	/**
	 * @brief Get sharpness
	 * @return Return sharpness
	*/
	DirectShowCameraProperty* DirectShowCameraProperties::getSharpness()
	{
		return m_sharpness;
	}

	/**
	 * @brief Get gamma
	 * @return Return gamma
	*/
	DirectShowCameraProperty* DirectShowCameraProperties::getGamma()
	{
		return m_gamma;
	}

	/**
	 * @brief Get color enable, 0(off) or 1(on)
	 * @return 
	*/
	DirectShowCameraProperty* DirectShowCameraProperties::getColorEnable()
	{
		return m_colorEnable;
	}

	/**
	 * @brief Get white balance
	 * @return Return white balance
	*/
	DirectShowCameraProperty* DirectShowCameraProperties::getWhiteBalance()
	{
		return m_whiteBalance;
	}

	/**
	 * @brief Get backlight compensation, 0(off) or 1(on)
	 * @return Return backlight compensation
	*/
	DirectShowCameraProperty* DirectShowCameraProperties::getBacklightCompensation()
	{
		return m_backlightCompensation;
	}

	/**
	 * @brief Get gain, +ve are brighter and -ve are darker
	 * @return Get gain
	*/
	DirectShowCameraProperty* DirectShowCameraProperties::getGain()
	{
		return m_gain;
	}

	/**
	 * @brief Get pan, in degree?
	 * @return Return pan
	*/
	DirectShowCameraProperty* DirectShowCameraProperties::getPan()
	{
		return m_pan;
	}

	/**
	 * @brief Get tilt, in degree?
	 * @return Return tilt
	*/
	DirectShowCameraProperty* DirectShowCameraProperties::getTilt()
	{
		return m_tilt;
	}

	/**
	 * @brief Get roll, in degree?
	 * @return Return roll
	*/
	DirectShowCameraProperty* DirectShowCameraProperties::getRoll()
	{
		return m_roll;
	}

	/**
	 * @brief Get zoom, in millimeters
	 * @return Return zoom
	*/
	DirectShowCameraProperty* DirectShowCameraProperties::getZoom()
	{
		return m_zoom;
	}

	/**
	 * @brief Get exposure, value = log2(sec) which means sec = 0.5^value(value < 0) or 2^value(value > 0) , eg. value = -3 sec = 0.125s, value = 2 sec = 4
	 * @return Return exposure
	*/
	DirectShowCameraProperty* DirectShowCameraProperties::getExposure()
	{
		return m_exposure;
	}

	/**
	 * @brief Get iris, fstop * 10
	 * @return Return iris
	*/
	DirectShowCameraProperty* DirectShowCameraProperties::getIris()
	{
		return m_iris;
	}

	/**
	 * @brief Get focus, in millimeters
	 * @return Return focus
	*/
	DirectShowCameraProperty* DirectShowCameraProperties::getFocus()
	{
		return m_focus;
	}

#pragma endregion Getter
}

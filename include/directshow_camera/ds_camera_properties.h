#pragma once
#ifndef DIRECTSHOW_CAMERA_PROPERTIES_H
#define DIRECTSHOW_CAMERA_PROPERTIES_H

//************Content************

#include <ds_camera_utils.h>

#include <ds_camera_property.h>

#include <string>
#include <iostream>

class DirectShowCameraProperties
{
private:

	// Properties
	DirectShowCameraProperty* m_brightness = new DirectShowCameraProperty("Brightness", VideoProcAmp_Brightness, DirectShowCameraProperty::USE_AM_VIDEO_PROC_AMP);
	DirectShowCameraProperty* m_contrast = new DirectShowCameraProperty("Contrast", VideoProcAmp_Contrast, DirectShowCameraProperty::USE_AM_VIDEO_PROC_AMP);
	DirectShowCameraProperty* m_hue = new DirectShowCameraProperty("Hue", VideoProcAmp_Hue, DirectShowCameraProperty::USE_AM_VIDEO_PROC_AMP);
	DirectShowCameraProperty* m_saturation = new DirectShowCameraProperty("Saturation", VideoProcAmp_Saturation, DirectShowCameraProperty::USE_AM_VIDEO_PROC_AMP);
	DirectShowCameraProperty* m_sharpness = new DirectShowCameraProperty("Sharpness", VideoProcAmp_Sharpness, DirectShowCameraProperty::USE_AM_VIDEO_PROC_AMP);
	DirectShowCameraProperty* m_gamma = new DirectShowCameraProperty("Gamma", VideoProcAmp_Gamma, DirectShowCameraProperty::USE_AM_VIDEO_PROC_AMP);
	DirectShowCameraProperty* m_colorEnable = new DirectShowCameraProperty("Color Enable", VideoProcAmp_ColorEnable, DirectShowCameraProperty::USE_AM_VIDEO_PROC_AMP); // 0(off), 1(on)
	DirectShowCameraProperty* m_whiteBalance = new DirectShowCameraProperty("White Balance", VideoProcAmp_WhiteBalance, DirectShowCameraProperty::USE_AM_VIDEO_PROC_AMP);
	DirectShowCameraProperty* m_backlightCompensation = new DirectShowCameraProperty("Backlight Compensation", VideoProcAmp_BacklightCompensation, DirectShowCameraProperty::USE_AM_VIDEO_PROC_AMP);  // 0(off), 1(on)
	DirectShowCameraProperty* m_gain = new DirectShowCameraProperty("Gain", VideoProcAmp_Gain, DirectShowCameraProperty::USE_AM_VIDEO_PROC_AMP);
	DirectShowCameraProperty* m_pan = new DirectShowCameraProperty("Pan", CameraControl_Pan, DirectShowCameraProperty::USE_AM_CAMERA_CONTROL);
	DirectShowCameraProperty* m_tilt = new DirectShowCameraProperty("Tilt", CameraControl_Tilt, DirectShowCameraProperty::USE_AM_CAMERA_CONTROL);
	DirectShowCameraProperty* m_roll = new DirectShowCameraProperty("Roll", CameraControl_Roll, DirectShowCameraProperty::USE_AM_CAMERA_CONTROL);
	DirectShowCameraProperty* m_zoom = new  DirectShowCameraProperty("Zoom", CameraControl_Zoom, DirectShowCameraProperty::USE_AM_CAMERA_CONTROL);
	DirectShowCameraProperty* m_exposure = new DirectShowCameraProperty("Exposure", CameraControl_Exposure, DirectShowCameraProperty::USE_AM_CAMERA_CONTROL);
	DirectShowCameraProperty* m_iris = new DirectShowCameraProperty("Iris", CameraControl_Iris, DirectShowCameraProperty::USE_AM_CAMERA_CONTROL);
	DirectShowCameraProperty* m_focus = new DirectShowCameraProperty("Focus", CameraControl_Focus, DirectShowCameraProperty::USE_AM_CAMERA_CONTROL);

	bool m_isinitialized = false;

public:

	// Constructor
	DirectShowCameraProperties();
	DirectShowCameraProperties(IBaseFilter* videoInputFilter, std::string* errorString = NULL);
	~DirectShowCameraProperties();

	void reset();
	void resetDefault(IBaseFilter* videoInputFilter, bool asAuto = true);
	void refresh(IBaseFilter* videoInputFilter, std::string* errorString);

	// Getter
	DirectShowCameraProperty* getBrightness();
	DirectShowCameraProperty* getContrast();
	DirectShowCameraProperty* getHue();
	DirectShowCameraProperty* getSaturation();
	DirectShowCameraProperty* getSharpness();
	DirectShowCameraProperty* getGamma();
	DirectShowCameraProperty* getColorEnable();
	DirectShowCameraProperty* getWhiteBalance();
	DirectShowCameraProperty* getBacklightCompensation();
	DirectShowCameraProperty* getGain();
	DirectShowCameraProperty* getPan();
	DirectShowCameraProperty* getTilt();
	DirectShowCameraProperty* getRoll();
	DirectShowCameraProperty* getZoom();
	DirectShowCameraProperty* getExposure();
	DirectShowCameraProperty* getIris();
	DirectShowCameraProperty* getFocus();

	// Class to string

	operator std::string() const
	{
		std::string result;
		if (m_isinitialized)
		{
			result = "---Camera Properties---\n";
			result += (std::string)*m_brightness + "\n";
			result += (std::string)*m_contrast + "\n";
			result += (std::string)*m_hue + "\n";
			result += (std::string)*m_saturation + "\n";
			result += (std::string)*m_sharpness + "\n";
			result += (std::string)*m_gamma + "\n";
			result += (std::string)*m_colorEnable + "\n";
			result += (std::string)*m_whiteBalance + "\n";
			result += (std::string)*m_backlightCompensation + "\n";
			result += (std::string)*m_gain + "\n";
			result += (std::string)*m_pan + "\n";
			result += (std::string)*m_tilt + "\n";
			result += (std::string)*m_roll + "\n";
			result += (std::string)*m_zoom + "\n";
			result += (std::string)*m_exposure + "\n";
			result += (std::string)*m_iris + "\n";
			result += (std::string)*m_focus;
		}
		else
		{
			result = "Camera Properties(Not initialized)";
		}

		return result;
	}

	friend std::ostream& operator << (std::ostream& out, const DirectShowCameraProperties& obj) {
		return out << (std::string)obj;
	}
};

//*******************************

#endif
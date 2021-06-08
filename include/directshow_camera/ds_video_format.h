#pragma once
#ifndef DIRECTSHOW_CAMERA_RESOLUTION_H
#define DIRECTSHOW_CAMERA_RESOLUTION_H

//************Content************

#include <ds_camera_utils.h>

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

namespace DirectShowCamera
{
	/**
	 * @brief A class to store the Directshow Video Format information
	 * 
	 */
	class DirectShowVideoFormat
	{
	// Static function
	public:

		static void sortAndUnique(std::vector<DirectShowVideoFormat>* directShowVideoFormats);
		static bool getVideoFormats(IAMStreamConfig* streamConfig, std::vector<DirectShowVideoFormat*>* videoFormat, bool keepAmMediaType, std::string* errorString = NULL, std::vector<GUID>* supportVideoTypes = NULL);
		static void release(std::vector<DirectShowVideoFormat*>* m_resolutions);

		static std::string to_string(std::vector<DirectShowVideoFormat>* directShowVideoFormats, bool detail = false);
		static std::string to_string(AM_MEDIA_TYPE* amMediaType);
		static std::string to_string(GUID guid);

		static std::vector<GUID> getMonochromeSubType();
		static bool isMonochrome(GUID guid);
		static std::vector<GUID> getSupportRGBSubType();
		static bool supportRGBConvertion(GUID guid);

	private:
		AM_MEDIA_TYPE* m_AmMediaType = NULL;
		GUID m_videoType = MEDIASUBTYPE_None;
		int m_bitPerPixel = 0;
		long m_totalSize = 0;
		int m_width = 0;
		int m_height = 0;
		bool m_isEmpty = true;

	public:

		// Constuctor
		DirectShowVideoFormat();
		DirectShowVideoFormat(GUID mediaType, int width, int height, int bitPerPixel, int totalSize);
		DirectShowVideoFormat(AM_MEDIA_TYPE* m_AmMediaType, bool keepAmMediaType);
		DirectShowVideoFormat(IAMStreamConfig* streamConfig, int videoFormatIndex, bool keepAmMediaType);
		void constructor(AM_MEDIA_TYPE* m_AmMediaType, bool keepAmMediaType);

		DirectShowVideoFormat(const DirectShowVideoFormat& directShowVideoFormat);

		~DirectShowVideoFormat();

		DirectShowVideoFormat clone(bool keepAmMediaType);

		// Getter
		bool isEmpty();
		int getWidth();
		int getHeight();
		int getBitPerPixel();
		long getTotalSize();
		GUID getVideoType();
		AM_MEDIA_TYPE* getAMMediaType();

		// Operator

		DirectShowVideoFormat& operator=(const DirectShowVideoFormat& directShowVideoFormat);

		bool operator < (const DirectShowVideoFormat& videoFormat) const;
		bool operator > (const DirectShowVideoFormat& videoFormat) const;

		bool operator == (const DirectShowVideoFormat& videoFormat) const;
		bool operator == (const AM_MEDIA_TYPE& am_MediaType) const;

		bool operator != (const DirectShowVideoFormat& videoFormat) const;
		bool operator != (const AM_MEDIA_TYPE& am_MediaType) const;

		// To string

		operator std::string() const
		{
			std::string result;
			if (!m_isEmpty)
			{
				result = "(" + std::to_string(m_width) + " x " + std::to_string(m_height) + " x " + std::to_string(m_bitPerPixel) + "bits )" + to_string(m_videoType);
			}
			else
			{
				result = "Empty video format.";
			}

			return result;
		}

		friend std::ostream& operator << (std::ostream& out, const DirectShowVideoFormat& obj) {
			return out << (std::string)obj;
		}
	};
}

//*******************************

#endif

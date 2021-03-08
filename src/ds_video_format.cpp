#include <ds_video_format.h>

#pragma region Static Function

/**
 * @brief Sort and unique the video format list.
 * @param[in, out] directShowVideoFormats Input the video format list, and sort it.
*/
void DirectShowVideoFormat::sortAndUnique(std::vector<DirectShowVideoFormat>* directShowVideoFormats)
{
	// Remove duplicates
	std::vector<DirectShowVideoFormat> videoFormatTemp;
	for (int i = 0; i < directShowVideoFormats->size(); i++)
	{
		// Check
		bool found = false;
		for (int j = i+1; j < directShowVideoFormats->size(); j++)
		{
			if (directShowVideoFormats->at(i) == directShowVideoFormats->at(j))
			{
				found = true;
				break;
			}
		}

		// Add to temp
		if (!found)
		{
			videoFormatTemp.push_back(directShowVideoFormats->at(i));
		}
	}

	// Sort
	directShowVideoFormats->assign(videoFormatTemp.begin(), videoFormatTemp.end());
	std::sort(directShowVideoFormats->begin(), directShowVideoFormats->end());
}

/**
 * @brief Collect all video format from IAMStreamConfig
 * @param[in] streamConfig IAMStreamConfig
 * @param[out] videoFormat Video format
 * @param[in] keepAmMediaType Set as true if you want to keep the AmMediaType
 * @param[out] errorString (Option) Error string, set as NULL to disabled.
 * @param[out] supportVideoTypes (Option) Support video type. It will only add Video format matched supportVideoTypes, set as NULL to disabled.
 * @return Return true if success.
*/
bool DirectShowVideoFormat::getVideoFormats(IAMStreamConfig* streamConfig, std::vector<DirectShowVideoFormat*>* videoFormats, bool keepAmMediaType, std::string* errorString, std::vector<GUID>* supportVideoTypes)
{
	HRESULT hr = NO_ERROR;
	bool result = true;
	int numOfResolution = 0;
	int configSize = 0;

	// Get number of resolution
	hr = streamConfig->GetNumberOfCapabilities(&numOfResolution, &configSize);
	result = DirectShowCameraUtils::checkIAMSCGetNumberOfCapabilitiesResult(hr, errorString, "Error on checking number of resolution");

	if (result)
	{
		// Confirm that this is a video structure.
		if (configSize == sizeof(VIDEO_STREAM_CONFIG_CAPS))
		{
			// Use the video capabilities structure.
			for (int i = 0; i < numOfResolution; i++)
			{
                DirectShowVideoFormat* videoFormat = new DirectShowVideoFormat(streamConfig, i, keepAmMediaType);

				// Check empty and suppport video types
				bool addVideoFormat = true;
				if (videoFormat->isEmpty())
				{
					addVideoFormat = false;
				}
				else if(supportVideoTypes)
				{
					// Check support video types
					if (std::count(supportVideoTypes->begin(), supportVideoTypes->end(), videoFormat->m_videoType) <= 0)
					{
						addVideoFormat = false;
					}
				}

				if (addVideoFormat)
				{

					// Add
					videoFormats->push_back(videoFormat);
				}
				else
				{
					delete videoFormat;
				}					
			}

			// Todo: sort and unique videoFormats(std::vector<DirectShowVideoFormat*>*) here. how to handle the m_AmMediaType(AM_MEDIA_TYPE*) ?
		}
	}

	return result;
}

/**
 * @brief Release memory of the video formats
 * @param videoFormats Video format to be release
*/
void DirectShowVideoFormat::release(std::vector<DirectShowVideoFormat*>* videoFormats)
{
	if (videoFormats && videoFormats->size() > 0)
	{
		for (int i=0;i< videoFormats->size();i++)
		{
			delete videoFormats->at(i);
			videoFormats->assign(i, NULL);
		}

		videoFormats->clear();
	}
}

/**
 * @brief Convert Media type and subtype GUID to description.
 * @param guid GUID
 * @return Return the description
*/
std::string DirectShowVideoFormat::to_string(GUID guid)
{
	std::string result;
	if (guid == MEDIATYPE_AnalogAudio)			result = "Analog audio.";
	else if (guid == MEDIATYPE_AnalogVideo)	result = "Analog video.";
	else if (guid == MEDIATYPE_Audio)			result = "Audio.";
	else if (guid == MEDIATYPE_AUXLine21Data)	result = "Line 21 data. Used by closed captions.";
	else if (guid == MEDIATYPE_File)			result = "File. (Obsolete)";
	else if (guid == MEDIATYPE_Interleaved)	result = "Interleaved audio and video. Used for Digital Video (DV).";
	else if (guid == MEDIATYPE_LMRT)			result = "Obsolete. Do not use.";
	else if (guid == MEDIATYPE_Midi)			result = "MIDI format.";
	else if (guid == MEDIATYPE_MPEG2_PES)		result = "MPEG-2 PES packets.";
	else if (guid == MEDIATYPE_MPEG2_SECTIONS)	result = "MPEG-2 section data";
	else if (guid == MEDIATYPE_ScriptCommand)	result = "Data is a script command, used by closed captions.";
	else if (guid == MEDIATYPE_Stream)			result = "Byte stream with no time stamps.";
	else if (guid == MEDIATYPE_Text)			result = "Text.";
	else if (guid == MEDIATYPE_Timecode)		result = "Timecode data. Note: DirectShow does not provide any filters that support this media type.";
	else if (guid == MEDIATYPE_URL_STREAM)		result = "Obsolete. Do not use.";
	else if (guid == MEDIATYPE_Video)			result = "Video.";
	else if (guid == MEDIASUBTYPE_None)						result += "None.";
	else if (guid == MEDIASUBTYPE_PCM)					result += "PCM audio.";
	else if (guid == MEDIASUBTYPE_MPEG1Packet)			result += "MPEG1 Audio packet.";
	else if (guid == MEDIASUBTYPE_MPEG1Payload)			result += "MPEG1 Audio Payload.";
	else if (guid == MEDIASUBTYPE_MPEG2_AUDIO)			result += "MPEG-2 audio data.";
	else if (guid == MEDIASUBTYPE_DVD_LPCM_AUDIO)		result += "DVD audio data.";
	else if (guid == MEDIASUBTYPE_MPEG2_AUDIO)			result += "MPEG-2 audio data.";
	else if (guid == MEDIASUBTYPE_DRM_Audio)				result += "Corresponds to WAVE_FORMAT_DRM.";
	else if (guid == MEDIASUBTYPE_IEEE_FLOAT)			result += "Corresponds to WAVE_FORMAT_IEEE_FLOAT.";
	else if (guid == MEDIASUBTYPE_DOLBY_AC3)				result += "Dolby data.";
	else if (guid == MEDIASUBTYPE_DOLBY_AC3_SPDIF)		result += "Dolby AC3 over SPDIF.";
	else if (guid == MEDIASUBTYPE_RAW_SPORT)				result += "Equivalent to MEDIASUBTYPE_DOLBY_AC3_SPDIF.";
	else if (guid == MEDIASUBTYPE_SPDIF_TAG_241h)		result += "Equivalent to MEDIASUBTYPE_DOLBY_AC3_SPDIF.";
	else if (guid == MEDIASUBTYPE_AIFF)					result += "Data from AIFF file.";
	else if (guid == MEDIASUBTYPE_Asf)					result += "Advanced Systems Format (ASF).";
	else if (guid == MEDIASUBTYPE_Avi)					result += "Data from AVI file.";
	else if (guid == MEDIASUBTYPE_AU)					result += "Data from AU file.";
	else if (guid == MEDIASUBTYPE_DssAudio)				result += "Dss Audio.";
	else if (guid == MEDIASUBTYPE_DssVideo)				result += "Dss Video.";
	else if (guid == MEDIASUBTYPE_MPEG1Audio)			result += "MPEG audio.";
	else if (guid == MEDIASUBTYPE_MPEG1System)			result += "MPEG system.";
	else if (guid == MEDIASUBTYPE_MPEG1Video)			result += "MPEG video.";
	else if (guid == MEDIASUBTYPE_MPEG1VideoCD)			result += "MPEG video CD.";
	else if (guid == MEDIASUBTYPE_WAVE)					result += "Data from WAV file.";
	else if (guid == MEDIASUBTYPE_AnalogVideo_NTSC_M)	result += "NSTC.";
	else if (guid == MEDIASUBTYPE_AnalogVideo_PAL_B)		result += "B PAL.";
	else if (guid == MEDIASUBTYPE_AnalogVideo_PAL_D)		result += "D PAL.";
	else if (guid == MEDIASUBTYPE_AnalogVideo_PAL_G)		result += "G PAL.";
	else if (guid == MEDIASUBTYPE_AnalogVideo_PAL_H)		result += "H PAL.";
	else if (guid == MEDIASUBTYPE_AnalogVideo_PAL_I)		result += "I PAL.";
	else if (guid == MEDIASUBTYPE_AnalogVideo_PAL_M)		result += "M PAL.";
	else if (guid == MEDIASUBTYPE_AnalogVideo_PAL_N)		result += "N PAL.";
	else if (guid == MEDIASUBTYPE_AnalogVideo_SECAM_B)	result += "B SECAM.";
	else if (guid == MEDIASUBTYPE_AnalogVideo_SECAM_D)	result += "D SECAM.";
	else if (guid == MEDIASUBTYPE_AnalogVideo_SECAM_G)	result += "G SECAM.";
	else if (guid == MEDIASUBTYPE_AnalogVideo_SECAM_H)	result += "H SECAM.";
	else if (guid == MEDIASUBTYPE_AnalogVideo_SECAM_K)	result += "K SECAM.";
	else if (guid == MEDIASUBTYPE_AnalogVideo_SECAM_K1)	result += "K1 SECAM.";
	else if (guid == MEDIASUBTYPE_AnalogVideo_SECAM_L)	result += "L SECAM.";
	else if (guid == MEDIASUBTYPE_AI44)					result += "DirectX Video Acceleration Video guid. For subpicture and text overlays.";
	else if (guid == MEDIASUBTYPE_IA44)					result += "DirectX Video Acceleration Video guid. For subpicture and text overlays.";
	else if (guid == MEDIASUBTYPE_dvsl)					result += "SD-DVCR (525-60 or 625-50), Datarate: 12.5 Mbps.";
	else if (guid == MEDIASUBTYPE_dvsd)					result += "SDL-DVCR (525-60 or 625-50), Datarate: 25 Mbps.";
	else if (guid == MEDIASUBTYPE_dvhd)					result += "HD-DVCR (1125-60 or 1250-50).";
	else if (guid == MEDIASUBTYPE_dv25)					result += " DVCPRO 25 (525-60 or 625-50).";
	else if (guid == MEDIASUBTYPE_dv50)					result += "DVCPRO 50 (525-60 or 625-50).";
	else if (guid == MEDIASUBTYPE_dvh1)					result += "DVCPRO 100 (1080/60i, 1080/50i, or 720/60P).";
	else if (guid == MEDIASUBTYPE_DVCS)					result += "DVCS.";
	else if (guid == MEDIASUBTYPE_DVSD)					result += "DVSD.";
	else if (guid == MEDIASUBTYPE_RGB1)					result += "RGB, 1 bit per pixel (bpp), palettized.";
	else if (guid == MEDIASUBTYPE_RGB4)					result += "RGB, 4 bpp, palettized.";
	else if (guid == MEDIASUBTYPE_RGB8)					result += "RGB, 8 bpp.";
	else if (guid == MEDIASUBTYPE_RGB555)				result += "RGB 555, 16 bpp.";
	else if (guid == MEDIASUBTYPE_RGB565)				result += "RGB 565, 16 bpp.";
	else if (guid == MEDIASUBTYPE_RGB24)					result += "RGB, 24 bpp.";
	else if (guid == MEDIASUBTYPE_RGB32)					result += "RGB, 32 bpp.";
	else if (guid == MEDIASUBTYPE_ARGB1555)				result += "RGB 555 with alpha channel.";
	else if (guid == MEDIASUBTYPE_ARGB32)				result += "RGB 32 with alpha channel.";
	else if (guid == MEDIASUBTYPE_ARGB4444)				result += "16-bit RGB with alpha channel; 4 bits per channel.";
	else if (guid == MEDIASUBTYPE_A2R10G10B10)			result += "32-bit RGB with alpha channel; 10 bits per RGB channel plus 2 bits for alpha. ";
	else if (guid == MEDIASUBTYPE_A2B10G10R10)			result += "32-bit RGB with alpha channel; 10 bits per RGB channel plus 2 bits for alpha. ";
	else if (guid == MEDIASUBTYPE_RGB32_D3D_DX7_RT)		result += "VMR-7 Subtypes, 32-bit RGB render target.";
	else if (guid == MEDIASUBTYPE_RGB16_D3D_DX7_RT)		result += "VMR-7 Subtypes, 16-bit RGB render target.";
	else if (guid == MEDIASUBTYPE_ARGB32_D3D_DX7_RT)		result += "VMR-7 Subtypes, 32-bit ARGB render target.";
	else if (guid == MEDIASUBTYPE_ARGB4444_D3D_DX7_RT)	result += "VMR-7 Subtypes, ARGB4444 render target. For subpicture graphics.";
	else if (guid == MEDIASUBTYPE_ARGB1555_D3D_DX7_RT)	result += "VMR-7 Subtypes, ARGB1555 render target. For subpicture graphics.";
	else if (guid == MEDIASUBTYPE_RGB32_D3D_DX9_RT)		result += "VMR-9 Subtypes, 32-bit RGB render target.";
	else if (guid == MEDIASUBTYPE_RGB16_D3D_DX9_RT)		result += "VMR-9 Subtypes, 16-bit RGB render target.";
	else if (guid == MEDIASUBTYPE_ARGB32_D3D_DX9_RT)		result += "VMR-9 Subtypes, 32-bit ARGB render target.";
	else if (guid == MEDIASUBTYPE_ARGB4444_D3D_DX9_RT)	result += "VMR-9 Subtypes, ARGB4444 render target. For subpicture graphics.";
	else if (guid == MEDIASUBTYPE_ARGB1555_D3D_DX9_RT)	result += "VMR-9 Subtypes, ARGB1555 render target. For subpicture graphics.";
	else if (guid == MEDIASUBTYPE_AYUV)					result += "Format:AYUV, Sampling:4:4:4, Packed, 8 Bits per channel.";
	else if (guid == MEDIASUBTYPE_YUY2)					result += "Format:YUY2, Sampling:4:2:2, Packed, 8 Bits per channel.";
	else if (guid == MEDIASUBTYPE_UYVY)					result += "Format:UYVY, Sampling:4:2:2, Packed, 8 Bits per channel.";
	else if (guid == MEDIASUBTYPE_IMC1)					result += "Format:IMC1, Sampling:4:2:0, Planar, 8 Bits per channel.";
	else if (guid == MEDIASUBTYPE_IMC2)					result += "(Format:IMC2, Sampling:4:2:0, Planar, 8 Bits per channel.";
	else if (guid == MEDIASUBTYPE_IMC3)					result += "Format:IMC3, Sampling:4:2:0, Planar, 8 Bits per channel.";
	else if (guid == MEDIASUBTYPE_IMC4)					result += "Format:IMC4, Sampling:4:2:0, Planar, 8 Bits per channel.";
	else if (guid == MEDIASUBTYPE_YV12)					result += "Format:YV12, Sampling:4:2:0, Planar, 8 Bits per channel.";
	else if (guid == MEDIASUBTYPE_NV12)					result += "Format:NV12, Sampling:4:2:0, Planar, 8 Bits per channel.";
	else if (guid == MEDIASUBTYPE_Y411)					result += "Format:Y411, Sampling:4:1:1, Packed, 8 Bits per channel.";
	else if (guid == MEDIASUBTYPE_Y41P)					result += "Format:Y41P, Sampling:4:1:1, Packed, 8 Bits per channel.";
	else if (guid == MEDIASUBTYPE_Y211)					result += "Format:Y211, Packed, 8 Bits per channel.";
	else if (guid == MEDIASUBTYPE_YVYU)					result += "Format:YVYU, Sampling:4:2:2, Packed, 8 Bits per channel.";
	else if (guid == MEDIASUBTYPE_YVU9)					result += "Format:YVU9, Planar, 8 Bits per channel.";
	else if (guid == MEDIASUBTYPE_IF09)					result += "Format:Indeo YVU9, Planar, 8 Bits per channel.";
	else if (guid == MEDIASUBTYPE_CFCC)					result += "MJPG format produced by some cards. (FOURCC 'CFCC').";
	else if (guid == MEDIASUBTYPE_CLJR)					result += "Cirrus Logic CLJR format. (FOURCC 'CLJR').";
	else if (guid == MEDIASUBTYPE_CPLA)					result += "Cinepak UYVY format. (FOURCC 'CPLA').";
	else if (guid == MEDIASUBTYPE_CLPL)					result += "A YUV format supported by some Cirrus Logic drivers. (FOURCC 'CLPL').";
	else if (guid == MEDIASUBTYPE_IJPG)					result += "Intergraph JPEG format. (FOURCC 'IJPG').";
	else if (guid == MEDIASUBTYPE_MDVF)					result += "A DV encoding format. (FOURCC 'MDVF').";
	else if (guid == MEDIASUBTYPE_MJPG)					result += "Motion JPEG (MJPG) compressed video. (FOURCC 'MJPG').";
	else if (guid == MEDIASUBTYPE_MPEG1Packet)			result += "MPEG1 Video Packet.";
	else if (guid == MEDIASUBTYPE_MPEG1Payload)			result += "MPEG1 Video Payload.";
	else if (guid == MEDIASUBTYPE_Overlay)				result += "Video delivered using hardware overlay.";
	else if (guid == MEDIASUBTYPE_Plum)					result += "(Plum MJPG format. (FOURCC 'Plum').";
	else if (guid == MEDIASUBTYPE_QTJpeg)				result += "QuickTime JPEG compressed data.";
	else if (guid == MEDIASUBTYPE_QTMovie)				result += "Apple QuickTime compression.";
	else if (guid == MEDIASUBTYPE_QTRle)					result += "QuickTime RLE compressed data.";
	else if (guid == MEDIASUBTYPE_QTRpza)				result += "QuickTime RPZA compressed data.";
	else if (guid == MEDIASUBTYPE_QTSmc)					result += "QuickTime SMC compressed data.";
	else if (guid == MEDIASUBTYPE_TVMJ)					result += "TrueVision MJPG format. (FOURCC 'TVMJ').";
	else if (guid == MEDIASUBTYPE_VPVBI)					result += "Video port vertical blanking interval (VBI) data.";
	else if (guid == MEDIASUBTYPE_VPVideo)				result += "Video port video data.";
	else if (guid == MEDIASUBTYPE_WAKE)					result += "MJPG format produced by some cards. (FOURCC 'WAKE').";

	return result;
}

/**
 * @brief Convert the DirectShowVideoFormat list to string.
 * @param directShowVideoFormats The list of DirectShowVideoFormat
 * @param printAMMediaTypeDetail Set as true if you want to print the detail of the AM_MediaType.
 * @return Return the converted string.
*/
std::string DirectShowVideoFormat::to_string(std::vector<DirectShowVideoFormat>* directShowVideoFormats, bool printAMMediaTypeDetail)
{
	if (directShowVideoFormats && directShowVideoFormats->size() > 0)
	{
		std::string result = "\n";
		for (int i = 0; i < directShowVideoFormats->size(); i++)
		{
			result += "Index: " + std::to_string(i) + "\n";

			AM_MEDIA_TYPE* amMediaType = directShowVideoFormats->at(i).getAMMediaType();
			if (printAMMediaTypeDetail && !amMediaType)
			{				
				result += DirectShowVideoFormat::to_string(amMediaType);
			}
			else
			{
				result += std::string(directShowVideoFormats->at(i)) + "\n";
			}			
		}

		return result;
	}
	else
	{
		return "null";
	}
	
}

/**
 * @brief Convert AM_MEDIA_TYPE to string
 * @param amMediaType AM_MEDIA_TYPE
 * @return Return the converted string
*/
std::string DirectShowVideoFormat::to_string(AM_MEDIA_TYPE* amMediaType)
{
	if (amMediaType == NULL)
	{
		return "null";
	}
	else
	{
		std::string result = "------AM_MEDIA_TYPE------\n";

		// Major Type
		result += "Major Type : ";
		GUID majortype = amMediaType->majortype;
		if (majortype == MEDIATYPE_AnalogAudio)			result += "(MEDIATYPE_AnalogAudio) " + to_string(majortype);
		else if (majortype == MEDIATYPE_AnalogVideo)	result += "(MEDIATYPE_AnalogVideo) " + to_string(majortype);
		else if (majortype == MEDIATYPE_Audio)			result += "(MEDIATYPE_Audio) " + to_string(majortype);
		else if (majortype == MEDIATYPE_AUXLine21Data)	result += "(MEDIATYPE_AUXLine21Data) " + to_string(majortype);
		else if (majortype == MEDIATYPE_File)			result += "(MEDIATYPE_File) " + to_string(majortype);
		else if (majortype == MEDIATYPE_Interleaved)	result += "(MEDIATYPE_Interleaved) " + to_string(majortype);
		else if (majortype == MEDIATYPE_LMRT)			result += "(MEDIATYPE_LMRT) " + to_string(majortype);
		else if (majortype == MEDIATYPE_Midi)			result += "(MEDIATYPE_Midi) " + to_string(majortype);
		else if (majortype == MEDIATYPE_MPEG2_PES)		result += "(MEDIATYPE_MPEG2_PES) " + to_string(majortype);
		else if (majortype == MEDIATYPE_MPEG2_SECTIONS)	result += "(MEDIATYPE_MPEG2_SECTIONS) " + to_string(majortype);
		else if (majortype == MEDIATYPE_ScriptCommand)	result += "(MEDIATYPE_ScriptCommand)" + to_string(majortype);
		else if (majortype == MEDIATYPE_Stream)			result += "(MEDIATYPE_Stream) " + to_string(majortype);
		else if (majortype == MEDIATYPE_Text)			result += "(MEDIATYPE_Text) " + to_string(majortype);
		else if (majortype == MEDIATYPE_Timecode)		result += "(MEDIATYPE_Timecode) " + to_string(majortype);
		else if (majortype == MEDIATYPE_URL_STREAM)		result += "(MEDIATYPE_URL_STREAM) " + to_string(majortype);
		else if (majortype == MEDIATYPE_Video)			result += "(MEDIATYPE_Video) " + to_string(majortype);

		else result += DirectShowCameraUtils::to_string(majortype);
		result += "\n";

		// Sub Type
		result += "Sub Type : ";
		GUID subtype = amMediaType->subtype;
		if (subtype == MEDIASUBTYPE_None)						result += "(MEDIASUBTYPE_None) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_PCM)					result += "(MEDIASUBTYPE_PCM) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_MPEG1Packet)			result += "(MEDIASUBTYPE_MPEG1Packet) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_MPEG1Payload)			result += "(MEDIASUBTYPE_MPEG1Payload) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_MPEG2_AUDIO)			result += "(MEDIASUBTYPE_MPEG2_AUDIO) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_DVD_LPCM_AUDIO)		result += "(MEDIASUBTYPE_DVD_LPCM_AUDIO) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_MPEG2_AUDIO)			result += "(MEDIASUBTYPE_MPEG2_AUDIO) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_DRM_Audio)				result += "(MEDIASUBTYPE_DRM_Audio) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_IEEE_FLOAT)			result += "(MEDIASUBTYPE_IEEE_FLOAT) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_DOLBY_AC3)				result += "(MEDIASUBTYPE_DOLBY_AC3) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_DOLBY_AC3_SPDIF)		result += "(MEDIASUBTYPE_DOLBY_AC3_SPDIF) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_RAW_SPORT)				result += "(MEDIASUBTYPE_RAW_SPORT) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_SPDIF_TAG_241h)		result += "(MEDIASUBTYPE_SPDIF_TAG_241h) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_AIFF)					result += "(MEDIASUBTYPE_AIFF) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_Asf)					result += "(MEDIASUBTYPE_Asf) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_Avi)					result += "(MEDIASUBTYPE_Avi) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_AU)					result += "(MEDIASUBTYPE_AU) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_DssAudio)				result += "(MEDIASUBTYPE_DssAudio) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_DssVideo)				result += "(MEDIASUBTYPE_DssVideo) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_MPEG1Audio)			result += "(MEDIASUBTYPE_MPEG1Audio) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_MPEG1System)			result += "(MEDIASUBTYPE_MPEG1System) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_MPEG1Video)			result += "(MEDIASUBTYPE_MPEG1Video) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_MPEG1VideoCD)			result += "(MEDIASUBTYPE_MPEG1VideoCD) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_WAVE)					result += "(MEDIASUBTYPE_WAVE) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_AnalogVideo_NTSC_M)	result += "(MEDIASUBTYPE_AnalogVideo_NTSC_M) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_AnalogVideo_PAL_B)		result += "(MEDIASUBTYPE_AnalogVideo_PAL_B) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_AnalogVideo_PAL_D)		result += "(MEDIASUBTYPE_AnalogVideo_PAL_D) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_AnalogVideo_PAL_G)		result += "(MEDIASUBTYPE_AnalogVideo_PAL_G) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_AnalogVideo_PAL_H)		result += "(MEDIASUBTYPE_AnalogVideo_PAL_H) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_AnalogVideo_PAL_I)		result += "(MEDIASUBTYPE_AnalogVideo_PAL_I) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_AnalogVideo_PAL_M)		result += "(MEDIASUBTYPE_AnalogVideo_PAL_M) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_AnalogVideo_PAL_N)		result += "(MEDIASUBTYPE_AnalogVideo_PAL_N) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_AnalogVideo_SECAM_B)	result += "(MEDIASUBTYPE_AnalogVideo_SECAM_B) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_AnalogVideo_SECAM_D)	result += "(MEDIASUBTYPE_AnalogVideo_SECAM_D) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_AnalogVideo_SECAM_G)	result += "(MEDIASUBTYPE_AnalogVideo_SECAM_G) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_AnalogVideo_SECAM_H)	result += "(MEDIASUBTYPE_AnalogVideo_SECAM_H) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_AnalogVideo_SECAM_K)	result += "(MEDIASUBTYPE_AnalogVideo_SECAM_K) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_AnalogVideo_SECAM_K1)	result += "(MEDIASUBTYPE_AnalogVideo_SECAM_K1) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_AnalogVideo_SECAM_L)	result += "(MEDIASUBTYPE_AnalogVideo_SECAM_L) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_AI44)					result += "(MEDIASUBTYPE_AI44) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_IA44)					result += "(MEDIASUBTYPE_IA44) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_dvsl)					result += "(MEDIASUBTYPE_dvsl) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_dvsd)					result += "(MEDIASUBTYPE_dvsd) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_dvhd)					result += "(MEDIASUBTYPE_dvhd) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_dv25)					result += "(MEDIASUBTYPE_dv25) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_dv50)					result += "(MEDIASUBTYPE_dv50) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_dvh1)					result += "(MEDIASUBTYPE_dvh1) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_DVCS)					result += "(MEDIASUBTYPE_DVCS) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_DVSD)					result += "(MEDIASUBTYPE_DVSD) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_RGB1)					result += "(MEDIASUBTYPE_RGB1) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_RGB4)					result += "(MEDIASUBTYPE_RGB4) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_RGB8)					result += "(MEDIASUBTYPE_RGB8) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_RGB555)				result += "(MEDIASUBTYPE_RGB555) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_RGB565)				result += "(MEDIASUBTYPE_RGB565) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_RGB24)					result += "(MEDIASUBTYPE_RGB24) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_RGB32)					result += "(MEDIASUBTYPE_RGB32) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_ARGB1555)				result += "(MEDIASUBTYPE_ARGB1555) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_ARGB32)				result += "(MEDIASUBTYPE_ARGB32) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_ARGB4444)				result += "(MEDIASUBTYPE_ARGB4444) 	" + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_A2R10G10B10)			result += "(MEDIASUBTYPE_A2R10G10B10) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_A2B10G10R10)			result += "(MEDIASUBTYPE_A2B10G10R10) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_RGB32_D3D_DX7_RT)		result += "(MEDIASUBTYPE_RGB32_D3D_DX7_RT) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_RGB16_D3D_DX7_RT)		result += "(MEDIASUBTYPE_RGB16_D3D_DX7_RT) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_ARGB32_D3D_DX7_RT)		result += "(MEDIASUBTYPE_ARGB32_D3D_DX7_RT) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_ARGB4444_D3D_DX7_RT)	result += "(MEDIASUBTYPE_ARGB4444_D3D_DX7_RT) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_ARGB1555_D3D_DX7_RT)	result += "(MEDIASUBTYPE_ARGB1555_D3D_DX7_RT) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_RGB32_D3D_DX9_RT)		result += "(MEDIASUBTYPE_RGB32_D3D_DX9_RT) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_RGB16_D3D_DX9_RT)		result += "(MEDIASUBTYPE_RGB16_D3D_DX9_RT) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_ARGB32_D3D_DX9_RT)		result += "(MEDIASUBTYPE_ARGB32_D3D_DX9_RT) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_ARGB4444_D3D_DX9_RT)	result += "(MEDIASUBTYPE_ARGB4444_D3D_DX9_RT) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_ARGB1555_D3D_DX9_RT)	result += "(MEDIASUBTYPE_ARGB1555_D3D_DX9_RT) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_AYUV)					result += "(MEDIASUBTYPE_AYUV) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_YUY2)					result += "(MEDIASUBTYPE_YUY2) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_UYVY)					result += "(MEDIASUBTYPE_UYVY) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_IMC1)					result += "(MEDIASUBTYPE_IMC1) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_IMC2)					result += "(MEDIASUBTYPE_IMC2) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_IMC3)					result += "(MEDIASUBTYPE_IMC3) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_IMC4)					result += "(MEDIASUBTYPE_IMC4) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_YV12)					result += "(MEDIASUBTYPE_YV12) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_NV12)					result += "(MEDIASUBTYPE_NV12) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_Y411)					result += "(MEDIASUBTYPE_Y411) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_Y41P)					result += "(MEDIASUBTYPE_Y41P) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_Y211)					result += "(MEDIASUBTYPE_Y211) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_YVYU)					result += "(MEDIASUBTYPE_YVYU) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_YVU9)					result += "(MEDIASUBTYPE_YVU9) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_IF09)					result += "(MEDIASUBTYPE_IF09) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_CFCC)					result += "(MEDIASUBTYPE_CFCC) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_CLJR)					result += "(MEDIASUBTYPE_CLJR) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_CPLA)					result += "(MEDIASUBTYPE_CPLA) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_CLPL)					result += "(MEDIASUBTYPE_CLPL) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_IJPG)					result += "(MEDIASUBTYPE_IJPG) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_MDVF)					result += "(MEDIASUBTYPE_MDVF) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_MJPG)					result += "(MEDIASUBTYPE_MJPG) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_MPEG1Packet)			result += "(MEDIASUBTYPE_MPEG1Packet) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_MPEG1Payload)			result += "(MEDIASUBTYPE_MPEG1Payload) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_Overlay)				result += "(MEDIASUBTYPE_Overlay) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_Plum)					result += "(MEDIASUBTYPE_Plum) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_QTJpeg)				result += "(MEDIASUBTYPE_QTJpeg) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_QTMovie)				result += "(MEDIASUBTYPE_QTMovie) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_QTRle)					result += "(MEDIASUBTYPE_QTRle) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_QTRpza)				result += "(MEDIASUBTYPE_QTRpza) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_QTSmc)					result += "(MEDIASUBTYPE_QTSmc) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_TVMJ)					result += "(MEDIASUBTYPE_TVMJ) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_VPVBI)					result += "(MEDIASUBTYPE_VPVBI) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_VPVideo)				result += "(MEDIASUBTYPE_VPVideo) " + to_string(subtype);
		else if (subtype == MEDIASUBTYPE_WAKE)					result += "(MEDIASUBTYPE_WAKE) " + to_string(subtype);
		else result += DirectShowCameraUtils::to_string(majortype);
		result += "\n";

		// Fix sample size
		result += "Fix sample size: ";
		result += amMediaType->bFixedSizeSamples ? "True\n" : "False\n";

		//Temporal Compression
		result += "Temporal Compression: ";
		result += amMediaType->bTemporalCompression ? "True\n" : "False\n";

		//Sample Size
		result += "Sample Size: " + std::to_string(amMediaType->lSampleSize) + " bytes\n";

		// Format Block
		result += "Format Block : ";
		GUID formatType = amMediaType->formattype;
		if (formatType == FORMAT_DvInfo)			result += "(FORMAT_DvInfo) DVINFO.";
		else if (formatType == FORMAT_MPEG2Video)	result += "(FORMAT_MPEG2Video) MPEG2VIDEOINFO.";
		else if (formatType == FORMAT_MPEGStreams)			result += "(FORMAT_MPEGStreams) AM_MPEGSYSTEMTYPE.";
		else if (formatType == FORMAT_MPEGVideo)	result += "(FORMAT_MPEGVideo) MPEG1VIDEOINFO.";
		else if (formatType == FORMAT_None)			result += "(FORMAT_None) None.";
		else if (formatType == FORMAT_VideoInfo)	result += "(FORMAT_VideoInfo) VIDEOINFOHEADER.";
		else if (formatType == FORMAT_VideoInfo2)			result += "(FORMAT_VideoInfo2) VIDEOINFOHEADER2.";
		else if (formatType == FORMAT_WaveFormatEx)			result += "(FORMAT_WaveFormatEx) WAVEFORMATEX.";
		else if (formatType == GUID_NULL)		result += "(GUID_NULL) None.";
		else result += DirectShowCameraUtils::to_string(formatType);
		result += "\n";

		//Sample Size
		result += "Format Block Size: " + std::to_string(amMediaType->cbFormat) + " bytes\n";

		// Format
		VIDEOINFOHEADER* videoInfoHeader = reinterpret_cast<VIDEOINFOHEADER*>(amMediaType->pbFormat);
		result += "Bit Rate: " + std::to_string(videoInfoHeader->dwBitRate) + " bits per second\n";
		result += "Bit Error Rate: " + std::to_string(videoInfoHeader->dwBitErrorRate) + " bit errors per second\n";
		result += "Frame average display time: " + std::to_string(videoInfoHeader->AvgTimePerFrame * 100) + "ns per frame\n";
		result += "Frame Total Size: " + std::to_string(videoInfoHeader->bmiHeader.biSize) + " bytes\n";
		result += "Frame Size: " + std::to_string(videoInfoHeader->bmiHeader.biWidth) + " x " + std::to_string(videoInfoHeader->bmiHeader.biHeight) + "\n";
		result += "Number of bits per pixel: " + std::to_string(videoInfoHeader->bmiHeader.biBitCount) + " bits\n";
		result += "Horizontal resolution: " + std::to_string(videoInfoHeader->bmiHeader.biXPelsPerMeter) + " pixels per meter\n";
		result += "Vertical resolution: " + std::to_string(videoInfoHeader->bmiHeader.biYPelsPerMeter) + " pixels per meter\n";

		return result;
	}
}

/**
 * @brief Get the supported RGB media subtype. Order by the less convertion.
 * @return Return the RGB list
*/
std::vector<GUID> DirectShowVideoFormat::getSupportRGBSubType()
{
	std::vector<GUID> result(17);
	result.push_back(MEDIASUBTYPE_RGB24);
	result.push_back(MEDIASUBTYPE_RGB32);
	result.push_back(MEDIASUBTYPE_RGB555);
	result.push_back(MEDIASUBTYPE_RGB565);
	result.push_back(MEDIASUBTYPE_YUY2);
	result.push_back(MEDIASUBTYPE_YVYU);
	result.push_back(MEDIASUBTYPE_YUYV);
	result.push_back(MEDIASUBTYPE_IYUV);
	result.push_back(MEDIASUBTYPE_UYVY);
	result.push_back(MEDIASUBTYPE_YV12);
	result.push_back(MEDIASUBTYPE_YVU9);
	result.push_back(MEDIASUBTYPE_Y411);
	result.push_back(MEDIASUBTYPE_Y41P);
	result.push_back(MEDIASUBTYPE_Y211);
	result.push_back(MEDIASUBTYPE_AYUV);
	result.push_back(MEDIASUBTYPE_MJPG);

	return result;
}

/**
 * @brief Return true if the media subtype can be converted to RGB in directshow filter
 * @param guid Media subtype
 * @return 
*/
bool DirectShowVideoFormat::supportRGBConvertion(GUID guid)
{
	std::vector<GUID> supportSubType = DirectShowVideoFormat::getSupportRGBSubType();
	bool result = false;
	for (GUID mediaSubType : supportSubType)
	{
		if (guid == mediaSubType)
		{
			result = true;
			break;
		}
	}

	return result;
}

/**
 * @brief Get the supported monochrome media subtype. Order by the less convertion.
 * @return Return the monochrome list
*/
std::vector<GUID> DirectShowVideoFormat::getMonochromeSubType()
{
	std::vector<GUID> result(3);
	result.push_back(MEDIASUBTYPE_GREY);
	result.push_back(MEDIASUBTYPE_Y8);
	result.push_back(MEDIASUBTYPE_Y800);

	return result;
}

/**
 * @brief Return true if the media subtype is a monochrome type.
 * @param guid Media subtype
 * @return
*/
bool DirectShowVideoFormat::isMonochrome(GUID guid)
{
	std::vector<GUID> supportSubType = DirectShowVideoFormat::getMonochromeSubType();
	bool result = false;
	for (GUID mediaSubType : supportSubType)
	{
		if (guid == mediaSubType)
		{
			result = true;
			break;
		}
	}

	return result;
}

#pragma endregion Static Function

#pragma region constructor and destructor

/**
 * @brief Constructor to create a empty DirectShowVideoFormat
*/
DirectShowVideoFormat::DirectShowVideoFormat()
{
	m_isEmpty = true;
}

/**
 * @brief Constructor
 * @param amMediaType AM_MediaType
 * @param keepAmMediaType Set as true to include the AM_MediaType pointer in this object.
*/
DirectShowVideoFormat::DirectShowVideoFormat(AM_MEDIA_TYPE* amMediaType, bool keepAmMediaType)
{
	constructor(amMediaType, keepAmMediaType);
}

/**
 * @brief Constructor
 * @param streamConfig IAMStreamConfig
 * @param videoFormatIndex Index in the IAMStreamConfig
 * @param keepAmMediaType Set as true to include the AM_MediaType pointer in this object.
*/
DirectShowVideoFormat::DirectShowVideoFormat(IAMStreamConfig* streamConfig, int videoFormatIndex, bool keepAmMediaType)
{
	VIDEO_STREAM_CONFIG_CAPS scc;
	HRESULT hr = streamConfig->GetStreamCaps(videoFormatIndex, &m_AmMediaType, (BYTE*)&scc);
	if (SUCCEEDED(hr))
	{
		constructor(m_AmMediaType, false);

		// does not keep AM_MEDIA_TYPE
		if (!m_isEmpty && !keepAmMediaType)
		{
			DirectShowCameraUtils::deleteMediaType(&m_AmMediaType);
		}
	}
}

/**
 * @brief Constructor
 * @param amMediaType AM_MediaType
 * @param keepAmMediaType Set as true to include the AM_MediaType pointer in this object. If the pointer is kept, it will be released when this object be destoryed.
*/
void DirectShowVideoFormat::constructor(AM_MEDIA_TYPE* amMediaType, bool keepAmMediaType)
{
	if (amMediaType->majortype == MEDIATYPE_Video)
	{
		// Get Video Type
		m_videoType = amMediaType->subtype;
		m_totalSize = amMediaType->lSampleSize;

		// Get the width and height
		VIDEOINFOHEADER* videoInfoHeader = reinterpret_cast<VIDEOINFOHEADER*>(amMediaType->pbFormat);
		m_width = videoInfoHeader->bmiHeader.biWidth;
		m_height = videoInfoHeader->bmiHeader.biHeight;
		m_bitPerPixel = videoInfoHeader->bmiHeader.biBitCount;

		m_isEmpty = false;
	}

	// Store AM_MediaType
	if (keepAmMediaType)
	{
		m_AmMediaType = amMediaType;
	}
}

/**
 * @brief Copy constructor
 * @param directShowVideoFormat DirectShowVideoFormat
*/
DirectShowVideoFormat::DirectShowVideoFormat(const DirectShowVideoFormat& directShowVideoFormat)
{
	*this = directShowVideoFormat;
}

/**
 * @brief Destructor
*/
DirectShowVideoFormat::~DirectShowVideoFormat()
{
	if (m_AmMediaType != NULL)
	{
		DirectShowCameraUtils::deleteMediaType(&m_AmMediaType);
	}	
}

/**
 * @brief Clone
 * @param keepAmMediaType Set as true to pass the AM_MediaType pointer to the new DirectShowVideoFormat. Otherwise the AM_MediaType in the new DirectShowVideoFormat will be NULL.
 * @return Return the copy of this DirectShowVideoFormat
*/
DirectShowVideoFormat DirectShowVideoFormat::clone(bool keepAmMediaType)
{
	DirectShowVideoFormat result = *this;

	if (!keepAmMediaType)
	{
		result.m_AmMediaType = NULL;
	}
	
	return result;
}

#pragma endregion constructor and destructor

#pragma region Getter

/**
 * @brief Return true if this is empty.
 * @return Return true if this is empty. 
*/
bool DirectShowVideoFormat::isEmpty()
{
	return m_isEmpty;
}

/**
 * @brief Return the frame width
 * @return Return the frame width
*/
int DirectShowVideoFormat::getWidth()
{
	return m_width;
}

/**
 * @brief Return the frame height
 * @return Return the frame height
*/
int DirectShowVideoFormat::getHeight()
{
	return m_height;
}

/**
 * @brief Return the bit per pixel
 * @return Return the bit per pixel
*/
int DirectShowVideoFormat::getBitPerPixel()
{
	return m_bitPerPixel;
}

/**
 * @brief Return the frame size in byte
 * @return Return the frame size in byte
*/
long DirectShowVideoFormat::getTotalSize()
{
	return m_totalSize;
}

/**
 * @brief Return the video type.
 * @return Return the video type.
*/
GUID DirectShowVideoFormat::getVideoType()
{
	return m_videoType;
}

/**
 * @brief Return the AM_MediaType pointer. 
 * @return Return the AM_MediaType pointer.
*/
AM_MEDIA_TYPE* DirectShowVideoFormat::getAMMediaType()
{
	return m_AmMediaType;
}

#pragma endregion Getter

#pragma region Operator

/**
 * @brief Copy assignment operator
 * @param directShowVideoFormat DirectShowVideoFormat
 * @return 
*/
DirectShowVideoFormat& DirectShowVideoFormat::operator=(const DirectShowVideoFormat& directShowVideoFormat)
{
	if (this != &directShowVideoFormat)
	{
		if (!directShowVideoFormat.m_isEmpty)
		{
			m_AmMediaType = directShowVideoFormat.m_AmMediaType;
			m_videoType = directShowVideoFormat.m_videoType;
			m_bitPerPixel = directShowVideoFormat.m_bitPerPixel;
			m_totalSize = directShowVideoFormat.m_totalSize;
			m_width = directShowVideoFormat.m_width;
			m_height = directShowVideoFormat.m_height;

			m_isEmpty = false;
		}
		else
		{
			m_isEmpty = true;
		}
	}	

	return *this;
}

/**
 * @brief less than operator
 * @param videoFormat DirectShowVideoFormat
 * @return 
*/
bool DirectShowVideoFormat::operator < (const DirectShowVideoFormat& videoFormat) const
{
	if (m_isEmpty && videoFormat.m_isEmpty)
	{
		return false;
	}
	else if (m_isEmpty)
	{
		return true;
	}
	else if (videoFormat.m_isEmpty)
	{
		return false;
	}
	else
	{
		if (m_width < videoFormat.m_width)
		{
			return true;
		}
		else if (m_width == videoFormat.m_width)
		{
			if (m_height < videoFormat.m_height)
			{
				// m_height < height
				return true;
			}
			else if (m_height == videoFormat.m_height)
			{
				// m_height == height
				if (m_bitPerPixel < videoFormat.m_bitPerPixel)
				{
					return true;
				}
				else
				{
					return false;
				}				
			}
			else
			{
				// m_height > height
				return false;
			}
		}
		else
		{
			return false;
		}
	}
}

/**
 * @brief larger than operator
 * @param videoFormat DirectShowVideoFormat
 * @return
*/
bool DirectShowVideoFormat::operator > (const DirectShowVideoFormat& videoFormat) const
{
	if (*this == videoFormat || *this < videoFormat)
	{
		return false;
	}
	else
	{
		return true;
	}
}

/**
 * @brief Equal to operator
 * @param videoFormat DirectShowVideoFormat
 * @return Return tru if equal
*/
bool DirectShowVideoFormat::operator == (const DirectShowVideoFormat& videoFormat) const
{
	if (m_isEmpty && videoFormat.m_isEmpty)
	{
		return true;
	}
	else if (m_isEmpty || videoFormat.m_isEmpty)
	{
		return false;
	}
	else
	{
		if (m_width == videoFormat.m_width && m_height == videoFormat.m_height && m_videoType == videoFormat.m_videoType)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

/**
 * @brief Not equal to operator
 * @param videoFormat DirectShowVideoFormat
 * @return Return tru if equal
*/
bool DirectShowVideoFormat::operator != (const DirectShowVideoFormat& videoFormat) const
{
	if (*this == videoFormat)
	{
		return false;
	}
	else
	{
		return true;
	}
}

/**
 * @brief Equal to operator
 * @param am_MediaType AM_MEDIA_TYPE
 * @return Return true if equal
*/
bool DirectShowVideoFormat::operator == (const AM_MEDIA_TYPE& am_MediaType) const
{
	if (m_isEmpty)
	{
		return true;
	}
	else
	{
		// Get the width and height
		VIDEOINFOHEADER* videoInfoHeader = reinterpret_cast<VIDEOINFOHEADER*>(am_MediaType.pbFormat);
		int mediaTypeWidth = videoInfoHeader->bmiHeader.biWidth;
		int mediaTypeHeight = videoInfoHeader->bmiHeader.biHeight;

		if (m_width == mediaTypeWidth && m_height == mediaTypeHeight && m_videoType == am_MediaType.subtype)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

/**
 * @brief Not equal to operator
 * @param am_MediaType AM_MEDIA_TYPE
 * @return Return true if equal
*/
bool DirectShowVideoFormat::operator != (const AM_MEDIA_TYPE& am_MediaType) const
{
	if (*this == am_MediaType)
	{
		return false;
	}
	else
	{
		return true;
	}
}

#pragma endregion Operator

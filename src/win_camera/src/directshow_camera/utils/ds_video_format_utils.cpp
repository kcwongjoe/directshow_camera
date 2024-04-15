/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#include "directshow_camera/utils/ds_video_format_utils.h"

#include "directshow_camera/video_format/ds_guid.h"
#include "directshow_camera/utils/win32_utils.h"

namespace DirectShowVideoFormatUtils
{
    std::string ToString(AM_MEDIA_TYPE* amMediaType)
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
            if (majortype == MEDIATYPE_AnalogAudio)			result += "(MEDIATYPE_AnalogAudio) " + ToString(majortype);
            else if (majortype == MEDIATYPE_AnalogVideo)	result += "(MEDIATYPE_AnalogVideo) " + ToString(majortype);
            else if (majortype == MEDIATYPE_Audio)			result += "(MEDIATYPE_Audio) " + ToString(majortype);
            else if (majortype == MEDIATYPE_AUXLine21Data)	result += "(MEDIATYPE_AUXLine21Data) " + ToString(majortype);
            else if (majortype == MEDIATYPE_File)			result += "(MEDIATYPE_File) " + ToString(majortype);
            else if (majortype == MEDIATYPE_Interleaved)	result += "(MEDIATYPE_Interleaved) " + ToString(majortype);
            else if (majortype == MEDIATYPE_LMRT)			result += "(MEDIATYPE_LMRT) " + ToString(majortype);
            else if (majortype == MEDIATYPE_Midi)			result += "(MEDIATYPE_Midi) " + ToString(majortype);
            else if (majortype == MEDIATYPE_MPEG2_PES)		result += "(MEDIATYPE_MPEG2_PES) " + ToString(majortype);
            else if (majortype == MEDIATYPE_MPEG2_SECTIONS)	result += "(MEDIATYPE_MPEG2_SECTIONS) " + ToString(majortype);
            else if (majortype == MEDIATYPE_ScriptCommand)	result += "(MEDIATYPE_ScriptCommand)" + ToString(majortype);
            else if (majortype == MEDIATYPE_Stream)			result += "(MEDIATYPE_Stream) " + ToString(majortype);
            else if (majortype == MEDIATYPE_Text)			result += "(MEDIATYPE_Text) " + ToString(majortype);
            else if (majortype == MEDIATYPE_Timecode)		result += "(MEDIATYPE_Timecode) " + ToString(majortype);
            else if (majortype == MEDIATYPE_URL_STREAM)		result += "(MEDIATYPE_URL_STREAM) " + ToString(majortype);
            else if (majortype == MEDIATYPE_Video)			result += "(MEDIATYPE_Video) " + ToString(majortype);

            else result += Win32Utils::ToString(majortype);
            result += "\n";

            // Sub Type
            result += "Sub Type : ";
            GUID subtype = amMediaType->subtype;
            if (subtype == MEDIASUBTYPE_None)						result += "(MEDIASUBTYPE_None) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_PCM)					result += "(MEDIASUBTYPE_PCM) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_MPEG1Packet)			result += "(MEDIASUBTYPE_MPEG1Packet) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_MPEG1Payload)			result += "(MEDIASUBTYPE_MPEG1Payload) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_MPEG2_AUDIO)			result += "(MEDIASUBTYPE_MPEG2_AUDIO) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_DVD_LPCM_AUDIO)		result += "(MEDIASUBTYPE_DVD_LPCM_AUDIO) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_MPEG2_AUDIO)			result += "(MEDIASUBTYPE_MPEG2_AUDIO) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_DRM_Audio)				result += "(MEDIASUBTYPE_DRM_Audio) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_IEEE_FLOAT)			result += "(MEDIASUBTYPE_IEEE_FLOAT) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_DOLBY_AC3)				result += "(MEDIASUBTYPE_DOLBY_AC3) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_DOLBY_AC3_SPDIF)		result += "(MEDIASUBTYPE_DOLBY_AC3_SPDIF) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_RAW_SPORT)				result += "(MEDIASUBTYPE_RAW_SPORT) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_SPDIF_TAG_241h)		result += "(MEDIASUBTYPE_SPDIF_TAG_241h) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_AIFF)					result += "(MEDIASUBTYPE_AIFF) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_Asf)					result += "(MEDIASUBTYPE_Asf) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_Avi)					result += "(MEDIASUBTYPE_Avi) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_AU)					result += "(MEDIASUBTYPE_AU) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_DssAudio)				result += "(MEDIASUBTYPE_DssAudio) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_DssVideo)				result += "(MEDIASUBTYPE_DssVideo) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_MPEG1Audio)			result += "(MEDIASUBTYPE_MPEG1Audio) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_MPEG1System)			result += "(MEDIASUBTYPE_MPEG1System) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_MPEG1Video)			result += "(MEDIASUBTYPE_MPEG1Video) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_MPEG1VideoCD)			result += "(MEDIASUBTYPE_MPEG1VideoCD) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_WAVE)					result += "(MEDIASUBTYPE_WAVE) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_AnalogVideo_NTSC_M)	result += "(MEDIASUBTYPE_AnalogVideo_NTSC_M) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_AnalogVideo_PAL_B)		result += "(MEDIASUBTYPE_AnalogVideo_PAL_B) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_AnalogVideo_PAL_D)		result += "(MEDIASUBTYPE_AnalogVideo_PAL_D) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_AnalogVideo_PAL_G)		result += "(MEDIASUBTYPE_AnalogVideo_PAL_G) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_AnalogVideo_PAL_H)		result += "(MEDIASUBTYPE_AnalogVideo_PAL_H) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_AnalogVideo_PAL_I)		result += "(MEDIASUBTYPE_AnalogVideo_PAL_I) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_AnalogVideo_PAL_M)		result += "(MEDIASUBTYPE_AnalogVideo_PAL_M) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_AnalogVideo_PAL_N)		result += "(MEDIASUBTYPE_AnalogVideo_PAL_N) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_AnalogVideo_SECAM_B)	result += "(MEDIASUBTYPE_AnalogVideo_SECAM_B) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_AnalogVideo_SECAM_D)	result += "(MEDIASUBTYPE_AnalogVideo_SECAM_D) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_AnalogVideo_SECAM_G)	result += "(MEDIASUBTYPE_AnalogVideo_SECAM_G) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_AnalogVideo_SECAM_H)	result += "(MEDIASUBTYPE_AnalogVideo_SECAM_H) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_AnalogVideo_SECAM_K)	result += "(MEDIASUBTYPE_AnalogVideo_SECAM_K) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_AnalogVideo_SECAM_K1)	result += "(MEDIASUBTYPE_AnalogVideo_SECAM_K1) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_AnalogVideo_SECAM_L)	result += "(MEDIASUBTYPE_AnalogVideo_SECAM_L) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_AI44)					result += "(MEDIASUBTYPE_AI44) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_IA44)					result += "(MEDIASUBTYPE_IA44) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_dvsl)					result += "(MEDIASUBTYPE_dvsl) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_dvsd)					result += "(MEDIASUBTYPE_dvsd) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_dvhd)					result += "(MEDIASUBTYPE_dvhd) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_dv25)					result += "(MEDIASUBTYPE_dv25) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_dv50)					result += "(MEDIASUBTYPE_dv50) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_dvh1)					result += "(MEDIASUBTYPE_dvh1) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_DVCS)					result += "(MEDIASUBTYPE_DVCS) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_DVSD)					result += "(MEDIASUBTYPE_DVSD) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_RGB1)					result += "(MEDIASUBTYPE_RGB1) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_RGB4)					result += "(MEDIASUBTYPE_RGB4) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_RGB8)					result += "(MEDIASUBTYPE_RGB8) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_RGB555)				result += "(MEDIASUBTYPE_RGB555) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_RGB565)				result += "(MEDIASUBTYPE_RGB565) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_RGB24)					result += "(MEDIASUBTYPE_RGB24) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_RGB32)					result += "(MEDIASUBTYPE_RGB32) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_ARGB1555)				result += "(MEDIASUBTYPE_ARGB1555) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_ARGB32)				result += "(MEDIASUBTYPE_ARGB32) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_ARGB4444)				result += "(MEDIASUBTYPE_ARGB4444) 	" + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_A2R10G10B10)			result += "(MEDIASUBTYPE_A2R10G10B10) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_A2B10G10R10)			result += "(MEDIASUBTYPE_A2B10G10R10) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_RGB32_D3D_DX7_RT)		result += "(MEDIASUBTYPE_RGB32_D3D_DX7_RT) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_RGB16_D3D_DX7_RT)		result += "(MEDIASUBTYPE_RGB16_D3D_DX7_RT) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_ARGB32_D3D_DX7_RT)		result += "(MEDIASUBTYPE_ARGB32_D3D_DX7_RT) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_ARGB4444_D3D_DX7_RT)	result += "(MEDIASUBTYPE_ARGB4444_D3D_DX7_RT) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_ARGB1555_D3D_DX7_RT)	result += "(MEDIASUBTYPE_ARGB1555_D3D_DX7_RT) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_RGB32_D3D_DX9_RT)		result += "(MEDIASUBTYPE_RGB32_D3D_DX9_RT) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_RGB16_D3D_DX9_RT)		result += "(MEDIASUBTYPE_RGB16_D3D_DX9_RT) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_ARGB32_D3D_DX9_RT)		result += "(MEDIASUBTYPE_ARGB32_D3D_DX9_RT) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_ARGB4444_D3D_DX9_RT)	result += "(MEDIASUBTYPE_ARGB4444_D3D_DX9_RT) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_ARGB1555_D3D_DX9_RT)	result += "(MEDIASUBTYPE_ARGB1555_D3D_DX9_RT) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_AYUV)					result += "(MEDIASUBTYPE_AYUV) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_YUY2)					result += "(MEDIASUBTYPE_YUY2) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_UYVY)					result += "(MEDIASUBTYPE_UYVY) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_IMC1)					result += "(MEDIASUBTYPE_IMC1) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_IMC2)					result += "(MEDIASUBTYPE_IMC2) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_IMC3)					result += "(MEDIASUBTYPE_IMC3) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_IMC4)					result += "(MEDIASUBTYPE_IMC4) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_YV12)					result += "(MEDIASUBTYPE_YV12) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_NV12)					result += "(MEDIASUBTYPE_NV12) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_Y411)					result += "(MEDIASUBTYPE_Y411) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_Y41P)					result += "(MEDIASUBTYPE_Y41P) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_Y211)					result += "(MEDIASUBTYPE_Y211) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_YVYU)					result += "(MEDIASUBTYPE_YVYU) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_YVU9)					result += "(MEDIASUBTYPE_YVU9) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_IF09)					result += "(MEDIASUBTYPE_IF09) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_CFCC)					result += "(MEDIASUBTYPE_CFCC) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_CLJR)					result += "(MEDIASUBTYPE_CLJR) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_CPLA)					result += "(MEDIASUBTYPE_CPLA) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_CLPL)					result += "(MEDIASUBTYPE_CLPL) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_IJPG)					result += "(MEDIASUBTYPE_IJPG) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_MDVF)					result += "(MEDIASUBTYPE_MDVF) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_MJPG)					result += "(MEDIASUBTYPE_MJPG) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_MPEG1Packet)			result += "(MEDIASUBTYPE_MPEG1Packet) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_MPEG1Payload)			result += "(MEDIASUBTYPE_MPEG1Payload) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_Overlay)				result += "(MEDIASUBTYPE_Overlay) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_Plum)					result += "(MEDIASUBTYPE_Plum) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_QTJpeg)				result += "(MEDIASUBTYPE_QTJpeg) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_QTMovie)				result += "(MEDIASUBTYPE_QTMovie) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_QTRle)					result += "(MEDIASUBTYPE_QTRle) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_QTRpza)				result += "(MEDIASUBTYPE_QTRpza) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_QTSmc)					result += "(MEDIASUBTYPE_QTSmc) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_TVMJ)					result += "(MEDIASUBTYPE_TVMJ) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_VPVBI)					result += "(MEDIASUBTYPE_VPVBI) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_VPVideo)				result += "(MEDIASUBTYPE_VPVideo) " + ToString(subtype);
            else if (subtype == MEDIASUBTYPE_WAKE)					result += "(MEDIASUBTYPE_WAKE) " + ToString(subtype);
            else result += Win32Utils::ToString(majortype);
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
            else result += Win32Utils::ToString(formatType);
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

    std::string ToString(GUID guid)
    {
        std::string result;
        if (guid == MEDIATYPE_AnalogAudio)			result = "Analog audio.";
        else if (guid == MEDIATYPE_AnalogVideo)		result = "Analog video.";
        else if (guid == MEDIATYPE_Audio)			result = "Audio.";
        else if (guid == MEDIATYPE_AUXLine21Data)	result = "Line 21 data. Used by closed captions.";
        else if (guid == MEDIATYPE_File)			result = "File. (Obsolete)";
        else if (guid == MEDIATYPE_Interleaved)		result = "Interleaved audio and video. Used for Digital Video (DV).";
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
        else if (guid == MEDIASUBTYPE_None)					result += "None.";
        else if (guid == MEDIASUBTYPE_PCM)					result += "PCM audio.";
        else if (guid == MEDIASUBTYPE_MPEG1Packet)			result += "MPEG1 Audio packet.";
        else if (guid == MEDIASUBTYPE_MPEG1Payload)			result += "MPEG1 Audio Payload.";
        else if (guid == MEDIASUBTYPE_MPEG2_AUDIO)			result += "MPEG-2 audio data.";
        else if (guid == MEDIASUBTYPE_DVD_LPCM_AUDIO)		result += "DVD audio data.";
        else if (guid == MEDIASUBTYPE_MPEG2_AUDIO)			result += "MPEG-2 audio data.";
        else if (guid == MEDIASUBTYPE_DRM_Audio)			result += "Corresponds to WAVE_FORMAT_DRM.";
        else if (guid == MEDIASUBTYPE_IEEE_FLOAT)			result += "Corresponds to WAVE_FORMAT_IEEE_FLOAT.";
        else if (guid == MEDIASUBTYPE_DOLBY_AC3)			result += "Dolby data.";
        else if (guid == MEDIASUBTYPE_DOLBY_AC3_SPDIF)		result += "Dolby AC3 over SPDIF.";
        else if (guid == MEDIASUBTYPE_RAW_SPORT)			result += "Equivalent to MEDIASUBTYPE_DOLBY_AC3_SPDIF.";
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

    std::string IKsTopologyInfoNodeToString(GUID guid)
    {
        if (guid == KSNODETYPE_DEV_SPECIFIC)
        {
            return "KSNODETYPE_DEV_SPECIFIC";
        }
        else if (guid == KSNODETYPE_VIDEO_CAMERA_TERMINAL)
        {
            return "KSNODETYPE_VIDEO_CAMERA_TERMINAL";
        }
        else if (guid == KSNODETYPE_VIDEO_INPUT_MTT)
        {
            return "KSNODETYPE_VIDEO_INPUT_MTT";
        }
        else if (guid == KSNODETYPE_VIDEO_INPUT_TERMINAL)
        {
            return "KSNODETYPE_VIDEO_INPUT_TERMINAL";
        }
        else if (guid == KSNODETYPE_VIDEO_PROCESSING)
        {
            return "KSNODETYPE_VIDEO_PROCESSING";
        }
        else if (guid == KSNODETYPE_VIDEO_SELECTOR)
        {
            return "KSNODETYPE_VIDEO_SELECTOR";
        }
        else if (guid == KSNODETYPE_VIDEO_STREAMING)
        {
            return "KSNODETYPE_VIDEO_STREAMING";
        }
        else
        {
            return "UNKNOWN_NODE_TYPE";
        }
    }

    std::vector<GUID> SupportMonochromeSubType()
    {
        std::vector<GUID> result = {
            MEDIASUBTYPE_GREY,
            MEDIASUBTYPE_Y8,
            MEDIASUBTYPE_Y800
        };

        return result;
    }

    bool isMonochrome(GUID guid)
    {
        std::vector<GUID> supportSubType = SupportMonochromeSubType();
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

    std::vector<GUID> SupportRGBSubType()
    {
        std::vector<GUID> result = {
            MEDIASUBTYPE_RGB24,
            MEDIASUBTYPE_RGB32,
            MEDIASUBTYPE_RGB555,
            MEDIASUBTYPE_RGB565,
            MEDIASUBTYPE_YUY2,
            MEDIASUBTYPE_YVYU,
            MEDIASUBTYPE_YUYV,
            MEDIASUBTYPE_IYUV,
            MEDIASUBTYPE_UYVY,
            MEDIASUBTYPE_YV12,
            MEDIASUBTYPE_YVU9,
            MEDIASUBTYPE_Y411,
            MEDIASUBTYPE_Y41P,
            MEDIASUBTYPE_Y211,
            MEDIASUBTYPE_AYUV,
            MEDIASUBTYPE_MJPG
        };

        return result;
    }
    bool isSupportRGBConvertion(GUID guid)
    {
        std::vector<GUID> supportSubType = SupportRGBSubType();
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
}
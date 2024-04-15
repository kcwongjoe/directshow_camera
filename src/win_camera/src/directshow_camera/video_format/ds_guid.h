/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#pragma once
#ifndef WIN_CAMERA__DIRECTSHOW_CAMERA__VIDEO_FORMAT__DIRECTSHOW_GUID_H
#define WIN_CAMERA__DIRECTSHOW_CAMERA__VIDEO_FORMAT__DIRECTSHOW_GUID_H

//************Content************
#include <uuids.h>


// Media sub type
static const GUID MEDIASUBTYPE_GREY = { 0x59455247, 0x0000, 0x0010,{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };
static const GUID MEDIASUBTYPE_Y8 = { 0x20203859, 0x0000, 0x0010,{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };
static const GUID MEDIASUBTYPE_Y800 = { 0x30303859, 0x0000, 0x0010,{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };
static const GUID MEDIASUBTYPE_Y16 = { 0x20363159, 0x0000, 0x0010,{ 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71 } };

// The following has been included in the Windows SDK (uuids.h) so ignore it.
/*
static const GUID MEDIASUBTYPE_BY8 = { 0x20385942, 0x0000, 0x0010,{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };
static const GUID MEDIASUBTYPE_AYUV = { 0x56555941, 0x0000, 0x0010,{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };
static const GUID MEDIASUBTYPE_IYUV = { 0x56555949, 0x0000, 0x0010,{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };
static const GUID MEDIASUBTYPE_RGB24 = { 0xe436eb7d, 0x524f, 0x11ce,{ 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70 } };
static const GUID MEDIASUBTYPE_RGB32 = { 0xe436eb7e, 0x524f, 0x11ce,{ 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70 } };
static const GUID MEDIASUBTYPE_RGB555 = { 0xe436eb7c, 0x524f, 0x11ce,{ 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70 } };
static const GUID MEDIASUBTYPE_RGB565 = { 0xe436eb7b, 0x524f, 0x11ce,{ 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70 } };
static const GUID MEDIASUBTYPE_I420 = { 0x30323449, 0x0000, 0x0010,{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };
static const GUID MEDIASUBTYPE_UYVY = { 0x59565955, 0x0000, 0x0010,{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };
static const GUID MEDIASUBTYPE_Y211 = { 0x31313259, 0x0000, 0x0010,{  0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };
static const GUID MEDIASUBTYPE_Y411 = { 0x31313459, 0x0000, 0x0010,{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };
static const GUID MEDIASUBTYPE_Y41P = { 0x50313459, 0x0000, 0x0010,{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };
static const GUID MEDIASUBTYPE_YUY2 = { 0x32595559, 0x0000, 0x0010,{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };
static const GUID MEDIASUBTYPE_YUYV = { 0x56595559, 0x0000, 0x0010,{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };
static const GUID MEDIASUBTYPE_YV12 = { 0x32315659, 0x0000, 0x0010,{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };
static const GUID MEDIASUBTYPE_YVU9 = { 0x39555659, 0x0000, 0x0010,{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };
static const GUID MEDIASUBTYPE_YVYU = { 0x55595659, 0x0000, 0x0010,{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };
static const GUID MEDIASUBTYPE_MJPG = { 0x47504A4D, 0x0000, 0x0010,{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };
*/

//*******************************

#endif
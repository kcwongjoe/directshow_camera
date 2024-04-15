/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#pragma once
#ifndef WIN_CAMERA__DIRECTSHOW_CAMERA__DIRECTSHOW_HEADER_H
#define WIN_CAMERA__DIRECTSHOW_CAMERA__DIRECTSHOW_HEADER_H

//************Content************

// Direct show is native code, complier it in unmanaged
#pragma managed(push, off)
#include <dshow.h>
#pragma managed(pop)
#pragma comment(lib, "strmiids")

#include <vidcap.h>
#include <ksmedia.h>

//*******************************

#endif
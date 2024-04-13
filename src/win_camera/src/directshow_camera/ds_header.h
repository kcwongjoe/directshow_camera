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
/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "frame/frame_settings.h"

namespace DirectShowCamera
{

    FrameSettings::FrameSettings()
    {
        Reset();
    }

    void FrameSettings::Reset()
    {
        BGR = true;
        VerticalFlip = false;
    }
}
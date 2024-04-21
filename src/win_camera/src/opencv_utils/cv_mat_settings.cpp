/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#include "opencv_utils/cv_mat_settings.h"

#ifdef WITH_OPENCV2

namespace WinCamera
{

    OpenCVMatSettings::OpenCVMatSettings()
    {
        Reset();
    }

    void OpenCVMatSettings::Reset()
    {
        BGR = true;
        VerticalFlip = true;
    }
}

#endif
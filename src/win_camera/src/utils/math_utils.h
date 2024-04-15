/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#pragma once
#ifndef WIN_CAMERA__UTILS__MATH_UTILS_H
#define WIN_CAMERA__UTILS__MATH_UTILS_H

//************Content************

#include <math.h>

namespace Utils
{
    class MathUtils
    {
    public:
        /**
         * @brief A utils to convert degree to -180 to 180
         * @param degree Degree to be converted
         * @return Return a degree within -180 and 180
        */
        static int ConfirmDegreeIn180Range(int degree);
    };
}

//*******************************

#endif
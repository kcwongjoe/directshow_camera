/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#include "utils/math_utils.h"

namespace Utils
{
    int MathUtils::ConfirmDegreeIn180Range(int degree)
    {
        int result = 0;
        if (degree % 360 != 0)
        {
            bool positiveRegion = sin(degree * 0.01745329251994329576923690722222) > 0.0 ? true : false;

            if (degree > 0)
            {
                if (positiveRegion)
                {
                    // 0 to 180
                    result = degree % 180;
                }
                else
                {
                    // 0 to -180
                    result = degree % 180 - 180;
                }
            }
            else
            {
                if (positiveRegion)
                {
                    // 0 to 180
                    result = 180 + degree % 180;
                }
                else
                {
                    // 0 to -180
                    result = degree % 180;
                }
            }
        }

        return result;
    }
}
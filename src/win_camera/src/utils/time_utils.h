/**
* Copy right (c) 2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#pragma once
#ifndef WIN_CAMERA__UTILS__TIME_UTILS_H
#define WIN_CAMERA__UTILS__TIME_UTILS_H

//************Content************

#include <ctime>
#include <chrono>
#include <string>

namespace Utils
{
    class TimeUtils
    {
    public:

        /**
         * @brief Get current time
         *
         * @return time_t
         */
        static time_t GetNowTimeT();

        /**
         * @brief Get current time in string
         * 
         * @param[in] format (Option) time format. Default as "%Y-%m-%d %H:%M:%S"
         *
         * @return std::string
         */
        static std::string GetTimeNowString(std::string format = "%Y-%m-%d %H:%M:%S");

        /**
         * @brief Convert time_t to string
         *
         * @param[in] time time to be conveted
         * @param[in] format (Option) time format. Default as "%Y-%m-%d %H:%M:%S"
         * @return std::string
         */
        static std::string ToString(time_t time, std::string format = "%Y-%m-%d %H:%M:%S");

        /**
         * @brief Get millisecond from time
         * @param[in] time
         * @return
        */
        static int GetMilliseconds(std::chrono::system_clock::time_point time);
    };
}

//*******************************

#endif
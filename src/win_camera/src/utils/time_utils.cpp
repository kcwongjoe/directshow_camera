#include "utils/time_utils.h"

namespace Utils
{
    time_t TimeUtils::GetNowTimeT()
    {
        const auto now = std::chrono::system_clock::now();
        time_t nowTimet = std::chrono::system_clock::to_time_t(now);

        return nowTimet;
    }

    std::string TimeUtils::GetTimeNowString(std::string format)
    {
        return ToString(GetNowTimeT(), format);
    }

    std::string TimeUtils::ToString(time_t time, std::string format)
    {
        // Initialize
        struct tm* currentTimeInfo;
        char buffer[80];

#pragma warning(suppress : 4996)
        currentTimeInfo = localtime(&time);

        // Convert to string
        strftime(buffer, sizeof(buffer), format.c_str(), currentTimeInfo);
        const std::string result(buffer);

        return result;
    }

    int TimeUtils::GetMilliseconds(std::chrono::system_clock::time_point time)
    {
        const auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(time);
        const auto fraction = time - seconds;
        const int ms = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(fraction).count());
        return ms;
    }
}
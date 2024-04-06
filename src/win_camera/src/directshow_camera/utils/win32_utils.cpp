#include "directshow_camera/utils/win32_utils.h"

#include <sstream>

namespace Win32Utils
{
    std::string BSTRToString(const BSTR bstr, const int cp)
    {
        std::string result = "";

        if (bstr)
        {
            // request content length in single-chars through a terminating nullchar in the BSTR.
            // note: BSTR's support imbedded nullchars, so this will only convert through the first nullchar.
            int res = WideCharToMultiByte(cp, 0, bstr, -1, NULL, 0, NULL, NULL);
            if (res > 0)
            {
                result.resize(res);
                WideCharToMultiByte(cp, 0, bstr, -1, &result[0], res, NULL, NULL);
            }
            else
            {   // no content. clear target
                result.clear();
            }
        }

        return result;
    }


    std::string ToString(const GUID guid)
    {
        std::ostringstream os;
        os.fill('0');

        os << std::uppercase;
        os.width(8);
        os << std::hex << guid.Data1;
        
        os << '-';

        os.width(4);
        os << std::hex << guid.Data2;
        
        os << '-';

        os.width(4);
        os << std::hex << guid.Data3;
        
        os << '-';

        os.width(2);
        os << std::hex << static_cast<short>(guid.Data4[0]);

        os.width(2);
        os << std::hex << static_cast<short>(guid.Data4[1]);

        os << '-';

        os.width(2);
        os << std::hex << static_cast<short>(guid.Data4[2]);

        os.width(2);
        os << std::hex << static_cast<short>(guid.Data4[3]);

        os.width(2);
        os << std::hex << static_cast<short>(guid.Data4[4]);

        os.width(2);
        os << std::hex << static_cast<short>(guid.Data4[5]);

        os.width(2);
        os << std::hex << static_cast<short>(guid.Data4[6]);

        os.width(2);
        os << std::hex << static_cast<short>(guid.Data4[7]);

        os << std::nouppercase;

        const std::string result = os.str();

         return result;
    }
}
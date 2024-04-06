#include "directshow_camera/utils/com_lib_utils.h"

#include <combaseapi.h>

namespace COMLibUtils
{
    bool COMLibUtils::s_isInitializedCOMLib = false;

    bool COMLibUtils::InitCOMLib()
    {
        HRESULT hr = NO_ERROR;

        if (!s_isInitializedCOMLib)
        {
            hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
        }

        return hr == S_OK;
    }

    void COMLibUtils::UninitCOMLib()
    {
        CoUninitialize();
        s_isInitializedCOMLib = false;
    }
}
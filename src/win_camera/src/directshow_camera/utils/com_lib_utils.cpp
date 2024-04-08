#include "directshow_camera/utils/com_lib_utils.h"

#include <combaseapi.h>

namespace COMLibUtils
{
    bool COMLibUtils::s_isInitializedCOMLib = false;

    bool COMLibUtils::InitCOMLib(std::string& errorString)
    {
        HRESULT hr = NO_ERROR;

        if (!s_isInitializedCOMLib)
        {
            hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
        }

        if (!SUCCEEDED(hr))
        {
            if (hr == RPC_E_CHANGED_MODE)
            {
                // A previous call to CoInitializeEx specified the concurrency model for this thread
                // as multithread apartment (MTA). This could also indicate that a change from
                // neutral-threaded apartment to single-threaded apartment has occurred.
                errorString = "Error on Initialize COM Library: The COM library is already initialized with a different concurrency model.";
            }
            else if (hr == E_INVALIDARG)
            {
                errorString = "Error on Initialize COM Library: The parameter dwCoInit specified an invalid value.";
            }
            else if (hr == E_OUTOFMEMORY)
            {
                errorString = "Error on Initialize COM Library: The COM library could not be initialized because there was not enough memory available to complete the operation.";
            }
            else if (hr == E_UNEXPECTED)
            {
                errorString = "Error on Initialize COM Library: An unexpected error has occurred.";
            }
            else
            {
                errorString = "Error on Initialize COM Library: HR(" + std::to_string(hr) + ")";
            }

            return false;
        }
        else
        {
            return true;
        }
    }

    void COMLibUtils::UninitCOMLib()
    {
        CoUninitialize();
        s_isInitializedCOMLib = false;
    }
}
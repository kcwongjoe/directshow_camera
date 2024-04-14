#include "directshow_camera/utils/check_hresult_utils.h"

#include <windows.h>
#include <windef.h>
#include <vfwmsgs.h>

namespace DirectShowCamera
{
    bool CheckHResultUtils::CheckCoCreateInstanceResult(const HRESULT hr, std::string& errorString, const std::string errorDescription)
    {
        bool result = true;
        if (hr != S_OK)
        {
            result = false;

            errorString = errorDescription + ": ";
            if (hr == REGDB_E_CLASSNOTREG)
            {
                errorString += "REGDB_E_CLASSNOTREG";
            }
            else if (hr == CLASS_E_NOAGGREGATION)
            {
                errorString += "CLASS_E_NOAGGREGATION";
            }
            else if (hr == E_NOINTERFACE)
            {
                errorString += "E_NOINTERFACE";
            }
            else if (hr == E_POINTER)
            {
                errorString += "The ppv parameter is NULL.";
            }
            else
            {
                errorString += "hr = (" + std::to_string(hr) + ")";
            }
        }

        return result;
    }

    bool CheckHResultUtils::CheckCreateClassEnumeratorResult(const HRESULT hr, std::string& errorString, const std::string errorDescription)
    {
        bool result = true;
        if (hr != S_OK)
        {
            result = false;

            // Convert to error string
            errorString = errorDescription + ": ";
            if (hr == E_OUTOFMEMORY)
            {
                errorString += "There is not enough memory available to create a class enumerator.";
            }
            else if (hr == S_FALSE)
            {
                errorString += "There is no any camera.";
            }
            else if (hr == E_POINTER)
            {
                errorString += "NULL pointer argument.";
            }
            else
            {
                errorString += "hr = (" + std::to_string(hr) + ")";
            }
        }

        return result;
    }

    bool CheckHResultUtils::CheckICGB2SetFiltergraphResult(const HRESULT hr, std::string& errorString, const std::string errorDescription)
    {
        bool result = true;
        if (hr != S_OK)
        {
            result = false;

            // Convert to error string
            errorString = errorDescription + ": ";
            if (hr == E_UNEXPECTED)
            {
                errorString += "Unexpected error.";
            }
            else if (hr == E_POINTER)
            {
                errorString += "NULL pointer argument.";
            }
            else
            {
                errorString += "hr = (" + std::to_string(hr) + ")";
            }
        }

        return result;
    }

    bool CheckHResultUtils::CheckIGBAddFilterResult(const HRESULT hr, std::string& errorString, const std::string errorDescription)
    {
        bool result = true;
        if (hr != S_OK && hr != VFW_S_DUPLICATE_NAME)
        {
            result = false;

            // Convert to error string
            errorString = errorDescription + ": ";
            if (hr == E_FAIL)
            {
                errorString += "Failure.";
            }
            else if (hr == E_OUTOFMEMORY)
            {
                errorString += "Insufficient memory.";
            }
            else if (hr == E_POINTER)
            {
                errorString += "NULL pointer argument memory.";
            }
            else if (hr == VFW_E_CERTIFICATION_FAILURE)
            {
                errorString += "Use of this filter is restricted by a software key.";
            }
            else if (hr == VFW_E_DUPLICATE_NAME)
            {
                errorString += "Failed to add a filter with a duplicate name.";
            }
            else
            {
                errorString += "hr = (" + std::to_string(hr) + ")";
            }
        }

        return result;
    }

    bool CheckHResultUtils::CheckICGB2FindInterfaceResult(const HRESULT hr, std::string& errorString, const std::string errorDescription)
    {
        bool result = true;
        if (hr != S_OK)
        {
            result = false;

            // Convert to error string
            errorString = errorDescription + ": ";
            if (hr == E_FAIL)
            {
                errorString += "Failure.";
            }
            else if (hr == E_NOINTERFACE)
            {
                errorString += "No such interface supported.";
            }
            else if (hr == E_POINTER)
            {
                errorString += "NULL pointer argument memory.";
            }
            else
            {
                errorString += "hr = (" + std::to_string(hr) + ")";
            }
        }

        return result;
    }

    bool CheckHResultUtils::CheckICGB2RenderStreamResult(const HRESULT hr, std::string& errorString, const std::string errorDescription)
    {
        bool result = true;
        if (hr != S_OK && hr != VFW_S_DUPLICATE_NAME)
        {
            result = false;

            // Convert to error string
            errorString = errorDescription + ": ";
            if (hr == E_FAIL)
            {
                errorString += "Failure.";
            }
            else if (hr == VFW_S_NOPREVIEWPIN)
            {
                errorString += "Preview was rendered through the Smart Tee Filter.";
            }
            else if (hr == E_INVALIDARG)
            {
                errorString += "Invalid argument.";
            }
            else if (hr == E_POINTER)
            {
                errorString += "NULL pointer argument.";
            }
            else if (hr == VFW_E_NOT_IN_GRAPH)
            {
                errorString += "A filter is not in the filter graph. Check whether call AddFilter() and connect your graph to the Capture Graph Builder by SetFiltergraph().";
            }
            else
            {
                errorString += "hr = (" + std::to_string(hr) + ")";
            }
        }

        return result;
    }

    bool CheckHResultUtils::CheckIIAMSCGetFormatResult(const HRESULT hr, std::string& errorString, const  std::string errorDescription)
    {
        bool result = true;
        if (hr != S_OK)
        {
            result = false;

            // Convert to error string
            errorString = errorDescription + ": ";
            if (hr == E_FAIL)
            {
                errorString += "Failure.";
            }
            else if (hr == E_OUTOFMEMORY)
            {
                errorString += "Insufficient memory.";
            }
            else if (hr == E_POINTER)
            {
                errorString += "NULL pointer argument.";
            }
            else if (hr == VFW_E_NOT_CONNECTED)
            {
                errorString += "The input pin is not connected.";
            }
            else
            {
                errorString += "hr = (" + std::to_string(hr) + ")";
            }
        }

        return result;
    }

    bool CheckHResultUtils::CheckIIAMSCSetFormatResult(const HRESULT hr, std::string& errorString, const std::string errorDescription)
    {
        bool result = true;
        if (hr != S_OK)
        {
            result = false;

            // Convert to error string
            errorString = errorDescription + ": ";
            if (hr == E_FAIL)
            {
                errorString += "Failure.";
            }
            else if (hr == E_OUTOFMEMORY)
            {
                errorString += "Insufficient memory.";
            }
            else if (hr == E_POINTER)
            {
                errorString += "NULL pointer argument.";
            }
            else if (hr == VFW_E_INVALIDMEDIATYPE)
            {
                errorString += "This media type is not valid.";
            }
            else if (hr == VFW_E_NOT_CONNECTED)
            {
                errorString += "The input pin is not connected.";
            }
            else if (hr == VFW_E_NOT_STOPPED)
            {
                errorString += "Cannot set the type; the filter is not stopped.";
            }
            else if (hr == VFW_E_WRONG_STATE)
            {
                errorString += "Cannot set the type; the filter is not stopped.";
            }
            else
            {
                errorString += "hr = (" + std::to_string(hr) + ")";
            }
        }

        return result;
    }

    bool CheckHResultUtils::CheckIAMSCGetNumberOfCapabilitiesResult(const HRESULT hr, std::string& errorString, const std::string errorDescription)
    {
        bool result = true;
        if (hr != S_OK)
        {
            result = false;

            // Convert to error string
            errorString = errorDescription + ": ";
            if (hr == E_POINTER)
            {
                errorString += "NULL pointer argument.";
            }
            else if (hr == VFW_E_NOT_CONNECTED)
            {
                errorString += "The input pin is not connected.";
            }
            else
            {
                errorString += "hr = (" + std::to_string(hr) + ")";
            }
        }

        return result;
    }

    bool CheckHResultUtils::CheckQueryInterfaceResult(const HRESULT hr, std::string& errorString, const std::string errorDescription)
    {
        bool result = true;
        if (hr != S_OK)
        {
            result = false;

            // Convert to error string
            errorString = errorDescription + ": ";
            if (hr == E_NOINTERFACE)
            {
                errorString += "Interface is not supported.";
            }
            else if (hr == E_POINTER)
            {
                errorString += "NULL pointer argument.";
            }
            else
            {
                errorString += "hr = (" + std::to_string(hr) + ")";
            }
        }

        return result;
    }
    bool CheckHResultUtils::CheckIKsPropertySetQuerySupportedResult(
        const HRESULT hr,
        std::string& errorString,
        const std::string errorDescription
    )
    {
        bool result = true;
        if (hr != S_OK)
        {
            result = false;

            // Convert to error string
            errorString = errorDescription + ": ";
            if (hr == E_NOTIMPL)
            {
                errorString += "Property set is not supported.";
            }
            else if (hr == E_PROP_ID_UNSUPPORTED)
            {
                errorString += "Property identifier (Id) is not supported for the specified property set.";
            }
            else if (hr == E_PROP_SET_UNSUPPORTED)
            {
                errorString += "Property set is not supported.";
            }
            else
            {
                errorString += "hr = (" + std::to_string(hr) + ")";
            }
        }

        return result;
    }
}
#pragma once
#ifndef WIN_CAMERA__DIRECTSHOW_CAMERA__UTILS__CHECK_HRESULT_UTILS_H
#define WIN_CAMERA__DIRECTSHOW_CAMERA__UTILS__CHECK_HRESULT_UTILS_H

//************Content************
#include <winerror.h>

namespace DirectShowCamera
{
    class CheckHResultUtils
    {
    public:

        /**
         * @brief Check the HResult for CoCreateInstance() and convert to error string
         * @param[in] hr HRESULT
         * @param[out] errorString The output error string, errorDescription + " : " + HRSTRING
         * @param[in] errorDescription Error description, default as "Error on CoCreateInstance()"
         * @return Return true if success
        */
        static bool CheckCoCreateInstanceResult(const HRESULT hr, std::string& errorString, const std::string errorDescription = "Error on CoCreateInstance()");

        /**
         * @brief Check the HResult for CreateClassEnumerator() and convert to error string
         * @param[in] hr HRESULT
         * @param[out] errorString The output error string, errorDescription + " : " + HRSTRING
         * @param[in] errorDescription (Option)Error description, default as "Error on CreateClassEnumerator()"
         * @return Return true if success
        */
        static bool CheckCreateClassEnumeratorResult(const HRESULT hr, std::string& errorString, const std::string errorDescription = "Error on CreateClassEnumerator()");

        /**
         * @brief Check the HResult for ICaptureGraphBuilder2::SetFiltergraph() and convert to error string
         * @param[in] hr HRESULT
         * @param[out] errorString The output error string, errorDescription + " : " + HRSTRING
         * @param[in] errorDescription (Option)Error description, default as "Error on ICaptureGraphBuilder2::SetFiltergraph()"
         * @return Return true if success
        */
        static bool CheckICGB2SetFiltergraphResult(const HRESULT hr, std::string& errorString, const std::string errorDescription = "Error on ICaptureGraphBuilder2::SetFiltergraph()");

        /**
         * @brief Check the HResult for IGraphBuilder::AddFilter() and convert to error string
         * @param[in] hr HRESULT
         * @param[out] errorString The output error string, errorDescription + " : " + HRSTRING
         * @param[in] errorDescription (Option)Error description, default as "Error on IGraphBuilder::AddFilter()"
         * @return Return true if success
        */
        static bool CheckIGBAddFilterResult(const HRESULT hr, std::string& errorString, const std::string errorDescription = "Error on IGraphBuilder::AddFilter()");

        /**
         * @brief Check the HResult for ICaptureGraphBuilder2::FindInterface() and convert to error string
         * @param[in] hr HRESULT
         * @param[out] errorString The output error string, errorDescription + " : " + HRSTRING
         * @param[in] errorDescription (Option)Error description, default as "Error on ICaptureGraphBuilder2::FindInterface()"
         * @return Return true if success
        */
        static bool CheckICGB2FindInterfaceResult(const HRESULT hr, std::string& errorString, const std::string errorDescription = "Error on ICaptureGraphBuilder2::FindInterface()");

        /**
         * @brief Check the HResult for ICaptureGraphBuilder2::RenderStream() and convert to error string
         * @param[in] hr HRESULT
         * @param[out] errorString The output error string, errorDescription + " : " + HRSTRING
         * @param[in] errorDescription (Option)Error description, default as "Error on ICaptureGraphBuilder2::RenderStream()"
         * @return Return true if success
        */
        static bool CheckICGB2RenderStreamResult(const HRESULT hr, std::string& errorString, const std::string errorDescription = "Error on ICaptureGraphBuilder2::RenderStream()");

        /**
         * @brief Check the HResult for IAMStreamConfig::GetFormat() and convert to error string
         * @param[in] hr HRESULT
         * @param[out] errorString The output error string, errorDescription + " : " + HRSTRING
         * @param[in] errorDescription (Option)Error description, default as "Error on IAMStreamConfig::GetFormat()"
         * @return Return true if success
        */
        static bool CheckIIAMSCGetFormatResult(const HRESULT hr, std::string& errorString, const std::string errorDescription = "Error on IAMStreamConfig::GetFormat()");

        /**
         * @brief Check the HResult for IAMStreamConfig::SetFormat() and convert to error string
         * @param[in] hr HRESULT
         * @param[out] errorString The output error string, errorDescription + " : " + HRSTRING
         * @param[in] errorDescription (Option)Error description, default as "Error on IAMStreamConfig::SetFormat()"
         * @return Return true if success
        */
        static bool CheckIIAMSCSetFormatResult(const HRESULT hr, std::string& errorStringL, const std::string errorDescription = "Error on IAMStreamConfig::SetFormat()");

        /**
         * @brief Check the HResult for IAMStreamConfig::GetNumberOfCapabilities() and convert to error string
         * @param[in] hr HRESULT
         * @param[out] errorString The output error string, errorDescription + " : " + HRSTRING
         * @param[in] errorDescription (Option)Error description, default as "Error on IAMStreamConfig::GetNumberOfCapabilities()"
         * @return Return true if success
        */
        static bool CheckIAMSCGetNumberOfCapabilitiesResult(const HRESULT hr, std::string& errorString, const std::string errorDescription = "Error on IAMStreamConfig::GetNumberOfCapabilities()");

        /**
         * @brief Check the HResult for IUnknown::QueryInterface and convert to error string
         * @param[in] hr HRESULT
         * @param[out] errorString The output error string, errorDescription + " : " + HRSTRING
         * @param[in] errorDescription (Option)Error description, default as "Error on IUnknown::QueryInterface"
         * @return Return true if success
        */
        static bool CheckQueryInterfaceResult(const HRESULT hr, std::string& errorString, const std::string errorDescription = "Error on IUnknown::QueryInterface()");

        /**
         * @brief Check the HResult for IKsPropertySet::QuerySupported and convert to error string
         * @param[in] hr HRESULT
         * @param[out] errorString The output error string, errorDescription + " : " + HRSTRING
         * @param[in] errorDescription (Option)Error description, default as "Error on IKsPropertySet::QuerySupported"
         * @return Return true if success
        */
        static bool CheckIKsPropertySetQuerySupportedResult(const HRESULT hr, std::string& errorString, const std::string errorDescription = "Error on IKsPropertySet::QuerySupported()");
    };
}


//*******************************

#endif
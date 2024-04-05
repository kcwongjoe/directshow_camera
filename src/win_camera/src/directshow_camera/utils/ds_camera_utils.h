#pragma once
#ifndef WIN_CAMERA__DIRECTSHOW_CAMERA__UTILS__DIRECTSHOW_CAMERA_UTILS_H
#define WIN_CAMERA__DIRECTSHOW_CAMERA__UTILS__DIRECTSHOW_CAMERA_UTILS_H

//************Content************
#include <windows.h>
#include <windef.h>

#include "directshow_camera/ds_guid.h"
#include <atlconv.h>
#include "directshow_camera/utils/check_hresult_utils.h"

// Direct show is native code, complier it in unmanaged
#pragma managed(push, off)
#include <dshow.h>
#pragma managed(pop)
#pragma comment(lib, "strmiids")

namespace DirectShowCameraUtils
{
    // ******To String******
    std::string BSTRToString(BSTR bstr, int cp = CP_UTF8);
    std::string ToString(GUID guid);

    // ******Release******
    void FreeMediaType(AM_MEDIA_TYPE& amMediaType);
    void DeleteMediaType(AM_MEDIA_TYPE** amMediaType);
    void DestroyGraph(IGraphBuilder* iGraphBuilder);
    void NukeDownStream(IGraphBuilder* iGraphBuilder, IBaseFilter* iBaseFilter);

    /**
     * @brief Safe release COM interface pointers. e.g. IBaseFilter* iBaseFilter; SafeRelease(&iBaseFilter);
     * @tparam T COM interface pointers
     * @param ppT Pointer of COM interface pointers
    */
    template <class T> void SafeRelease(T** ppT)
    {
        if (*ppT != NULL)
        {
            (*ppT)->Release();
            *ppT = NULL;
        }
    }

    /**
     * @brief A decorator to extract IMoniker and IPropertyBag. This can be use to retreve the camera information by setting clsid as CLSID_VideoInputDeviceCategory.
     *
     * @tparam Func void(IMoniker*, IPropertyBag*)
     * @param[in] clsid Category of IEnumMoniker
     * @param[in] func Lambda function to process IMoniker and IPropertyBag
     * @param[out] errorString Error String
     * @return bool Return true if success.
    */
    template <typename Func> bool IPropertyDecorator(
        GUID clsid,
        Func func,
        std::string& errorString
    )
    {
        bool result = true;

        // Create the System Device Enumerator
        ICreateDevEnum* iCreateDevEnum = NULL;
        if (result)
        {
            HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&iCreateDevEnum));
            result = DirectShowCamera::CheckHResultUtils::CheckCoCreateInstanceResult(hr, errorString, "Error on creating the ICreateDevEnum");
        }

        // Obtain a class enumerator for the video input category.
        IEnumMoniker* iEnumMoniker = NULL;
        if (result)
        {
            HRESULT hr = iCreateDevEnum->CreateClassEnumerator(clsid, &iEnumMoniker, 0);
            result = DirectShowCamera::CheckHResultUtils::CheckCreateClassEnumeratorResult(hr, errorString, "Error on obtaining the IEnumMoniker");
        }

        // Enumerate the monikers
        if (result)
        {
            IPropertyBag* propertyBag = NULL;
            IMoniker* moniker = NULL;

            try
            {
                while (iEnumMoniker->Next(1, &moniker, NULL) == S_OK)
                {
                    // Get a pointer to the IPropertyBag interface
#pragma warning( push )
#pragma warning( disable : 6387)
                    HRESULT hr = moniker->BindToStorage(NULL, NULL, IID_PPV_ARGS(&propertyBag));
#pragma warning( pop )
                    if (FAILED(hr))
                    {
                        SafeRelease(&moniker);
                        continue;
                    }

                    // Run the function
                    func(moniker, propertyBag);

                    // Release
                    SafeRelease(&propertyBag);
                    SafeRelease(&moniker);
                }
            }
            catch (...)
            {
                // Release
                SafeRelease(&propertyBag);
                SafeRelease(&moniker);
                SafeRelease(&iEnumMoniker);
                SafeRelease(&iCreateDevEnum);

                // Rethrow
                throw;
            }

            // Release
            SafeRelease(&iEnumMoniker);
            SafeRelease(&iCreateDevEnum);
        }
        return result;
    }

    /**
     * @brief A decorator to extract IPin and PIN_INFO from IMoniker. Use IPropertyDecorator() and AmMediaTypeDecorator() can retrieve the video format from cameras.
     * @tparam Func void(IPin*, PIN_INFO*)
     * @param[in] iMoniker IMoniker
     * @param[in] func Lambda function to process IPin and PIN_INFO
     * @param[out] errorString Error String
     * @return Return true if success.
    */
    template <typename Func> bool IPinDecorator(
        IMoniker* iMoniker,
        Func func,
        std::string& errorString
    )
    {
        if (!iMoniker) return false;

        bool result = true;

        // Get IBaseFilter
        IBaseFilter* iBaseFilter;
        if (result)
        {
#pragma warning( push )
#pragma warning( disable : 6387)
            HRESULT hr = iMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void**)&iBaseFilter);
#pragma warning( pop )
            if (FAILED(hr))
            {
                result = false;
            }
        }

        // Get IEnumPins
        IEnumPins* iEnumPin = NULL;
        if (result)
        {
            HRESULT hr = iBaseFilter->EnumPins(&iEnumPin);
            if (FAILED(hr))
            {
                SafeRelease(&iEnumPin);
                result = false;
            }
        }

        if (result)
        {
            // Start at beginning
            iEnumPin->Reset();

            ULONG pinFetched = 0;
            IPin* iPin = NULL;
            try
            {
                while (SUCCEEDED(iEnumPin->Next(1, &iPin, &pinFetched)) && pinFetched) {

                    // Get PIN_INFO
                    PIN_INFO pinInfo;
                    if (FAILED(iPin->QueryPinInfo(&pinInfo))) {
                        continue;
                    }

                    // Run
                    func(iPin, &pinInfo);

                    // Release
                    SafeRelease(&iPin);
                }
            }
            catch (...)
            {
                // Release
                SafeRelease(&iPin);
                SafeRelease(&iEnumPin);
                SafeRelease(&iBaseFilter);

                // Rethrow
                throw;
            }

            // Release
            SafeRelease(&iEnumPin);
            SafeRelease(&iBaseFilter);
        }

        return result;
    }

    /**
     * @brief A decorator to process AM_MEDIA_TYPE from IPin.
     * @tparam Func void(AM_MEDIA_TYPE*)
     * @param[in] iPin IPin
     * @param[in] func Lambda function to process AM_MEDIA_TYPE
     * @param[out] errorString Error String
     * @param[in] (Optional) releaseAMMediaType Set as false if you want to release AMMediaType in your function. Default as true which AMMediaType will be released internally after your funciton processed.
     * @return Return true if success.
    */
    template <typename Func> bool AmMediaTypeDecorator(
        IPin* iPin,
        Func func,
        std::string& errorString,
        bool releaseAMMediaType = true
    )
    {
        if (!iPin) return false;

        bool result = true;

        // Get IEnumMediaTypes
        IEnumMediaTypes* iEnumMediaType = NULL;
        if (FAILED(iPin->EnumMediaTypes(&iEnumMediaType)))
        {
            result = false;
        }

        if (result)
        {
            AM_MEDIA_TYPE* amMediaType = NULL;
            iEnumMediaType->Reset();

            ULONG amMediaTypeFetched = 0;
            try {
                while (SUCCEEDED(iEnumMediaType->Next(1, &amMediaType, &amMediaTypeFetched)) && amMediaTypeFetched) {

                    // Run
                    func(amMediaType);

                    //Release
                    if (releaseAMMediaType) DeleteMediaType(&amMediaType);
                }
            }
            catch (...)
            {
                // Release
                if (releaseAMMediaType) DeleteMediaType(&amMediaType);
                SafeRelease(&iEnumMediaType);

                // Rethrow
                throw;
            }

            // Release
            if (releaseAMMediaType) DeleteMediaType(&amMediaType);
            SafeRelease(&iEnumMediaType);
        }

        return result;
    }

    /**
     * @brief A decorator to process AM_MediaType
     * @tparam func void(AM_MEDIA_TYPE*)
     * @param[in] streamConfig StreamConfig to get the AM_MediaType
     * @param[in] func Lambda funciton for processing AM_MediaType
     * @param[out] errorString Error String
     * @return bool Return true if success.
    */
    template <typename MediaTypeFunc> bool AmMediaTypeDecorator(
        IAMStreamConfig* streamConfig,
        MediaTypeFunc func,
        std::string& errorString
    )
    {
        bool result = true;
        HRESULT hr = NO_ERROR;

        AM_MEDIA_TYPE* amMediaType;
        if (result)
        {
            hr = streamConfig->GetFormat(&amMediaType);
            result = DirectShowCamera::CheckHResultUtils::CheckIIAMSCGetFormatResult(hr, errorString, "Error on getting media type");
        }

        //Run the function
        if (result)
        {
            try
            {
                func(amMediaType);
            }
            catch (...)
            {
                //Delete media type
                DeleteMediaType(&amMediaType);
                throw;
            }

            //Delete media type
            DeleteMediaType(&amMediaType);
        }

        return result;
    }


    /**
     * @brief A decorator to process IAMVideoProcAmp
     * @tparam func void(IAMVideoProcAmp*)
     * @param[in] videoInputFilter Video Input Filter to get the IAMVideoProcAmp
     * @param[in] func Lambda funciton for processing IAMVideoProcAmp
     * @param[out] errorString Error String
     * @return bool Return true if success.
    */
    template <typename AmVideoProcAmpFunc> bool AmVideoProcAmpDecorator(
        IBaseFilter* videoInputFilter,
        AmVideoProcAmpFunc func,
        std::string& errorString
    )
    {
        bool success;
        HRESULT hr = NO_ERROR;

        IAMVideoProcAmp* amVideoProcAmp = NULL;
        hr = videoInputFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&amVideoProcAmp);
        success = DirectShowCamera::CheckHResultUtils::CheckQueryInterfaceResult(hr, errorString, "Error on getting IAMVideoProcAmp");

        //Run the function
        if (success)
        {
            try
            {
                func(amVideoProcAmp);
            }
            catch (...)
            {
                // Release
                SafeRelease(&amVideoProcAmp);

                // Rethrow
                throw;
            }

            // Release
            SafeRelease(&amVideoProcAmp);

            return true;
        }
        else
        {
            return false;
        }
    }

    /**
     * @brief A decorator to process IAMCameraControl
     * @tparam func void(IAMCameraControl*)
     * @param[in] videoInputFilter Video Input Filter to get the IAMCameraControl
     * @param[in] func Lambda funciton for processing IAMCameraControl
     * @param[out] errorString Error String
     * @return bool Return true if success.
     */
    template <typename CameraControlFunc> bool AmCameraControlDecorator(
        IBaseFilter* videoInputFilter,
        CameraControlFunc func,
        std::string& errorString = NULL
    )
    {
        bool success;
        HRESULT hr = NO_ERROR;

        IAMCameraControl* amCameraControl = NULL;
        hr = videoInputFilter->QueryInterface(IID_IAMCameraControl, (void**)&amCameraControl);
        success = DirectShowCamera::CheckHResultUtils::CheckQueryInterfaceResult(hr, errorString, "Error on getting camera control");

        //Run the function
        if (success)
        {
            try
            {
                func(amCameraControl);
            }
            catch (...)
            {
                // Release
                SafeRelease(&amCameraControl);

                // Rethrow
                throw;
            }

            // Release
            SafeRelease(&amCameraControl);

            return true;
        }
        else
        {
            return false;
        }
    }
}


//*******************************

#endif
#pragma once
#ifndef WIN_CAMERA__DIRECTSHOW_CAMERA__UTILS__DIRECTSHOW_CAMERA_UTILS_H
#define WIN_CAMERA__DIRECTSHOW_CAMERA__UTILS__DIRECTSHOW_CAMERA_UTILS_H

//************Content************

#include "directshow_camera/utils/check_hresult_utils.h"

#include "directshow_camera/ds_header.h"

namespace DirectShowCameraUtils
{

#pragma region Release

    // ******Release******
    void FreeMediaType(AM_MEDIA_TYPE& amMediaType);
    void DeleteMediaType(AM_MEDIA_TYPE** amMediaType);
    void DestroyGraph(IGraphBuilder* iGraphBuilder);
    void NukeDownStream(IGraphBuilder* iGraphBuilder, IBaseFilter* iBaseFilter);

    /**
     * @brief Safe release COM interface pointers. e.g. IBaseFilter* iBaseFilter; SafeRelease(&iBaseFilter);
     * @tparam T COM interface pointers
     * @param[in,out] ppT Pointer of COM interface pointers
    */
    template <class T> void SafeRelease(T** ppT)
    {
        if (*ppT != NULL)
        {
            (*ppT)->Release();
            *ppT = NULL;
        }
    }

#pragma endregion

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
        if (iMoniker == NULL) return false;

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
                while (iEnumPin->Next(1, &iPin, &pinFetched) == S_OK && pinFetched) {

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
     * @brief A decorator to extract AM_MEDIA_TYPE from IPin.
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
        if (iPin == NULL) return false;

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
                while (iEnumMediaType->Next(1, &amMediaType, &amMediaTypeFetched) == S_OK && amMediaTypeFetched) {

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
     * @brief A decorator to extract AM_MediaType from IAMStreamConfig
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
     * @brief A decorator to extract IAMVideoProcAmp from IBaseFilter
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
     * @brief A decorator to extract IAMCameraControl from IBaseFilter
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

    /**
     * @brief A decorator to extract IVideoProcAmp from IBaseFilter
     * @tparam func void(IVideoProcAmp *)
     * @param[in] videoInputFilter Video Input Filter to get the IVideoProcAmp 
     * @param[in] func Lambda funciton for processing IVideoProcAmp 
     * @param[out] errorString Error String
     * @return bool Return true if success.
     */
    template <typename VideoProcAmpFunc> bool VideoProcAmpDecorator(
        IBaseFilter* videoInputFilter,
        VideoProcAmpFunc func,
        std::string& errorString = NULL
    )
    {
        bool success;
        HRESULT hr = NO_ERROR;

        // Get IKsTopologyInfo
        IKsTopologyInfo* ksTopologyInfo = NULL;
        hr = videoInputFilter->QueryInterface(__uuidof(IKsTopologyInfo), (void**)&ksTopologyInfo);
        success = DirectShowCamera::CheckHResultUtils::CheckQueryInterfaceResult(hr, errorString, "Error on getting ksTopologyInfo");

        //Get IVideoProcAmp
        if (success)
        {
            // Get the number of nodes in IKsTopologyInfo
            auto numOfNodes = DWORD{ 0 };
            ksTopologyInfo->get_NumNodes(&numOfNodes);

            for (DWORD i = 0; i < numOfNodes; i++) {

                // Get node type
                GUID nodeType;
                hr = ksTopologyInfo->get_NodeType(i, &nodeType);

                if (hr == S_OK)
                {
                    // Check if nodeType is KSNODETYPE_VIDEO_PROCESSING which can expose IVideoProcAmp 
                    if (nodeType == KSNODETYPE_VIDEO_PROCESSING) {
                        IVideoProcAmp* videoProcAmp = NULL;
                        hr = ksTopologyInfo->CreateNodeInstance(i, __uuidof(IVideoProcAmp), (void**)&videoProcAmp);

                        //Run the function
                        if (hr == S_OK)
                        {
                            try
                            {
                                func(videoProcAmp);
                            }
                            catch (...)
                            {
                                // Release
                                SafeRelease(&videoProcAmp);
                                SafeRelease(&ksTopologyInfo);

                                // Rethrow
                                throw;
                            }

                            // Release
                            SafeRelease(&videoProcAmp);
                        }
                        else
                        {
                            errorString = "Error on getting IVideoProcAmp: hr = (" + std::to_string(hr) + ")";
                            success = false;
                        }

                        break;
                    }
                }
                else
                {
                    errorString = "Error on IKsTopologyInfo::get_NodeType: hr = (" + std::to_string(hr) + ")";
                    success = false;
                    break;
                }
            }

            // Release
            SafeRelease(&ksTopologyInfo);
        }

        return success;
    }
}


//*******************************

#endif
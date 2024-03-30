#include "directshow_camera/utils/ds_camera_utils.h"

namespace DirectShowCameraUtils
{
#pragma region string

    /**
     * @brief Casting GUID to string
     * @param guid GUID
     * @return Return GUID string {}
    */
    std::string to_string(GUID guid)
    {
        std::string result;

        // Get string
        LPOLESTR guidString;
        HRESULT hr = StringFromCLSID(guid, &guidString);
        if (SUCCEEDED(hr))
        {
            USES_CONVERSION;
            result = OLE2CA(guidString);
        }

        return result;
    }

    /**
     * @brief Convert BSTR to string
     * @param[in] bstr BSTR
     * @param[in] cp (Option) CodePage, Default as CP_UTF8
     * @return Return string
    */
    std::string bstrToString(BSTR bstr, int cp)
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

#pragma endregion string

#pragma region Time

    /**
     * @brief Convert time_t to string
     * 
     * @param[in] time time to be conveted
     * @param[in] format (Option) time format. Default as "%Y-%m-%d %H:%M:%S"
     * @return std::string 
     */
    std::string to_string(time_t time, std::string format)
    {
        // Initialize
        struct tm* currentTimeInfo;
        char buffer[80];

#pragma warning(suppress : 4996)
        currentTimeInfo = localtime(&time);

        // Convert to string
        strftime(buffer, sizeof(buffer), format.c_str(), currentTimeInfo);
        std::string result(buffer);

        return result;
    }

    /**
     * @brief Get millisecond from time
     * @param time 
     * @return 
    */
    int getMilliseconds(std::chrono::system_clock::time_point time)
    {
        auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(time);
        auto fraction = time - seconds;
        int ms = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(fraction).count());
        return ms;
    }

#pragma endregion Time

#pragma region Release

    /**
     * @brief Deletes the format block in an AM_MEDIA_TYPE. e.g. AM_MEDIA_TYPE mt; freeMediaType(mt);
     * @param amMediaType AM_MEDIA_TYPE
    */
    void freeMediaType(AM_MEDIA_TYPE& amMediaType)
    {
        if (amMediaType.cbFormat != 0)
        {
            CoTaskMemFree((PVOID)amMediaType.pbFormat);
            amMediaType.cbFormat = 0;
            amMediaType.pbFormat = NULL;
        }
        if (amMediaType.pUnk != NULL)
        {
            // pUnk should not be used.
            amMediaType.pUnk->Release();
            amMediaType.pUnk = NULL;
        }
    }

    /**
     * @brief Delete the pointer of AM_MEDIA_TYPE, include the format block. Use freeMediaType() for object. e.g. AM_MEDIA_TYPE* mt; deleteMediaType(&mt);
     * @param amMediaType AM_MEDIA_TYPE to be deleted
    */
    void deleteMediaType(AM_MEDIA_TYPE** amMediaType)
    {
        if (*amMediaType != NULL)
        {
            freeMediaType(**amMediaType);
            CoTaskMemFree(*amMediaType);
            *amMediaType = NULL;
        }
    }

    /**
     * @brief Destroy graph. Reference from opencv::cap_dshow.cpp
     * @param iGraphBuilder
    */
    void destroyGraph(IGraphBuilder* iGraphBuilder) {

        if (iGraphBuilder)
        {
            HRESULT hr = 0;

            int i = 0;
            while (hr == NOERROR)
            {
                IEnumFilters* pEnum = 0;
                ULONG cFetched = 0;

                // We must get the enumerator again every time because removing a filter from the graph
                // invalidates the enumerator. We always get only the first filter from each enumerator.
                hr = iGraphBuilder->EnumFilters(&pEnum);
                if (FAILED(hr)) { return; }

                IBaseFilter* pFilter = NULL;
                if (pEnum->Next(1, &pFilter, &cFetched) == S_OK)
                {
                    FILTER_INFO FilterInfo;
                    memset(&FilterInfo, 0, sizeof(FilterInfo));
                    hr = pFilter->QueryFilterInfo(&FilterInfo);
                    FilterInfo.pGraph->Release();

                    int count = 0;
                    char buffer[255];
                    memset(buffer, 0, 255 * sizeof(char));

                    while (FilterInfo.achName[count] != 0x00)
                    {
                        buffer[count] = (char)FilterInfo.achName[count];
                        count++;
                    }

                    // Remove filter
                    hr = iGraphBuilder->RemoveFilter(pFilter);
                    if (FAILED(hr)) { return; }

                    pFilter->Release();
                    pFilter = NULL;
                }
                pEnum->Release();
                pEnum = NULL;

                if (cFetched == 0)
                    break;
                i++;
            }
        }
    }

    /**
     * @brief Take apart the graph from the capture device downstream to the null renderer
     * @param iGraphBuilder
     * @param iBaseFilter
    */
    void nukeDownStream(IGraphBuilder* iGraphBuilder, IBaseFilter* iBaseFilter) {

        if (iGraphBuilder && iBaseFilter)
        {

            IPin* pP, * pTo;
            ULONG u;
            IEnumPins* pins = NULL;
            PIN_INFO pininfo;
            HRESULT hr = iBaseFilter->EnumPins(&pins);
            if (hr != S_OK || !pins)
                return;
            pins->Reset();
            while (hr == NOERROR)
            {
                hr = pins->Next(1, &pP, &u);
                if (hr == S_OK && pP)
                {
                    pP->ConnectedTo(&pTo);
                    if (pTo)
                    {
                        hr = pTo->QueryPinInfo(&pininfo);
                        if (hr == NOERROR)
                        {
                            if (pininfo.dir == PINDIR_INPUT)
                            {
                                nukeDownStream(iGraphBuilder, pininfo.pFilter);
                                iGraphBuilder->Disconnect(pTo);
                                iGraphBuilder->Disconnect(pP);
                                iGraphBuilder->RemoveFilter(pininfo.pFilter);
                            }
                            pininfo.pFilter->Release();
                            pininfo.pFilter = NULL;
                        }
                        pTo->Release();
                    }
                    pP->Release();
                }
            }
            pins->Release();
        }
    }

#pragma endregion Release

#pragma region COM Library

    /**
     * @brief Initialize COM library
     *
     * @return Return the status of initialized.
    */
    bool initCOMLib()
    {
        HRESULT hr = NO_ERROR;

        if (!s_isInitializedCOMLib)
        {
            hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
        }

        return SUCCEEDED(hr);
    }

    /**
     * @brief Uninitialize COM library
    */
    void uninitCOMLib()
    {
        CoUninitialize();
        s_isInitializedCOMLib = false;
    }

#pragma endregion COM Library
}
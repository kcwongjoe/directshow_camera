/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#include "directshow_camera/utils/ds_camera_utils.h"

namespace DirectShowCameraUtils
{
#pragma region Release

    /**
     * @brief Deletes the format block in an AM_MEDIA_TYPE. e.g. AM_MEDIA_TYPE mt; FreeMediaType(mt);
     * @param[in,out] amMediaType AM_MEDIA_TYPE
    */
    void FreeMediaType(AM_MEDIA_TYPE& amMediaType)
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
     * @brief Delete the pointer of AM_MEDIA_TYPE, include the format block. Use FreeMediaType() for object. e.g. AM_MEDIA_TYPE* mt; DeleteMediaType(&mt);
     * @param[in,out] amMediaType AM_MEDIA_TYPE to be deleted
    */
    void DeleteMediaType(AM_MEDIA_TYPE** amMediaType)
    {
        if (*amMediaType != NULL)
        {
            FreeMediaType(**amMediaType);
            CoTaskMemFree(*amMediaType);
            *amMediaType = NULL;
        }
    }

    /**
     * @brief Destroy graph. Reference from opencv::cap_dshow.cpp
     * @param[in,out] iGraphBuilder
    */
    void DestroyGraph(IGraphBuilder* iGraphBuilder) {

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
     * @param[in,out] iGraphBuilder
     * @param[in] iBaseFilter
    */
    void NukeDownStream(IGraphBuilder* iGraphBuilder, IBaseFilter* iBaseFilter) {

        if (iGraphBuilder && iBaseFilter)
        {

            IPin* pP, * pTo;
            ULONG u;
            IEnumPins* pins = NULL;
            PIN_INFO pininfo;
            HRESULT hr = iBaseFilter->EnumPins(&pins);
            if (hr != S_OK || pins == NULL)
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
                                NukeDownStream(iGraphBuilder, pininfo.pFilter);
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
}
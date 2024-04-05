#include "directshow_camera/ds_video_format_list.h"

namespace DirectShowCamera
{
    DirectShowVideoFormatList::DirectShowVideoFormatList()
    {

    }

    DirectShowVideoFormatList::DirectShowVideoFormatList(std::vector<DirectShowVideoFormat> videoFormatList)
    {
        m_videoFormatList = videoFormatList;
        for (int i=0;i<m_videoFormatList.size();i++)
        {
            m_amMediaTypeList.push_back(NULL);
        }
    }

    DirectShowVideoFormatList::DirectShowVideoFormatList(
        IAMStreamConfig* streamConfig,
        std::string& errorString,
        bool& success,
        std::vector<GUID>* supportVideoTypes
    )
    {
        success = Import(streamConfig, errorString, supportVideoTypes);
    }

    DirectShowVideoFormatList::~DirectShowVideoFormatList()
    {
        Clear();
    }

    void DirectShowVideoFormatList::Clear()
    {
        m_videoFormatList.clear();
        DeleteAMMediaTypeList();
    }

    bool DirectShowVideoFormatList::Update(IAMStreamConfig* streamConfig, std::string& errorString, std::vector<GUID>* supportVideoTypes)
    {
        // Clear old data
        Clear();

        // Update new data
        return Import(streamConfig, errorString, supportVideoTypes);
    }

    int DirectShowVideoFormatList::Size() const
    {
        return m_videoFormatList.size();
    }

    AM_MEDIA_TYPE* DirectShowVideoFormatList::getAMMediaType(int index) const
    {
        return m_amMediaTypeList.at(index);
    }

    std::string DirectShowVideoFormatList::ToStringInDetail()
    {
        if (m_videoFormatList.size() > 0)
        {
            std::string result = "\n";
            for (int i = 0; i < m_videoFormatList.size(); i++)
            {
                result += "Index: " + std::to_string(i) + "\n";
                if (!m_amMediaTypeList.at(i))
                {
                    result += DirectShowVideoFormat::to_string(m_amMediaTypeList.at(i));
                }
                else
                {
                    // AmMediaType is null, use DirectShowVideoFormat string instead
                    result += std::string(m_videoFormatList.at(i)) + "\n";
                }
            }
            return result;
        }
        else {
            return "null";
        }
    }

    std::vector<DirectShowVideoFormat> DirectShowVideoFormatList::getVideoFormatList() const
    {
        return m_videoFormatList;
    }

    DirectShowVideoFormat DirectShowVideoFormatList::getVideoFormat(int index) const
    {
        return m_videoFormatList.at(index);
    }

    void DirectShowVideoFormatList::DeleteAMMediaTypeList()
    {
        for (int i=0;i<m_amMediaTypeList.size();i++)
        {
            if (m_amMediaTypeList.at(i) != NULL)
            {
                DirectShowCameraUtils::DeleteMediaType(&m_amMediaTypeList.at(i));
            }
        }
        m_amMediaTypeList.clear();
    }

    bool DirectShowVideoFormatList::Import(
        IAMStreamConfig* streamConfig,
        std::string& errorString,
        std::vector<GUID>* supportVideoTypes
    )
    {
        HRESULT hr = NO_ERROR;
        bool result = true;
        int numOfResolution = 0;
        int configSize = 0;

        // Get number of resolution
        hr = streamConfig->GetNumberOfCapabilities(&numOfResolution, &configSize);
        result = CheckHResultUtils::CheckIAMSCGetNumberOfCapabilitiesResult(hr, errorString, "Error on checking number of resolution");

        if (result)
        {
            // Confirm that this is a video structure.
            if (configSize == sizeof(VIDEO_STREAM_CONFIG_CAPS))
            {
                // Use the video capabilities structure.
                for (int i = 0; i < numOfResolution; i++)
                {
                    // Get AmMediaType
                    VIDEO_STREAM_CONFIG_CAPS scc;
                    AM_MEDIA_TYPE* amMediaType = NULL;
                    HRESULT hr = streamConfig->GetStreamCaps(i, &amMediaType, (BYTE*)&scc);

                    // Convert AmMediaType to VideoFormat
                    DirectShowVideoFormat videoFormat = DirectShowVideoFormat::Create(amMediaType);

                    // Check empty and suppport video types
                    bool addVideoFormat = true;
                    if (videoFormat.isEmpty())
                    {
                        addVideoFormat = false;
                    }
                    else if (supportVideoTypes)
                    {
                        // Check support video types
                        if (std::count(supportVideoTypes->begin(), supportVideoTypes->end(), videoFormat.getVideoType()) <= 0)
                        {
                            addVideoFormat = false;
                        }
                    }
                    
                    // Found
                    if (addVideoFormat)
                    {
                        // Add
                        m_videoFormatList.push_back(videoFormat);
                        m_amMediaTypeList.push_back(amMediaType);
                    }
                }

                // Todo: sort and unique videoFormats(std::vector<DirectShowVideoFormat*>*) here.
            }
        }

        return result;
    }
}

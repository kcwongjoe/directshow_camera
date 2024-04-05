#pragma once
#ifndef WIN_CAMERA__DIRECTSHOW_CAMERA__DIRECTSHOW_VIDEO_FORMAT_LIST_H
#define WIN_CAMERA__DIRECTSHOW_CAMERA__DIRECTSHOW_VIDEO_FORMAT_LIST_H

//************Content************

#include "directshow_camera/ds_video_format.h"

namespace DirectShowCamera
{
    /**
     * @brief A class to store the list of Directshow Video Format information
     * 
     */
    class DirectShowVideoFormatList
    {
    public:
        DirectShowVideoFormatList();

        DirectShowVideoFormatList(
            std::vector<DirectShowVideoFormat> m_videoFormatList
        );

        DirectShowVideoFormatList(
            IAMStreamConfig* streamConfig,
            std::string& errorString,
            bool& success,
            std::vector<GUID>* supportVideoTypes = NULL
        );

        ~DirectShowVideoFormatList();

        void Clear();

        bool Update(
            IAMStreamConfig* streamConfig,
            std::string& errorString,
            std::vector<GUID>* supportVideoTypes = NULL
        );

        int Size() const;

        std::vector<DirectShowVideoFormat> getVideoFormatList() const;
        DirectShowVideoFormat getVideoFormat(int index) const;

        AM_MEDIA_TYPE* getAMMediaType(int index) const;

        // To string

        std::string ToStringInDetail();

        operator std::string() const
        {
            if (m_videoFormatList.size() > 0)
            {
                std::string result = "\n";
                for (int i = 0; i < m_videoFormatList.size(); i++)
                {
                    result += "Index: " + std::to_string(i) + "\n";
                    result += std::string(m_videoFormatList.at(i)) + "\n";
                }
                return result;
            }
            else {
                return "null";
            }
        }

        friend std::ostream& operator << (std::ostream& out, const DirectShowVideoFormatList& obj) {
            return out << (std::string)obj;
        }

    private:
        void DeleteAMMediaTypeList();

        bool Import(
            IAMStreamConfig* streamConfig,
            std::string& errorString,
            std::vector<GUID>* supportVideoTypes
        );

    private:

        // VideoFormatList and AmMediaTypeList is pair.
        std::vector<DirectShowVideoFormat> m_videoFormatList;
        std::vector<AM_MEDIA_TYPE*> m_amMediaTypeList;
    };
}

//*******************************

#endif

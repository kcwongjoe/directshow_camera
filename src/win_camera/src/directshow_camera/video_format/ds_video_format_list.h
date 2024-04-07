#pragma once
#ifndef WIN_CAMERA__DIRECTSHOW_CAMERA__VIDEO_FORMAT__DIRECTSHOW_VIDEO_FORMAT_LIST_H
#define WIN_CAMERA__DIRECTSHOW_CAMERA__VIDEO_FORMAT__DIRECTSHOW_VIDEO_FORMAT_LIST_H

//************Content************

#include "directshow_camera/video_format/ds_video_format.h"

namespace DirectShowCamera
{
    /**
     * @brief A class to store the list of Directshow Video Format information
     * 
     */
    class DirectShowVideoFormatList
    {
    public:

        /**
        * @brief Sort and unique the video format list.
        * @param[in, out] directShowVideoFormats Input the video format list, and sort it.
        */
        static void SortAndUnique(std::vector<DirectShowVideoFormat>& directShowVideoFormats);

    public:

        /**
        * @brief Constructer. This constructor create a emply list and you can use Update() later to update the list.
        */
        DirectShowVideoFormatList();

        /**
        * @brief Constructer. This constructor create a list with the given video format list and no AM_MEDIA_TYPE list. 
        *        Object created by this constructor can only be used to store information and can not be used to
        *        set video format in DirecShow.
        *
        * @param[in] m_videoFormatList The list of DirectShowVideoFormat    
        */
        DirectShowVideoFormatList(
            std::vector<DirectShowVideoFormat> m_videoFormatList
        );

        /**
        * @brief Constructer. This is the suggested constructor to used.
        * 
        * @param[in] streamConfig The IAMStreamConfig object
        * @param[out] errorString The error string
        * @param[out] success The success flag
        * @param[in] supportVideoTypes (Optional) The list of support video types
        */
        DirectShowVideoFormatList(
            IAMStreamConfig* streamConfig,
            std::string& errorString,
            bool& success,
            std::optional<std::vector<GUID>> supportVideoTypes = std::nullopt
        );

        /**
        * @brief Destructer
        */
        ~DirectShowVideoFormatList();

        /**
        * @brief Clear and release memory
        */
        void Clear();

        /**
        * @brief Update the list
        * 
        * @param[in] streamConfig The IAMStreamConfig object
        * @param[out] errorString The error string
        * @param[in] supportVideoTypes (Optional) The list of support video types
        */
        bool Update(
            IAMStreamConfig* streamConfig,
            std::string& errorString,
            std::optional<std::vector<GUID>> supportVideoTypes = std::nullopt
        );

        /**
        * @brief Get the size of the list
        */
        int Size() const;

        /**
        * @brief Get the list of video format
        */
        std::vector<DirectShowVideoFormat> getVideoFormatList() const;

        /**
        * @brief Get the video format at the given index
        * 
        * @param[in] index The index of the video format
        */
        DirectShowVideoFormat getVideoFormat(const int index) const;

        /**
        * @brief Get the AM_MEDIA_TYPE at the given index
        * 
        * @param[in] index The index of the AM_MEDIA_TYPE
        */
        AM_MEDIA_TYPE* getAMMediaType(const int index) const;

        // To string

        /**
        * @brief Return a string that contains the list of video format in detail from AM_MEDIA_TYPE
        */
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

        /**
        * @brief Delete the AM_MEDIA_TYPE list and release memory
        */
        void DeleteAMMediaTypeList();

        /**
        * @brief Import the video format list
        * 
        * @param[in] streamConfig The IAMStreamConfig object
        * @param[out] errorString The error string
        * @param[in] supportVideoTypes (Optional) The list of support video types
        */
        bool Import(
            IAMStreamConfig* streamConfig,
            std::string& errorString,
            std::optional<std::vector<GUID>> supportVideoTypes = std::nullopt
        );

    private:

        // VideoFormatList and AmMediaTypeList must be a pair.
        std::vector<DirectShowVideoFormat> m_videoFormatList;
        std::vector<AM_MEDIA_TYPE*> m_amMediaTypeList;
    };
}

//*******************************

#endif

/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__FRAME_H
#define WIN_CAMERA__WIN_CAMERA__FRAME_H

//************Content************

#include <guiddef.h>

#include "opencv_utils/cv_mat_convertor.h"
#include "utils/gdi_plus_utils.h"

#include <memory>
#include <ostream>
#include <string>
#include <functional>
#include <filesystem>

namespace WinCamera
{
    class Frame
    {
    public:
        typedef std::function<void(unsigned char* data, unsigned long& frameIndex)> ImportDataFunc;

    public:


#pragma region Constructor and Destructor
        /**
        * @brief Constructor
        */
        Frame();

        /**
        * @brief Destructor
        */
        ~Frame();

        /**
        * @brief Copy constructor
        */
        Frame(const Frame& other);

        /**
        * @brief Move constructor
        */
        Frame(Frame&& other) noexcept;

        /**
        * @brief Clear the frame
        */
        void Clear();

#pragma endregion Constructor and Destructor

#pragma region Frame

        /**
        * @brief Import data
        * @param[in] frameSize Frame size in bytes
        * @param[in] width Frame width in pixel
        * @param[in] height Frame height in pixel
        * @param[in] frameType Frame type
        * @param[in] importDataFunc A function to import data. The function should be in the form of void(unsigned char* data, unsigned long& frameIndex)
        */
        void ImportData(
            const long frameSize,
            const int width,
            const int height,
            const GUID frameType,
            ImportDataFunc importDataFunc
        );

        /**
         * @brief Get frame
         * @param[out] numOfBytes   Number of bytes of the frame.
         * @param[in] clone         Whether return a frame clone. If true, the frame will be cloned to the return pointer so that data will 
         *                          still be there even this Frame has been deleted. You must release the memory of the return pointer after use
         *                          by yourself. If false, the internal data pointer will return. If this Frame is deleted, the return data pointer
         *                          will be invalid. For example, if you create this Frame as local variable and passing the data pointer to
         *                          outside the function, you should set clone as true and release the data pointer after use.
         * @return Return the frame in bytes (BGR)
        */
        unsigned char* getFrame(int& numOfBytes, const bool clone = false);

#pragma endregion Frame

#pragma region Getter

        /**
        * @brief Return true if the frame is empty
        * @return Return true if the frame is empty
        */
        bool isEmpty() const;

        /**
         * @brief Return the last frame index. It use to identify whether a new frame. Index will only be updated when you call getFrame() or gatMat();
         * @return Return the last frame index.
        */
        unsigned long getFrameIndex() const;

        /**
         * @brief Get the frame width in pixel
         * @return Return the frame width
        */
        int getWidth() const;

        /**
         * @brief Get the frame height in pixel
         * @return Return the frame height
        */
        int getHeight() const;

        /**
         * @brief Get frame size in bytes
         * @return Return the the frame size in bytes
        */
        int getFrameSize() const;

#pragma endregion Getter

#ifdef WITH_OPENCV2
#pragma region OpenCV

        /**
         * @brief Set cv::mat settings
         * @param[in] settings Settings
        */
        void setCVMatSettings(const OpenCVMatSettings settings);

        /**
         * @brief Get cv::Mat of the current frame
         * @return Return cv::Mat
        */
        cv::Mat getMat();

#pragma endregion OpenCV
#endif

#pragma region Operator

        /**
        * @brief Copy assignment
        */
        Frame& operator=(const Frame& other)
        {
            if (this != &other)
            {
                m_width = other.m_width;
                m_height = other.m_height;
                m_frameSize = other.m_frameSize;
                m_frameIndex = other.m_frameIndex;
                m_frameType = other.m_frameType;
                if (other.m_data != nullptr)
                {
                    m_data = std::make_unique<unsigned char[]>(m_frameSize);
                    memcpy(m_data.get(), other.m_data.get(), m_frameSize);
                }
            }
            return *this;
        }


        /**
        * @brief Move assignment
        */
        Frame& operator=(Frame&& other) noexcept
        {
            if (this != &other)
            {
                m_width = other.m_width;
                m_height = other.m_height;
                m_frameSize = other.m_frameSize;
                m_frameIndex = other.m_frameIndex;
                m_frameType = other.m_frameType;
                m_data = std::move(other.m_data);
            }
            return *this;
        }

        /**
        * @brief equal operator
        */
        bool operator==(const Frame& other) const
        {
            if (m_width != other.m_width) return false;
            if (m_height != other.m_height) return false;
            if (m_frameSize != other.m_frameSize) return false;
            if (m_frameIndex != other.m_frameIndex) return false;
            if (m_frameType != other.m_frameType) return false;

            if (m_data == nullptr && other.m_data != nullptr) return false;
            if (m_data != nullptr && other.m_data == nullptr) return false;
            if (m_data != nullptr && other.m_data != nullptr)
            {
                for (int i = 0; i < m_frameSize; i++)
                {
                    if (m_data[i] != other.m_data[i]) return false;
                }
            }
            return true;
        }

        /**
        * @brief not equal operator
        */
        bool operator!=(const Frame& other) const
        {
            return !(*this == other);
        }

#pragma endregion Operator

        /**
        * @brief Save the frame to a image file. The Save function require running the GDI+ library which is running when the WinCamrea object exist.
        * @param[in] path Path to save the image. Supported format are png, jpg, jpeg, bmp, tiff
        * @param[in] (Optional) encoderParams Encoder parameters. default is NULL. See https://learn.microsoft.com/en-us/windows/win32/api/gdiplusimaging/nl-gdiplusimaging-encoderparameters
        */
        void Save(
            const std::filesystem::path path,
            const Gdiplus::EncoderParameters* encoderParams = NULL
        );

    private:
        std::unique_ptr<unsigned char[]> m_data = nullptr;
        long m_frameSize = 0; // In number of byte

        int m_width = -1;
        int m_height = -1;
        GUID m_frameType;

        unsigned long m_frameIndex = 0;

#ifdef WITH_OPENCV2
        OpenCVMatConverter m_matConvertor;
#endif
    };

}

//*******************************

#endif
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

#include <memory>
#include <ostream>
#include <string>
#include <functional>

namespace WinCamera
{
    class Frame
    {
    public:
        typedef std::function<void(unsigned char* data, unsigned long& frameIndex)> ImportDataFunc;

    public:

        /**
        * @brief Constructor
        */
        Frame();

        /**
        * @brief Clear the frame
        */
        void Clear();

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

    private:
        std::unique_ptr<unsigned char[]> m_data = nullptr;
        long m_frameSize = 0; // In number of byte

        int m_width = -1;
        int m_height = -1;
        GUID m_frameType;

        unsigned long m_frameIndex = 0;
    };

}

//*******************************

#endif
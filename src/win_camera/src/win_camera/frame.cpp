/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#include "win_camera/frame.h"

namespace WinCamera
{


    Frame::Frame()
    {
        Clear();
    }

    void Frame::Clear()
    {
        m_width = -1;
        m_height = -1;
        m_frameSize = 0;
        if (m_data != nullptr) m_data.reset();
    }

    void Frame::ImportData(
        const long frameSize,
        const int width,
        const int height,
        const GUID frameType,
        ImportDataFunc importDataFunc
    )
    {
        // Check
        if (frameSize <= 0) throw std::invalid_argument("Frame size(" + std::to_string(frameSize) + ") can't be <= 0.");
        if (width <= 0) throw std::invalid_argument("Width(" + std::to_string(width) + ") can't be <= 0.");
        if (height <= 0) throw std::invalid_argument("Height(" + std::to_string(height) + ") can't be <= 0.");

        // Reset
        Clear();

        // Set
        m_width = width;
        m_height = height;
        m_frameType = frameType;
        m_frameSize = frameSize;

        // Allocate memory
        m_data = std::make_unique<unsigned char[]>(frameSize);

        // Import
        importDataFunc(m_data.get(), m_frameIndex);
    }

    unsigned char* Frame::getFrame(int& numOfBytes, const bool clone)
    {
        numOfBytes = m_frameSize;

        if (clone)
        {
            // Allocate memory
            auto data = new unsigned char[m_frameSize];

            // Copy
            memcpy(data, m_data.get(), m_frameSize);

            return data;
        }
        else
        {
            return m_data.get();
        }
    }

    bool Frame::isEmpty() const
    {
        return m_data == nullptr || m_frameSize == 0;
    }

    unsigned long Frame::getFrameIndex() const
    {
        return m_frameIndex;
    }

    int Frame::getWidth() const
    {
        return m_width;
    }

    int Frame::getHeight() const
    {
        return m_height;
    }

    int Frame::getFrameSize() const
    {
        return m_frameSize;
    }
}
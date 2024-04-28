/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#include "frame/frame.h"

#include "utils/path_utils.h"

namespace WinCamera
{
#pragma region Constructor and Destructor

    Frame::Frame()
    {
        Clear();
    }

    Frame::~Frame()
    {
    }

    Frame::Frame(const Frame& other)
    {
        m_width = other.m_width;
        m_height = other.m_height;
        m_frameSize = other.m_frameSize;
        m_frameIndex = other.m_frameIndex;
        m_frameType = other.m_frameType;
        m_data = std::make_unique<unsigned char[]>(m_frameSize);
        memcpy(m_data.get(), other.m_data.get(), m_frameSize);
    }

    Frame::Frame(Frame&& other) noexcept
    {
        m_width = other.m_width;
        m_height = other.m_height;
        m_frameSize = other.m_frameSize;
        m_frameIndex = other.m_frameIndex;
        m_frameType = other.m_frameType;
        m_data = std::move(other.m_data);

        // Reset other after move
        other.Clear();
    }

    void Frame::Clear()
    {
        m_width = -1;
        m_height = -1;
        m_frameSize = 0;
        m_frameIndex = 0;
        if (m_data != nullptr) m_data.reset();


#ifdef WITH_OPENCV2
        m_matConvertor.Reset();
#endif
    }

#pragma endregion Constructor and Destructor

#pragma region Frame

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

#pragma endregion Frame

#pragma region Getter

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

#pragma endregion Getter

#ifdef WITH_OPENCV2
#pragma region OpenCV

    void Frame::setFrameSettings(const FrameSettings settings)
    {
        m_matConvertor.setFrameSettings(settings);
    }

    cv::Mat Frame::getMat()
    {
        m_matConvertor.setVideoType(m_frameType);

        return m_matConvertor.convert(m_data.get(), getWidth(), getHeight());
    }

#pragma endregion OpenCV
#endif

    void Frame::Save(
        const std::filesystem::path path,
        const Gdiplus::EncoderParameters* encoderParams
    )
    {
        // Get file Extension
        const auto fileExtension = Utils::PathUtils::getExtension(path);

        // Get encoder
        CLSID pngClsid;
        bool success = false;
        if (fileExtension == "png")
        {
            success = Utils::GDIPLUSUtils::GetPngEncoderClsid(pngClsid);
        }
        else if (fileExtension == "jpg" || fileExtension == "jpeg")
        {
            success = Utils::GDIPLUSUtils::GetJpegEncoderClsid(pngClsid);
        }
        else if (fileExtension == "bmp")
        {
            success = Utils::GDIPLUSUtils::GetBmpEncoderClsid(pngClsid);
        }
        else if (fileExtension == "tiff")
        {
            success = Utils::GDIPLUSUtils::GetTiffEncoderClsid(pngClsid);
        }
        else
        {
            throw std::invalid_argument("File type(" + fileExtension + ") is not supported.");
        }
        if (!success) throw std::runtime_error("Can't get encoder for file type(" + fileExtension + ").");

        // Create bitmap
        Gdiplus::Bitmap bitmap(m_width, m_height, PixelFormat24bppRGB);
        Utils::GDIPLUSUtils::DrawBitmap(bitmap, m_data.get(), m_frameSize);

        // Save
        bitmap.Save(path.wstring().c_str(), &pngClsid, encoderParams);
    }
}
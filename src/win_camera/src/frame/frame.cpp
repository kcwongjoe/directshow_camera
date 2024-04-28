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
        m_frameSettings = other.m_frameSettings;
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
        m_frameSettings = other.m_frameSettings;
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
        m_frameSettings.Reset();
        if (m_data != nullptr) m_data.reset();
    }

#pragma endregion Constructor and Destructor

#pragma region Frame

    void Frame::ImportData(
        const long frameSize,
        const int width,
        const int height,
        const GUID frameType,
        const FrameSettings frameSettings,
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
        m_frameSettings = frameSettings;

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

    FrameSettings& Frame::getFrameSettings()
    {
        return m_frameSettings;
    }

#pragma endregion Getter

#ifdef WITH_OPENCV2
#pragma region OpenCV

    cv::Mat Frame::getMat()
    {
        // Check
        FrameDecoder::CheckSupportVideoType(m_frameType);

        // Convert
        return FrameDecoder::DecodeFrameToCVMat(
            m_data.get(),
            m_frameType,
            m_width, 
            m_height,
            m_frameSettings.VerticalFlip,
            !m_frameSettings.BGR
        );
    }

#pragma endregion OpenCV
#endif

    void Frame::Save(
        const std::filesystem::path path,
        const Gdiplus::EncoderParameters* encoderParams
    )
    {
        // Check video type
        FrameDecoder::CheckSupportVideoType(m_frameType);

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
        Gdiplus::PixelFormat pixelFormat;
        if (FrameDecoder::isMonochromeFrameType(m_frameType))
        {
            pixelFormat = PixelFormat8bppIndexed;
        }
        else if (FrameDecoder::is16BitMonochromeFrameType(m_frameType))
        {
            pixelFormat = PixelFormat16bppGrayScale;
        }
        else
        {
            pixelFormat = PixelFormat24bppRGB;
        }
        Gdiplus::Bitmap bitmap(m_width, m_height, pixelFormat);

        // Draw
        if (m_frameSettings.VerticalFlip)
        {
            // Create a image buffer
            auto data = new unsigned char[m_frameSize];

            try {
                // Flip the image into the buffer
                FrameDecoder::DecodeFrame(
                    m_data.get(),
                    data,
                    m_frameType,
                    m_width,
                    m_height,
                    true,
                    false
                );

                // Draw
                Utils::GDIPLUSUtils::DrawBitmap(bitmap, data, m_frameSize);

                // Delete the buffer
                delete[] data;
            }
            catch (...)
            {
                delete[] data;
                throw;
            }
        }
        else
        {
            Utils::GDIPLUSUtils::DrawBitmap(bitmap, m_data.get(), m_frameSize);
        }

        // Save
        bitmap.Save(path.wstring().c_str(), &pngClsid, encoderParams);
    }
}
/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include "camera/camera_thread.h"

#include "utils/time_utils.h"

namespace DirectShowCamera
{
    
#pragma region Constructor and Destructor

    CameraThread::CameraThread()
    {
        Reset();
        m_camera = std::make_shared<Camera>();
    }

    CameraThread::CameraThread(std::shared_ptr<Camera>& camera)
    {
        Reset();
        m_camera = camera;
    }

    CameraThread::~CameraThread()
    {
        Stop(false);
    }

    void CameraThread::Reset()
    {
        m_stopThread = false;
        m_stopCapture = false;
        m_waitForStopTimeout = 3000;
        m_capturedFrame.Clear();
    }

#pragma endregion Constructor and Destructor

#pragma region Thread control

    void CameraThread::Start(const bool startCapture)
    {
        if (!m_isRunning)
        {
            // Start Capture
            if (startCapture && m_camera->isOpened())
                m_camera->StartCapture();

            // Start the thread
            m_stopThread = false;
            m_thread = std::thread(&CameraThread::Run, this);
            m_thread.detach();

        }
    }

    bool CameraThread::Stop(const bool async, const bool stopCapture)
    {
        // CameraThread already stopped.
        if(!(this && m_isRunning)) return true;

        // Call for stop
        m_stopCapture = stopCapture;
        m_stopThread = true;

        if (async == false)
        {
            // Sync mode

            // Wait
            int waitingTime = 0;
            int delayTime = 100;
            while (m_isRunning && (waitingTime <= m_waitForStopTimeout || m_waitForStopTimeout == 0))
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));
                waitingTime += delayTime;
            }

            // Return
            if (m_waitForStopTimeout > 0 && waitingTime > m_waitForStopTimeout)
            {
                // Time out
                return false;
            }
            else
            {
                // Success
                return true;
            }
        }
        else
        {
            // Async mode
            return true;
        }
        
    }

    void CameraThread::Run()
    {
        // Set as running
        m_isRunning = true;

        while (!m_stopThread)
        {
            if (m_camera)
            {
                // Open camera
                if (m_camera->isOpened())
                {

                    // Get Image
                    bool success = m_camera->getFrame(m_capturedFrame, true);
                    if (success)
                    {
                        // Save Image
                        if (m_saveImage)
                        {
                            // Get now time
                            auto now = std::chrono::system_clock::now();
                            time_t nowTimet = std::chrono::system_clock::to_time_t(now);

                            // Create image name
                            std::string imageName = Utils::TimeUtils::ToString(nowTimet, "%Y_%m_%d_%H_%M_%S") + "_" + std::to_string(Utils::TimeUtils::GetMilliseconds(now)) + ".jpg";
                            std::string imagePath = (m_saveImagePath.empty()) ? imageName : m_saveImagePath + "/" + imageName;

                            // Save
                            if (m_saveImageInAsync)
                            {
                                // Save image in async mode
                                std::thread t([this, imagePath]() {
                                    m_capturedFrame.Save(imagePath);
                                }
                                );
                                t.detach();
                            }
                            else
                            {
                                // Save image in sync mode
                                m_capturedFrame.Save(imagePath);
                            }
                        }

                        // Process
                        if (m_capturedProcess != nullptr)
                        {
                            m_capturedProcess(m_capturedFrame);
                        }
                    }
                }
                else
                {
                    // Wait 1s and check connection again
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
            }
            else
            {
                // Stop thread if camera not exist
                m_stopThread = true;
            }
        }

        // Stop capture
        if (m_stopCapture)
        {
            m_camera->StopCapture();
            m_stopCapture = false;
        }

        // Set as not running
        m_isRunning = false;
    }

    bool CameraThread::isRunning() const
    {
        return m_isRunning;
    }

    void CameraThread::setWaitForStopTimeout(const int timeout)
    {
        // Exception
        if (timeout < 0)
            throw std::invalid_argument("timeout(" + std::to_string(timeout) + ") must be >= 0.");

        m_waitForStopTimeout = timeout;
    }

    int CameraThread::getWaitForStopTimeout() const
    {
        return m_waitForStopTimeout;
    }

#pragma endregion Thread control

#pragma region Save Image

    void CameraThread::setSaveImagePath(const std::string path)
    {
        m_saveImagePath = path;
    }

    void CameraThread::EnableSaveImage(const bool enable, const bool saveInAsync)
    {
        m_saveImage = enable;
        m_saveImageInAsync = saveInAsync;
    }

#pragma endregion Save Image

    void CameraThread::setCapturedProcess(CapturedProcess capturedProcess)
    {
        m_capturedProcess = capturedProcess;
    }

    std::shared_ptr<Camera> CameraThread::getCamera()
    {
        return m_camera;
    }

    Frame& CameraThread::getFrame()
    {
        return m_capturedFrame;
    }
}
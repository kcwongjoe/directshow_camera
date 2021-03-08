#include "uvc_camera_looper.h"

#pragma region Constructor and Destructor

/**
 * @brief Constructor
*/
UVCCameraLooper::UVCCameraLooper()
{
    reset();
    m_camera = new UVCCamera();
}

/**
 * @brief Constructor
 * @param camera Camera
*/
UVCCameraLooper::UVCCameraLooper(UVCCamera* camera)
{
    reset();
    m_camera = camera;
}

/**
 * @brief Destructor
*/
UVCCameraLooper::~UVCCameraLooper()
{
    stop(false);
    m_capturedImage.release();
}

/**
 * @brief Reset variables
*/
void UVCCameraLooper::reset()
{
    m_stopThread = false;
    m_stopCapture = false;
    m_waitForStopTimeout = 3000;
    m_capturedImage = cv::Mat();
}

#pragma endregion Constructor and Destructor

#pragma region Looper control

/**
 * @brief Start looper
 * @param startCapture Set it as true if you want to run startCapture() on the camera. Default as true. 
*/
void UVCCameraLooper::start(bool startCapture)
{
    if (!m_isRunning)
    {
        // Start Capture
        if (startCapture && m_camera->isOpened())
            m_camera->startCapture();

        // Start the thread
        m_stopThread = false;
        m_thread = std::thread(&UVCCameraLooper::run, this);
        m_thread.detach();

    }
}

/**
 * @brief Stop the looper. This stop function default operating in sync mode which will wait for the thread stopped.
 *
 * @param async Default as false. Set it as true to stop in async mode.
 * @param stopCapture Set it as true if you want to run stopCapture() after looper is stopped. Default as true. 
 * @return Return true if success to close. Return false if timeout.
*/
bool UVCCameraLooper::stop(bool async, bool stopCapture)
{
    if (m_isRunning)
    {
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
    else
    {
        // Looper already stopped.
        return true;
    }
    
}

/**
 * @brief Run
*/
void UVCCameraLooper::run()
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
                cv::Mat image = m_camera->getMat();
                if (!image.empty())
                {
                    m_capturedImage = image;

                    // Save Image
                    if (m_saveImage)
                    {
                        // Get now time
                        auto now = std::chrono::system_clock::now();
                        time_t nowTimet = std::chrono::system_clock::to_time_t(now);

                        // Create image name
                        std::string imageName = DirectShowCameraUtils::to_string(nowTimet, "%Y_%m_%d_%H_%M_%S") + "_" + std::to_string(DirectShowCameraUtils::getMilliseconds(now)) + ".jpg";
                        std::string imagePath = (m_saveImagePath.empty()) ? imageName : m_saveImagePath + "/" + imageName;

                        // Save
                        if (m_saveImageInAsync)
                        {
                            // Save image in async mode
                            std::thread t([this, imagePath]() {
                                cv::imwrite(imagePath, m_capturedImage);
                                }
                            );
                            t.detach();
                        }
                        else
                        {
                            // Save image in sync mode
                            cv::imwrite(imagePath, m_capturedImage);
                        }
                    }

                    // Process
                    if (m_capturedProcess != nullptr)
                    {
                        m_capturedProcess(image);
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
        m_camera->stopCapture();
        m_stopCapture = false;
    }

    // Set as not running
    m_isRunning = false;
}

/**
 * @brief Return true if thread is running.
 *
 * @return Return true if thread is running.
 */
bool UVCCameraLooper::isRunning()
{
    return m_isRunning;
}

#pragma endregion Looper control

#pragma region Wait for stop timeout

/**
 * @brief Set the wait for stopping timeout
 *
 * @param timeout in millisecond. Set as 0 to disable the timeout. Default as 3 seconds.
 */
void UVCCameraLooper::setWaitForStopTimeout(int timeout)
{
    // Exception
    if (timeout < 0)
        throw std::invalid_argument("timeout(" + std::to_string(timeout) + ") must be >= 0.");

    m_waitForStopTimeout = timeout;
}

/**
 * @brief Get the wait for stopping timeout
 *
 * @return Timeout in millisecond.
 */
int UVCCameraLooper::getWaitForStopTimeout()
{
    return m_waitForStopTimeout;
}

#pragma endregion Wait for stop timeout

#pragma region Save Image

/**
 * @brief Set the save image folder path
 * @param path Path
*/
void UVCCameraLooper::setSaveImagePath(std::string path)
{
    m_saveImagePath = path;
}

/**
 * @brief Set as true to save image automatically. See setSaveImagePath()
 * @param enable Set as true to save image.
 * @param saveInAsync (Option) Set as true to save image in async mode. Default as true;
*/
void UVCCameraLooper::enableSaveImage(bool enable, bool saveInAsync)
{
    m_saveImage = enable;
    m_saveImageInAsync = saveInAsync;
}

#pragma endregion Save Image

void UVCCameraLooper::setCapturedProcess(CapturedProcess capturedProcess)
{
    m_capturedProcess = capturedProcess;
}

UVCCamera* UVCCameraLooper::getCamera()
{
    return m_camera;
}

cv::Mat UVCCameraLooper::getImage()
{
    return m_capturedImage;
}


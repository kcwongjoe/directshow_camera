/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/win_camera/issues
**/

#pragma once
#ifndef WIN_CAMERA__WIN_CAMERA__WIN_CAMERA_THREAD_H
#define WIN_CAMERA__WIN_CAMERA__WIN_CAMERA_THREAD_H

//************Content************

#include "win_camera/win_camera.h"

// Include Opencv
#ifdef WITH_OPENCV2
#include <opencv2/opencv.hpp>
#endif

#include <thread>
#include <string>
#include <functional>
#include <memory>

namespace WinCamera
{
    /**
     * @brief A Thread for WinCamera to continuously capture images.
     * 
     */
    class WinCameraThread {
    public:

        typedef std::function<void(Frame& frame)> CapturedProcess;

    public:
#pragma region Constructor and Destructor

        /**
         * @brief Constructor
        */
        WinCameraThread();

        /**
         * @brief Constructor
         * @param[in] camera Camera
        */
        WinCameraThread(std::shared_ptr<WinCamera>& camera);

        /**
         * @brief Destructor
        */
        ~WinCameraThread();

#pragma endregion Constructor and Destructor

        /**
         * @brief Set the capture process
         *
         * @param[in] capturedProcess void(Frame frame) The function to process the captured image.
         */
        void setCapturedProcess(CapturedProcess capturedProcess);

#pragma region Thread control

        /**
         * @brief Start looper
         * @param[in] startCapture (Optional) Set it as true if you want to run startCapture() on the camera. Default as true.
        */
        void Start(const bool startCapture = true);

        /**
         * @brief Stop the looper. This stop function default operating in sync mode which will wait for the thread stopped.
         *
         * @param[in] async (Optional) Set it as true to stop in async mode. Default as false.
         * @param[in] stopCapture (Optional) Set it as true if you want to run stopCapture() after looper is stopped. Default as true.
         * @return Return true if success to close. Return false if timeout.
        */
        bool Stop(const bool async = false, const bool stopCapture = true);

        /**
         * @brief Return true if thread is running.
         *
         * @return Return true if thread is running.
         */
        bool isRunning() const;

        /**
         * @brief Set the wait for stopping timeout
         *
         * @param[in] timeout in millisecond. Set as 0 to disable the timeout. Default setting was 3 seconds.
         */
        void setWaitForStopTimeout(const int timeout);

        /**
         * @brief Get the wait for stopping timeout
         *
         * @return Timeout in millisecond.
         */
        int getWaitForStopTimeout() const;

#pragma endregion Thread control

#pragma region Save Image

        /**
         * @brief Set the save image folder path
         * @param[in] path Path for imaged to be saved
         * @see EnableSaveImage()
        */
        void setSaveImagePath(const std::string path);

        /**
         * @brief Set as true to save image automatically.
         * @param[in] enable Set as true to save image.
         * @param[in] saveInAsync (Optional) Set as true to save image in async mode. Default as true;
         * @note The image will be saved in the folder set by setSaveImagePath()
         * @see setSaveImagePath()
        */
        void EnableSaveImage(const bool enable, const bool saveInAsync = true);

#pragma endregion Save Image

        /**
         * @brief Get the last capture image
         *
         * @return Return the last capture image
         */
        Frame& getFrame();

        /**
         * @brief Get the camera pointer
         *
         * @return Return the camera pointer
         */
        std::shared_ptr<WinCamera> getCamera();

    private:
        /**
        * @brief The thread processing to be run
        */
        void Run();

        /**
         * @brief Reset variables
        */
        void Reset();

    private:
        bool m_stopThread = false;
        bool m_stopCapture = false;
        bool m_isRunning = false;
        std::thread m_thread;
        int m_waitForStopTimeout = 3000;

        std::string m_saveImagePath;
        bool m_saveImage = false;
        bool m_saveImageInAsync = true;

        std::shared_ptr<WinCamera> m_camera = nullptr;
        Frame m_capturedFrame;
        CapturedProcess m_capturedProcess = nullptr;
    };
}

//*******************************

#endif


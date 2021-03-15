#pragma once
#ifndef UVC_CAMERA_LOOPER_H
#define UVC_CAMERA_LOOPER_H

//************Content************

#include <thread>
#include <uvc_camera.h>
namespace DirectShowCamera
{
    /**
     * @brief UVC Camera Looper
     * 
     */
    class UVCCameraLooper {
    public:

        typedef std::function<void(cv::Mat image)> CapturedProcess;

        UVCCameraLooper();

        UVCCameraLooper(UVCCamera* camera);

        ~UVCCameraLooper();

        void setCapturedProcess(CapturedProcess capturedProcess);
        void setSaveImagePath(std::string path);
        void enableSaveImage(bool enable, bool saveInAsync = true);

        void start(bool startCapture = true);
        bool stop(bool async = false, bool stopCapture = true);
        bool isRunning();

        void setWaitForStopTimeout(int timeout);
        int getWaitForStopTimeout();

        cv::Mat getImage();

        UVCCamera* getCamera();

    private:
        bool m_stopThread = false;
        bool m_stopCapture = false;
        bool m_isRunning = false;
        std::thread m_thread;
        int m_waitForStopTimeout = 3000;

        std::string m_saveImagePath;
        bool m_saveImage = false;
        bool m_saveImageInAsync = true;

        UVCCamera* m_camera;
        cv::Mat m_capturedImage;
        CapturedProcess m_capturedProcess = NULL;

        void run();
        void reset();
    };
}

//*******************************

#endif


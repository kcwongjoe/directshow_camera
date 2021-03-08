#pragma once
#ifndef DIRECTSHOW_GRABBER_CALLBACK_H
#define DIRECTSHOW_GRABBER_CALLBACK_H

//************Content************

#include <string>
#include <mutex>
#include <chrono>

#include <windows.h>
#include <ds_guid.h>

class SampleGrabberCallback : public ISampleGrabberCB {
	private:

		/**
		 * @brief A mutex for the rame copying.
		*/
		std::mutex m_bufferMutex;

		/**
		 * @brief Current frame data.
		*/
		unsigned char* m_pixelsBuffer;

		/**
		 * @brief The current frame size in bytes.
		*/
		int m_bufferSize = 0;

		/**
		 * @brief Current Frame index. Such as ID of the current frame. It use to identify whether a new frame.
		*/
		unsigned long m_frameIndex = 1;

		int m_latestPixelCount = 0;
		int m_numOfRepeatPixelCount = 0;
		static const int m_resetBufferCount = 5;

		std::chrono::system_clock::time_point m_lastFrameTime;
		double m_fps = 0;

	public:

		SampleGrabberCallback();
		~SampleGrabberCallback();

		// Buffer size
		void setBufferSize(int numOfBytes);
		int getBufferSize();

		bool getFrame(unsigned char* frame, unsigned long* frameIndex = NULL, int* numOfBytes = NULL, bool copyNewFrameOnly = false, unsigned long previousFrameIndex = 0);
		double getFPS();
		std::chrono::system_clock::time_point getLastFrameCaptureTime();

		/**
		 * @brief Minimum FPS. FPS below this value will be identified as 0.
		*/
		double minimumFPS = 0.5;

		//------------------------------------------------
		STDMETHODIMP_(ULONG) AddRef() { return 1; }
		STDMETHODIMP_(ULONG) Release() { return 2; }

		//------------------------------------------------
		STDMETHODIMP QueryInterface(REFIID, void** ppvObject);

		//------------------------------------------------
		STDMETHODIMP SampleCB(double, IMediaSample* pSample);


		// Not implemented
		STDMETHODIMP BufferCB(double, BYTE*, long);
	};


//*******************************

#endif
#include<ds_camera_stub.h>

namespace DirectShowCamera
{

#pragma region Life cycle

	/**
	 * @brief Constructor
	*/
	DirectShowCameraStub::DirectShowCameraStub()
	{
		m_fps = DirectShowCameraStubDefaultSetting::getFps();
	}

	/**
	 * @brief Desctructor
	*/
	DirectShowCameraStub::~DirectShowCameraStub()
	{
		release();
	}

	/**
	 * @brief Release
	*/
	void DirectShowCameraStub::release()
	{
		if (this)
		{
			HRESULT hr = NOERROR;
			
			stop();

			// Release video format
			DirectShowVideoFormat::release(m_videoFormats);
			m_videoFormats = NULL;
			m_currentVideoFormatIndex = -1;

			//Release Properties
			delete m_properties;
			m_properties = NULL;

			m_isOpening = false;
		}
	}

#pragma endregion Life cycle

#pragma region Connection

	/**
	 * @brief Build the directshow graph
	 * @param videoInputFilter Video input filter. Look up from DirectShowCamera::getCamera()
	 * @param videoFormat Video Format. Look up from DirectShowCameraDevice::getDirectShowVideoFormats()
	 * @return Return true if success
	*/
	bool DirectShowCameraStub::open(IBaseFilter** videoInputFilter, DirectShowVideoFormat* videoFormat)
	{
		m_isOpening = true;

		// Create camera properties
		DirectShowCameraStubDefaultSetting::getProperties(&m_properties);

		// Update video format
		updateVideoFormatList();
		m_currentVideoFormatIndex = getVideoFormatIndex(videoFormat);

		return true;
	}

	/**
	 * @brief Close
	*/
	void DirectShowCameraStub::close()
	{
		release();
	}

	/**
	 * @brief Return true if camera was opened
	 * @return Return true if camera was opened
	*/
	bool DirectShowCameraStub::isOpening()
	{
		return m_isOpening;
	}

	/**
	 * @brief It can be check camera whether disconnected accidently.
	 * @return Return true if camera disconnected.
	*/
	bool DirectShowCameraStub::checkDisconnection()
	{
		// Device doesn't started
		if (!m_isOpening) return true;

		return m_disconnectCamera;
	}

	/**
	 * @brief Set the disconnection process. When the process was set, a thread will start to keep check the connection. If camera is disconnected, this process will run and then run stop() internally.
	 * @param func void()
	*/
	void DirectShowCameraStub::setDisconnectionProcess(std::function<void()> func)
	{
		m_disconnectionProcess = func;

		startCheckConnectionThread();
	}

	/**
	 * @brief Start a thread to check the device connection
	*/
	void DirectShowCameraStub::startCheckConnectionThread()
	{
		if (m_isCapturing && m_disconnectionProcess)
		{
			if (!m_isRunningCheckConnectionThread)
			{
				m_stopCheckConnectionThread = false;

				m_checkConnectionThread = std::thread(
					[this]()
					{
						// Parameter
						int step = 1000;

						// Thread start running
						m_isRunningCheckConnectionThread = true;

						while (!m_stopCheckConnectionThread)
						{
							// Sleep
							std::this_thread::sleep_for(std::chrono::milliseconds(step));

							// Check disconnection
							if (checkDisconnection())
							{
								m_disconnectCamera = false;

								// Do something
								m_disconnectionProcess();

								// Stop capture
								stop();

								m_stopCheckConnectionThread = true;
							}
						}

						// Thread stoped
						m_isRunningCheckConnectionThread = false;
					}
				);
				m_checkConnectionThread.detach();
			}
		}
	}

	/**
	 * @brief Disconnect camera. It is used to simulate a camera disconnection for testing DisconnectionProcess.
	*/
	void DirectShowCameraStub::disconnetCamera()
	{
		m_disconnectCamera = true;
	}

#pragma endregion Connection

#pragma region start/stop

	/**
	 * @brief Start capture
	 * @return Return true if success
	*/
	bool DirectShowCameraStub::start()
	{
		bool result = true;

		if (m_isOpening)
		{
			m_isCapturing = true;

			// Start check disconnection thread
			startCheckConnectionThread();
		}
		else
		{
			result = false;
			m_errorString = "Graph is not initialized, please call initialize().";
		}

		return result;
	}

	/**
	 * @brief Stop Capture
	 * @return Return true if success
	*/
	bool DirectShowCameraStub::stop()
	{
		HRESULT hr = NOERROR;
		bool result = true;

		if (this)
		{
			if (m_isOpening)
			{
				if (m_isCapturing)
				{
					// Reset isCapturing
					m_isCapturing = false;

					// Stop the check disconnection thread
					m_stopCheckConnectionThread = true;
				}
			}
			else
			{
				result = false;
				m_errorString = "Graph is not initialized, please call initialize().";
			}
		}

		return result;
	}

	/**
	 * @brief Return true if camera is capturing
	 * @return Return true if camera is capturing
	*/
	bool DirectShowCameraStub::isCapturing()
	{
		return m_isCapturing;
	}

#pragma endregion start/stop

#pragma region Frame

	void DirectShowCameraStub::setGetFrameFunction(GetFrameFunc func)
	{
		m_getFrameFunc = func;
	}

	/**
	 * @brief Get current frame
	 * @param[out] frame Frame bytes
	 * @param[out] frameIndex Index of frame, use to indicate whether a new frame. Default is NULL
	 * @param[out] numOfBytes Number of bytes of the frames. Default is NULL
	 * @param[in] copyNewFrameOnly Set it as true if only want to collect new frame. Default is false
	 * @param[in] previousFrameIndex The previous frame index, use to idendify whether a new frame. This variable work with copyNewFrameOnly. Default as 0.
	 * @return Return true if success.
	*/
	bool DirectShowCameraStub::getFrame(unsigned char* frame, unsigned long* frameIndex, int* numOfBytes, bool copyNewFrameOnly, unsigned long previousFrameIndex)
	{
		if (m_isCapturing && frame)
		{
			if (m_getFrameFunc)
			{
				// Return the user define image
				m_getFrameFunc(frame, frameIndex, numOfBytes, copyNewFrameOnly, previousFrameIndex);
			}
			else
			{
				// Return the default image
				DirectShowCameraStubDefaultSetting::getFrame(
					m_videoFormats->at(m_currentVideoFormatIndex)->getWidth(),
					m_videoFormats->at(m_currentVideoFormatIndex)->getHeight(),
					frame,
					frameIndex,
					numOfBytes,
					previousFrameIndex);
			}

			return true;
		}
		else
		{
			return false;
		}
	}

	/**
	 * @brief Minimum FPS. FPS below this value will be identified as 0. Default as 0.5
	 * @param minimumFPS
	*/
	void DirectShowCameraStub::setMinimumPFS(double minimumFPS)
	{
		if (minimumFPS < 0) minimumFPS = 0;
		m_fps = minimumFPS;
	}

	/**
	 * @brief Get Frame per second.
	 * @return Return fps.
	*/
	double DirectShowCameraStub::getFPS()
	{
		if (isOpening())
		{
			return m_fps;
		}
		else
		{
			return 0;
		}
	}

	/**
	 * @brief Get frame size in bytes.
	 * @return
	*/
	long DirectShowCameraStub::getFrameTotalSize()
	{

		if (m_isCapturing)
		{
			int result = m_videoFormats->at(m_currentVideoFormatIndex)->getHeight() * m_videoFormats->at(m_currentVideoFormatIndex)->getWidth() * 3;
			return result;
		}
		else
		{
			return 0;
		}
	}

	/**
	 * @brief Get frame type. Such as MEDIASUBTYPE_RGB24
	 * @return
	*/
	GUID DirectShowCameraStub::getFrameType()
	{
		return MEDIASUBTYPE_RGB24;
	}

#pragma endregion Frame

#pragma region Video Format

	/**
	 * @brief Update video formats
	*/
	bool DirectShowCameraStub::updateVideoFormatList()
	{
		// Create video format
		DirectShowCameraStubDefaultSetting::getVideoFormat(&m_videoFormats);

		return true;
	}

	/**
	 * @brief Get index from the Video Format list
	 * @param videoFormat
	 * @return Return -1 if not found
	*/
	int DirectShowCameraStub::getVideoFormatIndex(DirectShowVideoFormat* videoFormat)
	{
		int result = -1;
		if (m_videoFormats)
		{
			for (int i = 0; i < m_videoFormats->size(); i++)
			{
				if (*m_videoFormats->at(i) == *videoFormat)
				{
					result = i;
					break;
				}
			}
		}

		return result;
	}

	/**
	 * @brief Get current video format index.
	 * @return
	*/
	int DirectShowCameraStub::getCurrentVideoFormatIndex()
	{
		return m_currentVideoFormatIndex;
	}

	/**
	 * @brief Get current video format
	 * @return
	*/
	DirectShowVideoFormat DirectShowCameraStub::getCurrentVideoFormat()
	{
		if (m_currentVideoFormatIndex >= 0)
		{
			return m_videoFormats->at(m_currentVideoFormatIndex)->clone(false);
		}
		else
		{
			return DirectShowVideoFormat();
		}

	}

	/**
	 * @brief Get current video format list of this opened camera.
	 * @return
	*/
	std::vector<DirectShowVideoFormat> DirectShowCameraStub::getVideoFormatList()
	{
		if (m_videoFormats)
		{
			std::vector<DirectShowVideoFormat> result;
			for (int i = 0; i < m_videoFormats->size(); i++)
			{
				result.push_back(m_videoFormats->at(i)->clone(false));

			}
			return result;
		}
		else
		{
			return std::vector<DirectShowVideoFormat>();
		}
	}

	/**
	 * @brief Set video format. It is suggested to set video format in the open(). It may not succes to change the video format after opened camera.
	 * @param videoFormat Video format to be set
	 * @return Return true if success.
	*/
	bool DirectShowCameraStub::setVideoFormat(DirectShowVideoFormat* videoFormat)
	{
		bool result = false;
		updateVideoFormatList();
		int index = getVideoFormatIndex(videoFormat);
		if (index >= 0)
		{
			result = setVideoFormat(index);
		}

		return result;
	}

	/**
	 * @brief Set video format. It is suggested to set video format in the open(). It may not succes to change the video format after opening camera.
	 * @param videoFormatIndex Index of the video foramt list.
	 * @return Return true if success.
	*/
	bool DirectShowCameraStub::setVideoFormat(int videoFormatIndex)
	{
		bool result = true;
		HRESULT hr = NO_ERROR;

		if (result)
		{
			if (!m_isOpening)
			{
				result = false;
				m_errorString = "Graph is not initialized, please call initialize().";
			}
			else if (videoFormatIndex < 0 || videoFormatIndex >= m_videoFormats->size())
			{
				result = false;
				m_errorString = "Video format index(" + std::to_string(videoFormatIndex) + ") is out of range(0," + std::to_string(m_videoFormats->size()) + ").";
			}
			else
			{
				// Set
				m_currentVideoFormatIndex = videoFormatIndex;
			}
		}

		return result;
	}

#pragma endregion Video Format

#pragma region Properties

	/**
	 * @brief Refresh properties
	 */
	void DirectShowCameraStub::refreshProperties()
	{
		
	}

	/**
	 * @brief Get properties
	 * @return Return properties
	*/
	DirectShowCameraProperties* DirectShowCameraStub::getProperties()
	{
		return m_properties;
	}

	/**
	 * @brief Reset properties to default
	 *
	 * @param[in] asAuto (Option) Set as true if you also want to set properties to auto. Default as true.
	 */
	void DirectShowCameraStub::resetDefault(bool asAuto)
	{
		if (m_isOpening)
		{
			m_properties->resetDefault(NULL, asAuto);
		}
	}

	/**
	 * @brief Set property value
	 * @param property Property
	 * @param value Value to be set
	 * @param isAuto Set as true for auto mode, false for manual mode
	 * @return Return true if success
	*/
	bool DirectShowCameraStub::setValue(DirectShowCameraProperty* property, long value, bool isAuto)
	{
		if (m_isOpening)
		{
			return property->setValue(NULL, value, isAuto, &m_errorString);
		}
		else
		{
			m_errorString = "Error on setting " + property->getName() + " property: Camera is not open.";
			return false;
		}
	}

#pragma endregion Properties

#pragma region getCamera

	/**
	 * @brief Get the available camera list
	 * @param[out] cameraDevices Camera Devices.
	 * @return Return true if success
	*/
	bool DirectShowCameraStub::getCameras(std::vector<DirectShowCameraDevice>* cameraDevices)
	{
		DirectShowCameraStubDefaultSetting::getCamera(cameraDevices);
		return true;
	}

	/**
	 * @brief Get the video input filter based on the camera index
	 * @param[in] cameraIndex Camera index
	 * @param[out] videoInputFilter Output video input filter
	 * @return Return true if success.
	*/
	bool DirectShowCameraStub::getCamera(int cameraIndex, IBaseFilter** videoInputFilter)
	{
		*videoInputFilter = NULL;
		return true;
	}

	/**
	 * @brief Get the video input filter based on the Camera device path
	 * @param[in] devicePath Camera device path
	 * @param[out] videoInputFilter Output video input filter
	 * @return Return true if success.
	*/
	bool DirectShowCameraStub::getCamera(std::string devicePath, IBaseFilter** videoInputFilter)
	{
		*videoInputFilter = NULL;
		return true;
	}

	/**
	 * @brief Get the video input filter based on the Camera device object
	 * @param[in] device Camera device
	 * @param[out] videoInputFilter Output video input filter
	 * @return Return true if success.
	*/
	bool DirectShowCameraStub::getCamera(DirectShowCameraDevice device, IBaseFilter** videoInputFilter)
	{
		return getCamera(device.getDevicePath(), videoInputFilter);
	}

#pragma endregion getCamera

	/**
	 * @brief Get the last error
	 * @return Return the last error
	*/
	std::string DirectShowCameraStub::getLastError()
	{
		return m_errorString;
	}

}
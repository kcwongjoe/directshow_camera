/**
* Copy right (c) 2020-2024 Ka Chun Wong. All rights reserved.
* This is a open source project under MIT license (see LICENSE for details).
* If you find any bugs, please feel free to report under https://github.com/kcwongjoe/directshow_camera/issues
**/

#include <gtest/gtest.h>

#include "camera/camera.h"
#include "directshow_camera/stub/ds_camera_stub.h"


class TestUVCCameraStubF : public ::testing::Test {
protected:
    void SetUp() override
    {

    }

    void TearDown() override
    {

    }

    const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera> stub = std::make_shared<DirectShowCamera::DirectShowCameraStub>();
    DirectShowCamera::DirectShowCameraStub* cameraStub = dynamic_cast<DirectShowCamera::DirectShowCameraStub*>(stub.get());
    DirectShowCamera::Camera camera = DirectShowCamera::Camera(stub);
};

/**
 * @brief 
 * <pre>
 * <b>TestID:</b> stub_getcamera01
 * <b>Title:</b> Test DirectShow Camera Stub getCameras()
 * </pre>
 * 
 * @details
 * <pre>
 * <b>Description:</b>
 *   Test the DirectShowCameraStub get camera function in UVCCamera
 * <b>Precondition:</b>
 * <b>Assumption:</b>
 * <b>Test Steps:</b>
 *   1. Get Cameras from UVCCamera as data1
 *   2. Get expect cameras from DirectShowCameraStubDefaultSetting as data2
 *   3. Test data1.size() == data2.size()
 *   4. Test data1.Description == data2.Description
 *   5. Test data1.DevicePath == data2.DevicePath
 *   6. Test data1.FriendlyName == data2.FriendlyName
 * <b>Expected Result:</b>
 *   3. True
 *   4. True
 *   5. True
 *   6. True
 * </pre>
 */
TEST_F(TestUVCCameraStubF, TestGetCamera)
{
    // Get cameras from UVCCamera
    std::vector<DirectShowCamera::CameraDevice> cameraDeivceList = camera.getCameras();

    // Get expect result
    std::vector<DirectShowCamera::DirectShowCameraDevice> expectDirectShowCameraDevices;
    DirectShowCamera::DirectShowCameraStubDefaultSetting::getCamera(expectDirectShowCameraDevices);
    
    // Check
    ASSERT_EQ(cameraDeivceList.size(), expectDirectShowCameraDevices.size()) << "Test UVCCamera(DirectShowCameraStub)::getCameras fail.";
    for (int i=0;i< cameraDeivceList.size();i++)
    {
        EXPECT_EQ(cameraDeivceList[i].getDescription(), expectDirectShowCameraDevices[i].getDescription()) << "Test UVCCamera(DirectShowCameraStub)::getCameras fail.";
        EXPECT_EQ(cameraDeivceList[i].getDevicePath(), expectDirectShowCameraDevices[i].getDevicePath()) << "Test UVCCamera(DirectShowCameraStub)::getCameras fail.";
        EXPECT_EQ(cameraDeivceList[i].getFriendlyName(), expectDirectShowCameraDevices[i].getFriendlyName()) << "Test UVCCamera(DirectShowCameraStub)::getCameras fail.";
    }
    
}

/**
 * @brief
 * <pre>
 * <b>TestID:</b> stub_capture01
 * <b>Title:</b> Test DirectShow Camera Stub fucntions which is related to capture
 * </pre>
 *
 * @details
 * <pre>
 * <b>Description:</b>
 *   Test the DirectShowCameraStub capture function in UVCCamera
 * <b>Precondition:</b>
 * <b>Assumption:</b>
 * <b>Test Steps:</b>
 *   1. open UVCCamera
 *   2. Start capture
 *   3. getFrame() in bytes and compare with DirectShowCameraStubDefaultSetting
 *   4. Close
 * <b>Expected Result:</b>
 *   1. True
 *   2. Return True and isCapture == True
 *   3. Retrun True; Same size; Same bytes
 *   4. True
 * </pre>
 */
TEST(TestUVCCameraStub, TestCapture)
{
    // Create camera
    const std::shared_ptr<DirectShowCamera::AbstractDirectShowCamera> stub = std::make_shared<DirectShowCamera::DirectShowCameraStub>();
    DirectShowCamera::DirectShowCameraStub* cameraStub = dynamic_cast<DirectShowCamera::DirectShowCameraStub*>(stub.get());
    DirectShowCamera::Camera camera = DirectShowCamera::Camera(stub);

    // Get cameras from UVCCamera
    std::vector<DirectShowCamera::CameraDevice> cameraDeivceList = camera.getCameras();

    // Open the first camera in the biggest resolution
    std::vector <std::pair<int, int>> resolutions = cameraDeivceList[0].getResolutions();
    int width = resolutions[resolutions.size() - 1].first;
    int height = resolutions[resolutions.size() - 1].second;
    ASSERT_TRUE(camera.Open(cameraDeivceList[0],
            width,
            height
        )
    ) << "Fail: camera.open()";
    EXPECT_TRUE(camera.isOpened()) << "Fail: camera.isOpened()";

    // Start capture
    EXPECT_TRUE(camera.StartCapture()) << "Fail: camera.startCapture()";
    EXPECT_TRUE(camera.isCapturing()) << "Fail: camera.isCapturing()";

    // Get getFrame
    DirectShowCamera::Frame frame;
    EXPECT_TRUE(camera.getFrame(frame)) << "Fail: camera.getFrame()";

    // Get Expect frame
    DirectShowCamera::Frame expectedFrame;
    DirectShowCamera::DirectShowCameraStubDefaultSetting::getFrame(
        expectedFrame,
        1,  //  Frame index is 1 as only call getFrame once
        width,
        height
    );

    // Check getFrame
    EXPECT_EQ(frame, expectedFrame) << "Fail: camera.getFrame()";

    // Close
    EXPECT_TRUE(camera.Close()) << "Fail: camera.close()";
    EXPECT_FALSE(camera.isOpened()) << "Fail: camera.close()";
    EXPECT_FALSE(camera.isCapturing()) << "Fail: camera.close()";

}
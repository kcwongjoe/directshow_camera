#include <gtest/gtest.h>

#include <uvc_camera.h>
#include <iostream>
#include <ds_camera_stub.h>


using namespace DirectShowCamera;


class TestUVCCameraStub : public ::testing::Test {
protected:
    void SetUp() override
    {

    }

    void TearDown() override
    {

    }

    DirectShowCameraStub* stub = new DirectShowCameraStub();
    UVCCamera camera = UVCCamera(stub);
};


/**
 * @brief Title: 
 * Description: 
 * Precondition: 
 * Assumption: 
 * Test Steps:
 * Expected Result:
 *  
 */
TEST_F(TestUVCCameraStub, TestGetCamera)
{
    // Get cameras from UVCCamera
    std::vector<CameraDevice> cameraDeivceList = camera.getCameras();

    // Get expect result
    std::vector<DirectShowCameraDevice> expectDirectShowCameraDevices;
    DirectShowCameraStubDefaultSetting::getCamera(&expectDirectShowCameraDevices);    
    
    // Check
    ASSERT_EQ(cameraDeivceList.size(), expectDirectShowCameraDevices.size());
    for (int i=0;i< cameraDeivceList.size();i++)
    {
        EXPECT_EQ(cameraDeivceList[i].getDescription(), expectDirectShowCameraDevices[i].getDescription());
        EXPECT_EQ(cameraDeivceList[i].getDevicePath(), expectDirectShowCameraDevices[i].getDevicePath());
        EXPECT_EQ(cameraDeivceList[i].getFriendlyName(), expectDirectShowCameraDevices[i].getFriendlyName());
    }
    
}
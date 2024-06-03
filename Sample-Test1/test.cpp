#pragma once
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../Project1/DeviceDriver.cpp"

using namespace testing;
using namespace std;

class MockFlashMemoryDevice : public FlashMemoryDevice
{
public:
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

class DriverTestFixture : public testing::Test {
public:
	MockFlashMemoryDevice fakeFlash;
	DeviceDriver deviceDriver{ &fakeFlash };
};

TEST_F(DriverTestFixture, Read5timesCheck) {
	EXPECT_CALL(fakeFlash, read(0x1000)).Times(5);
	int ret = deviceDriver.read(0x1000);
}

TEST_F(DriverTestFixture, ReadValidrData) {

	EXPECT_CALL(fakeFlash, read(0x1000)).WillRepeatedly(Return(10));
	EXPECT_EQ(10, deviceDriver.read(0x1000));
}


TEST_F(DriverTestFixture, ReadInvalidData) {

	EXPECT_CALL(fakeFlash, read(0x1000))
		.WillOnce(Return(9))
		.WillOnce(Return(9))
		.WillOnce(Return(9))
		.WillRepeatedly(Return(10));
	try {
		deviceDriver.read(0x1000);
		FAIL();
	}
	catch (ReadFailException e)
	{
		EXPECT_EQ(string{ e.what() }, string{ "Invalidn Data" });
	}
}

TEST_F(DriverTestFixture, WritePass) {

	EXPECT_CALL(fakeFlash, read(0x30))
		.WillOnce(Return(0xFF))
		.WillOnce(Return(0xFF))
		.WillOnce(Return(0xFF))
		.WillOnce(Return(0xFF))
		.WillOnce(Return(0xFF))
		.WillRepeatedly(Return(0xAA));

	deviceDriver.write(0x30, 0xAA);

	EXPECT_EQ(0xAA, deviceDriver.read(0x30));
}

TEST_F(DriverTestFixture, WriteFail) {

	EXPECT_CALL(fakeFlash, read(0x30))
		.WillRepeatedly(Return(0xAA));
	try {
		deviceDriver.write(0x30, 0xAA);
		FAIL();
	}
	catch (WriteFailException e)
	{
		EXPECT_EQ(string{ e.what() }, string{ "Not Clean" });
	}
}



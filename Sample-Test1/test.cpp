#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../Project1/DeviceDriver.cpp"
#include "../Project1/Application.cpp"

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


class AppTestFixture : public testing::Test {
public:
	MockFlashMemoryDevice fakeFlash;
	DeviceDriver deviceDriver{ &fakeFlash };
	//MockApplication app{ &deviceDriver };
	Application app{ &deviceDriver };
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

	EXPECT_THROW(deviceDriver.read(0x1000), ReadFailException);
}

TEST_F(DriverTestFixture, WritePass) {

	EXPECT_CALL(fakeFlash, read(0x30))
		.WillOnce(Return(0xFF))
		.WillOnce(Return(0xFF))
		.WillOnce(Return(0xFF))
		.WillOnce(Return(0xFF))
		.WillOnce(Return(0xFF))
		.WillRepeatedly(Return(0xAA));

	EXPECT_CALL(fakeFlash, write(0x30, 0xAA));  //write도 한번 호출될거 라고 알려주는 거임

	deviceDriver.write(0x30, 0xAA);

	EXPECT_EQ(0xAA, deviceDriver.read(0x30));
}

TEST_F(DriverTestFixture, WriteFail) {

	EXPECT_CALL(fakeFlash, read(0x30))
		.WillRepeatedly(Return(0xAA));

	EXPECT_THROW(deviceDriver.write(0x30, 0xAA), WriteFailException);
}

TEST_F(AppTestFixture, ReadChunkTest) {

	EXPECT_CALL(fakeFlash, read(_))
		.Times((0x50+1)*5)
		.WillRepeatedly(Return(0xAA));

	app.ReadAndPrint(0x00, 0x50);
}


TEST_F(AppTestFixture, WriteChunkTest) {

	EXPECT_CALL(fakeFlash, read(_))
		.WillRepeatedly(Return(0xFF));

	app.WriteAll(0xAA);

	EXPECT_CALL(fakeFlash, read)
		.Times((Write_End+1) * 5)
		.WillRepeatedly(Return(0xAA));

	app.ReadAndPrint(Write_Start, Write_End);
}



TEST_F(AppTestFixture, WriteChunkTestFail) {

	EXPECT_CALL(fakeFlash, read(_))
		.WillRepeatedly(Return(0xAA));

	EXPECT_THROW(app.WriteAll(0xAA), WriteFailException);

}

#pragma once
#include <vector>
#include <iostream>
#include "DeviceDriver.h"

using namespace std;

const long Write_Start = 0x0;
const long Write_End = 0x40;

class Application {
public:
    Application(DeviceDriver* devieceDriver);
    void ReadAndPrint(long startAddr, long endAddr);
    void WriteAll(int value);

protected:
    DeviceDriver* m_DeviceDriver;

private:
    void printReadData(vector<int> readData);
};
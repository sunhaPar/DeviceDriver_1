#include <vector>
#include "Application.h"

Application::Application(DeviceDriver* devieceDriver) : m_DeviceDriver(devieceDriver)
{}


void Application::ReadAndPrint(long startAddr, long endAddr)
{
	vector<int> readData = {};
	for (long addr = startAddr; addr <= endAddr; addr++)
	{
		readData.push_back(m_DeviceDriver->read(addr));
	}
}

void Application::WriteAll(int value)
{
	for (long addr = Write_Start; addr <= Write_End; addr++)
	{
		m_DeviceDriver->write(addr, value);
	}
}

void Application::printReadData(vector<int> readData)
{
	for (int data : readData)
	{
		cout << data << " ";
	}
}
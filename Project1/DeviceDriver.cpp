#include "DeviceDriver.h"
#include <stdexcept>

using namespace std;
DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{}

class ReadFailException : public std::exception{
public:
    char const* what() const override
    {
      return  "Invalidn Data";
    }
};

class WriteFailException : public std::exception {
public:
    char const* what() const override
    {
       return "Not Clean";
    }
};

int DeviceDriver::read(long address)
{
    // TODO: implement this method properly
    int pre = (int)(m_hardware->read(address));
    for (int i = 0; i < 4; i++)
    {
        int next = (int)(m_hardware->read(address));
        if (pre != next)
        {
            throw ReadFailException();
        }
    }
    return pre;
}

void DeviceDriver::write(long address, int data)
{
    if((read(address)) != 0xFF)   throw WriteFailException();
  
    m_hardware->write(address, (unsigned char)data);
}
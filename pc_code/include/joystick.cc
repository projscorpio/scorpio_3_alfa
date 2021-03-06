#include "joystick.hh"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sstream>
#include "unistd.h"
#include "bits/stdc++.h"

std::string CommunicationModule::coding(void)
{
	std::string ax[4];
	for(int i = 0; i < 4; i++){
    std::stringstream ss;
    ss << std::hex << this->AXIS[i];
    ax[i] = "0x" + ss.str();
    //std::cout << ax[i] <<std::endl;
	}
	std::string stringToSend;
	stringToSend = ax[0]+"_"+ax[1]+"_"+ax[2]+"_"+ax[3];
	std::cerr<<stringToSend<<std::endl;
	return stringToSend;
}

void CommunicationModule::buttonSetting(int number, bool value)
{
	this->BUTTON[number] = value;
}

void CommunicationModule::axisSetting(int number, int value)
{
	this->AXIS[number] = value;
}

Joystick::Joystick()
{
  openPath("/dev/input/js0");
}

Joystick::Joystick(int joystickNumber)
{
  std::stringstream sstm;
  sstm << "/dev/input/js" << joystickNumber;
  openPath(sstm.str());
}

Joystick::Joystick(std::string devicePath)
{
  openPath(devicePath);
}

Joystick::Joystick(std::string devicePath, bool blocking)
{
  openPath(devicePath, blocking);
}

void Joystick::openPath(std::string devicePath, bool blocking)
{
  // Open the device using either blocking or non-blocking
  _fd = open(devicePath.c_str(), blocking ? O_RDONLY : O_RDONLY | O_NONBLOCK);
}

bool Joystick::sample(JoystickEvent* event)
{
  int bytes = read(_fd, event, sizeof(*event));

  if (bytes == -1)
    return false;

  // NOTE if this condition is not met, we're probably out of sync and this
  // Joystick instance is likely unusable
  return bytes == sizeof(*event);
}

bool Joystick::isFound()
{
  return _fd >= 0;
}

Joystick::~Joystick()
{
  close(_fd);
}

std::ostream& operator<<(std::ostream& os, const JoystickEvent& e)
{
  os << "type=" << static_cast<int>(e.type)
     << " number=" << static_cast<int>(e.number)
     << " value=" << static_cast<int>(e.value);
  return os;
}

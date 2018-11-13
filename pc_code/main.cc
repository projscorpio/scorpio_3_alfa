#include "joystick.hh"
#include "udp_server.hh"
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
  // Create an instance of Joystick
  Joystick joystick("/dev/input/js0");

  // Ensure that it was found and that we can use it
  if (!joystick.isFound())
  {
    printf("open failed.\n");
    exit(1);
  }

  CommunicationModule * buffor = new CommunicationModule;
  UDP_Client * client = new UDP_Client("192.168.1.107",6969);
  while (true)
  {
    // Restrict rate
    usleep(1000);

    // Attempt to sample an event from the joystick
    JoystickEvent event;
    if (joystick.sample(&event))
    {
      if (event.isButton())
      {
        //printf("Button %u is %s\n",
        //event.number,
        //event.value == 0 ? "up" : "down");
	  buffor->buttonSetting(event.number,event.value);
      }
      else if (event.isAxis())
      {
        //printf("Axis %u is at position %d\n", event.number, event.value);
	buffor->axisSetting(event.number,event.value);
      }
    }
	system("clear");
	//strcpy(data,buffor->coding().c_str());
	client->send_data(buffor->coding().c_str());
  }
}

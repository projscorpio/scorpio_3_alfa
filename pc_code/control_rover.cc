#include "include/joystick.hh"
#include "include/udp_server.hh"
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
  // Create an instance of Joystick
  Joystick joystick("/dev/input/js0");

  // Ensure that it was found and that we can use it
  if (!joystick.isFound())
  {
    printf("[Joystick][Init] Joystick not detected.\n");
    exit(1);
  }else{
    printf("[Joystick][Init] Joystick detected.\n");
  }

  CommunicationModule * buffor = new CommunicationModule;
  UDP_Client * client = new UDP_Client("192.168.4.1", DRV_PORT);
  while (true)
  {
    // Restrict rate
    usleep(300);

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
	//system("clear");
	client->send_data(buffor->coding().c_str());
  }
}

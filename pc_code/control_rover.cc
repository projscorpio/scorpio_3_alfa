#include "include/joystick.hh"
#include "include/udp_server.hh"
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

#define TIMER_SEC 5
#define TIMER_USEC 0

// Create an instance of Joystick
Joystick joystick("/dev/input/js0");
UDP_Client * client;

void sig_term(int signo){
  fprintf(stderr, "[Joystick][Terminate] Signal received");
  client->send_data(RETURN_0_0_VALUE);
  exit(signo);
}

void sig_alrm(int signo){
  while(joystick.isFound()){
    fprintf(stderr, "[Joystick][Plug check] Joystick not detected\n");
    client->send_data(RETURN_0_0_VALUE);
  }
}

int main(void)
{
  // Ensure that it was found and that we can use it
  if (!joystick.isFound())
  {
    printf("[Joystick][Init] Joystick not detected.\n");
    exit(1);
  }else{
    printf("[Joystick][Init] Joystick detected.\n");
  }

  // take over signals 
  signal(SIGTERM, sig_term);
  signal(SIGINT, sig_term);
  signal(SIGALRM, sig_alrm);

  struct itimerval js_timer;
  js_timer.it_interval.tv_sec = js_timer.it_value.tv_sec = TIMER_SEC;
  js_timer.it_interval.tv_usec = js_timer.it_value.tv_usec = TIMER_USEC;
  setitimer(ITIMER_REAL, &js_timer, 0);

  CommunicationModule * buffor = new CommunicationModule;
  client = new UDP_Client(SRV_IP, DRV_PORT);
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

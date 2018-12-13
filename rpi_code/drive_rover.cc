#include <limits.h>
#include <math.h>
#include <iostream>
#include <new>
#include <cstdio>
#include <pigpio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <csignal>
#include "include/header.hh"
#include "include/udp_server.hh"

void rover_stop(void){
  gpioWrite(INA_R_0,HIGH);
  gpioWrite(INB_R_0,HIGH);

  gpioWrite(INA_R_1,HIGH);
  gpioWrite(INB_R_1,HIGH);

	gpioPWM(PWM_R_0, abs(0));
	gpioPWM(PWM_R_1, abs(0));
  printf("[Drive Rover] Rover stop");
}

void sig_int(int signum){
  fprintf(stderr, "[Drive Rover] Interrupt signal received");
  rover_stop();
  exit(signum);
}

void sig_term(int signum){
  fprintf(stderr, "[Drive Rover] Terminate signal received");
  rover_stop();
  exit(signum);
}

int main(void)
{
	std::cout<<"[Drive Rover] OK: Raspberry Pi init"<<std::endl;
  signal(SIGTERM, sig_term);
  signal(SIGINT, sig_int);
	if( gpioInitialise() < 0)
		{
			std::cerr<<"[Drive Rover] Error: Library fault"<<std::endl;
			exit(1);
		}else std::cout<<"[Drive Rover] OK: Library linked!"<<std::endl;
	UDP_Server *serv = new UDP_Server(DRV_PORT);
  controllerPositions* libr = new controllerPositions;
	while(1)
	{
		libr->reading(serv->rcv_data("0"));
		libr->setting();

		gpioDelay(200);
	}
	gpioTerminate();
	delete libr;
	delete serv;
	return 0;
}


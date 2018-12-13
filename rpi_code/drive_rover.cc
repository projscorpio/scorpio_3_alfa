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

void sig_int(int signum){
  fprintf(stderr, "[Drive Rover] Interrupt signal received");
  //libr->reading(RETURN_0_0_VALUE);
  exit(signum);
}

void sig_term(int signum){
  fprintf(stderr, "[Drive Rover] Terminate signal received");
  //libr->reading(RETURN_0_0_VALUE);
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


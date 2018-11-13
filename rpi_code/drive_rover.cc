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
#include "include/header.hh"
#include "include/udp_server.hh"

int main(void)
{
	system("clear");
	std::cout<<"OK: Raspberry Pi init"<<std::endl;
	if( gpioInitialise() < 0)
		{
			std::cerr<<"Error: Library fault"<<std::endl;
			exit(1);
		}
		else std::cout<<"OK: Library linked!"<<std::endl;
	controllerPositions* libr = new controllerPositions;
	UDP_Server *serv = new UDP_Server(6969);
	while(1)
	{
		libr->reading(serv->rcv_data("0"));
		libr->setting();

		//gpioWrite(LED,HIGH);
		//std::cout<<"Port 17: "<<gpioRead(LED)<<std::endl;
		//gpioDelay(500000);
		//gpioWrite(LED,LOW);
		//std::cout<<"Port 17: "<<gpioRead(LED)<<std::endl;
		gpioDelay(500);
	}
	gpioTerminate();
	delete libr;
	delete serv;
	return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#ifndef __UDP_SERVER_H__
#define __UDP_SERVER_H__

#define MAXLINE 1024
#define DRV_PORT 8080
#define SRV_IP ""

class UDP_Server
{
  private:
    int port;
    int sock_fd;
    struct sockaddr_in serv_addr, client_addr;
    char* buffer;

  public:
    UDP_Server(int port_id);
    ~UDP_Server();
    char* rcv_data(char* response);
};

class UDP_Client
{
  private:
    int sock_fd;
    struct sockaddr_in serv_addr;
    char* buffer;
  public:
    UDP_Client(int port, char* srv_ip);
    ~UDP_Client();
    char* send_data(char* msg);
};
inline void error_msg(char* msg){
  perror(msg);
  exit(EXIT_FAILURE);
}

#endif //__UDP_SERVER_H__
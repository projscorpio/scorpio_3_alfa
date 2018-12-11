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

#define SERVER_TIMEOUT_VAL_S  0 //seconds
#define SERVER_TIMEOUT_VAL_US 700 //microseconds
#define CLIENT_TIMEOUT_VAL_S  1 //seconds
#define CLIENT_TIMEOUT_VAL_US 500 //microseconds
#define MAXLINE 1024
#define DRV_PORT 7001
#define SRV_IP "192.168.4.1"
#define RETURN_0_0_VALUE "0x0_0x0_0x0_0x0"

/*
 * UDP Server Class. 
 * UDP_Server(int port_id);
 *   port_id -- port to communication
 *   create sockets
 * char* rcv_data(char* response)
 *   get data form port. 
 *   response -- send to client.
 *   return value -- data from client
 */
class UDP_Server
{
  private:
    int sock_fd;
    struct sockaddr_in serv_addr, client_addr;
    struct timeval timeout;
    char* buffer;

  public:
    UDP_Server(const int port_id);
    ~UDP_Server();
    char* rcv_data(const char* response);
};


/*
 * UDP Client Class
 * UDP_Client(int port_id, char* stv_ip)
 *   port_id -- port to communication
 *   srv_ip -- server ip
 * char* send_data(char* msg)
 *   msg -- send msg
 *   return value -- data from server
 */
class UDP_Client
{
  private:
    int sock_fd;
    struct sockaddr_in serv_addr;
    struct timeval timeout;
    char* buffer;
  public:
    UDP_Client(const char* srv_ip, const int port_id);
    ~UDP_Client();
    char* send_data(const char* msg);
};


inline void error_msg(const char* msg){
  perror(msg);
  //exit(EXIT_FAILURE);
}

#endif //__UDP_SERVER_H__

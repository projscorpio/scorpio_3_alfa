#include "udp_server.hpp"

/*
 * UDP SERVER
 */

UDP_Server::UDP_Server(int port_id){
  port=port_id;
  this->buffer=(char*) malloc(sizeof(*buffer)*MAXLINE);

  if((this->sock_fd=socket(AF_INET, SOCK_DGRAM, 0))<0)
    error_msg("[UDP_Server][socket]");
  memset(&this->serv_addr, 0, sizeof(this->serv_addr));
  memset(&this->client_addr, 0, sizeof(this->client_addr));

  this->serv_addr.sin_family=AF_INET;
  this->serv_addr.sin_addr.s_addr=INADDR_ANY;
  this->serv_addr.sin_port=htons(port_id);
 
  if(bind(this->sock_fd, (const struct sockaddr*) &this->serv_addr, 
        sizeof(this->serv_addr))<0)
    error_msg("[UDP_Server][bind]");
}

UDP_Server::~UDP_Server(){
  close(this->sock_fd);
}

char* UDP_Server::rcv_data(char* response){
  int len=0, n=0;
  n=recvfrom(this->sock_fd, (char*) this->buffer, MAXLINE, MSG_WAITALL,
      (struct sockaddr*) &this->client_addr, (socklen_t*) &len);
  this->buffer[n]='\n';
  sendto(this->sock_fd, (const char*) response, strlen( (const char*)response),
      MSG_CONFIRM, (const struct sockaddr*) &this->client_addr, len);
  return buffer;
}

/*
 * UDP Client
 */

UDP_Client::UDP_Client(int port, char* srv_ip){
  this->buffer=(char*) malloc(sizeof(*buffer)*MAXLINE);
  if((this->sock_fd=socket(AF_INET, SOCK_DGRAM, 0))<0)
    error_msg("[UDP_Client][socket]");
  memset(&this->serv_addr, 0, sizeof(this->serv_addr));

  this->serv_addr.sin_family=AF_INET;
  this->serv_addr.sin_port=htons(port);
  this->serv_addr.sin_addr.s_addr=INADDR_ANY;

  if(inet_aton(srv_ip, &this->serv_addr.sin_addr)<0)
    error_msg("[UDP_Client][inet_aton]");
}

UDP_Client::~UDP_Client(){
  close(this->sock_fd);
  free(buffer);
}

char* UDP_Client::send_data(char* msg){
  int n=0, len=0;
  sendto(this->sock_fd, (const char*) msg, strlen(msg),
      MSG_CONFIRM, (const struct sockaddr*) &this->serv_addr, 
      sizeof(this->serv_addr));
#ifdef DEGUB
  printf("[UDP_Client][sendto] Data: %s", msg);
#endif
  n=recvfrom(this->sock_fd, (char*) buffer, MAXLINE, 
      MSG_WAITALL, (struct sockaddr*) &this->serv_addr, (socklen_t*) &len);
  buffer[n]='\n';
  return buffer;
}

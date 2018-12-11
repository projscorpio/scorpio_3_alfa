#include "udp_server.hh"

/*
 * UDP SERVER
 */

UDP_Server::UDP_Server(const int port_id){
  this->buffer=(char*) malloc(sizeof(*buffer)*MAXLINE);

  if((this->sock_fd=socket(AF_INET, SOCK_DGRAM, 0))<0)
    error_msg("[UDP_Server][socket]");
  memset(&this->serv_addr, 0, sizeof(this->serv_addr));
  memset(&this->client_addr, 0, sizeof(this->client_addr));

  this->serv_addr.sin_family=AF_INET;
  this->serv_addr.sin_addr.s_addr=INADDR_ANY;
  this->serv_addr.sin_port=htons(port_id);

  this->timeout.tv_sec=SERVER_TIMEOUT_VAL_S;
  this->timeout.tv_usec=SERVER_TIMEOUT_VAL_US;

  if(setsockopt(this->sock_fd, SOL_SOCKET, SO_RCVTIMEO, 
        (char*) &this->timeout, sizeof(this->timeout))<0)
    error_msg("[UDP_Server][Timeout_set]");
 
  if(bind(this->sock_fd, (const struct sockaddr*) &this->serv_addr, 
        sizeof(this->serv_addr))<0)
    error_msg("[UDP_Server][bind]");
  printf("[UDP_Server][Init] Ok\n");
}

UDP_Server::~UDP_Server(){
  close(this->sock_fd);
  free(this->buffer);
  printf("[UDP_Server][Close] Ok\n");
}

char* UDP_Server::rcv_data(const char* response){
  int len=sizeof(this->client_addr), n=0;
  bzero(this->buffer, MAXLINE);
  if((n=recvfrom(this->sock_fd, (char*) this->buffer, MAXLINE, 0,
      (struct sockaddr*) &this->client_addr, (socklen_t*) &len))<0){
    fprintf(stderr,"[UDP_Server][Receive timeout]\n");
    strcpy(this->buffer, RETURN_0_0_VALUE);
    return this->buffer;
    }
  this->buffer[n]='\n';

  printf("[UDP_Server][rcvfrom] Data: %s\n", this->buffer);
  printf("[UDP_Server][rcvfrom] Response: %s\n", response);
  
  sendto(this->sock_fd, (const char*) response, strlen( (const char*)response),
      MSG_DONTWAIT, (const struct sockaddr*) &this->client_addr, len);
  return this->buffer;
}

/*
 * UDP Client
 */

UDP_Client::UDP_Client(const char* srv_ip, const int port_id){
  this->buffer=(char*) malloc(sizeof(*buffer)*MAXLINE);
  if((this->sock_fd=socket(AF_INET, SOCK_DGRAM, 0))<0)
    error_msg("[UDP_Client][socket]");
  memset(&this->serv_addr, 0, sizeof(this->serv_addr));

  this->serv_addr.sin_family=AF_INET;
  this->serv_addr.sin_port=htons(port_id);

  this->timeout.tv_sec=CLIENT_TIMEOUT_VAL_S;
  this->timeout.tv_usec=CLIENT_TIMEOUT_VAL_US;

  if(setsockopt(this->sock_fd, SOL_SOCKET, SO_RCVTIMEO, 
        (char*) &this->timeout, sizeof(this->timeout))<0)
    error_msg("[UDP_Client][Timeout_set]");

  if(inet_aton(srv_ip, &this->serv_addr.sin_addr)<0)
    error_msg("[UDP_Client][inet_aton]");
  printf("[UDP_Client][Init] Ok\n");
}

UDP_Client::~UDP_Client(){
  close(this->sock_fd);
  free(buffer);
  printf("[UDP_Client][Close] Ok\n");
}

char* UDP_Client::send_data(const char* msg){
  int n=0, len=sizeof(this->serv_addr);
  bzero(this->buffer, MAXLINE);
  if(sendto(this->sock_fd, (const char*) msg, strlen(msg),
      MSG_DONTWAIT, (const struct sockaddr*) &this->serv_addr, 
      sizeof(this->serv_addr))<0)
    error_msg("[UDP_Client][sendto]");

  printf("[UDP_Client][sendto] Data: %s\n", msg);

  if((n=recvfrom(this->sock_fd, (char*) this->buffer, MAXLINE, 
      0, (struct sockaddr*) &this->serv_addr, (socklen_t*) &len))<0)
    error_msg("[UPD_Client][recvfrom]");
  this->buffer[n]='\n';
  return this->buffer;
}


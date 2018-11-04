#include "udp_server.hh"

/*
 * UDP SERVER
 */

UDP_Server::UDP_Server(int port_id){
  this->buffer=(char*) malloc(sizeof(*buffer)*MAXLINE);

  if((this->sock_fd=socket(AF_INET, SOCK_DGRAM, 0))<0)
    error_msg("[UDP_Server][socket]");
  memset(&this->serv_addr, 0, sizeof(this->serv_addr));
  memset(&this->client_addr, 0, sizeof(this->client_addr));

  this->serv_addr.sin_family=AF_INET;
  this->serv_addr.sin_addr.s_addr=INADDR_ANY;
  this->serv_addr.sin_port=htons(port_id);

  this->timeout.tv_sec=TIMEOUT_VAL_S;
  this->timeout.tv_usec=TIMEOUT_VAL_US;

  if(setsockopt(this->sock_fd, SOL_SOCKET, SO_RCVTIMEO, 
        (char*) &this->timeout, sizeof(this->timeout))<0)
    error_msg("[UDP_Server][Timeout_set]");
 
  if(bind(this->sock_fd, (const struct sockaddr*) &this->serv_addr, 
        sizeof(this->serv_addr))<0)
    error_msg("[UDP_Server][bind]");
}

UDP_Server::~UDP_Server(){
  close(this->sock_fd);
}

char* UDP_Server::rcv_data(char* response){
  int len=0, n=0;
  if((n=recvfrom(this->sock_fd, (char*) this->buffer, MAXLINE, MSG_WAITALL,
      (struct sockaddr*) &this->client_addr, (socklen_t*) &len))<0){
    fprintf(stderr,"[UDP_Server][Receive timeout]\n");
    strcpy(this->buffer, RETURN_0_0_VALUE);
    return buffer;
    }
  this->buffer[n]='\n';
#ifdef DEGUB
  printf("[UDP_Server][rcvfrom] Data: %s", this->buffer);
#endif
  sendto(this->sock_fd, (const char*) response, strlen( (const char*)response),
      MSG_CONFIRM, (const struct sockaddr*) &this->client_addr, len);
  return buffer;
}

/*
 * UDP Client
 */

UDP_Client::UDP_Client(int port_id, char* srv_ip){
  this->buffer=(char*) malloc(sizeof(*buffer)*MAXLINE);
  if((this->sock_fd=socket(AF_INET, SOCK_DGRAM, 0))<0)
    error_msg("[UDP_Client][socket]");
  memset(&this->serv_addr, 0, sizeof(this->serv_addr));

  this->serv_addr.sin_family=AF_INET;
  this->serv_addr.sin_port=htons(port_id);
  this->serv_addr.sin_addr.s_addr=INADDR_ANY;

  this->timeout.tv_sec=TIMEOUT_VAL_S;
  this->timeout.tv_usec=TIMEOUT_VAL_US;

  this->fds.fd=this->sock_fd;
  this->fds.events=POLLOUT;

  if(setsockopt(this->sock_fd, SOL_SOCKET, SO_SNDTIMEO, 
        (char*) &this->timeout, sizeof(this->timeout))<0)
    error_msg("[UDP_Client][Timeout_set]");

  if(inet_aton(srv_ip, &this->serv_addr.sin_addr)<0)
    error_msg("[UDP_Client][inet_aton]");
}

UDP_Client::~UDP_Client(){
  close(this->sock_fd);
  free(buffer);
}

char* UDP_Client::send_data(char* msg){
  int n=0, len=0;
  if(!this->is_available()){
    fprintf(stderr, "[UDP_Client][send_data timeout]\n");
    return "1";
  }
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

bool UDP_Client::is_available(){
  if(poll(&this->fds, NULL, 1000*TIMEOUT_VAL_S+TIMEOUT_VAL_US)>0)
    return true;
  return false;
}

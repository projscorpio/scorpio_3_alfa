#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT     8080
#define MAXLINE 1024

void error_msg(char* msg){
  perror(msg);
  exit(EXIT_FAILURE);
}
int main(){
  int sock_fd;
  char buffer[MAXLINE];
  char *response="0";
  struct sockaddr_in serv_addr, client_addr;

  if((sock_fd=socket(AF_INET, SOCK_DGRAM, 0))<0)
    error_msg("[udp_server][socket]");
  memset(&client_addr, 0, sizeof(client_addr));
  memset(&serv_addr, 0, sizeof(serv_addr));

  serv_addr.sin_family=AF_INET;
  serv_addr.sin_addr.s_addr=INADDR_ANY;
  serv_addr.sin_port=htons(PORT);

  if(bind(sock_fd, (const struct sockaddr*) &serv_addr, sizeof(serv_addr))<0    )
    error_msg("[udp_server][bind]");
  while(1){
    int len=0, n=0;
    n=recvfrom(sock_fd, (char *)buffer, MAXLINE, MSG_WAITALL,
        (struct sockaddr*) &client_addr, &len);
    buffer[n]='\n';
    printf("Client: %s\n", buffer);
    sendto(sock_fd, (const char *)response, strlen(response),
        MSG_CONFIRM, (const struct sockaddr *) &client_addr, len);
  }
  return 0;
}

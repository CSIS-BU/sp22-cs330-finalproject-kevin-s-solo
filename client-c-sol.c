/*****************************************************************************
 * client-c-sol.c                                                                 
 * Name: Kevin Splinter
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <err.h>

#define SEND_BUFFER_SIZE 2048


/* TODO: client()*/
int client(char *server_ip, char *server_port, char *router_ip, char *router_port) {
  // declare variables
  int sockfd; // listen on sock_fd, new connection on new_fd
  struct addrinfo hints, *servinfo, *routinfo, *p;
  struct sockaddr_in client_addr, *server_addr; // client's address information
  socklen_t sin_size;
  int error;
  int yes = 1;                  // used in setsockopt()
  char *status;
  char ipAddr[INET_ADDRSTRLEN]; // number of byte for IPv4 address
  unsigned char buffer[SEND_BUFFER_SIZE], serialize_buf[SEND_BUFFER_SIZE];

  // build address data structure with getaddrinfo()
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  //creates the struct for the server with the routers ip and port
  error = getaddrinfo(server_ip, server_port, &hints, &servinfo);
  if (error) {
    errx(1, "%s", gai_strerror(error));
  }

  //creates the struct for the router with the routers ip and port
  error = getaddrinfo(router_ip, router_port, &hints, &routinfo);
  if (error) {
    errx(1, "%s", gai_strerror(error));
  }

  // create a socket. here, s is the new socket descriptor
  if ((sockfd = socket(routinfo->ai_family, routinfo->ai_socktype,
                       routinfo->ai_protocol)) < 0) {
    // handle the error if failed to create a socket
    perror("client: socket");
    exit(-1);
  }  
  
  // conncet to the server
  if(connect(sockfd, routinfo->ai_addr,  routinfo->ai_addrlen) < 0) {
    // handle the error if failed to connect
    perror("client: connect");
    exit(-1);
  }
  //creates the buuffer for the header and the data
  unsigned char messageWithHeader[SEND_BUFFER_SIZE];
  //takes the ip and port from the appropreate structs
  server_addr = (struct sockaddr_in*)servinfo->ai_addr;
  uint32_t serverIP = ntohl(server_addr->sin_addr.s_addr); //4 bytes
  unsigned short int serverPort = ntohs(server_addr->sin_port); //2 bytes
  printf("%hu\n",serverPort);
  //creates a garbage sourcePort number
  unsigned short int sourcePort;
  //puts the header in the first 12 bytes of the array
  //copys the serverip to the char array
  memcpy(messageWithHeader, &serverIP, 4);
  //copys the serverport to the char array
  memcpy((messageWithHeader + 4), &serverPort,2);
  //copys the serverip again for the client b/c the ip's are the same
  memcpy((messageWithHeader + 6), &serverIP, 4);
  //copys the garbage port number just as a place holder for the header
  memcpy((messageWithHeader + 10), &sourcePort, 2);

  int size;
  //reads in from std with the buffer size minus the header
  while((size=read(0, buffer, SEND_BUFFER_SIZE-12))) {
    memcpy((messageWithHeader + 12), buffer, size);
    //sending the size of the message + 12 for the header
    if(send(sockfd, messageWithHeader, size + 12, 0) < 0) {
      perror("client: send");
    }
  }
  //printf("\n%x\n", *(uint32_t*)(messageWithHeader + 6));

  // Done, close the s socket descriptor
  close(sockfd);

  return 0;
}

/*
 * main()
*/
int main(int argc, char **argv) {
  char *server_ip;
  char *server_port;
  char *router_ip;
  char *router_port;

  if (argc != 5) {
    fprintf(stderr, "Usage: ./client-c [server IP] [server port] [router IP] [router port] < [message]\n");
    exit(EXIT_FAILURE);
  }
  
  server_ip = argv[1];
  server_port = argv[2];
  //added a router port agument
  router_ip = argv[3];
  router_port = argv[4];
  
  return client(server_ip, server_port, router_ip, router_port);
}

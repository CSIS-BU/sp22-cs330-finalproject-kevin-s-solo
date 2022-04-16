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


/* TODO: client()
 * Open socket and send message from stdin.
 * Return 0 on success, non-zero on failure
*/
int client(char *server_ip, char *server_port, char *router_ip, char *router_port) {
  // declare variables
  int sockfd; // listen on sock_fd, new connection on new_fd
  struct addrinfo hints, *servinfo, *routinfo, *p;
  struct sockaddr_in client_addr, *server_addr; // client's address information
  socklen_t sin_size;
  int error;
  char *status;
  char ipAddr[INET_ADDRSTRLEN]; // number of byte for IPv4 address
  unsigned char buffer[SEND_BUFFER_SIZE], serialize_buf[SEND_BUFFER_SIZE];

  // build address data structure with getaddrinfo()
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  //create  struct for  server with routers ip / port
  error = getaddrinfo(server_ip, server_port, &hints, &servinfo);
  if (error) {
    errx(1, "%s", gai_strerror(error));
  }

  //create struct for router with routers ip / port
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
  
  // conncet to the server using the socket descriptor s
  // Note: different from the server, the client does NOT create
  // another socket descriptor for communication
  if(connect(sockfd, routinfo->ai_addr,  routinfo->ai_addrlen) < 0) {
    // handle the error if failed to connect
    perror("client: connect");
    exit(-1);
  }

  //create buffer for header / data
 


  //take the ip and port from the appropreate structs
 
  //create a garbage sourcePort number


  //put the header in the first 12 bytes of the array
  //copy the serverip to the char array
 

  //copy the serverport to the char array


  //copy the serverip again for the client b/c the ip's are the same

  //copy the garbage port number just as a place holder for the header



  //read in from std with the buffer size minus the header


  close(sockfd);

  return 0;
}

/*
 * main()
 * Parse command-line arguments and call client function
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

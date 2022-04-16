/*****************************************************************************
 * router-c.c
 * Name: Kevin Splinter
 *****************************************************************************/

#include <arpa/inet.h> // for inet_ntop()
#include <err.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define QUEUE_LENGTH 10
#define RECV_BUFFER_SIZE 2048

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in *)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

/* TODO: server()
 * Open socket and wait for client to connect
 * Print received message to stdout
 * Return 0 on success, non-zero on failure
 */
int router(char *router_port) {
  // declare variables
  int sockfd, new_sockfd, server_sockfd; // listen on sock_fd, new connection on new_fd
  struct addrinfo hints, hints2, *routinfo, *servinfo, *p;
  struct sockaddr_in client_addr; // client's address information
  socklen_t length;
  int error;
  int size;
  int yes = 1;                  // used in setsockopt()
  char ipAddr[INET_ADDRSTRLEN]; // number of byte for IPv4 address
  unsigned char buffer[RECV_BUFFER_SIZE], serialize_buf[RECV_BUFFER_SIZE];

  // build address data structure with getaddrinfo()
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // use my IP

  error = getaddrinfo(NULL, router_port, &hints, &routinfo);
  if (error) {
    errx(1, "%s", gai_strerror(error));
  }

  // create a socket. here, s is the new socket descriptor
  if ((sockfd = socket(routinfo->ai_family, routinfo->ai_socktype,
                       routinfo->ai_protocol)) < 0) {
    // handle the error if failed to create a socket
    perror("router: socket error");
    exit(-1);
  }

  // bind the socket to the port we passed in to getaddrinfo()
  if (bind(sockfd, routinfo->ai_addr, routinfo->ai_addrlen) < 0) {
    // handle the error if failed to bind
    close(sockfd);
    perror("router: bind error\n");
    exit(-1);
  }
  freeaddrinfo(routinfo);

  if (routinfo == NULL) {
    fprintf(stderr, "router: failed to bind\n");
    exit(-1);
  }

  // prepare the server for incoming requests.

  //creates vars for the imcomming message ip and port
  uint32_t server_ip;
  unsigned short server_port;
  //hardcode the port numbers for the servers and the ip to be used in getaddrinfo
  char ip[10] = {'l','o','c','a','l','h','o','s','t','\0'};
  unsigned short int server1PortINT = 6789;
  unsigned short int server2PortINT = 9876;
  char server1PortARR[5] = {'6','7','8','9','\0'};
  char server2PortARR[5] = {'9','8','7','6','\0'};
  // wait for connection, then start exchanging messages
  while (1) {
    //NEED TO FILL THIS OUT
     
    
    }
    fflush(stdout);
    /*
     * Done, close the new _ s descriptor, i.e., release the connection.
     * Note: you should NOT close the socket descriptor s
     */
    close(new_sockfd);
  }
  return 0;
}

/*
 * main():
 * Parse command-line arguments and call server function
 */
int main(int argc, char **argv) {
  char *router_port;

  if (argc != 2) {
    fprintf(stderr, "Usage: ./router-c [router port]\n");
    exit(EXIT_FAILURE);
  }

  router_port = argv[1];

  return router(router_port);
}

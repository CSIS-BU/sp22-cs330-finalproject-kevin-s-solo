/*****************************************************************************
 * server-c-sol2.c
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
#include <stdbool.h>

#define QUEUE_LENGTH 10
#define RECV_BUFFER_SIZE 2048

//Trie Structure from GeeksForGeeks.com

#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])
 
// Alphabet size (# of symbols)
#define ALPHABET_SIZE (10)
 
// Converts key current character into index
// use only 'a' through 'z' and lower case
#define CHAR_TO_INDEX(c) (c-0)

//Trie node
struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
 
    // isEndOfWord is true if the node represents
    // end of a word
    bool isEndOfWord;
};

// Returns new trie node (initialized to NULLs)
struct TrieNode *getNode(void)
{
    struct TrieNode *pNode = NULL;
 
    pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
 
    if (pNode)
    {
        int i;
 
        pNode->isEndOfWord = false;
 
        for (i = 0; i < ALPHABET_SIZE; i++)
            pNode->children[i] = NULL;
    }
 
    return pNode;
}


// If not present, inserts key into trie
// If the key is prefix of trie node, just marks leaf node
void insert(struct TrieNode *root, const char *key)
{
    int level;
    int length = strlen(key);
    int index;
 
    struct TrieNode *pCrawl = root;
 
    for (level = 0; level < length; level++)
    {
        index = CHAR_TO_INDEX(key[level]);
        if (!pCrawl->children[index])
            pCrawl->children[index] = getNode();
 
        pCrawl = pCrawl->children[index];
    }
 
    // mark last node as leaf
    pCrawl->isEndOfWord = true;
}
 
// Returns true if key presents in trie, else false
bool search(struct TrieNode *root, const char *key)
{
    int level;
    int length = strlen(key);
    int index;
    struct TrieNode *pCrawl = root;
 
    for (level = 0; level < length; level++)
    {
        index = CHAR_TO_INDEX(key[level]);
 
        if (!pCrawl->children[index])
            return false;
 
        pCrawl = pCrawl->children[index];
    }
 
    return (pCrawl->isEndOfWord);
}


//Router Portion


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in *)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

/* TODO: server()*/
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

  // the following overcomes the bind() error "address already in use"
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    perror("setsockopt");
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

  // prepare the server 
  if (listen(sockfd, QUEUE_LENGTH) < 0) {
    perror("router: listen error");
    exit(-1);
  }
  //creates vars for the imcomming message ip and port
  uint32_t server_ip;
  unsigned short server_port;
  //hardcode the port numbers for the servers and the ip to be used in getaddrinfo
  char ip[10] = {'l','o','c','a','l','h','o','s','t','\0'};
  unsigned short int server1PortINT = 11224;
  unsigned short int server2PortINT = 11225;
  char server1PortARR[6] = {'1','1','2','2','4','\0'};
  char server2PortARR[5] = {'1','1','2','2','5','\0'};
  // wait for connection, then start exchanging messages
  while (1) {
    
    length = sizeof client_addr;
    if ((new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr,
                             &length)) < 0) {
      // handle the error if failed to accept }
      perror("router: accept error");
      exit(-1); //continue;
    }


    //Trie search needs to go in here

    while((size = recv(new_sockfd, buffer, RECV_BUFFER_SIZE, 0)) > 0) {
      //pulls out the ip and port from the message header
      server_ip = *(uint32_t*)(buffer);
      server_port = *(unsigned short int*)(buffer + 4);
      printf("DestIP: %x, DestPort: %u\n", server_ip, server_port);
      //checks which server the message should go to




      if(server1PortINT == server_port){
        //gets the addr info of server1 and puts it into servinfo
        error = getaddrinfo(ip, server1PortARR, &hints, &servinfo);
      }else if(server2PortINT == server_port){
        //gets the addr info of server2 and puts it into servinfo
        error = getaddrinfo(ip, server2PortARR, &hints, &servinfo);
      }else{
        //if the server port doesn't match prints a message and loops back to waiting for another client
        perror("router: The server doesn't exits");
        break;
      }




      
      fflush(stdout);
      //checks if gettaddrinfo worked
      if (error) {
        errx(1, "%s", gai_strerror(error));
      }
      //creates a new server_sockfd 
      if ((server_sockfd = socket(servinfo->ai_family, servinfo->ai_socktype,
                       servinfo->ai_protocol)) < 0) {
        // handle the error if failed to create a socket
        perror("router: socket");
        exit(-1);
      }
      //tries to connect to the server with the new server_sockfd
      if(connect(server_sockfd, servinfo->ai_addr,  servinfo->ai_addrlen) < 0) {
        // handle the error if failed to connect
        perror("router: connect");
        exit(-1);
      }
      //sends the clients buffer to the serve
      if(send(server_sockfd, buffer, size, 0) < 0) {
        perror("router: send");
      }
      //closes the socket to the server
      close(server_sockfd);
    }
    fflush(stdout);
  
    close(new_sockfd);
  }
  return 0;
}

/*
 * main()
 */
int main(int argc, char **argv) {
  char *router_port;

  //hardcoded ip addresses for servers
  //10.0.0.0 and 20.0.0.0
  char serverIPs[][100] = {"000010100000000000000000", "000101000000000000000000"};

  struct TrieNode *root = getNode();

  int i;
	for (i = 0; i < ARRAY_SIZE(keys); i++)
		insert(root, keys[i]);

  if (argc != 2) {
    fprintf(stderr, "Usage: ./router-c [router port]\n");
    exit(EXIT_FAILURE);
  }

  router_port = argv[1];

  return router(router_port);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define MAXLINE 4096

int client_setup(char *host,char *port)
/* Outputs a socket descriptor using the IP and port number*/
{
    struct addrinfo hints, *addr_list, *x ;
    int listenfd , n ;
    memset(&hints, 0, sizeof(struct addrinfo)) ;

    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags= AI_ADDRCONFIG;
    hints.ai_flags |= AI_NUMERICSERV;

    // Returns a list of struct addrinfo
    n = getaddrinfo(host, port, &hints, &addr_list);

    for(x = addr_list; x; x = x->ai_next)
    {
        if ((listenfd = socket(x->ai_family, x->ai_socktype, x->ai_protocol)) < 0){
        // Used to determine whether not the socket will be an end point for a successful connection
          continue ;
        }
        printf("trying to establish connection ...\n" );
        if (connect(listenfd, x->ai_addr, x->ai_addrlen) == 0){
        // Determines if socket descriptor can be asscoiated with the server's socket address
          puts("Connection accepted");
          break;
        }
        close(listenfd);
    }

    if (!x){
      perror("internal error");
      return -1;
    }

    freeaddrinfo(addr_list);
    return listenfd ;
  }

  int client_connect(int listenfd)
  /* Establishes a connection by using accept()
    Parameters:
      listenfd (int): The listening descriptor of a server
  */
  {
   // client's socket address
   char buffer[MAXLINE];
   int n;

  		//Send the message back to client
    while((fgets(buffer, MAXLINE, stdin)) != NULL)
    {
      n = send(listenfd , buffer , strlen(buffer) , 0);
      if( n < 0)
      {
        perror("internal error");
        return 1;
      }

    }
    close(listenfd);
    return 0;
    }


int udp_client(char *host,int port)
{
  /* Establishes a connection using UDP
  */
  struct sockaddr_in serv;
  int listenfd, n;
  char buffer[MAXLINE];
  char *buf;

  memset(&serv, 0, sizeof(serv));

  if ((listenfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("internal error");
    exit(1);
  }

  serv.sin_family = AF_INET;
  serv.sin_addr.s_addr = inet_addr(host);
  serv.sin_port = htons(port);
  printf("establishing connection ...\n" );
    //Send the message back to client
  while(1)
  {
    buf = fgets(buffer, MAXLINE, stdin);
    if (buf == NULL)
      break;

    //Send message
    n = sendto(listenfd , buffer, strlen(buffer), 0, (struct sockaddr *)
               &serv, sizeof(serv));
    if( n < 0)
    {
      perror("internal error");
      return 1;
    }

  }
  close(listenfd);
  return 0;
}

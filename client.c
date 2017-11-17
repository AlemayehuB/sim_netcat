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
#include<pthread.h>
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

//********************* UDP ************************************************
struct thr_str{
  int fd;
  struct sockaddr_in serv;
};
void* send_udp(void *thr_str)
{
  char buffer[MAXLINE];
  char *buf;
  struct thr_str pair = *(struct thr_str*)thr_str;
  int fd = pair.fd;
  struct sockaddr_in serv = pair.serv;

  while(1)
  {
    buf = fgets(buffer, MAXLINE, stdin);
    if (buf == NULL)
      break;

    //Send message
    if(sendto(fd, buffer, strlen(buffer), 0, (struct sockaddr *)
               &serv, sizeof(serv)) < 0)
    {
      perror("internal error");
      return NULL;
    }

  }
  return NULL;
}

void* recv_udp(void *thr_str)
{
  char buffer[MAXLINE];
  struct thr_str pair = *(struct thr_str*)thr_str;
  int fd = pair.fd;
  struct sockaddr_in serv = pair.serv;
  int len;
  while (1) {
    // receive from client
    if(recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&serv,(socklen_t *) &len)< 0)
    {
      perror("internal error");
      exit(1);
    }
    //write onto the screen
    write(1, buffer, strlen(buffer));
  }
  return NULL;
}

int client_udp(char *host,int port)
{
  /* Establishes a connection using UDP
  */
  //struct params par;
  struct sockaddr_in serv;
  int listenfd, len;
  struct thr_str pair;
  pthread_t thread_id1,thread_id2;

  memset(&serv, 0, sizeof(serv));

  if ((listenfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("internal error");
    exit(1);
  }
  printf("establishing connection ...\n" );
  //Send the message back to client
  serv.sin_family = AF_INET;
  serv.sin_addr.s_addr = inet_addr(host);
  serv.sin_port = htons(port);
  len = sizeof(serv);
  pair.fd = listenfd;
  pair.serv = serv;
  //Sending & Receiving Data
  if (pthread_create(&(thread_id1), NULL, &send_udp, (void*) &pair)<0)
      printf("Can't connect :send");
  if (pthread_create(&(thread_id2), NULL, &recv_udp, (void*) &pair)<0)
      printf("Can't connect :Recv");

  return 0;
}

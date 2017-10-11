#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define MAXLINE 4096


int server_listen(int port)
/* Establishes the listening descriptor on the server side
  Parameters:
    port (char *): ready to receive connection requests
  Result:
    listenfd (int): the listening descriptor that works
*/
{
  struct sockaddr_in server;

  int listenfd ;
  const int ov =1;
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if(listenfd<0){
    perror("internal error");
    exit(1);
    ///continue ;
  }
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &ov, sizeof(int));
  if ((bind(listenfd, (struct sockaddr *) &server,sizeof(server))) < 0){
  // Determines if socket descriptor can be asscoiated with the server's socket address
    perror("internal error");
    exit(1);
  }

  if (listen(listenfd,10) < 0)
  {
    perror("internal error");
    exit(1);
  }

  return listenfd ;
}

int server_connect(int listenfd)
/* Establishes a connection by using accept()
  Parameters:
    listenfd (int): The listening descriptor of a server
*/
{
  struct sockaddr_in result_addr ; // client's socket address
  int connectfd;
  int result_len;
  int n;
  char *buffer=NULL;
  char client_hostname[MAXLINE], client_port[MAXLINE];

  buffer = malloc(500);
	//Receive a message from client
	while(1)
	{

    result_len = sizeof(struct sockaddr_in) ;

    connectfd = accept(listenfd, (struct sockaddr *)&result_addr, (socklen_t*)&result_len);
    printf("%d\n", connectfd);
    if (connectfd < 0){
      perror("internal error");
      return 1;
    }
    getnameinfo((struct sockaddr *) (socklen_t*)&result_addr, result_len, client_hostname, MAXLINE,
    client_port, MAXLINE, 0);
    printf("Connected to (%s, %s)\n", client_hostname, client_port);
		//Send the message back to client
    while(1)
    {
    n = recv(connectfd, buffer , MAXLINE , 0);
    if (!n) break; // done reading
    if (n< 0) {
      perror("internal error");
      exit(1);
    }
    write(1, buffer, strlen(buffer));

		//printf("%s", buffer)
	 }
   close(connectfd);
 }
 close(listenfd);
  exit(0);


}



int udp_server(int port)
{
  struct sockaddr_in server,client;
  int listenfd,n;
  char buffer[500];

  memset(&server, 0, sizeof(server));

  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  listenfd = socket(PF_INET, SOCK_DGRAM,0);
  if(listenfd < 0){
    perror("internal error");
    exit(1);
  }
  if ((n= bind(listenfd, (struct sockaddr *) &server,sizeof(server))) < 0){
  // Determines if socket descriptor can be asscoiated with the server's socket address
    perror("internal error");
    exit(1);
  }
  int client_len;
  while (1) {
      // receive from client
  		n = recvfrom(listenfd, buffer, sizeof(buffer), 0,
  		                   (struct sockaddr *)&client,
  		                   (socklen_t*)&client_len);
      if (n < 0){
        perror("internal error");
        exit(1);
      }
      puts("Connection Made");
      //write onto the screen
      write(1, buffer, strlen(buffer));
  	}

  close(listenfd);
  return 1;
}

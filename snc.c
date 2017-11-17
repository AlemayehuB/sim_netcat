#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "server.h"
#include "client.h"

int main(int argc, char *argv[])
{
  int listenfd = 0;
  if (argc > 5){
    perror("invalid or missing options");
    return -1;
  }
  if(atoi(argv[argc -1]) ==0){
    perror("invalid or missing options");
    return -1;
  }


  if (strcmp(argv[1], "-l") == 0){

    if (strcmp(argv[2], "-u") == 0)
      udp_server(atoi(argv[argc -1]));
    else{
      listenfd = server_listen(atoi(argv[argc -1]));
      server_connect(listenfd);
    }
  }
  else{
    if ((argc < 3) || (argc > 4)){
      perror("invalid or missing options");
      return -1;
    }
    else{
      if(strcmp(argv[1], "-u") == 0){
        if (argv[3] == NULL){
          perror("invalid or missing options");
          return -1;
        }
        else{
        client_udp(argv[2],atoi(argv[3]));
      }
      }
      else{
        listenfd = client_setup(argv[argc-2],argv[argc-1]);
        client_connect(listenfd);
      }
    }
  }
  printf("Connection Terminated\n");
  return 1;
}

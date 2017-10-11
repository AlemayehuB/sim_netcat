#ifndef CLIENT_H
#define CLIENT_H

int client_setup(char *host, char *port);
/* Starts the process of establishing a
conneciton with server using TCP*/
int client_connect(int listenfd);
/*  Establishes a connection with the server
is able to send data
Uses TCP */
int udp_client(char *host, int port);
/*  Establishes a connection with the server
is able to send data
Uses UDP */
#endif

#ifndef SERVER_H
#define SERVER_H

int server_listen(int port);
/* Creates a socket descriptor
Uses various socket functions and
binds the socket on the server side
returning a listen descriptor using TCP*/

int server_connect(int listenfd);
/* Establishes a connection using the
listening descriptor and stays connected
using Ctrl+D
Uses TCP*/

int udp_server(int port);
/* Creates a socket descriptor
Uses various socket functions and
binds the socket on the server side
Uses UDP*/
#endif

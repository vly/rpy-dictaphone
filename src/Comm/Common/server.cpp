#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

Server::Server()
{
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0, sizeof(struct sockaddr_in));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(9999);

	bind(server_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        listen(server_socket, 10);   
}

Server::~Server()
{

}

void Server::startServer()
{
	client_socket = accept(server_socket, (struct sockaddr*)NULL, NULL);
}

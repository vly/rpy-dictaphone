#include "client.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define HOSTNAME "127.0.0.1"

Client::Client()
	: isRunning(true)
{

}

Client::~Client()
{

}

void Client::connectToServer()
{
	if(connect(client_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        {   
                printf("\n Error: Connect Failed\n");
                exit(1);
        } 
}
	
bool Client::setUpService()
{
	// Communication
        memset(&serv_addr, 0, sizeof(struct sockaddr_in));
        if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {   
                printf("\n Error: Could not create socket \n");
        }   

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(9999);

        if(inet_pton(AF_INET, HOSTNAME, &serv_addr.sin_addr) <= 0)
        {   
                printf("\n Error: Connect Failed \n");
                exit(1);
	}

	connectToServer();
	createThreads();

	// TODO: Voice Recode
}

void Client::startMainLoop()
{
	while(isRunning)
	{
		
	}
}

void Client::stopMainLoop()
{
	isRunning = false;
}

void Client::receiveMessage(const Message& m)
{

}

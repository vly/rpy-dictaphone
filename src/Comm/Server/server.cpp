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

#include "face_detect.h"
#include "pir_monitor.h"
#include "maestro.h"

Server::Server()
	: isRunning(true)
{

}

Server::~Server()
{

}

void Server::startPirDetect()
{
	pir_monitor::GetInstance()->startMainLoop();
}

void Server::startServer()
{
	client_socket = accept(server_socket, (struct sockaddr*)NULL, NULL);
}
	
bool Server::setUpService()
{
	// Commnication
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0, sizeof(struct sockaddr_in));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(9999);

	bind(server_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        listen(server_socket, 10);   
		
	startServer();
	createThreads();

	// TODO: pir ...
	pir_monitor::GetInstance()->setUpService(); 

	// face_detection
	face_detect::GetInstance()->setUpService();
	//face_detect::GetInstance()->startMainLoop();

	// maestro
	Maestro::GetInstance()->setUpService();

	return true;
}

void Server::startMainLoop()
{
	while(isRunning)
	{
		startPirDetect();
		usleep(10);
	}
}

void Server::stopMainLoop()
{
	isRunning = false;
}

void Server::receiveMessage(const Message& m)
{
	// TODO: Handle messages ...
}
	
bool Server::dispatchMessage(Message* m)
{
	bool bHandle = false;
	if(m->_impl->head->mh_handle_proxy == proxy_server)
	{
		// TODO:... dispatch
		bHandle = true;
	}
	else 
		this->sendMessage(m);

	return bHandle;
}

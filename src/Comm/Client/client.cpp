#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char** argv)
{
	int sockfd = 0, n = 0;
	char recvBuff[BUFFER_SIZE];
	struct sockaddr_in serv_addr;

	const char* hostName = "127.0.0.1";
	
	memset(recvBuff, 0, BUFFER_SIZE);
	memset(&serv_addr, 0, sizeof(struct sockaddr_in));
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Error: Could not create socket \n");
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(9999);

	if(inet_pton(AF_INET, hostName, &serv_addr.sin_addr) <= 0)
	{
		printf("\n Error: Connect Failed \n");
		return EXIT_FAILURE;
	}

	if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\n Error: Connect Failed\n");
		return EXIT_FAILURE;
	}
	
	while((n = read(sockfd, recvBuff, sizeof(recvBuff) - 1)) > 0)
	{
		recvBuff[n] = 0;
		if(fputs(recvBuff, stdout) == EOF)
		{
			printf("\n Error: Fputs error\n");
			return EXIT_FAILURE;
		}
	}

	if(n < 0)
	{
		printf("\n Read error \n");
	}
	
	return 0;
}

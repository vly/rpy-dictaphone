#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int arc, char** argv)
{
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;

	char sendBuff[BUFFER_SIZE];

	time_t ticks;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(struct sockaddr_in));
	memset(sendBuff, 0, BUFFER_SIZE);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(9999);

	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	listen(listenfd, 10);

	while(1)
	{
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

		ticks = time(NULL);
		snprintf(sendBuff, sizeof(sendBuff), ".%24s\r\n", ctime(&ticks));
		write(connfd, sendBuff, strlen(sendBuff));
		
		close(connfd);
		sleep(1);
	}
}

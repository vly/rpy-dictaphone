#define LISTEN_THREAD 0
#define WRITE_THREAD  1
#define READ_THREAD   2
#define THREAD_NUM    3

#include <pthread.h>
#include "message_query.h"
#include "message.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

class Comm 
{
protected:
	struct sockaddr_in serv_addr;
	int server_socket;
	int client_socket;

	pthread_t threads[THREAD_NUM];
	int thread_ids[THREAD_NUM];
	bool thread_running[THREAD_NUM];
public:
	MessageQuery* query;
public:
	Comm();
	~Comm();

	void createThreads();
	bool isThreadRunning(int thread_index);

	int sendMessage(Message* m);
	Message* readMessage();
	virtual void dispatchMessage(Message* m) = 0;
};

void* writeThread(void* arg);
void* readThread(void* arg);

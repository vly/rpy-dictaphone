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

#include <iostream>
using std::cout;	using std::endl;

#include "comm.h"

#ifdef TEST
#undef TEST
#define TEST 0
#endif

Comm::Comm()
{
	query = MessageQuery::GetInstance();
	memset(thread_running, 1, THREAD_NUM);
}

Comm::~Comm()
{

}

void Comm::createThreads()
{
	thread_ids[WRITE_THREAD] = pthread_create(&threads[WRITE_THREAD], NULL, writeThread, this);
		
	thread_ids[READ_THREAD] = pthread_create(&threads[READ_THREAD], NULL, readThread, this);

//	pthread_join(threads[WRITE_THREAD], NULL);
//	pthread_join(threads[READ_THREAD], NULL);
}
	
bool Comm::isThreadRunning(int thread_index)
{
	return thread_running[thread_index];
}
	
int  Comm::sendMessage(Message* m)
{
	void* p = m->messageData();
	write(client_socket,  p, m->messageLength());
	free(p);
	return 0;
}

Message* Comm::readMessage()
{
	char buffer[BUFFER_SIZE] = {0};
	int count = 0;
	count = read(client_socket, buffer, sizeof(buffer) - 1);
	buffer[count] = 0;
	Message* m = Message::createMessage(buffer);
	//query->pushMessage(m);
	return m;
}

void* writeThread(void* arg)
{
	printf("write thread start\n");
	
	Comm* s = (Comm*)arg;
	char buffer[BUFFER_SIZE];
	while(s->isThreadRunning(WRITE_THREAD))
	{
		//printf("write thread running\n");
		//Message m = s->query.popMessage();
		memset(buffer, 0, BUFFER_SIZE);
#if 0	
		int id = 0;	

		char data[BUFFER_SIZE] = {0};
		memset(data, 0, BUFFER_SIZE);
		scanf("%s", data);

		Message* m = new Message();
		m->initMessage(id, data);
#endif
		
		Message* m = MessageQuery::GetInstance()->popMessage();
		//s->query.pushMessage(m);
		s->sendMessage(m);
		delete m;
	}
	return NULL;
}

void* readThread(void* arg)
{
	printf("read thread start\n");

	Comm* s = (Comm*)arg;
	while(s->isThreadRunning(READ_THREAD))
	{
		//printf("read thread running\n");
		//Message m = s->query.popMessage();
		Message* m = s->readMessage();
		printf("%s\n", m->messageBodyData());
		if(s->dispatchMessage(m))
			delete(m);
	}
	return NULL;
}

#if TEST
int main(int arc, char** argv)
{
	Comm* s = new Comm();;
	s->createThreads();
	delete s;
	cout << "yangyuan main Thread" << endl;
}
#endif

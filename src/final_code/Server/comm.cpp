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
using std::cout;        using std::endl;

#include "comm.h"

Comm::Comm()
{
  query = MessageQuery::GetInstance();
  memset(thread_running, 1, THREAD_NUM);
}

Comm::~Comm()
{}

void Comm::createThreads()
{
  thread_ids[WRITE_THREAD] = pthread_create(&threads[WRITE_THREAD],
                                            NULL,
                                            writeThread,
                                            this);

  thread_ids[READ_THREAD] = pthread_create(&threads[READ_THREAD],
                                           NULL,
                                           readThread,
                                           this);
}

bool Comm::isThreadRunning(int thread_index)
{
  return thread_running[thread_index];
}

int Comm::sendMessage(Message *m)
{
  void *p = m->messageData();

  write(client_socket, p, m->messageLength());
  free(p);
  return 0;
}

Message * Comm::readMessage()
{
  char buffer[BUFFER_SIZE] = { 0 };
  int  count               = 0;

  count         = read(client_socket, buffer, sizeof(buffer) - 1);
  buffer[count] = 0;
  Message *m = Message::createMessage(buffer);
  return m;
}

void* writeThread(void *arg)
{
  printf("write thread start\n");

  Comm *s = (Comm *)arg;
  char  buffer[BUFFER_SIZE];

  while (s->isThreadRunning(WRITE_THREAD))
  {
    memset(buffer, 0, BUFFER_SIZE);

    Message *m = MessageQuery::GetInstance()->popMessage();
    s->sendMessage(m);
    delete m;
  }
  return NULL;
}

void* readThread(void *arg)
{
  printf("read thread start\n");

  Comm *s = (Comm *)arg;

  while (s->isThreadRunning(READ_THREAD))
  {
    Message *m = s->readMessage();

    printf("%s\n", m->messageBodyData());

    if (s->dispatchMessage(m)) delete m;
  }
  return NULL;
}

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
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define HOSTNAME "192.168.1.126"

Client::Client()
  : isRecording(false)
    , isRunning(true)
{}

Client::~Client()
{
  printf("shut down recording");
  sendCommand("shutdown\r");
}

void Client::connectToServer()
{
CONNECTION:
	if(connect(client_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        {   
                printf("\n Error: Connect Failed\n");
		goto CONNECTION;
                exit(1);
        } 
}

void* inputFunction(void *arg)
{
  Client *client = (Client *)arg;

  char c;

  scanf("%c", &c);

  if (c == 27) client->isRunning = false;

  return NULL;
}

bool Client::setUpService()
{
  memset(&serv_addr, 0, sizeof(struct sockaddr_in));

  if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("\n Error: Could not create socket \n");
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port   = htons(9999);

  if (inet_pton(AF_INET, HOSTNAME, &serv_addr.sin_addr) <= 0)
  {
    printf("\n Error: Connect Failed \n");
    exit(1);
  }

  connectToServer();
  createThreads();


  pthread_create(&input_thread, NULL, inputFunction, (void *)this);

  // TODO: Voice Recode
  return true;
}

void Client::startMainLoop()
{
  while (isRunning)
  {}
}

void Client::stopMainLoop()
{
  isRunning = false;
}

void Client::receiveMessage(const Message& m)
{}

bool Client::dispatchMessage(Message *m)
{
  bool bHandle = false;

  if (m->_impl->head->mh_handle_proxy == proxy_client)
  {
    printf("client dispatching message");

    if (m->_impl->head->mh_id == M_BEGIN_AUDIO_RECORDING) startAudioRecording();

    else if (m->_impl->head->mh_id == M_END_AUDIO_RECORDING) stopAudioRecording();


    bHandle = true;
  }
  else this->sendMessage(m);

  return bHandle;
}

#define FIFO_NAME "/tmp/osp.fifo"

int Client::sendCommand(char *cmd) {
  int fd = open(FIFO_NAME, O_WRONLY);

  if (write(fd, cmd, strlen(cmd)) == -1) {
    perror(":(");
    return -1;
  }

  close(fd);

  return 1;
}

void Client::startAudioRecording()
{
  printf("start Audio Recording\n");

  if (!isRecording)
  {
    sendCommand("start\r");
    isRecording = true;
  }
}

void Client::stopAudioRecording()
{
  printf("start Audio Recording\n");

  if (isRecording)
  {
    sendCommand("stop\r");
    isRecording = false;
  }
}

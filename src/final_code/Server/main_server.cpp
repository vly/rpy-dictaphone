#include "server.h"

int main(int argc, char **argv)
{
  Server *s = Server::GetInstance();

  s->setUpService();
  s->startMainLoop();

  return 0;
}

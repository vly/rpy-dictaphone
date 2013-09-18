#include <iostream>
#include "server.h"

int main(int argc, char** argv)
{
       	Server* s = new Server();
	s->startServer();
        s->createThreads();
        delete s;	

	return 0;
}

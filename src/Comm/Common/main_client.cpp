#include <iostream>
#include "client.h"

int main(int argc, char** argv)
{
	Client* c = new Client();
	c->connectToServer();
	c->createThreads();
	delete c;
	
	return 0;
}

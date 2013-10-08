/**
 * Entery of client
 */

#include "client.h"

int main(int argc, char** argv)
{
	Client* c = new Client();
	c->setUpService();
	c->startMainLoop();
	delete c;
	
	return 0;
}

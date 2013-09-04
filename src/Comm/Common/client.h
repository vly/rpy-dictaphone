/**
 * Client 
 * Created by Alfred yang
 * 04-09-2013
 *
 */


#include "comm.h"

class Client : public Comm
{
public:
	Client();
	~Client();

	void connectToServer();
};

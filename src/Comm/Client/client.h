/**
 * Client 
 * Created by Alfred yang
 * 04-09-2013
 *
 */


#include "comm.h"
#include "singleton.h"
#include "base_service.h"

class Client : public Comm
	     , public Singleton<Client>
	     , public base_service
{
	bool isRunning;

public:
	Client();
	~Client();

	virtual bool setUpService();
	virtual void startMainLoop();
	virtual void stopMainLoop();
	virtual void receiveMessage(const Message& m);

private:
	void connectToServer();
};

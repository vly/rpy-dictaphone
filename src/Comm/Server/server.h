/**
 * Server
 * Created by Alfred Yang
 * 04-09-2013
 */

#include "comm.h"
#include "singleton.h"
#include "base_service.h"

class Server : public Comm
	     , public Singleton<Server>
	     , public base_service
{
	bool isRunning;

public:
	Server();
	~Server();

	virtual bool setUpService();
	virtual void startMainLoop();
	virtual void stopMainLoop();
	virtual void receiveMessage(const Message& m);
	virtual bool dispatchMessage(Message* m);
	
private:
	void startPirDetect();
	void startServer();
};

/**
 * The basic class for all the sub system
 * Created by Alfred Yang
 * 13-09-2013
 */

#ifndef __BASE_SERVICE_H__
#define __BASE_SERVICE_H__

#include "message.h"
#include "message_query.h"

class base_service
{
public:
	virtual bool setUpService() = 0;
	virtual void startMainLoop() = 0;
	virtual void stopMainLoop() = 0;
	
	virtual void postMessage(Message* m)
	{
		MessageQuery::GetInstance()->pushMessage(m);
	}

	virtual void receiveMessage(const Message& m) = 0;
};

#endif //__BASE_SERVICE_H__

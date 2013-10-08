/**
 * Message Query
 * Create by Alfred Yang
 * 03-09-2013
 */

#ifndef __MESSAGE_QUERY_H__
#define __MESSAGE_QUERY_H__

#include <list>
#include <pthread.h>
#include "singleton.h"
using std::list;

class Message;

class MessageQuery : public Singleton<MessageQuery>
{
	list<Message*> _query;

	pthread_mutex_t _mutex;
	pthread_cond_t _con;
public:
	MessageQuery();
	~MessageQuery();

	void pushMessage(Message* m);
	Message* popMessage();
};

#endif //__MESSAGE_QUERY_H__

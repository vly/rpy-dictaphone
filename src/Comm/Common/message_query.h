/**
 * Message Query
 * Create by Alfred Yang
 * 03-09-2013
 */

#include <list>
#include <pthread.h>
using std::list;

class Message;

class MessageQuery
{
	list<Message> _query;

	pthread_mutex_t _mutex;
	pthread_cond_t _con;
public:
	MessageQuery();
	~MessageQuery();

	void pushMessage(const Message& m);
	Message popMessage();
#if 0	
	bool isEmpty() const;
	void lock();
	void unlock();
#endif
};


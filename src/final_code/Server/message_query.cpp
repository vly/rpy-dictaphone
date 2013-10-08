#include "message_query.h"
#include "message.h"

MessageQuery::MessageQuery()
{
  pthread_mutex_init(&_mutex, NULL);
  pthread_cond_init(&_con, NULL);
}

MessageQuery::~MessageQuery()
{
  pthread_mutex_destroy(&_mutex);
  pthread_cond_destroy(&_con);
}

void MessageQuery::pushMessage(Message *m)
{
  pthread_mutex_lock(&_mutex);
  _query.push_back(m);
  pthread_cond_signal(&_con);
  pthread_mutex_unlock(&_mutex);
}

Message * MessageQuery::popMessage()
{
  pthread_mutex_lock(&_mutex);

  while (_query.empty()) pthread_cond_wait(&_con, &_mutex);

  Message *reVal = _query.front();
  _query.pop_front();
  pthread_mutex_unlock(&_mutex);
  return reVal;
}

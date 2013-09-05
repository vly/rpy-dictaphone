#include "message.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if TEST
#include <string>
#include <iostream>
using std::string;	using std::cout;
using std::endl;
#endif


Message::Message()
{
	_impl = new message();
	_impl->head = new message_head();
	_impl->body = (message_body*)malloc(sizeof(message_body));
}

Message::Message(const Message& other)
{
	_impl = new message();
	_impl->head = new message_head();
	_impl->body = (message_body*)malloc(sizeof(message_body));
	
	memcpy(_impl, other._impl, sizeof(message));
}

Message& Message::operator=(const Message& rhs)
{
	if(&rhs != this)
	{
		_impl = new message();
		_impl->head = new message_head();
		_impl->body = (message_body*)malloc(sizeof(message_body));
	
		memcpy(_impl, rhs._impl, sizeof(message));
	}

	return *this;
}

Message Message::createMessage(void* data)
{
	Message m;

	int* p = (int*)data;

	m._impl->head->mh_id = *(p + 0);
	m._impl->head->mh_size = *(p + 1);
	m._impl->head->mh_total_package = *(p + 2);
	m._impl->head->mh_pack_num = *(p + 3);
	char* c = (char*)((p + 4) + 1);
	int len = strlen(c);
	m._impl->body = (message_body*)malloc(sizeof(char) * (len + 1) + sizeof(int));
	m._impl->body->mb_len = *(p + 4);	
	memcpy(m._impl->body->mb_data, c, len);
	
	return m;	
}

void Message::initMessage(int id, string message_data)
{
	_impl->head->mh_id = id;

	// from this project, the message lenth shall never over 1024
	// that is to say, one package for one message
	_impl->head->mh_total_package = 1;
	_impl->head->mh_pack_num = 0;

	_impl->body = (message_body*)malloc(sizeof(int) + 
			sizeof(char) * (message_data.size() + 1));
	_impl->body->mb_len = message_data.size();
	memcpy(_impl->body->mb_data, message_data.c_str(), message_data.size());
	*(_impl->body->mb_data + message_data.size()) = 0;

	_impl->head->mh_size = sizeof(message_head) + sizeof(int)
		+ sizeof(char) * (message_data.size() + 1);
}

int Message::messageLength() const
{
	return _impl->head->mh_size;
}

void* Message::messageData() const
{
	void* p = malloc(messageLength());
	memset(p, 0, messageLength());
	memcpy(p, _impl->head, sizeof(message_head));
	int* pi = ((int*)p + 4);
	*pi = _impl->body->mb_len;
	char* c = (char*)(pi + 1);
	memcpy(c, _impl->body->mb_data, _impl->body->mb_len);
	c[_impl->body->mb_len] = 0;
	return p;
}

char* Message::messageBodyData() const
{
	return _impl->body->mb_data;
}

#if TEST
int main(int argc, char** argv)
{
	Message m;
	m.initMessage(M_START, "Alfred yang");

	cout << m.messageLength() << endl;
	cout << "Message ID: " << m._impl->head->mh_id << endl
	     << "Message Size: " << m._impl->head->mh_size << endl
	     << "Message Data: " << m._impl->body->mb_data << endl;

	Message m1 = m;
	cout << m1.messageLength() << endl;
	cout << "Message ID: " << m1._impl->head->mh_id << endl
	     << "Message Size: " << m1._impl->head->mh_size << endl
	     << "Message Data: " << m1._impl->body->mb_data << endl;

	Message m2 = Message::createMessage(m.messageData());
	cout << m2.messageLength() << endl;
	cout << "Message ID: " << m2._impl->head->mh_id << endl
	     << "Message Size: " << m2._impl->head->mh_size << endl
	     << "Message Data: " << m1._impl->body->mb_data << endl;
	
	return 0;
}
#endif

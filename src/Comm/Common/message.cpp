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

void Message::initMessage(int id, string message_data)
{
	_impl->head->mh_id = id;

	// from this project, the message lenth shall never over 1024
	// that is to say, one package for one message
	_impl->head->mh_total_package = 1;
	_impl->head->mh_pack_num = 0;

	_impl->body->mb_data = (char*)malloc(
		sizeof(char) * (message_data.size() + 1));
	_impl->body->mb_len = message_data.size();
	memcpy(_impl->body->mb_data, message_data.c_str(), message_data.size());
	*(_impl->body->mb_data + message_data.size()) = 0;

	_impl->head->mh_size = sizeof(message_head) + sizeof(message_body)
		+ sizeof(char) * (message_data.size() + 1);
}

int Message::messageLength() const
{
	return _impl->head->mh_size;
}

void* Message::messageData() const
{
	return (void*)(_impl->head);
}

#if TEST
int main(int argc, char** argv)
{
	Message m;
	m.initMessage(M_START, "Alfred yang");

	cout << m.messageLength() << endl;
//	cout << (char*)(m.messageData()) << endl;

	cout << "Message ID: " << m._impl->head->mh_id << endl
	     << "Message Size: " << m._impl->head->mh_size << endl
	     << "Message Data: " << m._impl->body->mb_data << endl;
	
	return 0;
}
#endif

#include "message.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

Message::~Message()
{
	delete _impl->head;
	delete _impl->body;
	delete _impl;
}

Message* Message::createMessage(void* data)
{
	Message* m = new Message();

	int* p = (int*)data;

	m->_impl->head->mh_id = *(p + 0);
	m->_impl->head->mh_size = *(p + 1);
	m->_impl->head->mh_handle_proxy = *(p + 2);
	m->_impl->head->mh_reserve = *(p + 3);
	char* c = (char*)((p + 4) + 1);
	int len = strlen(c);
	m->_impl->body = (message_body*)malloc(sizeof(char) * (len + 1) + sizeof(int));
	m->_impl->body->mb_len = *(p + 4);	
	memcpy(m->_impl->body->mb_data, c, len);
	m->_impl->body->mb_data[len] = 0;
	
	return m;	
}

void Message::initMessage(int id, string message_data)
{
	_impl->head->mh_id = id;

	// from this project, the message lenth shall never over 1024
	// that is to say, one package for one message
	_impl->head->mh_handle_proxy = getMessageHandleProxy(id);
	_impl->head->mh_reserve = 0;

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
	
int  Message::getMessageHandleProxy(int id)
{
	int reVal = -1;
	switch (id)
	{
		case M_START: 
		case M_END: 
		case M_DETECTED: 
		case M_UNDETECTED: 
		case M_PIR_DETECTED: 
		case M_PIR_UN_DETECTED: 
			reVal = proxy_server;
			break;
		case M_BEGIN_AUDIO_RECORDING: 
		case M_END_AUDIO_RECORDING: 
			reVal = proxy_client;
			break;
	}
	return reVal;
}


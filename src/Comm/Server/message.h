/**
 * Define Communication protocol
 * Create by Alfred Yang
 * 03-09-2013
 */
#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <string>
using std::string;

#define TEST 0

enum message_id
{
	// face detect, base from 0
	M_START = 0,		// Start Vedio, open the comera
	M_END,			// End Vedio, stop the comera
	M_DETECTED,
	M_UNDETECTED,

	// other function, shall have different base	
	M_PIR_DETECTED = 100, 
	M_PIR_UN_DETECTED,

	// other function, 
	M_BEGIN_AUDIO_RECORDING = 200,
	M_END_AUDIO_RECORDING,
};

enum message_handle_proxy
{
	proxy_server = 0,
	proxy_client,
};

struct message_head
{
	int mh_id;
	int mh_size;
	int mh_handle_proxy;
	int mh_reserve;
};

struct message_body
{
	int mb_len;
	char mb_data[1];
};

struct message
{
	message_head* head;
	message_body* body;
};

class Server;

class Message
{
	friend class Server;
#if TEST
public:
#endif
	message* _impl;	

public:
	Message();
	Message(const Message& other);
	Message& operator=(const Message& rhs);
	Message(void* data, int size);
	~Message();

public:
	void  initMessage(int id, string message_data);
	int   messageLength() const;
	void* messageData() const;
	char* messageBodyData() const;
	int   getMessageHandleProxy(int id);

	static Message* createMessage(void* data);
};

#endif //__MESSAGE_H__

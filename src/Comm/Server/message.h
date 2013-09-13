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
};

struct message_head
{
	int mh_id;
	int mh_size;
	int mh_total_package;
	int mh_pack_num;
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


class Message
{
#if TEST
public:
#endif
	message* _impl;	

public:
	Message();
	Message(const Message& other);
	Message& operator=(const Message& rhs);
	Message(void* data, int size);

public:
	void  initMessage(int id, string message_data);
	int   messageLength() const;
	void* messageData() const;
	char* messageBodyData() const;
	
	static Message createMessage(void* data);
};

#endif //__MESSAGE_H__

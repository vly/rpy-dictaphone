/**
 * Pir Monitor Control
 * Based on Neil's Code
 * This is the main thread to create/end face_detecting thread
 * Created on 24-09-2013
 */

#ifndef __PIR_MONITOR_H__
#define __PIR_MONITOR_H__

#include "singleton.h"
#include "base_service.h"
#include <time.h>

class pir_monitor 
	: public Singleton<pir_monitor>
	, public base_service
{
	bool isRunning;
	time_t last_sensor;
	
public:
	pir_monitor();
	~pir_monitor(); 

	virtual bool setUpService();
	virtual void startMainLoop();
	virtual void stopMainLoop();
	virtual void receiveMessage(const Message& m);
};

#endif //__PIR_MONITOR_H__

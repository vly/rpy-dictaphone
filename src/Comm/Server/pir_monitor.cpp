/**
 * Pir Monitor Control
 * Based on Neil's Code
 * This is the main thread to create/end face_detecting thread
 * Created on 24-09-2013
 */

#include "pir_monitor.h"
#include "face_detect.h"
#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define NA_PIN RPI_GPIO_P1_07
#define NA_POLL_DELAY 33
#define NA_PIN_STILL_VALUE 1
	
pir_monitor::pir_monitor()
{

}

pir_monitor::~pir_monitor()
{
	bcm2835_close();
}

bool pir_monitor::setUpService()
{
	bool reVal = true;
#if 1
	if (!bcm2835_init()) {
        	printf("Can't access hardware, did you use sudo?\n");
		exit(EXIT_FAILURE);
		reVal = false;
    	}

	// Setup pin to use
	bcm2835_gpio_fsel(NA_PIN, BCM2835_GPIO_FSEL_INPT);

    	// Use pull up
    	bcm2835_gpio_set_pud(NA_PIN, BCM2835_GPIO_PUD_UP);
#endif
	return reVal;
}

void pir_monitor::stopMainLoop()
{
	isRunning = false;
}

void pir_monitor::startMainLoop()
{
#if 1
	uint8_t value = NA_PIN_STILL_VALUE;

	time(&last_sensor);
	isRunning = true;
    	
	while (isRunning) 
	{
        	// Read pin value
        	value = bcm2835_gpio_lev(NA_PIN);

        	if(value != NA_PIN_STILL_VALUE)
		{
			printf("dectect movement\n");
			face_detect* fd = face_detect::GetInstance();
			if(!fd->isMainThreadRunning())
			{
				printf("begin face detection");
				fd->startMainLoop();
			}

			time(&last_sensor);
#if 1
			// message to start Recording
			char buffer[100];
			memset(buffer, 0, 100);
			sprintf(buffer, "detected: %d\n", (int)last_sensor);
			Message* m = new Message();
			m->initMessage(M_BEGIN_AUDIO_RECORDING, buffer);
			this->postMessage(m);
#endif
        	} 
		else 
		{
			//printf("no detection\n");
			time_t new_sensor;
			time(&new_sensor);
			
			if(difftime(new_sensor, last_sensor) > 30)
			{
				face_detect::GetInstance()->stopMainLoop();
				// message to stop Recording
#if 1
				char buffer[100];
				memset(buffer, 0, 100);
				sprintf(buffer, "detected: %d\n", (int)new_sensor);
				Message* m = new Message();
				m->initMessage(M_END_AUDIO_RECORDING, buffer);
				this->postMessage(m);
#endif
			}
		}

        	// Pause before polling again
        	delay(NA_POLL_DELAY);
	}

#endif 

#if 0

	isRunning = true;
    	uint8_t value = NA_PIN_STILL_VALUE;
	time(&last_sensor);
	while(isRunning)
	{
		//printf("begin pir main loop\n");
		// Read pin value
 	       	value = bcm2835_gpio_lev(NA_PIN);

		// No Movement detection
	        if(value == NA_PIN_STILL_VALUE)	
		{
			//printf("no detection\n");
			time_t new_sensor;
			time(&new_sensor);
			
			if(difftime(new_sensor, last_sensor) > 30)
			{
				face_detect::GetInstance()->stopMainLoop();
				// message to stop Recording
#if 0
				char buffer[100];
				memset(buffer, 0, 100);
				sprintf(buffer, "detected: %d\n", (int)new_sensor);
				Message* m = new Message();
				m->initMessage(M_END_AUDIO_RECORDING, buffer);
				this->postMessage(m);
#endif
			}
		}
		// Movement detection
		else
		{
			//printf("dectect movement\n");
			face_detect* fd = face_detect::GetInstance();
			if(!fd->isMainThreadRunning())
			{
				printf("begin face detection");
				fd->startMainLoop();
			}

			time(&last_sensor);
#if 0
			// message to start Recording
			char buffer[100];
			memset(buffer, 0, 100);
			sprintf(buffer, "detected: %d\n", (int)last_sensor);
			Message* m = new Message();
			m->initMessage(M_BEGIN_AUDIO_RECORDING, buffer);
			this->postMessage(m);
#endif
		}

        	// Pause before polling again
        	delay(NA_POLL_DELAY);	
		//usleep(500);
	}
#endif
}

void pir_monitor::receiveMessage(const Message& m)
{
	// TODO: ...
}

#undef NA_PIN
#undef NA_POLL_DELAY
#undef NA_PIN_STILL_VALUE

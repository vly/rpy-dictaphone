//
//  maestro.h
//  ServoInterface
//
//  Created by Neil Ang on 16/09/13.
//  Copyright (c) 2013 Neil Ang. All rights reserved.
//

/**
 * Add singleton design pattern to the final project
 */

#ifndef __maestro__
#define __maestro__

#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include "singleton.h"
#include "base_service.h"

// http://stackoverflow.com/questions/134569/c-exception-throwing-stdstring
struct MaestroException : public std::exception
{
    std::string s;
    MaestroException(std::string ss) : s(ss) {}
    ~MaestroException() throw () {}
    const char* what() const throw() { return s.c_str(); }
};

#ifndef __servo__
#define __servo__

class Servo {
    
private:
    unsigned char _channel;
    unsigned int  _min;
    unsigned int  _max;
    unsigned short _pos;
    unsigned short _home;
    
public:
    Servo(unsigned char c, unsigned int mn, unsigned int mx, unsigned int hm) :_channel(c), _min(mn), _max(mx), _home(hm) {}
    
    unsigned char getChannel() { return _channel; }
    unsigned short getHome() { return _home; }
    unsigned short getMin() { return _min; }
    unsigned short getMax() { return _max; }
    unsigned short getPos() { return _pos; }
    void           setPos(unsigned short p) { _pos = p; } 
};

#endif /* defined(__servo__) */

#ifdef __APPLE__
#define NA_DEVICE "/dev/cu.usbmodem00065291"
#else
//#define NA_DEVICE "/dev/serial/by-path/platform-bcm2708_usb-usb-0:1.2.2:1.0"
#define NA_DEVICE "/dev/ttyACM0"
#endif

// Maestro
class Maestro 
	: public Singleton<Maestro>
	, public base_service 
{

private:
    const char* _device;
    int         _fd;

public:
        Servo* horizontalServo;
        Servo* verticalServo;
    
public:
   
#if 0 
    Maestro(const char* dev) : _device(dev) {
        _fd = open(_device, O_RDWR | O_NOCTTY);
        if (_fd == -1) {
            perror(_device);
            throw MaestroException("Invalid Device");
        }
    }
#endif
	Maestro(): _device(NA_DEVICE) {}

    ~Maestro() { close(_fd); }
    
    int getError(Servo * servo);
    int goHome(Servo*);
    int getPosition(Servo*);
    int setPosition(Servo*, unsigned short);
    int stepUp(Servo*);
    int stepDown(Servo*);

    //bool isMoving(Servo*);
    
    //void log(std::string);

	virtual bool setUpService();
	virtual void startMainLoop();
	virtual void stopMainLoop();
	virtual void receiveMessage(const Message& m);

private:
	void openDevice();
};

#endif /* defined(__maestro__) */

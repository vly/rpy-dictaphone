//
//  maestro.h
//  ServoInterface
//
//  Created by Neil Ang on 16/09/13.
//  Copyright (c) 2013 Neil Ang. All rights reserved.
//

#ifndef __maestro__
#define __maestro__

#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>


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
    
public:
    Servo(unsigned char c, unsigned int mn, unsigned int mx) :_channel(c), _min(mn), _max(mx){}
    
    unsigned char getChannel() { return _channel; }
    unsigned int  getMin() { return _min; }
    unsigned int  getMax() { return _max; }
    
};

#endif /* defined(__servo__) */


// Maestro
class Maestro {

private:
    const char* _device;
    int         _fd;
    
public:
    
    Maestro(const char* dev) : _device(dev) {
        _fd = open(_device, O_RDWR | O_NOCTTY);
        if (_fd == -1) {
            perror(_device);
            throw MaestroException("Invalid Device");
        }
    }
    ~Maestro() { close(_fd); }
    
    int goHome(Servo*);
    int getPosition(Servo*);
    int setPosition(Servo*, unsigned short);
    int stepUp(Servo*);
    int stepDown(Servo*);

    bool isMoving(Servo*);
    
    void log(std::string);

};

#endif /* defined(__maestro__) */

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
    
    int goHome(unsigned char);
    int getPosition(unsigned char);
    int setPosition(unsigned char, unsigned short);
    bool isMoving(unsigned char);

    void log(std::string);

};

#endif /* defined(__maestro__) */

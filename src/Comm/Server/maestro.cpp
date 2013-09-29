//
//  maestro.cpp
//  ServoInterface
//
//  Created by Neil Ang on 16/09/13.
//  Copyright (c) 2013 Neil Ang. All rights reserved.
//

#include "maestro.h"
#define NA_STEP_VALUE 150

#if 0
int Maestro::goHome(Servo * servo) {
    unsigned char command[] = {0xA2, servo->getChannel()};
    if(write(_fd, command, sizeof(command)) == -1) {
        perror("error writing");
        return -1;
    }
    return 0;
}
#endif

int Maestro::goHome(Servo * servo) {
    unsigned char command[] = {0xA2, servo->getChannel()};
    if(write(_fd, command, sizeof(command)) == -1) {
        perror("error writing");
        return -1;
    }

    servo->setPos(servo->getHome());

    return 0;
}

#if 0
int Maestro::getError(Servo * servo) {
    
    unsigned char command[] = {0x93, servo->getChannel()};
    if(write(_fd, command, sizeof(command)) == -1) {
        perror("error writing");
        return -1;
    }
    
    unsigned char response[2];
    if(read(_fd,response,2) != 2)  {
        perror("error reading");
        return -1;
    }
    
    if(response[0] == 0x00) {
        return response[0] + 256*response[1];
    }
    
    return 0;
}
#endif

#if 0
int Maestro::getPosition(Servo * servo) {
    
    unsigned char command[] = {0x90, servo->getChannel()};
    if(write(_fd, command, sizeof(command)) == -1) {
        perror("error writing");
        return -1;
    }
    
    unsigned char response[2];
    if(read(_fd,response,2) != 2)  {
        perror("error reading");
        return -1;
    }
    
    return response[0] + 256*response[1];
}
#endif

int Maestro::getPosition(Servo * servo) {
    return (int)servo->getPos();
}

int Maestro::setPosition(Servo * servo, unsigned short target) {
    
    if (target > servo->getMax()) { target = servo->getMax(); }
    if (target < servo->getMin()) { target = servo->getMin(); }

    unsigned char command[] = {0x84, servo->getChannel(), static_cast<unsigned char>(target & 0x7F), static_cast<unsigned char>(target >> 7 & 0x7F)};

    if (write(_fd, command, sizeof(command)) == -1) {
        perror("error writing");
        return -1;
    }
	
    servo->setPos(target);    
    return 0;
}

int Maestro::stepUp(Servo* servo) {
    return setPosition(servo, getPosition(servo)+NA_STEP_VALUE);
}

int Maestro::stepDown(Servo* servo) {
    return setPosition(servo, getPosition(servo)-NA_STEP_VALUE);
}

#if 0
bool Maestro::isMoving(Servo * servo){
    
    unsigned char command[] = {0x93, servo->getChannel()};
    if(write(_fd, command, sizeof(command)) == -1) {
        perror("error writing");
        return -1;
    }
    
    unsigned char response[1];
    if(read(_fd, response, 1) != 1)  {
        perror("error reading");
        return -1;
    }

    return response[0] == 0x01;
}

void Maestro::log(std::string s) {
    std::cout << s << std::endl;
}
#endif

#define KEYCONTROL 0

#if KEYCONTROL
#include <termios.h>
#include <unistd.h>
#endif

#if 0
void busyWait(Maestro *maestro, Servo * servo)
{
    int pos = maestro->getPosition(servo);
    while(maestro->isMoving(servo)){
        usleep(100000);
        if(maestro->getPosition(servo) == pos){
            break;
        }   
        else {
            pos = maestro->getPosition(servo);
        }   
    }   
}
#endif 

void Maestro::openDevice()
{
        _fd = open(_device, O_RDWR | O_NOCTTY);
        if (_fd == -1) {
            perror(_device);
            throw MaestroException("Invalid Device");
        }
}

bool Maestro::setUpService()
{
	openDevice();
        horizontalServo = new Servo(0, 3968, 9216, 6104);
        verticalServo = new Servo(1, 3968, 9216, 8220);

#if 0
        this->setPosition(horizontalServo, 3968);
        busyWait(this, horizontalServo);
        this->setPosition(horizontalServo, 9216);
        busyWait(this, horizontalServo);
        this->goHome(horizontalServo);
        busyWait(this, horizontalServo);

        this->setPosition(verticalServo, 6912);
        busyWait(this, verticalServo);
        this->setPosition(verticalServo, 9984);
        busyWait(this, verticalServo);
        this->goHome(verticalServo);
#endif
        this->goHome(horizontalServo);
        this->goHome(verticalServo);
}

/**
 * actually do nothing
 */
void Maestro::startMainLoop()
{

}

/**
 * actually do nothing
 */
void Maestro::stopMainLoop()
{
	
}

void Maestro::receiveMessage(const Message& m)
{

}

#undef NA_STEP_VALUE

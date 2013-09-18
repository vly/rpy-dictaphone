//
//  maestro.cpp
//  ServoInterface
//
//  Created by Neil Ang on 16/09/13.
//  Copyright (c) 2013 Neil Ang. All rights reserved.
//

#include "maestro.h"
#define NA_STEP_VALUE 200

int Maestro::goHome(Servo * servo) {
    unsigned char command[] = {0xA2, servo->getChannel()};
    if(write(_fd, command, sizeof(command)) == -1) {
        perror("error writing");
        return -1;
    }
    return 0;
}

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

int Maestro::setPosition(Servo * servo, unsigned short target) {
    
    if (target > servo->getMax()) { target = servo->getMax(); }
    if (target < servo->getMin()) { target = servo->getMin(); }

    unsigned char command[] = {0x84, servo->getChannel(), static_cast<unsigned char>(target & 0x7F), static_cast<unsigned char>(target >> 7 & 0x7F)};

    if (write(_fd, command, sizeof(command)) == -1) {
        perror("error writing");
        return -1;
    }
    
    return 0;
}

int Maestro::stepUp(Servo* servo) {
    return setPosition(servo, getPosition(servo)+NA_STEP_VALUE);
}

int Maestro::stepDown(Servo* servo) {
    return setPosition(servo, getPosition(servo)-NA_STEP_VALUE);
}


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

#undef NA_STEP_VALUE

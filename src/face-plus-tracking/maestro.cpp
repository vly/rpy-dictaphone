//
//  maestro.cpp
//  ServoInterface
//
//  Created by Neil Ang on 16/09/13.
//  Copyright (c) 2013 Neil Ang. All rights reserved.
//

#include "maestro.h"
#define NA_STEP_VALUE 50

int Maestro::goHome(Servo * servo) {
    unsigned char command[] = {0xA2, servo->getChannel()};
    if(write(_fd, command, sizeof(command)) == -1) {
        perror("error writing");
        return -1;
    }

    servo->setPos(servo->getHome());

    return 0;
}

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



#undef NA_STEP_VALUE

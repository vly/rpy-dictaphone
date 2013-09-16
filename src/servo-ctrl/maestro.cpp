//
//  maestro.cpp
//  ServoInterface
//
//  Created by Neil Ang on 16/09/13.
//  Copyright (c) 2013 Neil Ang. All rights reserved.
//

#include "maestro.h"

int Maestro::goHome(unsigned char channel) {
    unsigned char command[] = {0xA2, channel};
    if(write(_fd, command, sizeof(command)) == -1) {
        perror("error writing");
        return -1;
    }
    return 0;
}

int Maestro::getPosition(unsigned char channel) {
    
    unsigned char command[] = {0x90, channel};
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

int Maestro::setPosition(unsigned char channel, unsigned short target) {
    
    unsigned char command[] = {0x84, channel, static_cast<unsigned char>(target & 0x7F), static_cast<unsigned char>(target >> 7 & 0x7F)};

    if (write(_fd, command, sizeof(command)) == -1) {
        perror("error writing");
        return -1;
    }
    
    return 0;
}

bool Maestro::isMoving(unsigned char channel){
    
    unsigned char command[] = {0x93, channel};
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

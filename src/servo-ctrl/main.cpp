//
//  main.cpp
//  ServoInterface
//
//  Created by Neil Ang on 16/09/13.
//  Copyright (c) 2013 Neil Ang. All rights reserved.
//

#include <iostream>
#include "maestro.h"
#include <time.h>

#ifdef __APPLE__
#define NA_DEVICE "/dev/cu.usbmodem00065291"
#else
#define NA_DEVICE "/dev/serial/by-path/platform-bcm2708_usb-usb-0:1.2.2:1.0"
#endif

using namespace std;

void busyWait(Maestro *maestro, unsigned char channel){
    while(maestro->isMoving(channel)){
        usleep(100000);
        cout << "now at " << maestro->getPosition(channel) << endl;
    }
}

int main(int argc, const char * argv[])
{

    try {
        Maestro maestro(NA_DEVICE);
        
        maestro.setPosition(0, 3968);
        busyWait(&maestro, 0);
        maestro.setPosition(0, 9216);
        busyWait(&maestro, 0);
        maestro.goHome(0);
        busyWait(&maestro, 1);

        maestro.setPosition(1, 6912);
        busyWait(&maestro, 1);
        maestro.setPosition(1, 9984);
        busyWait(&maestro, 1);
        maestro.goHome(1);
        
        cout << "Done" << endl;
        
    } catch (std::exception& e) {
        cout << e.what() << endl;
    }
    
    
    return 0;
}


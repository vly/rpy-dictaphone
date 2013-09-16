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

using namespace std;

void busyWait(Maestro *maestro, unsigned char channel){
    while(maestro->isMoving(channel)){
        usleep(100000);
    }
}

int main(int argc, const char * argv[])
{

    try {
        Maestro maestro("/dev/cu.usbmodem00065291");

        cout << "Moving to 8048" << endl;
        maestro.setPosition(0, 8048);
        busyWait(&maestro, 0);
        cout << "Position is: " << maestro.getPosition(0) << endl;
        
        cout << "Going home" << endl;
        maestro.goHome(0);
        busyWait(&maestro, 0);
        
        cout << "Position is: " << maestro.getPosition(0) << endl;

    } catch (std::exception& e) {
        cout << e.what() << endl;
    }
    
    
    return 0;
}


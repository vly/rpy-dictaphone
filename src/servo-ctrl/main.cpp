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


#define KEYCONTROL 1

#if KEYCONTROL
#include <termios.h>
#include <unistd.h>
#endif

#ifdef __APPLE__
#define NA_DEVICE "/dev/cu.usbmodem00065291"
#else
#define NA_DEVICE "/dev/serial/by-path/platform-bcm2708_usb-usb-0:1.2.2:1.0"
#endif


using namespace std;

#if KEYCONTROL
int mygetch(void) {
    struct termios oldt,
    newt;
    int ch;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}
#endif

void busyWait(Maestro *maestro, Servo * servo){
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

int main(int argc, const char * argv[])
{

    try {
        Maestro maestro(NA_DEVICE);
        Servo horizontalServo(0, 3968, 9216);
        Servo verticalServo(1, 3968, 9216);
        
        maestro.setPosition(&horizontalServo, 3968);
        busyWait(&maestro, &horizontalServo);
        maestro.setPosition(&horizontalServo, 9216);
        busyWait(&maestro, &horizontalServo);
        maestro.goHome(&horizontalServo);
        busyWait(&maestro, &horizontalServo);

        maestro.setPosition(&verticalServo, 6912);
        busyWait(&maestro, &verticalServo);
        maestro.setPosition(&verticalServo, 9984);
        busyWait(&maestro, &verticalServo);
        maestro.goHome(&verticalServo);
        
        #if KEYCONTROL
        cout << "Press q to quit" << endl;
        while (1) {
            int c = mygetch();
            string key;
            
            if (c == 119){
                maestro.stepUp(&verticalServo);
            } else if (c == 100){
                maestro.stepDown(&horizontalServo);
            } else if (c == 115){
                maestro.stepDown(&verticalServo);
            } else if (c == 97){
                maestro.stepUp(&horizontalServo);
            }
            
            if (c == 113) {
                break;
            }
        }
        #endif
        
        cout << "Done" << endl;
        
    } catch (std::exception& e) {
        cout << e.what() << endl;
    }
    
    
    return 0;
}


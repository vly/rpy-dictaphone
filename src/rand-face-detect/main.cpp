//
//  main.cpp
//  rand-face-detect
//
//  Created by Neil Ang on 20/08/13.
//  Copyright (c) 2013 Neil Ang. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define NA_DEFAULT_WAIT 3
#define NA_MIN_TOLERANCE 2
#define NA_DEFAULT_TOLERANCE 5

#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

int rand_num()
{
    return (rand() % 200 + 1) - 100;
}

int rand_with_tolerance(int t, int l)
{
    int i;
    do {
        i = rand_num();
    } while (i > l+t || i < l-t);
    return i;
}

int main(int argc, const char * argv[])
{
    int x = 0;
    int y = 0;

    int t = NA_DEFAULT_TOLERANCE;
    int s = NA_DEFAULT_WAIT;

    for (int i = 1; i < argc; i++) {
        int v = i+1 <= argc;
        if (strcmp(argv[i], "-t") == 0 && v) {
            t = std::stoi(argv[i+1]);
            t = MAX(t, NA_MIN_TOLERANCE);
        }
        else if (strcmp(argv[i], "-s") == 0 && v) {
            s = std::stoi(argv[i+1]);
            s = MAX(s, 0);
        }
        else if (strcmp(argv[i], "-h") == 0) {
            std::cout << "Usage: -t <tolerance> -s <seconds>" << std::endl;
            return EXIT_SUCCESS;
        }

    }

    while (true) {
        x = rand_with_tolerance(t, x);
        y = rand_with_tolerance(t, y);
        printf("%d,%d\n", x, y);
        sleep(s);
    }

    return EXIT_SUCCESS;
}

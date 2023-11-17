/*!
 * @file turbo.h
 *
 * This is a library for working with Pfeiffer TC100 
 * turbopump controllers
 *
 * Written by Brett Longworth
 *
 * BSD license, all text above must be included in any redistribution
 */
#ifndef turbo_h
#define turbo_h

#include <Arduino.h>

class Turbo
{
private:

public:
    Turbo();
    void begin();
    void start();
    void stop();
    void setSpeed(int TB_Spd4);
    int readStatus(char *x, int a, int b);
    void getStatusTurboA(int ST[]);
    void getStatusTurboB(int ST[]);
    int check(int TB_Spd1);
};

#endif
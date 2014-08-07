//Library for controlling ultrasonic module HCSR04
//Ported by hiawoood from arduino library orgininally created by ITead studio.
//Instantiate object by supplying the proper pin numbers of "trigger" and "echo"
//Editted by MohammadShahid

#ifndef HCSR04_H
#define HCSR04_H

#include "mbed.h"

class HCSR04 {
  public:
    HCSR04(PinName t, PinName e);
    long echo_duration();
    int distance();

    private:
        DigitalOut trig;
        DigitalIn echo;
        Timer timer;
        long duration;
        int distacne_cm;
};

#endif
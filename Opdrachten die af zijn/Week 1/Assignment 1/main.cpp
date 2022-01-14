#include "mbed.h"
#include <cstdio>

Ticker flipflop;
Ticker flipflop2;
DigitalOut output_a0(A0);
DigitalOut output_a1(A1);

void rood() 
{
    output_a0.write(!output_a0.read());
}

void groen() 
{
    output_a1.write(!output_a1.read());
}

int main()
{
    flipflop.attach(&rood, 1s);
    flipflop2.attach(&groen, 2s);

    while (true) {
        ThisThread::sleep_for(200ms);
    }
}

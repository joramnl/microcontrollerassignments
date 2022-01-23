#include "mbed.h"

Ticker tick;

DigitalOut PIN0(A0);
DigitalOut PIN1(A1);

bool state = false;

void switchState()
{
    PIN0 = !PIN0;
    PIN1 = !PIN1;
}

int main()
{
    PIN0 = !PIN0;

    tick.attach(&switchState, 1s);

    while (true) {
        
    }
}


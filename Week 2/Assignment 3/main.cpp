#include "mbed.h"

Ticker tick;

DigitalInOut PIN1(A0, PIN_OUTPUT, PullNone, NULL);
DigitalInOut PIN2(A1, PIN_OUTPUT, PullNone, NULL);
DigitalInOut PIN3(A2, PIN_OUTPUT, PullNone, NULL);
DigitalInOut PIN4(A3, PIN_OUTPUT, PullNone, NULL);

void switchLEDs()
{

    PIN1 = !PIN1;
    PIN2 = !PIN2;
    PIN3 = !PIN3;
    PIN4 = !PIN4;
}

int main()
{
    PIN1 = 0;
    PIN2 = 1;
    PIN3 = 1;
    PIN4 = 0;

    tick.attach(&switchLEDs, 1s);

    while (true) {

    }
}


#include "mbed.h"

Ticker t;
DigitalOut green(PA_1);

// main() runs in its own thread in the OS
int main()
{
    t.attach([](){
        green.write(!green.read());
    },1s);
}


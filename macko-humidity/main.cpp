#include "mbed.h"
#include <cstdio>

DigitalOut PIN_0(A0);
DigitalIn PIN_1(A1);

int main()
{
    printf("Application started\n");
    
    while (true) {
        ThisThread::sleep_for(200ms);

        PIN_0.write(1);

        printf("pin1: %d\n", PIN_1.read());
    }
}

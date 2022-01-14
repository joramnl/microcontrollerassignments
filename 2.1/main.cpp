#include "mbed.h"
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <stdexcept>
#include <math.h>

#define elements(a) sizeof(a)/sizeof(a[0])

// main() runs in its own thread in the OS

DigitalOut led_g(PA_0);
DigitalOut led_r(PA_1);

int main(){
    puts("BOOT!\n\n");
    bool state;
    while(true){
        led_g.write(state);
        led_r.write(!state);
        ThisThread::sleep_for(1s);
        state = !state;
    }
}


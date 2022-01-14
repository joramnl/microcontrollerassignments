#include "mbed.h"
#include <algorithm>
#include <chrono>
#include <cstdio>

#define elements(a) sizeof(a)/sizeof(a[0])

// main() runs in its own thread in the OS

DigitalOut pin_pairs[2][2]{
    {DigitalOut(PA_2), DigitalOut(PA_3)}, // controls LED1
    {DigitalOut(PA_1), DigitalOut(PA_4)}  // controls LED2
};

/// resets all pins of the H bridge
inline void reset(){
    for (DigitalOut (&pair)[2] : pin_pairs){
        for (DigitalOut &led : pair){
            led.write(0);
        }
    }
}

int main(){
    puts("BOOT!\n\n");
    bool req_reset = false;
    for(direction=false;;direction=!direction){
        // set all pins in the pairs to ON
        for (DigitalOut &pin : pin_pairs[direction]){
            pin.write(1);
        }
        ThisThread::sleep_for(1s);
        // set all pins off
        reset();
        ThisThread::sleep_for(1s);
    }
}


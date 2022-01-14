#include "mbed.h"
#include <algorithm>
#include <chrono>
#include <cstdio>

#define elements(a) sizeof(a)/sizeof(a[0])

// main() runs in its own thread in the OS

DigitalInOut pin_1(PA_0);
DigitalInOut pin_2(PA_1);
DigitalInOut pin_3(PA_2);

enum PinDir{
    POS = 1,
    NEG = 2,
    OFF = 3
};

inline void update(DigitalInOut& pin, PinDir dir){
    switch (dir) {
        case POS:
            pin.output();
            pin.write(1);
            break;
        case NEG:
            pin.input();
            break;
        case OFF:
            pin.output();
            pin.write(0);
    }
}

inline void update(PinDir one, PinDir two, PinDir three){
    update(pin_1,one  );
    update(pin_2,two  );
    update(pin_3,three);
}

int main(){
    puts("BOOT!\n\n");
    uint8_t reqled = 1;
    while(true){
        // LED 1
        switch (reqled) {
            case 1:update(POS,NEG,OFF);break;
            case 2:update(NEG,POS,OFF);break;
            case 3:update(OFF,POS,NEG);break;
            case 4:update(OFF,NEG,POS);break;
            case 5:update(POS,OFF,NEG);break;
            default:
                // reset cycle
                reqled = 1;
                continue;
        }
        reqled += 1;
        ThisThread::sleep_for(1s);
    }
}


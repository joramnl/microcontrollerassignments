#include "mbed.h"
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <stdexcept>
#include <math.h>

#define elements(a) sizeof(a)/sizeof(a[0])

// main() runs in its own thread in the OS

struct rgb8{
	float values[3];
};

float value(float i) {
	return fmod(i,360.0) < 240
		? 1.0 - cos(fmod(i,360.0f) * 3 * 3.1615 / 360)
		: 0.0f;
}

void set_hue(rgb8 * const color, const float hue){
	color->values[0] = value(hue    );
	color->values[1] = value(hue+120);
	color->values[2] = value(hue+240);
}

AnalogOut led_red(PA_0);
AnalogOut led_grn(PA_2);
AnalogOut led_blu(PA_1);

AnalogIn color_change(PA_3);

#define COLOR_CHANGE_MAX 0.2

int main(){
    puts("BOOT!\n\n");
    float hue = 0;
    float change;
    rgb8 color;
    while(true){
        set_hue(&color, hue);
        led_red.write(color.values[0]);
        led_grn.write(color.values[1]);
        led_blu.write(color.values[2]);

        ThisThread::sleep_for(20ms);
        hue += fmod(color_change.read() * COLOR_CHANGE_MAX + hue, 360);
    }
}


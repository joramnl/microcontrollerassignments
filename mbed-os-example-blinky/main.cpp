#include "mbed.h"
#include <cstdio>

Ticker greenTicker;
Ticker redTicker;

InterruptIn S1(A0);
InterruptIn S2(A1);
DigitalOut LED_GREEN(A2);
DigitalOut LED_RED(A3);
DigitalOut LED_1(LED1);
DigitalOut LED_2(LED3);

bool redBlinking = false;
bool greenBlinking = false;

void toggleGreen() 
{
    LED_GREEN.write(!LED_GREEN.read());
}

void toggleRed() 
{
    LED_RED.write(!LED_RED.read());
}

void S1Fall() 
{
  if (redBlinking) {
    redTicker.detach();
    LED_RED.write(1);
    redBlinking = false;
    return;
  }

  redBlinking = true;
  redTicker.attach(&toggleRed, 1s);
}

void S2Fall() 
{
  if (greenBlinking) {
    greenTicker.detach();
    LED_GREEN.write(0);
    greenBlinking = false;
    return;
  }

  greenBlinking = true;
  greenTicker.attach(&toggleGreen, 2s);
}

int main()
{
    printf("Application started\n");

    LED_GREEN.write(0);
    LED_RED.write(1);

    S1.fall(&S1Fall);
    S2.fall(&S2Fall);

    while (true) {
        ThisThread::sleep_for(200ms);

        // printf("redBlinking: %s\n", redBlinking ? "Yes" : "No");
        printf("greenBlinking: %s\n", greenBlinking ? "Yes" : "No");
    }
}

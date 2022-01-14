#include "mbed.h"
#include <cstdio>

DigitalInOut PIN1(A0, PIN_OUTPUT, PullNone, NULL);
DigitalInOut PIN2(A1, PIN_OUTPUT, PullNone, NULL);
DigitalInOut PIN3(A2, PIN_OUTPUT, PullNone, NULL);

Ticker tick;

enum State {
    STATE_LED1,
    STATE_LED2,
    STATE_LED3,
    STATE_LED4,
    STATE_LED5
};

int NextState = STATE_LED1;

void switchState()
{
  if (NextState == STATE_LED1) {
      PIN1.output();
      PIN2.output();
      PIN3.input();

      PIN1 = 0;
      PIN2 = 1;

      NextState = STATE_LED2;

      return;
  }
  
  if (NextState == STATE_LED2) {
      PIN1.output();
      PIN2.output();
      PIN3.input();

      PIN1 = 1;
      PIN2 = 0;

      NextState = STATE_LED3;

      return;
  }
  
  if (NextState == STATE_LED3) {
      PIN1.input();
      PIN2.output();
      PIN3.output();

      PIN2 = 0;
      PIN3 = 1;

      NextState = STATE_LED4;

      return;
  }
  
  if (NextState == STATE_LED4) {
      PIN1.input();
      PIN2.output();
      PIN3.output();

      PIN2 = 1;
      PIN3 = 0;

      NextState = STATE_LED5;

      return;
  }
  
  if (NextState == STATE_LED5) {
      PIN1.output();
      PIN2.input();
      PIN3.output();

      PIN1 = 0;
      PIN3 = 1;

      NextState = STATE_LED1;

      return;
  }
}

int main()
{

    tick.attach(&switchState, 1s);

    while (true) {
        printf("State: %d\n\r", NextState);
        ThisThread::sleep_for(200ms);
    }
}

#include "mbed.h"
#include <cstdio>

PwmOut blue(D3);
PwmOut green(D5);
PwmOut red(D9);
AnalogIn pot(A0);

float getFloatValue(int i) 
{
    if ((i % 360) < 240) 
    {
        return (1.0f - cos((i % 360) * 3 * 3.1615 / 360));
    } 
    else 
    {
        return 0.0f;
    }
}

void setLedsToValue(float i) 
{
    green = getFloatValue(i);
    blue = getFloatValue(i + 120);
    red = getFloatValue(i + 240);
}

int main()
{
    int i = 0;
    int countingUp = true;

    while (true) 
    {

        int potReading = (int)(pot.read() * 360);

        if (potReading == 0)
        {
            setLedsToValue(i);

            // Go up or down
            if (countingUp) i++;
            if (!countingUp) i--;
            if (i == 360 || i == 0) countingUp = !countingUp;
            printf("i = %d\r\n", i);
        }
        else 
        {
            printf("Pot: %d\n\r", potReading);
            setLedsToValue(potReading);
        }
        
        ThisThread::sleep_for(10ms);
    }
}
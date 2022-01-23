#include <stdio.h>
#include <wiringPi.h>

#define LED1	0
#define LED2	1
#define LED3	2
#define S1		3
#define S2		4

volatile int currentLEDOn = 51; // start somewhere high so we can press down immediately. not ideal, but it works without a hacky workaround

// Button 1 press
void S1Pressed ()
{
	currentLEDOn--;
	delay(1000); // Delay to prevent multiple clicks being registered

}

// Button 2 press
void S2Pressed ()
{
	currentLEDOn++;
    delay(1000); // Delay to prevent multiple clicks being registered
}


int main ()
{
	wiringPiSetup () ; // Initialize wiring pi

    wiringPiISR (S1, INT_EDGE_FALLING, &S1Pressed); // Button 1 intterupt
	wiringPiISR (S2, INT_EDGE_FALLING, &S2Pressed); // Button 2 intterupt

	// Enable LED gpio ports
	pinMode (LED1, OUTPUT) ;
    pinMode (LED2, OUTPUT) ;
    pinMode (LED3, OUTPUT) ;

	for (;;)
	{
	    // To avoid a rewrite, when currentLEDOn goes below 0, it will get reset to 51.
	    if (currentLEDOn < 0)
	    {
	        currentLEDOn = 51;
	    }

		printf("Counter : %d\n\r", currentLEDOn);

		delay(100); // 100 ms delay

		digitalWrite (LED1, (currentLEDOn % 3 == LED1) ? LOW : HIGH);
        digitalWrite (LED2, (currentLEDOn % 3 == LED2) ? LOW : HIGH);
		digitalWrite (LED3, (currentLEDOn % 3 == LED3) ? LOW : HIGH);
	}

	return 0;
}

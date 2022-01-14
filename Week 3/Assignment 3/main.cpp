#include "mbed.h"
#include <cstdio>

AnalogIn VoutPin(A0);

float Vsupply = 3.3; //power supply voltage (3.3 V rail) -STM32 ADC pin is NOT 5 V tolerant
float Vout; //Voltage divider output
float R_NTC; //NTC thermistor resistance in Ohms
float R_10k = 9840; //10k resistor measured resistance in Ohms (other element in the voltage divider)
float B_param = 3700; //B-coefficient of the thermistor
float T0 = 298.15; //25°C in Kelvin
float Temp_K; //Temperature measured by the thermistor (Kelvin)
float Temp_C; //Temperature measured by the thermistor (Celsius)

int main()
{
    unsigned int a, beta = 3975, units, tens;
    float temperature, resistance;
    while (true) 
    {
        a = VoutPin.read_u16(); /* Read analog value */
        
        /* Calculate the resistance of the thermistor from analog votage read. */
        resistance= (float) 10000.0 * ((65536.0 / a) - 1.0);
        
        /* Convert the resistance to temperature using Steinhart's Hart equation */
        temperature=(1/((log(resistance/10000.0)/beta) + (1.0/298.15)))-273.15; 
        
        printf("%.1f°C\n\r", temperature);

        ThisThread::sleep_for(25ms);
    }
}
/*
 * GccApplication1.c
 *
 * Created: 5/5/2023 9:53:56 AM
 * Author : pabas
 */ 


#define F_CPU 1000000UL /* CPU frequency 1MHz */
#include <avr/io.h>
#include <util/delay.h>

float ADCValue, mv;
int temperature_celsius, tens_digit , ones_digit;

// Lookup table for 7-segment display
int data[] = {
	0b00111111,
	0b00000110,
	0b01011011,
	0b01001111,
	0b01100110,
	0b01101101,
	0b01111101,
	0b00000111,
	0b01111111,
	0b01101111
};

void setup(void){
	 // Set direction of ports
	 DDRA = 0x00;        // ADC input on PA0
	 DDRB = DDRC = DDRD = 0xff;   // Output ports
	 
	// Turn on PB3 to power LM35 sensor
	PORTB |= (1<<PB3);
	 
	 // Initialize ADC
	 ADMUX |= ((1<<REFS0)|(1<<REFS1));   // Set reference voltage to 2.56V
	 ADCSRA |= (1<<ADEN);    // Enable ADC
	 
}
int main(void)
{
	setup();
    while (1)
    {
        // Start ADC conversion
        ADCSRA |= (1<<ADSC);
		
        // Wait for conversion to finish
        while(ADCSRA&(1<<ADSC));
		
        // Convert ADC output to temperature in °C
        ADCValue = ADC;
        mv = (ADCValue*2560)/1023;
        temperature_celsius = mv/10;
		
		if((temperature_celsius<=99) && (temperature_celsius>=0)){
			 // Display temperature on 2-digit 7-segment display
			 ones_digit = temperature_celsius % 10;
			 tens_digit = temperature_celsius / 10;
			 PORTD = data[ones_digit];
			 PORTC = data[tens_digit];
		}else{
			PORTD=0b01000000;
			PORTC=0b01000000;
		}
		
       

        // Turn on LED connected to PB2 if temperature is above 50°C
        if (temperature_celsius > 50)
        {
            PORTB |= (1<<PB2);
        }
        else
        {
            PORTB &= (~(1<<PB2));
        }
		
        // Wait for a few milliseconds before starting the next conversion
        _delay_ms(10);
    }
}

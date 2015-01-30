/*
 * Tiny85_pwm_enc.c
 *
 * Created: 30.1.2015. 9:30:57
 *  Author: akosanovic
 */ 
 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint16_t enc = 127;


int main(void)
{
	DDRB   |= (1 << PB0);                   // PWM output on PB0 OCR0A
	// phase correct PWM, 8-bit
	TCCR0A = (1 << COM0A1)| (1 << WGM02) | (1 << WGM00);
	OCR0A = 0x00;                         // initial PWM pulse width
	// clock source = clock / 1 ; start PWM
	TCCR0B = (1 << CS00);
	
	// Assure that pin PB2 and PB1 are inputs
	DDRB &= ~(1<<PB2);
	DDRB &= ~(1<<PB1);

	// Enable the pull-up resistors
	PORTB |= (1<<PB2)|(1<<PB1);
	
	GIMSK = 0b00100000;		// Enable and PCINT
	PCMSK = 0b00000010;		// Enable PCINT1
	sei();
	
	
	while(1)
	{
	  OCR0A = enc;
	}
}

ISR(PCINT0_vect)
{	 
	if( (PINB & (1 << PINB1)) == 0 ){	//falling edge
	  	// When an interrupt occurs, we only have to check the level of
	  	// of pin PB2 to determine the direction
	  if(PINB & _BV(PB2)){
			// Increase enc
			if (enc<255)enc++;
			}
		
		else{
			// Decrease enc
			if (enc>0)enc--;
			}
			
	}
}

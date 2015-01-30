/*
 * PWM_Encoder.c
 *
 * Created: 29.1.2015. 13:49:52
 *  Author: akosanovic
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include<avr/interrupt.h>

uint16_t enc = 0;

// Subroutine declarations
void initInterrupts(void);

int main(void)
{
	DDRB   |= (1 << PB1);                   // PWM output on PB1 OCR1A
	// phase correct PWM, 8-bit
	TCCR0A = (1 << COM0A1)| (1 << WGM02) | (1 << WGM00);
	OCR1A = 0x00;                         // initial PWM pulse width
	// clock source = clock / 1 ; start PWM
	TCCR0B = (1 << CS00);
	
	while(1)
	{
		//OCR1A  = 0x3FF;     // maximum 10-bit value
	}
}

// Routine to setup INT0
void initInterrupts(void)
{
	// Assure that pin PB2 (INT0) and PB0 are inputs
	DDRB &= ~(1<<PB2);
	DDRB &= ~(1<<PB0);

	// Enable the pull-up resistors
	PORTB |= (1<<PB2)|(1<<PB0);

	// Falling edge in INT0 (PB0 / pin7) to cause interrupt
	MCUCR |= (1<<ISC01);

	// Enable and INT0
	GIMSK |= (1<<INT0);
}

// The Interrupt Service Routine for external INT1
ISR(INT0_vect)
{
	// When an interrupt occurs, we only have to check the level of
	// of pin PD5 to determine the direction
	if (PIND & _BV(PB0)){
	// Increase enc
		if (enc<1024){
			enc++;
			OCR1A = enc;
			}
	}
	else{
		if (enc>0){
		// Decrease enc
		enc--;
		OCR1A = enc;
		}
	}
}

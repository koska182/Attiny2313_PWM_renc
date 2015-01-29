/*
 * PWM_Encoder.c
 *
 * Created: 29.1.2015. 13:49:52
 *  Author: akosanovic
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

uint16_t enc = 0;

// Subroutine declarations
void initInterrupts(void);

int main(void)
{
	DDRB   |= (1 << PB3);                   // PWM output on PB3
	// phase correct PWM, 10-bit
	TCCR1A = (1 << COM1A1)| (1 << WGM11) | (1 << WGM10);
	OCR1A = 0x00;                         // initial PWM pulse width
	// clock source = clock / 1 ; start PWM
	TCCR1B = (1 << CS10);
	
	while(1)
	{
		//OCR1A  = 0x3FF;     // maximum 10-bit value
	}
}

// Routine to setup INT1
void initInterrupts(void)
{
	// Assure that pin PD3 (INT1) and PD5 are inputs
	DDRD &= ~(1<<PD5);
	DDRD &= ~(1<<PD3);

	// Enable the pull-up resistors
	PORTD |= (1<<PD5)|(1<<PD3);

	// Falling edge in INT1 (PD3 / pin17) to cause interrupt
	MCUCR |= (1<<ISC11);

	// Enable and INT1
	GIMSK |= (1<<INT1);
}

// The Interrupt Service Routine for external INT1
ISR(INT1_vect)
{
	// When an interrupt occurs, we only have to check the level of
	// of pin PD5 to determine the direction
	if (PIND & _BV(PD5)){
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

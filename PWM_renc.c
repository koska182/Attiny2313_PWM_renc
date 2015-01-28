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
 GICR |= (1<<INT1);
}

ISR(INT1_vect)
{
 // When an interrupt occurs, we only have to check the level of
 // of pin PD5 to determine the direction
 if (PIND & _BV(PD5))
  // Increase enc
  enc++;
 else
  // Decrease enc
  enc--;
}




// TC0_PWM.c
#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRB   |= (1 << PB2);                   // PWM output on PB2
    TCCR0A = (1 << COM0A1) | (1 << WGM00);  // phase correct PWM mode
    OCR0A  = 0x10;                          // initial PWM pulse width
    
    TCCR0B = (1 << CS01);   // clock source = CLK/8, start PWM
    
    while(1)
    {
        // change PWM pulse width every 2 seconds
        _delay_ms(2000);
        OCR0A  = 0x10;
        _delay_ms(2000);
        OCR0A  = 0x30;
        _delay_ms(2000);
        OCR0A  = 0x50;
        _delay_ms(2000);
        OCR0A  = 0xA0;
    }
}

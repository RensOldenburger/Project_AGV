#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "Library.h"

volatile int timerteller = 0;

void signaal_geven()
{
    PORT_LED |= (1<<LED_1);         // LED 1 aan
    PORT_LED &= ~(1<<LED_2);        // LED 2 uit
    PORT_buzzer |= (1<<buzzer);     // buzzer aan
    _delay_ms(500);
    PORT_LED |= (1<<LED_2);         // LED 2 aan
    PORT_LED &= ~(1<<LED_1);        // LED 1 uit
    PORT_buzzer &= ~(1<<buzzer);    // buzzer uit
    _delay_ms(500);
}

void h_bridgeR_set_percentage(signed char percentage)
{
	if (percentage >= 0 && percentage <= 100)
	{
			OCR1A = (65536*percentage)/100;
	}
}

void h_bridgeL_set_percentage(signed char percentage)
{
	if (percentage >= 0 && percentage <= 100)
	{
			OCR3A = (65536*percentage)/100;
	}
}

//H-brug PWMR
ISR(TIMER1_OVF_vect)
{
    PORT ^= (1 << PEN1);
}
//H-brug PWMR
ISR(TIMER1_COMPA_vect)
{
    PORT |= (1 << PEN1);
}

//H-brug PWML
ISR(TIMER3_OVF_vect)
{
    PORT ^= (1 << PEN2);
}
//H-brug PWML
ISR(TIMER3_COMPA_vect)
{
    PORT |= (1 << PEN2);
}

//Timer doorrijden
ISR(TIMER0_OVF_vect)
{
    timerteller ++;
}


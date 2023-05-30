/*
LCD_Naar_Locatie(x, y); verandert waar je iets neerzet op het scherm(waar de cursor staat)
Stuur_LCD_String("x"); hiermee kan je teksten neerzetten op de plek van de cursor
LCD_Scherm_Leeg(); hiermee maak je het scherm leeg
itoa(variabele, array, 10); hiermee kan je variabelen omzetten naar strings -- moet je een array voor maken -- 10 is voor decimaal
*/

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>
#include <avr/interrupt.h>
#include "LCDscherm.h"

int pulsUS1 = 0;
int pulsUS2 = 0;
int i = 0;

void initding()
{
    TCCR1B |= (1<<CS10);//no prescaler
    DDRB |= (1 << PB4);
    DDRB |= (1 << PB5);

    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC00);

    EIMSK |= (1 << INT1);
    EICRA |= (1 << ISC10);
}

ISR(INT0_vect)
{
    if(i == 1)
    {
        TCCR1B = 0;
        pulsUS1 = TCNT1;
        TCNT1 = 0;
        i = 0;
    }
    if(i == 0)
    {
        TCCR1B |= (1<<CS10);
        i = 1;
    }
}

int main(void)
{
    initding();
    sei();

    init_LCDscherm();
    char arrayUS1[16];
    char arrayUS2[16];
    int lengteUS1 = 0;
    int lengteUS2 = 0;

    while(1)
    {
        PORTB |= (1<<PB4);
        _delay_us(15);
        PORTB &= ~(1<<PB4);

        lengteUS1 = pulsUS1 * 0.01071875;
        lengteUS2 = pulsUS2 * 0.01071875;
        itoa(lengteUS1, arrayUS1, 10);
        itoa(lengteUS2, arrayUS2, 10);

        LCD_Naar_Locatie(1, 1);
        Stuur_LCD_String("A1 = ");

        LCD_Naar_Locatie(6, 1);
        Stuur_LCD_String(arrayUS1);

        LCD_Naar_Locatie(1, 2);
        Stuur_LCD_String("A2 = ");

        LCD_Naar_Locatie(6, 2);
        Stuur_LCD_String(arrayUS2);

        _delay_ms(10);
        LCD_Scherm_Leeg();
    }

    return 0;
}

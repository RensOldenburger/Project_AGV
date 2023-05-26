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

int pulsrechts = 0;
int rechtsteller = 0;
int pulslinks = 0;
int linksteller = 0;
int pulsvoor = 0;
int voorteller = 0;
int pulsachter = 0;
int achterteller = 0;

void initding()
{
    //rechts
    TCCR1B |= (1<<CS10);//no prescaler
    DDRB |= (1 << PB4);

    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC00);

    //links
    TCCR3B |= (1<<CS30);//no prescaler
    DDRB |= (1 << PB5);

    EIMSK |= (1 << INT1);
    EICRA |= (1 << ISC10);

    //voor
    TCCR4B |= (1<<CS40);//no prescaler
    DDRB |= (1 << PB6);

    EIMSK |= (1 << INT2);
    EICRA |= (1 << ISC20);

    //achter
    TCCR5B |= (1<<CS50);//no prescaler
    DDRB |= (1 << PB7);

    EIMSK |= (1 << INT3);
    EICRA |= (1 << ISC30);
}

ISR(INT0_vect)
{
    if(rechtsteller == 1)
    {
        TCCR1B = 0;
        pulsrechts = TCNT1;
        TCNT1 = 0;
        rechtsteller = 0;
    }
    if(rechtsteller == 0)
    {
        TCCR1B |= (1<<CS10);
        rechtsteller = 1;
    }
}

ISR(INT1_vect)
{
    if(linksteller == 1)
    {
        TCCR3B = 0;
        pulslinks = TCNT3;
        TCNT3 = 0;
        linksteller = 0;
    }
    if(linksteller == 0)
    {
        TCCR3B |= (1<<CS30);
        linksteller = 1;
    }
}

ISR(INT2_vect)
{
    if(voorteller == 1)
    {
        TCCR4B = 0;
        pulsvoor = TCNT4;
        TCNT4 = 0;
        voorteller = 0;
    }
    if(voorteller == 0)
    {
        TCCR4B |= (1<<CS40);
        voorteller = 1;
    }
}

ISR(INT3_vect)
{
    if(achterteller == 1)
    {
        TCCR5B = 0;
        pulsachter = TCNT5;
        TCNT5 = 0;
        achterteller = 0;
    }
    if(achterteller == 0)
    {
        TCCR5B |= (1<<CS50);
        achterteller = 1;
    }
}

int main(void)
{
    initding();
    sei();
    init_LCDscherm();

    char arrayrechts[16];
    char arraylinks[16];
    char arrayvoor[16];
    char arrayachter[16];

    int lengterechts = 0;
    int lengtelinks = 0;
    int lengtevoor = 0;
    int lengteachter = 0;

    while(1)
    {
        PORTB |= (1<<PB4);
        PORTB |= (1<<PB5);
        PORTB |= (1<<PB6);
        PORTB |= (1<<PB7);
        _delay_us(15);
        PORTB &= ~(1<<PB4);
        PORTB &= ~(1<<PB5);
        PORTB &= ~(1<<PB6);
        PORTB &= ~(1<<PB7);

        lengterechts = pulsrechts * 0.01071875;
        itoa(lengterechts, arrayrechts, 10);
        lengtelinks = pulslinks * 0.01071875;
        itoa(lengtelinks, arraylinks, 10);
        lengtevoor = pulsvoor * 0.01071875;
        itoa(lengtevoor, arrayvoor, 10);
        lengteachter = pulsachter * 0.01071875;
        itoa(lengteachter, arrayachter, 10);

        LCD_Naar_Locatie(1, 1);
        Stuur_LCD_String(arrayrechts);

        LCD_Naar_Locatie(11, 1);
        Stuur_LCD_String(arraylinks);

        LCD_Naar_Locatie(1, 2);
        Stuur_LCD_String(arrayvoor);

        LCD_Naar_Locatie(11, 2);
        Stuur_LCD_String(arrayachter);

        _delay_ms(10);
        LCD_Scherm_Leeg();
    }

    return 0;
}

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
int pulslinks = 0;
int i = 0;
int x = 0;
int ultrateller = 0;

void initding()
{
    TCCR1B |= (1<<CS10);//no prescaler
    TCCR3B |= (1<<CS30);//no prescaler
    DDRB |= (1 << PB4);
    DDRB |= (1 << PB5);

    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC00);

    EIMSK |= (1 << INT1);
    EICRA |= (1 << ISC10);

    DDRH |= (1 << PH6);
}

ISR(INT0_vect)
{
    if(i == 1)
    {
        TCCR1B = 0;
        pulsrechts = TCNT1;
        TCNT1 = 0;
        i = 0;
    }
    if(i == 0)
    {
        TCCR1B |= (1<<CS10);
        i = 1;
    }
}

ISR(INT1_vect)
{
    if(x == 1)
    {
        TCCR3B = 0;
        pulslinks = TCNT3;
        TCNT3 = 0;
        x = 0;
    }
    if(x == 0)
    {
        TCCR3B |= (1<<CS30);
        x = 1;
    }
}

int main(void)
{
    initding();
    sei();

    init_LCDscherm();
    char arrayrechts[16];
    char arraylinks[16];
    int lengterechts = 0;
    int lengtelinks = 0;
    int getal = 3;

    while(1)
    {
        PORTB |= (1<<PB4);
        PORTB |= (1<<PB5);
        _delay_us(15);
        PORTB &= ~(1<<PB4);
        PORTB &= ~(1<<PB5);

        lengterechts = pulsrechts * 0.01071875;
        lengtelinks = pulslinks * 0.01071875;
        itoa(lengterechts, arrayrechts, 10);
        itoa(lengtelinks, arraylinks, 10);

        switch(getal)
        {
        case 1://naar links(te dichtbij rechts)
            //Motoren naar links
            LCD_Naar_Locatie(1, 1);
            Stuur_LCD_String("Naar links");
            if(((lengtelinks >= 50) || (lengtelinks <= 0)) && ((lengterechts >= 50) || (lengterechts <= 0)))
            {
                getal = 3;

            }
            break;
        case 2://naar rechts(te dichtbij links)
            //Motoren naar rechts
            LCD_Naar_Locatie(1, 1);
            Stuur_LCD_String("Naar rechts");
            PORTH |= (1<<PH6);
            if(((lengtelinks >= 50) || (lengtelinks <= 0)) && ((lengterechts >= 50) || (lengterechts <= 0)))
            {
                getal = 3;

            }
            break;
        case 3://rechtdoor
            //Motoren naar rechtdoor
            LCD_Naar_Locatie(1, 1);
            Stuur_LCD_String("Rechtdoor");
            PORTH &= ~(1<<PH6);
            if((lengterechts <= 50) && (lengterechts >= 0))
            {
                getal = 1;

            }
            else if((lengtelinks <= 50) && (lengtelinks >= 0))
            {
                getal = 2;
            }
            break;
        }
        LCD_Naar_Locatie(1, 2);
        Stuur_LCD_String("A1 ");

        LCD_Naar_Locatie(4, 2);
        Stuur_LCD_String(arrayrechts);

        LCD_Naar_Locatie(8, 2);
        Stuur_LCD_String("A2 ");

        LCD_Naar_Locatie(11, 2);
        Stuur_LCD_String(arraylinks);

        _delay_ms(10);
        LCD_Scherm_Leeg();
    }

    return 0;
}

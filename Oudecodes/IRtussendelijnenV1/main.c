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

#define PEN1	PA7//digital 29
#define PIN11	PA6//digital 28
#define PIN12	PA5//digital 27
#define PIN21	PA4//digital 26
#define PIN22	PA3//digital 25
#define PEN2	PA2//digital 24
#define PORT    PORTA
#define DDR     DDRA

void h_bridgeR_set_percentage(signed char percentage);
void h_bridgeL_set_percentage(signed char percentage);

void initding()
{
    //timer PWM motorenR
    TCCR1B = (0 << CS12) | (0 << CS11) | (1 << CS10);
    TIMSK1 = (1 << OCIE1A) | (1 << TOIE1);

    OCR1A = 0;
    //timer PWM motorenL
    TCCR3B = (0 << CS32) | (0 << CS31) | (1 << CS30);
    TIMSK3 = (1 << OCIE3A) | (1 << TOIE3);

    OCR3A = 0;

    //pinout h-brug
    DDR |= (1 << PEN1);
    DDR |= (1 << PIN11);
    DDR |= (1 << PIN12);
    DDR |= (1 << PIN21);
    DDR |= (1 << PIN22);
    DDR |= (1 << PEN2);

    //H-brug pinnen LOW
    PORT &= ~(1 << PEN1);
	PORT &= ~(1 << PIN11);
	PORT &= ~(1 << PIN12);
	PORT &= ~(1 << PIN21);
	PORT &= ~(1 << PIN22);
	PORT &= ~(1 << PEN2);
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

int main(void)
{
    initding();
    sei();
    init_LCDscherm();
    int getal = 3;

    h_bridgeR_set_percentage(0);
    h_bridgeL_set_percentage(0);
/*
//vooruit
        PORT |= (1 << PIN12);
        PORT &= ~(1 << PIN11);
        PORT &= ~(1 << PIN22);
        PORT |= (1 << PIN21);
//achteruit
        PORT |= (1 << PIN11);
        PORT &= ~(1 << PIN12);
        PORT &= ~(1 << PIN21);
        PORT |= (1 << PIN22);
//rechts
        PORT &= ~(1 << PIN12);
        PORT |= (1 << PIN11);
        PORT &= ~(1 << PIN22);
        PORT |= (1 << PIN21);
//links
        PORT |= (1 << PIN12);
        PORT &= ~(1 << PIN11);
        PORT |= (1 << PIN22);
        PORT &= ~(1 << PIN21);
*/

    while(1)
    {
        if((PINB & (1 << PB7)) && (PINB & (1 << PB6)))
        {
            getal = 4;
        }
        switch(getal)
        {
        case 1://naar links(te dichtbij rechts)
            //Motoren naar links
            h_bridgeR_set_percentage(50);
            h_bridgeL_set_percentage(70);
            LCD_Naar_Locatie(1, 1);
            Stuur_LCD_String("Naar links");
            if(((PINB & (1 << PB7)) == 0) && ((PINB & (1 << PB6)) == 0))
            {
                getal = 3;

            }
            break;
        case 2://naar rechts(te dichtbij links)
            //Motoren naar rechts
            h_bridgeR_set_percentage(70);
            h_bridgeL_set_percentage(50);
            LCD_Naar_Locatie(1, 1);
            Stuur_LCD_String("Naar rechts");
            if(((PINB & (1 << PB7)) == 0) && ((PINB & (1 << PB6)) == 0))
            {
                getal = 3;

            }
            break;
        case 3://rechtdoor
            //Motoren naar rechtdoor
            h_bridgeR_set_percentage(70);
            h_bridgeL_set_percentage(70);
            LCD_Naar_Locatie(1, 1);
            Stuur_LCD_String("Rechtdoor");
            PORT |= (1 << PIN12);
            PORT &= ~(1 << PIN11);
            PORT &= ~(1 << PIN22);
            PORT |= (1 << PIN21);
            if(PINB & (1 << PB7))
            {
                getal = 1;

            }
            else if(PINB & (1 << PB6))
            {
                getal = 2;
            }
            break;
        case 4://stoppen niks in zicht
            //Motoren naar rechtdoor
            h_bridgeR_set_percentage(99);
            h_bridgeL_set_percentage(99);
            LCD_Naar_Locatie(1, 1);
            Stuur_LCD_String("STOP");
            if((PINB & (1 << PB7)) && ((PINB & (1 << PB6)) == 0))
            {
                getal = 1;

            }
            else if((PINB & (1 << PB6)) && ((PINB & (1 << PB7)) == 0))
            {
                getal = 2;
            }
            else if(((PINB & (1 << PB6)) == 0) && ((PINB & (1 << PB7)) == 0))
            {
                getal = 3;
            }
            break;
        }
        LCD_Naar_Locatie(1, 2);
        Stuur_LCD_String("IR1");

        LCD_Naar_Locatie(8, 2);
        Stuur_LCD_String("IR2");

        if(getal == 1)
        {
            LCD_Naar_Locatie(4, 2);
            Stuur_LCD_String("AAN");

            LCD_Naar_Locatie(11, 2);
            Stuur_LCD_String("UIT");
        }
        else if(getal == 2)
        {
            LCD_Naar_Locatie(4, 2);
            Stuur_LCD_String("UIT");

            LCD_Naar_Locatie(11, 2);
            Stuur_LCD_String("AAN");
        }
        else
        {
            LCD_Naar_Locatie(4, 2);
            Stuur_LCD_String("UIT");

            LCD_Naar_Locatie(11, 2);
            Stuur_LCD_String("UIT");
        }


        _delay_ms(10);
        LCD_Scherm_Leeg();
    }

    return 0;
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

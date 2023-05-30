/*
LCD_Naar_Locatie(x, y); verandert waar je iets neerzet op het scherm(waar de cursor staat)
Stuur_LCD_String("x"); hiermee kan je teksten neerzetten op de plek van de cursor
LCD_Scherm_Leeg(); hiermee maak je het scherm leeg
itoa(variabele, array, 10); hiermee kan je variabelen omzetten naar strings -- moet je een array voor maken -- 10 is voor decimaal
*/

#define PEN1	PC7//digital 30
#define PIN11	PC6//digital 31
#define PIN12	PC5//digital 32
#define PIN21	PC4//digital 33
#define PIN22	PC3//digital 34
#define PEN2	PC2//digital 35
#define PORT    PORTC
#define DDR     DDRC

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>
#include <avr/interrupt.h>

int pulsrechts = 0;
int rechtsteller = 0;
int pulslinks = 0;
int linksteller = 0;

void h_bridge_set_percentage(signed char percentage);

void initding()
{
    //rechts
    TCCR1B |= (1<<CS10);//no prescaler
    DDRB |= (1 << PB4);//digital pin 10

    EIMSK |= (1 << INT0);//digital pin 21
    EICRA |= (1 << ISC00);

    //links
    TCCR3B |= (1<<CS30);//no prescaler
    DDRB |= (1 << PB5);//digital pin 11

    EIMSK |= (1 << INT1);//digital pin 20
    EICRA |= (1 << ISC10);

    //timer PWM motoren
    TCCR0B = (0 << CS02) | (0 << CS01) | (1 << CS00);
    TIMSK0 = (1 << OCIE0A) | (1 << TOIE0);

    OCR0A = 0;

    TCCR4B = (1 << CS42);
    TIMSK4 = (1 << TOIE4);

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

//H-brug PWM
ISR(TIMER0_OVF_vect)
{
    PORT ^= (1 << PEN1);
    PORT ^= (1 << PEN2);
}
//H-brug PWM
ISR(TIMER0_COMPA_vect)
{
    PORT |= (1 << PEN1);
    PORT |= (1 << PEN2);
}

ISR(TIMER4_OVF_vect)
{

}

int main(void)
{
    initding();
    sei();

    h_bridge_set_percentage(10);

    int lengterechts = 0;
    int lengtelinks = 0;

    while(1)
    {
//onderste is de rechterwiel
//vooruit
//            PORT |= (1 << PIN12);
//            PORT &= ~(1 << PIN11);
//            PORT &= ~(1 << PIN22);
//            PORT |= (1 << PIN21);
//achteruit
//            PORT |= (1 << PIN11);
//            PORT &= ~(1 << PIN12);
//            PORT &= ~(1 << PIN21);
//            PORT |= (1 << PIN22);
//rechts
//            PORT &= ~(1 << PIN12);
//            PORT |= (1 << PIN11);
//            PORT &= ~(1 << PIN22);
//            PORT |= (1 << PIN21);
//links
//            PORT |= (1 << PIN12);
//            PORT &= ~(1 << PIN11);
//            PORT |= (1 << PIN22);
//            PORT &= ~(1 << PIN21);
//
        lengterechts = pulsrechts * 0.01071875;
        lengtelinks = pulslinks * 0.01071875;

        PORTB |= (1<<PB4);
        PORTB |= (1<<PB5);
        _delay_us(15);
        PORTB &= ~(1<<PB4);
        PORTB &= ~(1<<PB5);

        if((lengtelinks <= 120) && (lengtelinks >= 0))
        {
            PORT |= (1 << PIN12);
            PORT &= ~(1 << PIN11);
            PORT |= (1 << PIN22);
            PORT &= ~(1 << PIN21);

        }
        else if((lengterechts <= 120) && (lengterechts >= 0))
        {
            PORT &= ~(1 << PIN12);
            PORT |= (1 << PIN11);
            PORT &= ~(1 << PIN22);
            PORT |= (1 << PIN21);
        }
        else
        {
            PORT &= ~(1 << PIN11);
            PORT |= (1 << PIN12);
            PORT |= (1 << PIN21);
            PORT &= ~(1 << PIN22);
        }

    }

    return 0;
}

void h_bridge_set_percentage(signed char percentage)
{
	if (percentage >= 0 && percentage <= 100)
	{
			OCR0A = (255*percentage)/100;
	}
}

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

#define IRonderrechts	PB7
#define IRonderlinks	PB6
#define IRbakrechts	PB5
#define IRbaklinks	PB4
#define IRregister	PINB

#define LED_1       PE5     // D3
#define LED_2       PE4     // D2
#define DDR_LED     DDRE
#define PORT_LED    PORTE

#define buzzer      PE3     // D5
#define DDR_buzzer  DDRE
#define PORT_buzzer PORTE

#define PEN1	PA7//digital 29
#define PIN11	PA6//digital 28
#define PIN12	PA5//digital 27
#define PIN21	PA4//digital 26
#define PIN22	PA3//digital 25
#define PEN2	PA2//digital 24
#define PORT    PORTA
#define DDR     DDRA
/*
bij volle batterij
80
75
85
*/
#define snelheidrechtdoor    70
#define snelheidhard    65
#define snelheidzacht   75

void h_bridgeR_set_percentage(signed char percentage);
void h_bridgeL_set_percentage(signed char percentage);
void signaal_aangeven();

volatile int timerteller = 0;

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

    //timer doorrijden
    TIMSK0 |= (1 << TOIE0);

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

	//LEDS
    DDR_LED |= (1<<LED_1) | (1<<LED_2);     // enable pins as output
    PORT_LED &= ~(1<<LED_1) & ~(1<<LED_2);  // put pins low
    //Buzzer
    DDR_buzzer |= (1<<buzzer);              // enable pins as output
    PORT_buzzer &= ~(1<<buzzer);            // put pins low
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

int main(void)
{
    initding();
    sei();
    int toestand = 0;

/*
//vooruit
        PORT |= (1 << PIN11);
        PORT &= ~(1 << PIN12);
        PORT &= ~(1 << PIN21);
        PORT |= (1 << PIN22);
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
        if(((IRregister & (1<<IRbakrechts)) == 0) || ((IRregister & (1<<IRbaklinks)) == 0))
        {
            toestand = 5;
        }
        switch(toestand)
        {
        case 0://begin state
            h_bridgeR_set_percentage(snelheidrechtdoor);
            h_bridgeL_set_percentage(snelheidrechtdoor);
            PORT |= (1 << PIN11);
            PORT &= ~(1 << PIN12) ;
            PORT &= ~(1 << PIN21);
            PORT |= (1 << PIN22);
            if(((IRregister & (1 << IRonderrechts)) == 0) && ((IRregister & (1 << IRonderlinks)) == 0))
            {
                toestand = 3;

            }
            break;
        case 1://naar links(te dichtbij rechts)
            //Motoren naar links
            h_bridgeR_set_percentage(snelheidhard);
            h_bridgeL_set_percentage(snelheidzacht);
            if(((IRregister & (1 << IRonderrechts)) == 0) && ((IRregister & (1 << IRonderlinks)) == 0))
            {
                toestand = 3;
            }
            else if((IRregister & (1 << IRonderrechts)) && (IRregister & (1 << IRonderlinks)))
            {
                toestand = 4;
            }
            break;
        case 2://naar rechts(te dichtbij links)
            //Motoren naar rechts
            h_bridgeR_set_percentage(snelheidzacht);
            h_bridgeL_set_percentage(snelheidhard);
            if(((IRregister & (1 << IRonderrechts)) == 0) && ((IRregister & (1 << IRonderlinks)) == 0))
            {
                toestand = 3;
            }
            else if((IRregister & (1 << IRonderrechts)) && (IRregister & (1 << IRonderlinks)))
            {
                toestand = 4;
            }
            break;
        case 3://rechtdoor
            //Motoren naar rechtdoor
            PORT |= (1 << PIN11);
            PORT &= ~(1 << PIN12) ;
            PORT &= ~(1 << PIN21);
            PORT |= (1 << PIN22);
            h_bridgeR_set_percentage(snelheidrechtdoor);
            h_bridgeL_set_percentage(snelheidrechtdoor);
            if(IRregister & (1 << IRonderrechts))
            {
                toestand = 1;
            }
            else if(IRregister & (1 << IRonderlinks))
            {
                toestand = 2;
            }
            else if((IRregister & (1 << IRonderrechts)) && (IRregister & (1 << IRonderlinks)))
            {
                toestand = 4;
            }
            break;
        case 4://Rijstrook wisselen naar links
            TCCR0B = (1 << CS02) | (0 << CS01) | (1 << CS00);
            if(timerteller == 15000)//een seconde zijn voorbij
            {
                timerteller = 0;
                TCCR0B = 0;
                h_bridgeR_set_percentage(snelheidhard);
                h_bridgeL_set_percentage(snelheidzacht);
                if(((IRregister & (1 << IRonderlinks)) == 0) && ((IRregister & (1 << IRonderrechts)) == 0))
                {
                    toestand = 3;
                }
            }
            break;
        case 5://signaal geven
            //motoren uit
            PORT &= ~(1 << PIN11);//MOETEN ZE 1 OF 0!!!!
            PORT &= ~(1 << PIN12);
            PORT &= ~(1 << PIN21);
            PORT &= ~(1 << PIN22);
            for (int i = 0; i < 10; i++)
            {
                signaal_aangeven();
            }
            toestand = 3;
            break;
        }

    }

    return 0;
}

void signaal_aangeven()
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

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>
#include <avr/interrupt.h>
#include "Bluetooth.h"
#include "Library.h"

int signaalteller = 0;
volatile int count = 0;

/*
Bluetooth getallen:
1: Bluetooth verbonden
2: Bluetooth verbroken
3: Aanknop
4: Autonoomknop
5: Volgknop
6: Uitknop
7: Noodknop ingedrukt
8: Noodknop uitgedrukt
*/

int main(void)
{
    init();

    initTimer();
    int waarde;

    int toestand = 0;

    while(1)
    {
        //waarde = GetDistance();

        if(Bluetooth_Getal == 2)//Bluetooth verbroken
        {
            toestand = 0;
        }
        if(Bluetooth_Getal == 6)//Uitknop ingedrukt
        {
            toestand = 1;
        }
        if(Bluetooth_Getal == 7)// || (Noodknopport & (1<<Noodknoppin)) == 0))
        {
            toestand = 99;
        }
        if(((IRregister & (1 << IRbaklinks)) == 0) || ((IRregister & (1 << IRbakrechts)) == 0))//Plantenbak gedetecteerd
        {
            if(bakvar != 1)
            {
                bakvar = 1;
                TCCR4B |= (1 << CS42);
                toestand = 8;
            }
        }
        if(waarde < 30)
        {
            toestand = 9;
        }
        switch(toestand)
        {
        case 99://Noodtoestand
            h_bridgeR_set_percentage(snelheiduit);//motoren uit
            h_bridgeL_set_percentage(snelheiduit);
            if(timerteller <= 30)
            {
                PORT_buzzer &= ~(1<<buzzer);//buzzer
                PORT_LED &= ~(1<<LED_1);//LED
                PORT_LED &= ~(1<<LED_2);//LED
            }
            if(timerteller >= 30)
            {
                PORT_buzzer |= (1<<buzzer);//buzzer
                PORT_LED |= (1<<LED_1);//LED
                PORT_LED |= (1<<LED_2);//LED
                if(timerteller >= 60)
                {
                    timerteller = 0;
                }
            }
            if((Bluetooth_Getal == 8))// && ((Noodknopport & (1<<Noodknoppin)) != 0))
            {
                toestand = 2;
            }
            break;
        case 0:
            PORT_LED |= (1<<LED_2);
            h_bridgeL_set_percentage(snelheiduit);//motoren uit
            h_bridgeR_set_percentage(snelheiduit);
            if(Bluetooth_Getal == 1)//Bluetooth verbonden
            {
                toestand = 1;
            }
            break;
        case 1://wachten op aan knop
            PORT_LED |= (1<<LED_1);
            h_bridgeL_set_percentage(snelheiduit);//motoren uit
            h_bridgeR_set_percentage(snelheiduit);
            if(Bluetooth_Getal == 3)//Aanknop
            {
                toestand = 2;
            }
            break;
        case 2://toestand keuze case
            PORT_LED &= ~(1<<LED_1);
            if(Bluetooth_Getal == 4)//Autonoomknop
            {
                toestand = 3;
            }
            break;
        case 3://rijstrook inrijden
            PORT_LED &= ~(1<<LED_2);
            h_bridgeR_set_percentage(snelheidrechtdoor);
            h_bridgeL_set_percentage(snelheidrechtdoor);
            rechtdoorrijden();
            if(((IRregister & (1 << IRonderrechts)) == 0) && ((IRregister & (1 << IRonderlinks)) == 0))
            {
                toestand = 4;
            }
            break;
        case 4://Tussen de balken rechtdoor
            //Motoren naar rechtdoor
            rechtdoorrijden();
            h_bridgeR_set_percentage(snelheidrechtdoor);
            h_bridgeL_set_percentage(snelheidrechtdoor);
            if(IRregister & (1 << IRonderrechts))
            {
                toestand = 5;
            }
            else if(IRregister & (1 << IRonderlinks))
            {
                toestand = 6;
            }
            else if((IRregister & (1 << IRonderrechts)) && (IRregister & (1 << IRonderlinks)))
            {
                //toestand = 7;
            }
            break;
        case 5://Naar links rijden
            //Motoren naar rechts
            h_bridgeR_set_percentage(snelheidhard);
            h_bridgeL_set_percentage(snelheidzacht);
            if(((IRregister & (1 << IRonderrechts)) == 0) && ((IRregister & (1 << IRonderlinks)) == 0))
            {
                toestand = 4;
            }
            else if((IRregister & (1 << IRonderrechts)) && (IRregister & (1 << IRonderlinks)))
            {
                //toestand = 7;
            }
            break;
        case 6://Naar rechts rijden
            //Motoren naar links
            h_bridgeR_set_percentage(snelheidzacht);
            h_bridgeL_set_percentage(snelheidhard);
            if(((IRregister & (1 << IRonderrechts)) == 0) && ((IRregister & (1 << IRonderlinks)) == 0))
            {
                toestand = 4;
            }
            else if((IRregister & (1 << IRonderrechts)) && (IRregister & (1 << IRonderlinks)))
            {
                toestand = 7;
            }
            break;
        case 7://Balken niet meer in zicht en bocht maken
            _delay_ms(2000);
            h_bridgeR_set_percentage(snelheidhard);
            h_bridgeL_set_percentage(snelheidzacht);
            if(((IRregister & (1 << IRonderlinks)) == 0) && ((IRregister & (1 << IRonderrechts)) == 0))
            {
                toestand = 4;
            }
            break;
        case 8://signaleren
            h_bridgeR_set_percentage(snelheiduit);
            h_bridgeL_set_percentage(snelheiduit);
            signaal_geven();
            toestand = 4;
            break;
        case 9://Voorultrasoon ziet iets
            h_bridgeR_set_percentage(snelheiduit);
            h_bridgeL_set_percentage(snelheiduit);
            if(waarde > 30)
            {
                toestand = 4;
            }
            break;
        }
    }

    return 0;
}

void init()
{
    initBluetooth();
    sei();

    //timer PWM motorenR
    TCCR1B = (0 << CS12) | (0 << CS11) | (1 << CS10);
    TIMSK1 = (1 << OCIE1A) | (1 << TOIE1);

    OCR1A = 0;
    //timer PWM motorenL
    TCCR3B = (0 << CS32) | (0 << CS31) | (1 << CS30);
    TIMSK3 = (1 << OCIE3A) | (1 << TOIE3);

    OCR3A = 0;

    //timer noodtoestand
    TCCR0B = (1 << CS02) | (1 << CS00);
    TIMSK0 |= (1 << TOIE0);

    //timer noodtoestand
    //TCCR4B = (1 << CS42) | (1 << CS40);
    TIMSK4 |= (1 << TOIE4);

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
    //PORT_LED |= (1<<LED_2);

    //Buzzer
    DDR_buzzer |= (1<<buzzer);              // enable pins as output
    PORT_buzzer &= ~(1<<buzzer);            // put pins low

    //knoppen
	DDRF &= ~(1<<Noodknoppin);
	PORTF |= (1<<Noodknoppin);   		// enAble alle knoppen voor input
}


void initTimer(void){
    Ultraregister |= (1<<Voorultrasoontrig);
    UltraPort &= ~(1<<Voorultrasoontrig);
    TCCR2A = 0;
    TCCR2B |= (1<<CS20);
    //16000000 / 256 = 62,500
    //1 / 62500 = 0.000016s
    //62500x/sec
    TIMSK2 = (1<<TOIE2);
    sei();
}

int GetDistance(void){

    int Distance;

    TriggerPulse();

    while ((Ultraregister & (1<<Voorultrasoonecho)) == 0){

    }
    TCNT2 = 0;
    TIFR2 = 1<<TOV2;
    count = 0;

    while ((Ultraregister & (1<<Voorultrasoonecho)) != 0){

    }
    Distance = count;

    Distance = Distance * 0.008 * 343;

    //340  m/s
    //34 cm / ms
    //340   mm / ms
    //0.34  mm / us
    //5.44 * count geeft distance in mm

    return Distance;
}

void TriggerPulse (void){
    UltraPort |= (1<<Voorultrasoontrig);
    _delay_us(10);
    UltraPort &= (~(1<<Voorultrasoontrig));
}
ISR(TIMER2_OVF_vect){
    count++;
}

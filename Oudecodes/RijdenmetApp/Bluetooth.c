#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "Bluetooth.h"

int Bluetooth_Getal = 0;//BLUETOOTH GETAL
int verandergetal = 0;//ONNODIG
int veranderandergetal = 0;//ONNODIG

ISR(USART0_RX_vect)
{
    Bluetooth_Getal = UDR0;
    if (verandergetal == 0)//ONNODIG
    {//ONNODIG
        verandergetal++;//ONNODIG
    }//ONNODIG
    else//ONNODIG
    {//ONNODIG
        verandergetal = 0;//ONNODIG
    }//ONNODIG
}

void init(void)
{
    //Bluetooth
    UBRR0H = (BRC >> 8);
    UBRR0L = BRC;

    UCSR0B = (1 << RXEN0) |(1 << RXCIE0) |(1 << TXEN0) | (1 << TXCIE0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

    sei();
}

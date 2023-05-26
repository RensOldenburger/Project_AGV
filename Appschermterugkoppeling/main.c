/*
LCD_Naar_Locatie(x, y); verandert waar je iets neerzet op het scherm(waar de cursor staat)
Stuur_LCD_String("x"); hiermee kan je teksten neerzetten op de plek van de cursor
LCD_Scherm_Leeg(); hiermee maak je het scherm leeg
itoa(variabele, array, 10); hiermee kan je variabelen omzetten naar strings -- moet je een array voor maken -- 10 is voor decimaal
*/
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "LCDenBluetooth.h"
#define TX_BUFFER_SIZE 128

char serialBuffer[TX_BUFFER_SIZE];
uint8_t serialReadPos = 0;
uint8_t serialWritePos = 0;

void appendSerial(char c);
void serialWrite(char c[]);

int main(void)
{
    init();

    LCD_Naar_Locatie(4, 1);
    Stuur_LCD_String("Begonnen");
    LCD_Naar_Locatie(4, 2);
    Stuur_LCD_String("Start");

    char array[100];

    while(1)
    {
        serialWrite("huhujkoi");
        _delay_ms(1000);
    }

    return 0;
}

void appendSerial(char c)
{
    serialBuffer[serialWritePos] = c;
    serialWritePos++;
    if(serialWritePos >= TX_BUFFER_SIZE)
    {
        serialWritePos = 0;
    }
}

void serialWrite(char c[])
{
    for(uint8_t i = 0; i< strlen(c); i ++)
    {
        appendSerial(c[i]);
    }
    if(UCSR0A & (1 << UDRE0))
    {
        UDR0 = 0;
    }
}

ISR(USART_TX_vect)
{
    if(serialReadPos != serialWritePos)
    {
        UDR0 = serialBuffer[serialReadPos];
        serialReadPos++;

        if(serialReadPos >= TX_BUFFER_SIZE)
        {
            serialReadPos++;
        }
    }
}

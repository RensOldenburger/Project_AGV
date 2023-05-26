#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "LCDenBluetooth.h"

char LCD_Pos_Eerste_Vakje[2] = {0, 64};

int Bluetooth_Getal = 0;//BLUETOOTH GETAL
int verandergetal = 0;//ONNODIG
int veranderandergetal = 0;//ONNODIG

void Check_LCD_isbezig(void);
void LCD_Aan_Uit(void);
void Stuur_LCD_Character(unsigned char LCD_character);

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
    //LCD scherm
    DataDir_LCDControl |= (1 << LCDEnable) | (1 << ReadWrite) | (1 << LCDRS);//Data directory register goed zetten
    _delay_ms(15);//delay moet volgens datasheet

    LCD_Scherm_Leeg();
    Stuur_LCD_Opdracht(0x38);//opdracht voor LCD in 8bit mode
    _delay_us(50);//delay moet volgens datasheet
    Stuur_LCD_Opdracht(0b00001100);//scherm een beetje aanpassen
    _delay_us(50);//delay moet volgens datasheet

    //Bluetooth
    UBRR0H = (BRC >> 8);
    UBRR0L = BRC;

    UCSR0B = (1 << RXEN0) |(1 << RXCIE0) |(1 << TXEN0) |(1 << TXCIE0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

    sei();
}

void Check_LCD_isbezig(void)//wachten tot LCD informatie kan opnemen
{
    DataDir_LCDpoort = 0;
    LCDControl |= (1 << ReadWrite);
    LCDControl &= ~(1 << LCDRS);

    while(LCDpoort >= 0x80)
    {
        LCD_Aan_Uit();
    }

    DataDir_LCDpoort = 0xFF;
}

void LCD_Aan_Uit(void)//LCD(enable) snel aan en uit zetten
{
    LCDControl |= 1 << (LCDEnable);
    _delay_us(50);
    LCDControl &= ~(1 << LCDEnable);
}

void Stuur_LCD_Opdracht(unsigned char LCD_opdracht)//opdracht naar de LCD sturen
{
    Check_LCD_isbezig();
    LCDpoort = LCD_opdracht;
    LCDControl &= ~((1 << ReadWrite) | (1 << LCDRS));
    LCD_Aan_Uit();
    LCDpoort = 0;
}

void Stuur_LCD_Character(unsigned char LCD_character)//character naar de LCD sturen
{
    Check_LCD_isbezig();
    LCDpoort = LCD_character;
    LCDControl &= ~(1 << ReadWrite);
    LCDControl |= (1 << LCDRS);
    LCD_Aan_Uit();
    LCDpoort = 0;
    _delay_ms(10);
}

void Stuur_LCD_String(char *stringcharacters)//dit is zodat je characters in string kunnen sturen
{
    while(*stringcharacters > 0)
    {
        Stuur_LCD_Character(*stringcharacters++);
    }
}

void LCD_Naar_Locatie(uint8_t x, uint8_t y)//opdracht voor cursor verplaatsing maken
{
    Stuur_LCD_Opdracht(0x80 + LCD_Pos_Eerste_Vakje[y-1] + (x-1));
}

void LCD_Scherm_Leeg(void)//Maakt het scherm leeg
{
    Stuur_LCD_Opdracht(0x01);//Leeg scherm 0x01
    _delay_ms(2);//altijd 2ms delay na scherm leegmaken
}

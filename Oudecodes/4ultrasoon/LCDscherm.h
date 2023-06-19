#ifndef LCDSCHERM_H_INCLUDED
#define LCDSCHERM_H_INCLUDED

#define LCDpoort            PORTC
#define DataDir_LCDpoort    DDRC
#define LCDControl          PORTL
#define DataDir_LCDControl  DDRL
#define LCDEnable           2
#define ReadWrite           1
#define LCDRS               0

void init_LCDscherm(void);
void Stuur_LCD_Opdracht(unsigned char LCD_opdracht);
void Stuur_LCD_String(char *stringcharacters);
void LCD_Naar_Locatie(uint8_t x, uint8_t y);
void LCD_Scherm_Leeg(void);

#endif // LCDSCHERM_H_INCLUDED

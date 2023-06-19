#ifndef LIBRARY_H_INCLUDED
#define LIBRARY_H_INCLUDED

#define IRonderrechts	PB7
#define IRonderlinks	PB6
#define IRbakrechts	    PB5
#define IRbaklinks	    PB4
#define IRregister	    PINB

#define Voorultrasoontrig	    PB4
#define Voorultrasoonecho	    PB4
#define Ultraregister	    PINB
#define UltraPort	    PORTB

#define LED_1           PH4
#define LED_2           PH5
#define DDR_LED         DDRH
#define PORT_LED        PORTH

#define buzzer          PH3
#define DDR_buzzer      DDRH
#define PORT_buzzer     PORTH

#define PEN1	        PA7//digital 29
#define PIN11	        PA6//digital 28
#define PIN12	        PA5//digital 27
#define PIN21	        PA4//digital 26
#define PIN22	        PA3//digital 25
#define PEN2	        PA2//digital 24
#define PORT            PORTA
#define DDR             DDRA

#define Noodknoppin     PF1
#define Noodknopport    DDRF

/*
bij volle batterij: 80,75,85
*/
#define snelheidrechtdoor    20
#define snelheidhard    45
#define snelheidzacht   55
#define snelheiduit  99

void h_bridgeR_set_percentage(signed char percentage);
void h_bridgeL_set_percentage(signed char percentage);
void signaal_geven();

extern volatile int timerteller;

//extern int pulsvoor;
//extern int voorteller;
//extern int pulsachter;
//extern int achterteller;
//extern int lengtevoor;
//extern int lengteachter;

#endif // LIBRARY_H_INCLUDED

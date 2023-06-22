/********************************************************
*   Code voor het detecteren van bomen en het           *
*   reageren hierop door te stoppen en een              *
*   signaal te geven via LED's en een buzzer.           *
*   Project:    AGV                                     *
*   Versie:     1.0                                     *
*********************************************************/

#include <avr/io.h>
#include <util/delay.h>

#define DEBOUNCE    _delay_ms(20)

//======== IR pins ========//
#define IR_R        PH4     // D7
#define IR_L        PH3     // D6
#define DDR_IR      DDRH
#define PORT_IR     PORTH
#define PIN_IR      PINH

//====== output pins ======//
#define LED_1       PE5     // D3
#define LED_2       PE4     // D2
#define DDR_LED     DDRE
#define PORT_LED    PORTE

#define buzzer      PE3     // D5
#define DDR_buzzer  DDRE
#define PORT_buzzer PORTE


/*...........................................................................
  Name:     IR_init
  Purpose:  init de IR sensor pinnen
  Entry:    -
  Exit:     -
  Notes:    -
*/
void LED_init (void)
    {
        DDRB |= (1<<PB7) | (1<<PB5);

        PORTB &= ~(1<<PB7);
    }
void IR_init(void)
{
    DDR_IR &= ~(1<<IR_L) & ~(1<<IR_R);      // eneble pins as input
    PORT_IR &= ~(1<<IR_L) & ~(1<<IR_R);     // disable pins pull-up resistor
}

/*...........................................................................
  Name:     output_init
  Purpose:  init de output pinnen voor de LED's en buzzer
  Entry:    -
  Exit:     -
  Notes:    -
*/
void output_init(void)
{
    DDR_LED |= (1<<LED_1) | (1<<LED_2);     // eneble pins as output
    PORT_LED &= ~(1<<LED_1) & ~(1<<LED_2);  // put pins low

    DDR_buzzer |= (1<<buzzer);              // eneble pins as output
    PORT_buzzer &= ~(1<<buzzer);            // put pins low
}

void init(void)
{
    IR_init();
    output_init();
    LED_init();
}

/*...........................................................................
  Naam:     output_aan
  Doel:     laat de LED's en de buzzer knipperen om de seconde
  Entry:    (aan) als je een 1 meegeeft gaat de functie werken, bij een 0 gebeurd er niks
  Exit:     geen return
  Notes:    gebruikt delays ipv timers dus code staat even stil
*/
void output_aan(int aan)
{
    if(aan == 1)
    {
        PORT_LED |= (1<<LED_1);         // LED 1 aan
        PORT_LED &= ~(1<<LED_2);        // LED 2 uit
        PORT_buzzer |= (1<<buzzer);     // buzzer aan
        _delay_ms(1000);
        PORT_LED |= (1<<LED_2);         // LED 2 aan
        PORT_LED &= ~(1<<LED_1);        // LED 1 uit
        PORT_buzzer &= ~(1<<buzzer);    // buzzer uit
        PORTB |= (1<<PB5);
        _delay_ms(1000);
    }
    else
        aan = 0;
}

int main(void)
{
    init();

    while(1)
    {
        if ((PIN_IR & (1<<IR_L)) == 0)
        {
            DEBOUNCE;
            // stop de motoren
            for (int i = 0; i < 10; i++)
            {
                output_aan(1);
            }
            // start de motoren
        }
        if ((PIN_IR & (1<<IR_R)) == 0)
        {
            DEBOUNCE;
            // stop de motoren
            for (int i = 0; i < 10; i++)
            {
                output_aan(1);
            }
            // start de motoren
        }
    }

    return 0;
}

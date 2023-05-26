/*
 */

#include <avr/io.h>

int main(void)
{

    while(1)
    {
        if ((PINF & (1<<PF4)) == 0)                 // noodstop indrukken
        {
            DEBOUNCE;
            PORTF &= ~(1<<PF7);                     // als de noodstop is ingedrukt brandt het ledje
            status = 0;
        }
        if (Bluetooth_Getal == 6)                   // noodstop app indrukken
        {
            DEBOUNCE;
            PORTF &= ~(1<<PF7);                     // als de noodstop is ingedrukt brandt het ledje
            Bluetooth_Getal = 20;
            status = 99;
            State_verander = status;
        }
        switch(status)
		{
		    case 0:

		        break;
            case 1:

                break;
            case 2:

                break;
		}
    }

    return 0;
}

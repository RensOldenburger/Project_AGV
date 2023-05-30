/*
rechte lijn rijden
 */

#include <avr/io.h>
init void(init)
{


}
int main(void)
{
    init();
    toestand = 0;
    // Insert code

    while(1)
    case 0:
        //kiezen voor automatisch

        toestand = 1;//ja
        toestand = 10;//nee
    break;

    case 1: //automatisch rijden
        //detecteer een route
        toestand = 2;//if gedetecteerd -> route rijden
        toeatand = 1;//if not -> wachten

    break;

    case 2://route rijden
        toestand = 3;//detectie bomen: IR
        toestand = 4;//detectie einde baan: ultrasoon
    break;

    case 3://detectie bomen
        toestand = 2; //geen bomen gedetecteerd
        toestand = 5; //bomen gedetecteerd
    break;

    case 4://detectie einde baan
        toestand = 2; //niet het einde
        toestand = 6;//einde baan
    break;

    case 5: //bomen gedetecteerd
        toestand = 2;//stoppen voor 2 sec en dan verder rijden
    break;

    case 6://einde baan gedetecteerd
        toestand = 7;//volgende baan
    break;

    case 7: //volgende baan beschikbaar?
        //nee = stoppen
        toestand = 8;//ja = bocht maken
    break;

    case 8://bocht maken
        toestand = 2;//terug naar route rijden
    break;

    case 10;//vinger detecteren
        //nee = stop
        //ja = vinger volgen
    break;





    ;

    return 0;
}

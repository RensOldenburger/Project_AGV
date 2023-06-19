#ifndef BLUETOOTH_H_INCLUDED
#define BLUETOOTH_H_INCLUDED

//Bluetooth
#define F_CPU   16000000
#define BUAD    9600
#define BRC     ((F_CPU/16/BUAD)-1)//buad rate calculator

extern int Bluetooth_Getal;
extern int verandergetal;//ONNODIG
extern int veranderandergetal;//ONNODIG
extern int Btgetallen[8];//BEETJE ONNODIG MISSCHIEN NIET HELEMAAL

void init(void);

#endif // LCDENBLUETOOTH_H_INCLUDED

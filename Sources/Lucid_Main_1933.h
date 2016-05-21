#ifndef LUCID_MAIN_H
#define	LUCID_MAIN_H

#define cTrue (1 == 1)
#define cFalse (1 == 0)
#define uint8 unsigned int8
#define uint16 unsigned int16

#include "SFR.h"
////////////////////////////////////////////////////////////////////////////////
#device ADC=16 //ICD=TRUE
//#device ICD=TRUE

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOPUT                    //No Power Up Timer
#FUSES NOMCLR                   //Master Clear pin enabled
#FUSES NOPROTECT                //Code not protected from reading
#FUSES NOCPD                    //No EE protection
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOCLKOUT                 //I/O function on OSC2
#FUSES IESO                     //Internal External Switch Over mode enabled
#FUSES FCMEN                    //Fail-safe clock monitor enabled
#FUSES NOWRT                    //Program memory not write protected
#FUSES NOVCAP                   //VCAP pin disabled
#FUSES STVREN                   //Stack full/underflow will cause reset
#FUSES BORV19                   //Brownout reset at 1.9V
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
////////////////////////////////////////////////////////////////////////////////

#use delay(internal=16000000)

#use rs232(baud=115200,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=PORT1)
#use i2c(Master,Fast,sda=PIN_C4,scl=PIN_C3,force_hw)

////////////////////////////////////////////////////////////////////////////////
void Main_Init(void);

////////////////////////////////////////////////////////////////////////////////
#endif	/* LUCID_MAIN_H */


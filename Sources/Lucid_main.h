#include <16F1503.h>
#device ADC=16

#FUSES NOWDT                 	//No Watch Dog Timer
#FUSES PUT                   	//Power Up Timer
#FUSES NOBROWNOUT            	//No brownout reset
#FUSES NOLVP                 	//No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(internal=16000000)
#use rs232(baud=115200,parity=O,xmit=PIN_A5,rcv=PIN_A4,bits=8,stream=PORT1)
#use i2c(Master,Fast,sda=PIN_C1,scl=PIN_C0,force_hw)

#define cTrue (1 == 1)
#define cFalse (1 == 0)
#define Left (1)
#define Right (0)

#Byte INTCON      =0x0B//0x8B
#Bit  GIE         =INTCON.7
#Bit  TMR0IE      =INTCON.5

#Byte OPTION_REG  =0x095

#Byte TRISA       =0x08C
#Byte TRISC       =0x08E

#Byte TMR0        =0x015

#Byte TMR1L      =0x016
#Byte TMR1H      =0x017
#Byte T1CON      =0x018
#Byte T1GCON     =0x019

#Byte TMR2       =0x01A
#Byte PR2        =0x01B
#Byte T2CON      =0x01C

#Byte PIR1       =0x011
#Bit  TMR2IF     =PIR1.1

#Byte PWM1DCL    =0x611
#Byte PWM1DCH    =0x612
#Byte PWM1CON    =0x613
#Byte PWM2DCL    =0x614
#Byte PWM2DCH    =0x615
#Byte PWM2CON    =0x616
#Byte PWM3DCL    =0x617
#Byte PWM3DCH    =0x618
#Byte PWM3CON    =0x619
#Byte PWM4DCL    =0x61A
#Byte PWM4DCH    =0x61B
#Byte PWM4CON    =0x61C

#Byte CWG1DBR    =0x691
#Byte CWG1DBF    =0x692
#Byte CWG1DBF    =0x692
#Byte CWG1CON0   =0x693
#Byte CWG1CON1   =0x694
#Byte CWG1CON2   =0x695

void SetPWM1Duty(unsigned int16 u16DutyCycleP);
void SetPWM2Duty(unsigned int16 u16DutyCycleP);
void SetPWM3Duty(unsigned int16 u16DutyCycleP);
void SetAllPWM( unsigned int16 u16PWM1P,
                unsigned int16 u16PWM2P,
                unsigned int16 u16PWM3P);
void Timer0_Init(void);
void PWM_Init(void);

unsigned int8 PWMDC[2][3] = {{20,30, 126},{0,0,0}};

unsigned int8 u8IncrementFlag=0;


#include <PWMandTime.h>
#include "Effects.h"

#INT_TIMER0
void Timer0_Interrupt(void)
{
//    unsigned int8 u8LeftLEDL=0;
//after tests TMR is set to this number
//and it is equal to 100us (5kHz)
    TMR0=70;
    //255-70=185
    
    u8ByteFlags |= EffectIncrementFlagMask;
}
////////////////////////////////////////////////////////////////////////////////
void PWM_Incrementation(void)
{
    if(u8PWMCounter <= PWMDC[Right][PWM1])
    {
        R_PWM1 = 1;
    }
    else
    {
        R_PWM1 = 0;
    }
    if(u8PWMCounter <= PWMDC[Right][PWM2])
    {
        R_PWM2 = 1;
    }
    else
    {
        R_PWM2 = 0;
    }

    if(u8PWMCounter <= PWMDC[Right][PWM3])
    {
        R_PWM3 = 1;
    }
    else
    {
        R_PWM3 = 0;
    }
///////////////////
    if(u8PWMCounter <= PWMDC[Left][PWM1])
    {
        L_PWM1 = 1;
    }
    else
    {
        L_PWM1 = 0;
    }

    if(u8PWMCounter <= PWMDC[Left][PWM2])
    {
        L_PWM2 = 1;
    }
    else
    {
        L_PWM2 = 0;
    }
    if(u8PWMCounter <= PWMDC[Left][PWM3])
    {
        L_PWM3 = 1;
    }
    else
    {
        L_PWM3 = 0;
    }
///////////////////

    if(u8PWMCounter >= MaxPWM)
    {
        u8PWMCounter = 0;
    }
    u8PWMCounter++;
}
////////////////////////////////////////////////////////////////////////////////
void Timer0_Init(void)
{
//TIMER0
//Oscilator = 1/(16MHz/4) = 250ns
//Prescaler is 1 : 2    
//
//tick is 500 ns
//TIMER0 = 70
//(500*10^-9 * 185) = 92.5 * 10^-6
//1 / (92.5 * 10-6 * 32) = 449.64 Hz
    //TIMER0
    //Prescaler is 1 : 2,
    //((16MHz/4)/2)
    OPTION_REG &= ~(0b10111111);
    TMR0IE = 1;     //enable_interrupts(INT_TIMER0);
}
////////////////////////////////////////////////////////////////////////////////
void I2C_Reade_Triger_Init(void)
{
//  PORTB0
//  Interupt happens in every 0.5s
    IOCBP |= RTCInterruptPIN; //IOCBP: INTERRUPT-ON-CHANGE POSITIVE EDGE REGISTER
    IOCBN |= RTCInterruptPIN; //IOCBN: INTERRUPT-ON-CHANGE NEGATIVE EDGE REGISTER
    
//WakeUp on RS232
//    IOCBN |= UARTInterruptPIN; //IOCBN: INTERRUPT-ON-CHANGE NEGATIVE EDGE REGISTER
    StartWakeUpFromUART();
    
    TRISB |= (UARTInterruptPIN || RTCInterruptPIN); //PORTB0 PORTB3 is input
    
    OPTION_REG &= ~(0x80); //Weak pull-ups are enabled 
                           //by individual WPUx latch values
    
    WPUB  |= RTCInterruptPIN; //PORTB0 enable PULL-UP resistor for RTCInterruptPIN
    
    IOCBF = 0; // INTERRUPT-ON-CHANGE FLAG REGISTER
    IOCIE = 1; // Interrupt-on-Change Enable bit
}
////////////////////////////////////////////////////////////////////////////////
#INT_RB
void Interrupt_on_Change_ISR(void)
{
    uint8 B = 0;
    
    B = PORTB;
    
    if(RTCInterruptPIN == (IOCBF & RTCInterruptPIN))//1s interupt reade clock
    {
        u8ByteFlags |= OneSecondTaskFlagMask;
        IOCBF &= ~RTCInterruptPIN; // INTERRUPT-ON-CHANGE FLAG REGISTER
    }
    if(UARTInterruptPIN == (IOCBF & UARTInterruptPIN))//UART
    {

//CMNT:0
//???? ???????? ?? ????? ? ??????? ????????? ??? ??????? ?? PORTB
//??? ?? ????????? ?? ????????? ?? ???????,
//?? ?? ???? ?? ????? ?????? ????????? ?? UART
//???? ?????????? ?? ???? ????????? ??? ?????? ?????? 
//?? ????????? ?? ????????? ?? ???????

        IOCBF &= ~UARTInterruptPIN; // INTERRUPT-ON-CHANGE FLAG REGISTER
		StopWakeUpFromUART();
        u8ByteFlags2 |= SleepDelayFlagMask;
        
//        putc('A');
    }

}
////////////////////////////////////////////////////////////////////////////////
void StartWakeUpFromUART(void)
{
    IOCBN |= UARTInterruptPIN;
}
////////////////////////////////////////////////////////////////////////////////
void StopWakeUpFromUART(void)
{
    IOCBN &= ~UARTInterruptPIN;
}
////////////////////////////////////////////////////////////////////////////////
void LEDInit(void)
{
    R_PWM1TRIS = 0;
    R_PWM2TRIS = 0;
    R_PWM3TRIS = 0;

    L_PWM1TRIS = 0;
    L_PWM2TRIS = 0;
    L_PWM3TRIS = 0;

    R_PWM1 = 1;
    R_PWM2 = 1;
    R_PWM3 = 1;

    L_PWM1 = 1;
    L_PWM2 = 1;
    L_PWM3 = 1;
    PWM_Incrementation();
}
////////////////////////////////////////////////////////////////////////////////

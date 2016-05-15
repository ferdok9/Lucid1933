
#include <16F1933.h>

#include "Lucid_Main_1933.h"

#include "RS232RX.c"
//#include "DS1307.c"
#include "DS13XX.c"
#include "PWMandTime.c"
#include "Effects.c"

void Main_Init(void)
{
    while(0 == HFIOFS);
    Delay_ms(200);
    
    putc(13);putc('S');putc('t');putc('a');putc('r');putc('t');
    
    I2C_Reade_Triger_Init();
    Timer0_Init();
    LEDInit();
    
    RCIF = 0;       //Clear USART Receive Interrupt Flag bit
    RCIE = 1;       // Enables the USART receive interrupt  //enable_interrupts(INT_RDA);//
    
    PEIE = 1;       //Peripheral Interrupt Enable bit
    GIE = 1;        //enable_interrupts(GLOBAL);
    
    DS_Init_3by();

    Blink();

}

void Main(void)
{
    uint8 u8ClockL[7];
    Main_Init();
    
    while(TRUE)
    {
        if( (0 != u8ByteFlags)  || (0 != u8ByteFlags2) || (0 != u8StaicByteFlags) )
        {
            if( SetSnoozeDelayFlagMask == ( u8ByteFlags & SetSnoozeDelayFlagMask ) )
            {
                SetSnoozeDelay_3by(cMsgClock);
                u8ByteFlags &= ~SetSnoozeDelayFlagMask;
            }

            if( SetInitialDelayFlagMask == ( u8ByteFlags & SetInitialDelayFlagMask ) )
            {
                SetInitialDelay_3by(cMsgClock);
                u8ByteFlags &= ~SetInitialDelayFlagMask;
            }
            
            if( AddInitialDelayFlagMask == ( u8ByteFlags2 & AddInitialDelayFlagMask ) )
            {
                putc(13);putc('O');putc('l');putc('d');
                DS_Print_Clock_3by(u8Alarm,1);
                putc(13);putc('N');putc('e');putc('w');
                
                DS_Init_Alarm_3by();
                u8ByteFlags2 &= ~AddInitialDelayFlagMask;
            }
            
            if( EffectIncrementFlagMask == ( u8ByteFlags & EffectIncrementFlagMask ) )
            {
                PWM_Incrementation();
                Effects_Task();
                u8ByteFlags &= ~EffectIncrementFlagMask;
            } 

            if( AddSnoozeDelayFlagMask == ( u8ByteFlags & AddSnoozeDelayFlagMask ) )
            {
                AddTimeToAlarm_3by();
                u8ByteFlags &= ~AddSnoozeDelayFlagMask;
            }
            
            if( SetAlarmFlagMask == ( u8ByteFlags & SetAlarmFlagMask ) )
            {
                DS_String_To_Massiv_Clock_3by(u8Alarm,cMsgClock);
                DS_Print_Clock_3by(u8Alarm,1);
                u8ByteFlags &= ~SetAlarmFlagMask;
            }
            
            if( ReadeAlarmFlagMask == ( u8ByteFlags2 & ReadeAlarmFlagMask ) )
            {
                putc(13);putc('R');putc('A');putc('l');putc('a');putc('r');putc('m');
                DS_Print_Clock_3by(&u8Alarm,1);
                u8ByteFlags2 &= ~ReadeAlarmFlagMask;
            }
            
            if( ReadeClockFlagMask == ( u8ByteFlags & ReadeClockFlagMask ) )
            {
                putc(13);putc('R');putc('e');putc('a');putc('d');putc('e');
                putc('C');putc('l');putc('o');putc('c');putc('k');
                DS_Power_Pin = 1;
                Delay_ms(2);
                DS_Read_Clock_3by(u8ClockL);
                DS_Power_Pin = 0;
                
                DS_Print_Clock_3by(u8ClockL,1);  
                u8ByteFlags &= ~ReadeClockFlagMask;
            }
                    
            if( SetClockFlagMask == ( u8ByteFlags & SetClockFlagMask ) )
            {
                if((':' == cMsgClock[5]) && (':' == cMsgClock[2]))
                {
                    putc(13);
                    putc('C');putc('l');putc('o');putc('c');putc('k');
                    putc('S');putc('e');putc('t');
                    IOCIE = 0;// Interrupt-on-Change Enable bit
                    DS_Power_Pin = 1;
                    
                    Delay_ms(2);
                    
                    putc(13);putc('O');putc('l');putc('d');  
                    DS_Read_Clock_3by(u8ClockL);
                    DS_Print_Clock_3by(u8ClockL,1);
                    
                    putc(13);putc('N');putc('e');putc('w');                   
                    DecodeInitPrintClock_3by(cMsgClock);
                    
                    DS_Power_Pin = 0;

                    IOCIE = 1;// Interrupt-on-Change Enable bit
                }                
                u8ByteFlags &= ~SetClockFlagMask;
            }
            if( OneSecondTaskFlagMask == ( u8ByteFlags & OneSecondTaskFlagMask ) )
            {
//                putc(13);putc('D');putc('S');putc('_');putc('T');putc('a');putc('s');putc('k');
                //After receiving of any character the MCU will be awake
                //for 5s(10 rising edge of PIN0 of portB ) 
                if( SleepFlagMask == ( u8StaicByteFlags & SleepFlagMask ) )
                {
                    DS_Power_Pin = 1;
                    Delay_ms(2);
                    DS_Task_Reade_Time_3by();
                    
                    if( SleepDelayFlagMask == ( u8ByteFlags2 & SleepDelayFlagMask) )
                    {
                        u8SleepCountL++;
                        if(10 <= u8SleepCountL)
                        {
                            u8ByteFlags2 &= ~SleepDelayFlagMask;
                            u8SleepCountL = 0;
                            putc(13);putc('S');putc('l');putc('e');putc('e');putc('p');
                        }
                    }
                    else
                    {
                        Delay_ms(2);
//                        TBD 2 ms?
                        DS_Power_Pin = 0;
                        StartWakeUpFromUART();
                        u8ByteFlags &= ~OneSecondTaskFlagMask;
                        Delay_ms(2);
                        SLEEP();
                    }
                }
                else
                {
                    DS_Power_Pin = 1;
                    DS_Task_Reade_Time_3by();
                }
                u8ByteFlags &= ~OneSecondTaskFlagMask;
            }
        }
    }
}

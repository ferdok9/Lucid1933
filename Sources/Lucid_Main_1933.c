
#include <16F1933.h>

#include "Lucid_Main_1933.h"

#include "RS232RX.c"
#include "DS1307.c"
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
    
    DS_Init();
}

void Main(void)
{
    uint8 u8SleepCountL = 0;
    uint8 u8ClockL[7];
    Main_Init();

    while(TRUE)
    {
        if( (0 != u16ByteFlags) || (0 != u8StaicByteFlags) )
        {
            if( SetSnoozeDelayFlagMask == ( u16ByteFlags & SetSnoozeDelayFlagMask ) )
            {
                SetSnoozeDelay_3by(cMsgClock);
                u16ByteFlags &= ~SetSnoozeDelayFlagMask;
            }

            if( SetInitialDelayFlagMask == ( u16ByteFlags & SetInitialDelayFlagMask ) )
            {
                SetInitialDelay_3by(cMsgClock);
                u16ByteFlags &= ~SetInitialDelayFlagMask;
            }

            if( EffectIncrementFlagMask == ( u16ByteFlags & EffectIncrementFlagMask ) )
            {
                Effects_Task();
                u16ByteFlags &= ~EffectIncrementFlagMask;
            } 

            if( AddSnoozeDelayFlagMask == ( u16ByteFlags & AddSnoozeDelayFlagMask ) )
            {
                AddTimeToAlarm();
                u16ByteFlags &= ~AddSnoozeDelayFlagMask;
            }
            
            if( SetAlarmFlagMask == ( u16ByteFlags & SetAlarmFlagMask ) )
            {
                DS_String_To_Massiv_Clock_3by(u8Alarm,cMsgClock);
                DS_Print_Clock_3by(u8Alarm,1);
                u16ByteFlags &= ~SetAlarmFlagMask;
            }

            if( ReadeClockFlagMask == ( u16ByteFlags & ReadeClockFlagMask ) )
            {
                putc(13);putc('R');putc('e');putc('a');putc('d');putc('e');
                putc('C');putc('l');putc('o');putc('c');putc('k');
                DS_Power_Pin = 1;
                Delay_ms(2);
                DS_Read_Clock_3by(u8ClockL);
                DS_Power_Pin = 0;
                
                DS_Print_Clock_3by(u8ClockL,1);  
                u16ByteFlags &= ~ReadeClockFlagMask;
            }
                    
            if( SetClockFlagMask == ( u16ByteFlags & SetClockFlagMask ) )
            {
                if((':' == cMsgClock[5]) && (':' == cMsgClock[2]))
                {
                    putc(13);putc('C');putc('l');putc('o');putc('c');putc('k');
                             putc('S');putc('e');putc('t');
                    IOCIE = 0;// Interrupt-on-Change Enable bit
                    DS_Power_Pin = 1;
                    
                    Delay_ms(2);
                    DecodeInitPrintClock_3by(cMsgClock);
                    
                    Delay_ms(200);
                    DS_Read_Clock_3by(u8ClockL);
                    
                    DS_Power_Pin = 0;
                    
                    DS_Print_Clock_3by(u8ClockL,1);
                    
                    IOCIE = 1;// Interrupt-on-Change Enable bit
                }                
                u16ByteFlags &= ~SetClockFlagMask;
            }
            if( OneSecondTaskFlagMask == ( u16ByteFlags & OneSecondTaskFlagMask ) )
            {
//                putc(13);putc('D');putc('S');putc('_');putc('T');putc('a');putc('s');putc('k');
                if( SleepFlagMask == ( u8StaicByteFlags & SleepFlagMask ) )
                {
                    DS_Power_Pin = 1;
                    Delay_ms(2);
                    DS_Task();
                    if( SleepDelayFlagMask == ( u16ByteFlags2 & SleepDelayFlagMask) )
                    {
                        u8SleepCountL++;
                        if(5 < u8SleepCountL)
                        {
                            u16ByteFlags2 &= ~SleepDelayFlagMask;
                            u8SleepCountL = 0;
                        }
                    }
                    else
                    {
                        Delay_ms(2);
//                        TBD 2 ms?
                        DS_Power_Pin = 0;

                        StartWakeUpFromUART();

                        u16ByteFlags &= ~OneSecondTaskFlagMask;
//                        putc(13);putc('S');putc('l');putc('e');putc('e');putc('p');
                        Delay_ms(2);
                        SLEEP();
                    }
                }
                else
                {
                    DS_Power_Pin = 1;
                    DS_Task();
                }
                u16ByteFlags &= ~OneSecondTaskFlagMask;
            }
        }
    }
}

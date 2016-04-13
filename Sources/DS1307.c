
#include "DS1307.h"
#include "DS13XX.h"

//-------------------- Formats date and time
void Transform_Time() {
//  seconds  =  ((seconds & 0xF0) >> 4)*10 + (seconds & 0x0F);  // Transform seconds
//  minutes  =  ((minutes & 0xF0) >> 4)*10 + (minutes & 0x0F);  // Transform months
//  hours    =  ((hours & 0xF0)  >> 4)*10  + (hours & 0x0F);    // Transform hours
//  year     =   (day & 0xC0) >> 6;                             // Transform year
//  day      =  ((day & 0x30) >> 4)*10    + (day & 0x0F);       // Transform day
//  month    =  ((month & 0x10)  >> 4)*10 + (month & 0x0F);     // Transform month
}

//-------------------- Output values to LCD
void Display_Time() {

//   (day / 10)   + 48);    // Print tens digit of day variable
//   (day % 10)   + 48);    // Print oness digit of day variable
//   (month / 10) + 48);
//   (month % 10) + 48);
//   Lcd_Chr(1,15,  year        + 49);    // Print year vaiable + 1 (start from year 2011)

//    (hours / 10)   + 48);
//    (hours % 10)   + 48);
//    (minutes / 10) + 48);
//    (minutes % 10) + 48);
//    (seconds / 10) + 48);
//    (seconds % 10) + 48);
}

////////////////////////////////////////////////////////////////////////////////
void DS_Task(void)
{
    uint8 DataL[16];
    u16TaskTimeCounterL++;
//    printf("\n\r%Lu",u16TaskTimeCounterL);
    if(1 == u16TaskTimeCounterL)
    {
        DS_Read_Clock(DataL);
        if( DebugPrintFlagMask == ( u8StaicByteFlags & DebugPrintFlagMask ) )
        {
            putc(13);putc('C');putc('l');putc('o');putc('c');putc('k');
            putc(' ');putc(' ');putc(' ');putc(' ');putc(' ');
            putc('A');putc('l');putc('a');putc('r');putc('m');
            
            putc(10);putc(13);
            DS_Print_Clock(DataL);
            putc(' ');putc(' ');
            DS_Print_Clock_3by(u8Alarm,0);
            putc(10);putc(13);
        }
        Menage_Alarma(DataL);

        u16TaskTimeCounterL=0;
    }
}
////////////////////////////////////////////////////////////////////////////////
void DS_Init(void)
{
    uint8 u8WaitForComunicationL=0;
    uint8 u8Data[6];

    DS_Power_Pin_TRIS = 0;//B0 enable communication power of DS1338
    DS_Power_Pin = 1;//outputs B0 is high to power DS1338
    Delay_ms(10);
    do
    {
        u8WaitForComunicationL = DS_Write_Test(1,0b00000100);
        Delay_ms(100);
    }while(0 == u8WaitForComunicationL);

    DS_Init_Clock(0,u8Data);
    DS_Init_OutClock();

}
////////////////////////////////////////////////////////////////////////////////
void Menage_Alarma(uint8 *pDataP)
{
    uint8 u8IsAlarmTimeL = 0;
    DS_Read_Clock(pDataP);
    u8IsAlarmTimeL = Comppare_Time( pDataP, u8Alarm );

    if(1 == u8IsAlarmTimeL)
    {
        u8Moove[5] = 1;
        u1StartFlagGlowAltL = 1;
        u8StaicByteFlags &= ~SleepFlagMask;
    }
    
//    printf("\n\r%d%d:%d%d:%d%d CHAS",pDataP[TenHour],pDataP[Hour],
//                            pDataP[TenMin],pDataP[Min],
//                            pDataP[TenSec],pDataP[Sec]);
//
//    printf("\n\r%d%d:%d%d:%d%d   Alarm",u8Alarm[TenHour],u8Alarm[Hour],
//                                u8Alarm[TenMin],u8Alarm[Min],
//                                u8Alarm[TenSec],u8Alarm[Sec]);

}
////////////////////////////////////////////////////////////////////////////////
void SetAlarm(void)
{
    DS_String_To_Massiv_Clock(u8Alarm,cMsgClock);

//    printf("\n\r%d%d:%d%d:%d%d   Set Alarm",u8Alarm[TenHour],u8Alarm[Hour],
//                                u8Alarm[TenMin],u8Alarm[Min],
//                                u8Alarm[TenSec],u8Alarm[Sec]);
}
////////////////////////////////////////////////////////////////////////////////
void AddTimeToAlarm(void)
{
    uint8 u8AlarmTempL[7];
    uint8 u8SnoozeDelayTempL[7];
    
    putc(13);
    putc('O');putc('l');putc('d');putc('A');putc('l');putc('a');
    putc('r');putc('m');
    putc(' ');putc(' ');
    putc('S');putc('n');putc('o');putc('o');putc('z');putc('e');

    putc(10);putc(13);
    DS_Print_Clock(u8Alarm);
    putc(' ');putc(' ');
    DS_Print_Clock(u8SnoozeDelay);
    putc(10);putc(13);
            
    DigitsToInt ( u8Alarm, u8AlarmTempL );
    DigitsToInt ( u8SnoozeDelay, u8SnoozeDelayTempL );

    AddTimeToClock( u8AlarmTempL, u8SnoozeDelayTempL );
    IntToDigits ( u8AlarmTempL, u8Alarm);

    putc('N');putc('e');putc('w');putc('A');putc('l');putc('a');
    putc('r');putc('m');
    putc(10);putc(13);
    DS_Print_Clock(u8Alarm);
    
}
////////////////////////////////////////////////////////////////////////////////
uint8 Comppare_Time(uint8 *pDataP, uint8 *pAlarmP)
{
    uint8 u8ReturnL = 0;
    if((pDataP[TenHour] == pAlarmP[TenHour]) &&
             (pDataP[Hour] == pAlarmP[Hour]) &&
         (pDataP[TenMin] == pAlarmP[TenMin]) &&
               (pDataP[Min] == pAlarmP[Min]) &&
         (pDataP[TenSec] == pAlarmP[TenSec]) &&
              (pDataP[Sec] == pAlarmP[Sec]))
    {
        u8ReturnL = 1;
    }
    return u8ReturnL;
}
////////////////////////////////////////////////////////////////////////////////
void hour_inc(void)
{
//unsigned char flag_inc=0;
//
//    if( !input(PIN_B4))
//    {
//        delay_ms(170);
//        read_rtc();
//        if(!input_state(pin_B2))
//        {
//            output_bit(PIN_B2,1);
//        }
//        else
//        {
//            output_bit(PIN_B2,0);
//        }
//        flag_inc=1;
//        if((rtenhour==1)|(rtenhour==0))
//        {
//            rhour++;
//            if(rhour==10)
//            {
//                rhour=0;
//                rtenhour++;
//                if(rtenhour==10)
//                {
//                    rtenhour=0;
//                }
//            }
//        }
//
//        if(rtenhour==2)
//        {
//            rhour++;
//            if(rhour==4)
//            {
//                rhour=0;
//                rtenhour=0;
//            }
//        }
//    }
//    if(flag_inc)
//    {
//        rtc_init(1);
//        print_hour_ds1307();
//    }
}
////////////////////////////////////////////////////////////////////////////////
void min_inc(void)
{
//    unsigned char flag_inc=0;
//
//    if( !input(PIN_B3))
//    {
//        delay_ms(170);
//        read_rtc();
//        if(!input_state(pin_B2))
//        {
//            output_bit(PIN_B2,1);
//        }
//        else
//        {
//            output_bit(PIN_B2,0);
//        }
//        flag_inc=1;
//        rmin++;
//        if(rmin==10)
//        {
//            rmin=0;
//            rtenmin++;
//            if(rtenmin==6)
//            {
//                rtenmin=0;
//            }
//        }
//    }
//    if(flag_inc)
//    {
//        rtc_init(1);
//        print_hour_ds1307();
//    }
}
////////////////////////////////////////////////////////////////////////////////
void DS_Init_Clock(uint8 u8FlagInitP, uint8 *pDataP)
{
    uint8 u8TenSecL=5;
    uint8 u8SecL=0;
    uint8 u8TenMinL=5;
    uint8 u8MinL=9;
    uint8 u8TenHourL=2;
    uint8 u8HourL=3;

    if( u8FlagInitP == 1)
    {
        u8TenSecL   = pDataP[TenSec];
        u8SecL      = pDataP[Sec];
        u8TenMinL   = pDataP[TenMin];
        u8MinL      = pDataP[Min];
        u8TenHourL  = pDataP[TenHour];
        u8HourL     = pDataP[Hour];
    }

    u8SecL&=Sec_Mask;
    u8MinL&=Min_Mask;
    u8HourL&=Hour_Mask;

    u8TenSecL <<= 4;
    u8TenMinL <<= 4;
    u8TenHourL <<= 4;

    u8TenSecL &= TenSec_Mask;
    u8TenMinL &= TenMin_Mask;
    u8TenHourL &= TenHour_Mask;

    u8SecL|=u8TensecL;
    u8MinL|=u8TenMinL;
    u8HourL|=u8TenHourL;

    u8SecL &= (~0b10000000);// the oscillator is enabled
    
//    pDataP[Sec]     = (u8SecL & Sec_Mask);
//    pDataP[TenSec]  = ((u8SecL & TenSec_Mask)>>4);
//    pDataP[Min]     = (u8MinL & Min_Mask);
//    pDataP[TenMin]  = ((u8MinL & TenMin_Mask)>>4);
//    pDataP[Hour]    = (u8HourL & Hour_Mask);
//    pDataP[TenHour] = ((u8HourL & TenHour_Mask)>>4);
//
//        printf("\n\r%d%d:%d%d:%d%d",pDataP[TenHour],pDataP[Hour],
//                           pDataP[TenMin],pDataP[Min],
//                           pDataP[TenSec],pDataP[Sec]);


    DS_Write(0,&u8SecL,1);
    DS_Write(1,&u8MinL,1);
    DS_Write(2,&u8HourL,1);
}
////////////////////////////////////////////////////////////////////////////////
void DS_Init_Data(uint8 u8FlagInitP, uint8 *pDataP)
{
    uint8 u8TenDateL    =1;
    uint8 u8DateL       =4;

    uint8 u8TenMonthL   =0;
    uint8 u8MonthL      =3;

    uint8 u8TenYearL    =0;
    uint8 u8YearL       =4;

    if(u8FlagInitP == 1)
    {
        u8TenDateL  = pDataP[TenDate];
        u8DateL     = pDataP[Date];
        u8TenMonthL = pDataP[TenMonth];
        u8MonthL    = pDataP[Month];
        u8TenYearL  = pDataP[TenYear];
        u8YearL     = pDataP[Year];
    }

    u8DateL &= Date_Mask;
    u8MonthL &= Month_Mask;
    u8YearL &= Year_Mask;

    u8TenDateL  <<= 4;
    u8TenMonthL <<= 4;
    u8TenYearL  <<= 4;

    u8TenDateL  &= TenDate_Mask;
    u8TenMonthL &= TenMonth_Mask;
    u8TenYearL  &= TenYear_Mask;

    u8DateL |= u8TenDateL;
    u8MonthL |= u8TenMonthL;
    u8YearL |= u8TenYearL;

    DS_Write(4,&u8DateL,1);
    DS_Write(5,&u8MonthL,1);
    DS_Write(6,&u8YearL,1);

}
////////////////////////////////////////////////////////////////////////////////
void DS_Init_OutClock(void)
{
    uint8 u8ClockOutL = 0;

    DS_Reade_Pointer(0, &u8ClockOutL, 1);
    u8ClockOutL &= (~0b10000000);// the oscillator is enabled
    DS_Write(0,&u8ClockOutL,1);

    u8ClockOutL = 0b00010000;//1 Hz output
    DS_Write(7,&u8ClockOutL,1);
}
////////////////////////////////////////////////////////////////////////////////
void DS_Print_Clock(uint8 *pDataP)
{
//    DS_Read_Clock(pDataP);
//    printf("\n\r%d%d:%d%d:%d%d",pDataP[TenHour],pDataP[Hour],
//                                pDataP[TenMin],pDataP[Min],
//                                pDataP[TenSec],pDataP[Sec]);
//    printf("\n\r");
//    putc(10);
//    putc(13);
    putc(pDataP[TenHour]+48);
    putc(pDataP[Hour]+48);
    putc(58);
    putc(pDataP[TenMin]+48);
    putc(pDataP[Min]+48);
    putc(58);
    putc(pDataP[TenSec]+48);
    putc(pDataP[Sec]+48);
}
////////////////////////////////////////////////////////////////////////////////
void DS_Print_Data(uint8 *pDataP)
{
    DS_Read_Data(pDataP);

//    printf("\n\r%d%d/%d%d/20%d%d  ",pDataP[TenDate],pDataP[Date],
//                           pDataP[TenMonth],pDataP[Month],
//                           pDataP[TenYear],pDataP[Year]);
//    switch(pDataP[Day])
//    {
//        case Monday :
//            printf("Mo");
//            break;
//        case Tuesday :
//            printf("Tu");
//            break;
//        case Wednesday :
//            printf("We");
//            break;
//        case Thursday :
//            printf("Th");
//            break;
//        case Friday :
//            printf("Fr");
//            break;
//        case Saturday :
//            printf("Sa");
//            break;
//        case Sunday :
//            printf("Su");
//            break;
//        default:
//            printf("XX");
//            break;
//    }

}
////////////////////////////////////////////////////////////////////////////////
void DS_Read_Clock(uint8 *pDataP)
{
    uint8 ClockBuff[3];

    DS_Reade_Pointer(0,ClockBuff,3);

    pDataP[Sec]     = (ClockBuff[0] & Sec_Mask);
    pDataP[TenSec]  = ((ClockBuff[0] & TenSec_Mask)>>4);
    pDataP[Min]     = (ClockBuff[1] & Min_Mask);
    pDataP[TenMin]  = ((ClockBuff[1] & TenMin_Mask)>>4);
    pDataP[Hour]    = (ClockBuff[2] & Hour_Mask);
    pDataP[TenHour] = ((ClockBuff[2] & TenHour_Mask)>>4);
}
////////////////////////////////////////////////////////////////////////////////
//this whil write on the addres of the clock
void DS_Read_Data(uint8 *pDataP)
{
    uint8 ClockBuff[4];

    DS_Reade_Pointer(3,ClockBuff,4);

    pDataP[Day]     = (ClockBuff[1] & Day_Mask);

    pDataP[Date]        = (uint8)( ClockBuff[1] & Date_Mask);
    pDataP[TenDate]     = (uint8)((ClockBuff[1] & TenDate_Mask)>>4);
    pDataP[Month]       = (uint8)( ClockBuff[2] & Month_Mask);
    pDataP[TenMonth]    = (uint8)((ClockBuff[2] & TenMonth_Mask)>>4);
    pDataP[Year]        = (uint8)( ClockBuff[3] & Year_Mask);
    pDataP[TenYear]     = (uint8)((ClockBuff[3] & TenYear_Mask)>>4);
}
////////////////////////////////////////////////////////////////////////////////
void DS_Write(uint8 u8AddresP, uint8 *pDataP, uint8 u8NumberOfBytesP)
{
    uint8 u8ByteCountL=1;
    i2c_start();
    i2c_write( DS3231_WriteX );

    i2c_write(u8AddresP);

    for( u8ByteCountL=1; u8ByteCountL <= u8NumberOfBytesP; u8ByteCountL++ )
    {
        i2c_write(pDataP[u8ByteCountL-1]);
    }

    i2c_stop();         // Restart
}///////////////////////////////////////////////////////////////////////////////
uint8 DS_Reade_Pointer(uint8 u8AddresP, uint8 *pDataP, uint8 u8NumberOfBytesP)
{
    uint8 u8ByteCountL=1;
    uint8 pAckL[3];

    i2c_start();
    pAckL[0] = i2c_write( DS3231_WriteX );
    pAckL[1] = i2c_write( u8AddresP );
    pAckL[1] <<= 1;
    i2c_start();
    pAckL[2] = i2c_write( DS3231_ReadX );
    pAckL[2] <<= 2;

    for( u8ByteCountL=1; u8ByteCountL < u8NumberOfBytesP; u8ByteCountL++ )
    {
        pDataP[u8ByteCountL-1] = i2c_read(TRUE);  // Data to device
    }

    pDataP[u8NumberOfBytesP-1] = i2c_read(FALSE);
    i2c_stop();         // Restart

    return ( pAckL[0] | pAckL[1] | pAckL[2] );
}
////////////////////////////////////////////////////////////////////////////////
uint8 DS_Write_Test(unsigned char rtcreg, unsigned int8 rtc_data)
{
//Example    ds1307_write_test(1,0b10000100);
    short ick[4]= {1,1,1,1};
    i2c_start();
    ick[0] = i2c_write(0xd0);
    ick[1] = i2c_write(rtcreg);
    ick[2] = i2c_write(rtc_data);
    i2c_stop();
    if(!(ick[0]||ick[1]||ick[2]))
    {/*lcd_gotoxy(17,1);*/
        putc(13);putc('D');putc('S');putc(' ');putc('O');putc('k');
        //printf("\r\nCommunication is OK");
        return 1;
    }
    else 
    {/*lcd_gotoxy(17,1);*/
        putc(13);putc('D');putc('S');putc(' ');putc('N');putc('O');putc('k');
        //printf("\r\nCommunication is NOT ok");
        return 0;
    }
}
////////////////////////////////////////////////////////////////////////////////
void SetSnoozeDelay(char * cMsgClockP)
{
#ifdef DebugPrintClock
    putc(13);putc('O');putc('l');putc('d');
    putc(10);
    putc(13);
    DS_Print_Clock(u8SnoozeDelay);
#endif
    
	DS_String_To_Massiv_Clock(u8SnoozeDelay,cMsgClockP);
    
#ifdef DebugPrintClock
    putc(13);putc('N');putc('e');putc('w');
	putc(10);
    putc(13);
    DS_Print_Clock(u8SnoozeDelay);
#endif
}
////////////////////////////////////////////////////////////////////////////////
void SetInitialDelay(char * cMsgClockP)
{
#ifdef DebugPrintClock
    putc(13);putc('O');putc('l');putc('d');
	putc(10);
    putc(13);
    DS_Print_Clock(u8InitialDelay);
#endif
    
	DS_String_To_Massiv_Clock(u8InitialDelay,cMsgClockP);
    
#ifdef DebugPrintClock
    putc(13);putc('N');putc('e');putc('w');
    putc(10);
    putc(13);
    DS_Print_Clock(u8InitialDelay);
#endif
}
////////////////////////////////////////////////////////////////////////////////
void DecodeClock(char * cMsgClockP)
{
    unsigned int8 u8ClockSetL[6];
	
    DS_String_To_Massiv_Clock(&u8ClockSetL,cMsgClockP);
    DS_Init_Clock(1, u8ClockSetL);
    putc(10);
    putc(13);
    DS_Print_Clock(u8ClockSetL);
}
////////////////////////////////////////////////////////////////////////////////
void DS_String_To_Massiv_Clock(uint8 * u8ClockSetP, char * cMsgClockP)
{
//   unsigned int8 u8ClockSetL[13];
    u8ClockSetP[Sec]      = (cMsgClockP[7] - 48);
    u8ClockSetP[TenSec]   = (cMsgClockP[6] - 48);

    u8ClockSetP[Min]      = (cMsgClockP[4] - 48);
    u8ClockSetP[TenMin]   = (cMsgClockP[3] - 48);

    u8ClockSetP[Hour]     = (cMsgClockP[1] - 48);
    u8ClockSetP[TenHour]  = (cMsgClockP[0] - 48);

//    printf("\n\r%d%d:%d%d:%d%d string",u8ClockSetP[TenHour],u8ClockSetP[Hour],
//                                u8ClockSetP[TenMin], u8ClockSetP[Min],
//                                u8ClockSetP[TenSec], u8ClockSetP[Sec]);
}
////////////////////////////////////////////////////////////////////////////////
void DecodeData(char * cMsgClock)
{
    unsigned int8 u8DataSetL[6];
    DS_String_To_Massiv_Data(&u8DataSetL,cMsgClock);

    DS_Init_Data(1, u8DataSetL);
//    printf("\n\r");
//    DS_Print_Data(u8DataSetL);
}
////////////////////////////////////////////////////////////////////////////////
void DS_String_To_Massiv_Data(uint8 * u8DateSetP, char * cMsgClock)
{
//    unsigned int8 u8ClockSetL[13];
    u8DateSetP[TenDate]     = (cMsgClock[0] - 48);
    u8DateSetP[Date]        = (cMsgClock[1] - 48);

    u8DateSetP[TenMonth]    = (cMsgClock[3] - 48);
    u8DateSetP[Month]       = (cMsgClock[4] - 48);

    u8DateSetP[TenYear]     = (cMsgClock[8] - 48);
    u8DateSetP[Year]        = (cMsgClock[9] - 48);
//    printf("\n\r%u%u/%u%u/%u%u string",
//                                u8DateSetP[TenDate],u8DateSetP[Date],
//                                u8DateSetP[TenMonth], u8DateSetP[Month],
//                                u8DateSetP[TenYear], u8DateSetP[Year]);
}
////////////////////////////////////////////////////////////////////////////////

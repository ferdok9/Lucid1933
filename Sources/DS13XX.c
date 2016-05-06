
#include "DS13XX_Low_level_com.c"
#include "DS13XX.h"
//#include "DS1307.h"
#include "ClockCalculationTransformation.c"

//3by - 3 bytes
//is in format {23,59,59}

////////////////////////////////////////////////////////////////////////////////
void DS_Print_Clock_3by(uint8 *pDataP, uint8 u8NewLineP)
{
//TBD Print_Data
//   (day / 10)   + 48);    // Print tens digit of day variable
//   (day % 10)   + 48);    // Print oness digit of day variable
//   (month / 10) + 48);
//   (month % 10) + 48);
//   Lcd_Chr(1,15,  year        + 49);    // Print year vaiable + 1 (start from year 2011)
    if(0 != u8NewLineP)
    {
       putc(10);
       putc(13);
    }
    putc((pDataP[MasHour] / 10) + 48);
    putc((pDataP[MasHour] % 10) + 48);
    putc(':');
    putc((pDataP[MasMin]  / 10) + 48);
    putc((pDataP[MasMin]  % 10) + 48);
    putc(':');
    putc((pDataP[MasSec]  / 10) + 48);
    putc((pDataP[MasSec]  % 10) + 48);
}

////////////////////////////////////////////////////////////////////////////////
void DS_Read_Clock_3by(uint8 *pDataP)
{
    uint8 ClockBuff[3];

    DS_Reade_Pointer(0,ClockBuff,3);
//TBD rede Data
    //  year     =   (day & 0xC0) >> 6;                             // Transform year
    //  day      =  ((day & 0x30) >> 4)*10    + (day & 0x0F);       // Transform day
    //  month    =  ((month & 0x10)  >> 4)*10 + (month & 0x0F);     // Transform month
    
    pDataP[MasSec]  = ((ClockBuff[0] & TenSec_Mask)  >> 4)*10 + (ClockBuff[0] & Sec_Mask);
    pDataP[MasMin]  = ((ClockBuff[1] & TenMin_Mask)  >> 4)*10 + (ClockBuff[1] & Min_Mask);
    pDataP[MasHour] = ((ClockBuff[2] & TenHour_Mask) >> 4)*10 + (ClockBuff[2] & Hour_Mask);
}
////////////////////////////////////////////////////////////////////////////////
//cMsgClockP - 23:59:56
void DS_String_To_Massiv_Clock_3by(uint8 * u8ClockSetP, char * cMsgClockP)
{
    u8ClockSetP[MasSec]  = ((cMsgClockP[6] - 48)*10 + (cMsgClockP[7] - 48));
    u8ClockSetP[MasMin]  = ((cMsgClockP[3] - 48)*10 + (cMsgClockP[4] - 48));
    u8ClockSetP[MasHour] = ((cMsgClockP[0] - 48)*10 + (cMsgClockP[1] - 48));
}
////////////////////////////////////////////////////////////////////////////////
void DecodeInitPrintClock_3by(char * cMsgClockP)
{
    unsigned int8 u8ClockSetL[6];
	
    DS_String_To_Massiv_Clock_3by(&u8ClockSetL,cMsgClockP);
    DS_Init_Clock_3by(1, u8ClockSetL);
    DS_Print_Clock_3by(u8ClockSetL,1);
    
}
////////////////////////////////////////////////////////////////////////////////
//pDataP is in format {23,59,59}
   void DS_Init_Clock_3by(uint8 u8FlagInitP, uint8 *pDataP)
{

    uint8 u8SecL=52;
    uint8 u8MinL=59;
    uint8 u8HourL=23;

    if( u8FlagInitP == 1)
    {
        u8SecL  = ((( pDataP[MasSec] / 10 ) << 4 ) & TenSec_Mask ) | (( pDataP[MasSec] % 10 ));
        u8MinL  = ((( pDataP[MasMin] / 10 ) << 4 ) & TenMin_Mask ) | (( pDataP[MasMin] % 10 ));
        u8HourL = ((( pDataP[MasHour] / 10 ) << 4 ) & TenHour_Mask ) | (( pDataP[MasHour] % 10 ));
    }
    
    u8SecL &= (~0b10000000);// the oscillator is enabled

    DS_Write(0,&u8SecL,1);
    DS_Write(1,&u8MinL,1);
    DS_Write(2,&u8HourL,1);
}
////////////////////////////////////////////////////////////////////////////////
void DS_Init_3by(void)
{
    uint8 u8WaitForComunicationL=0;
    uint8 u8Data[3]= {23,59,52};

    DS_Power_Pin_TRIS = 0;//B0 enable communication power of DS1338
    DS_Power_Pin = 1;//outputs B0 is high to power DS1338
    Delay_ms(10);
    do
    {
        u8WaitForComunicationL = DS_Write_Test(1,0b00000100);
        Delay_ms(100);
    }while(0 == u8WaitForComunicationL);

    DS_Init_Clock_3by(1,u8Data);
    DS_Init_OutClock_3by();
}
////////////////////////////////////////////////////////////////////////////////
void DS_Init_Alarm_3by(void)
{
    uint8 u8Data[3]= {23,59,52};
    uint8 u8ForCountL=0;
    
    DS_Power_Pin_TRIS = 0;//B0 enable communication power of DS1338
    DS_Power_Pin = 1;     //outputs B0 is high to power DS1338
    Delay_ms(10);
    DS_Read_Clock_3by(u8Data);
    DS_Print_Clock_3by(u8Data,1);
    
    AddTimeToClock( u8Data, u8InitialDelay );
    
    for(u8ForCountL = 0; 3 > u8ForCountL; u8ForCountL++)
    {
        u8Alarm[u8ForCountL] = u8Data[u8ForCountL];
    }
}
////////////////////////////////////////////////////////////////////////////////
void DS_Init_OutClock_3by(void)
{
    uint8 u8ClockOutL = 0;

    DS_Reade_Pointer(0, &u8ClockOutL, 1);
    u8ClockOutL &= (~0b10000000);// the oscillator is enabled
    DS_Write(0,&u8ClockOutL,1);

    u8ClockOutL = 0b00010000;//1 Hz output
//    u8ClockOutL = 0b00010001;//4.096  kHz output
//    u8ClockOutL = 0b00010010;//8.192  kHz output
//    u8ClockOutL = 0b00010011;//32.768 kHz output
    
    DS_Write(7,&u8ClockOutL,1);
}
////////////////////////////////////////////////////////////////////////////////
void SetSnoozeDelay_3by(char * cMsgClockP)
{
#ifdef DebugPrintClock
    putc(13);putc('O');putc('l');putc('d');
    DS_Print_Clock_3by(u8SnoozeDelay,1);
#endif
    
	DS_String_To_Massiv_Clock_3by(u8SnoozeDelay,cMsgClockP);
    
#ifdef DebugPrintClock
    putc(13);putc('N');putc('e');putc('w');
    DS_Print_Clock_3by(u8SnoozeDelay,1);
#endif
}
////////////////////////////////////////////////////////////////////////////////
void SetInitialDelay_3by(char * cMsgClockP)
{
#ifdef DebugPrintClock
    putc(13);putc('O');putc('l');putc('d');
    DS_Print_Clock_3by(u8InitialDelay,1);
#endif
    
	DS_String_To_Massiv_Clock_3by(u8InitialDelay,cMsgClockP);
    
#ifdef DebugPrintClock
    putc(13);putc('N');putc('e');putc('w');
    DS_Print_Clock_3by(u8InitialDelay,1);
#endif
}
////////////////////////////////////////////////////////////////////////////////
void AddTimeToAlarm_3by(void)
{
    putc(13);
    putc('O');putc('l');putc('d');putc('A');putc('l');putc('a');
    putc('r');putc('m');
    putc(' ');putc(' ');
    putc('S');putc('n');putc('o');putc('o');putc('z');putc('e');

    DS_Print_Clock_3by(u8Alarm, 1);
    putc(' ');putc(' ');
    DS_Print_Clock_3by(u8SnoozeDelay, 0);
    putc(10);putc(13);
            
    AddTimeToClock( u8Alarm, u8SnoozeDelay );

    putc('N');putc('e');putc('w');
    putc('A');putc('l');putc('a');putc('r');putc('m');
    DS_Print_Clock_3by(u8Alarm, 1);
}
////////////////////////////////////////////////////////////////////////////////
void DS_Task_Reade_Time_3by(void)
{
    uint8 DataL[16];
    u16TaskTimeCounterL++;
//    printf("\n\r%Lu",u16TaskTimeCounterL);
    if(1 == u16TaskTimeCounterL)
    {
        DS_Read_Clock_3by(DataL);
        if( DebugPrintFlagMask == ( u8StaicByteFlags & DebugPrintFlagMask ) )
        {
            putc(13);
            putc('C');putc('l');putc('o');putc('c');putc('k');
            putc(' ');putc(' ');putc(' ');putc(' ');putc(' ');
            putc('A');putc('l');putc('a');putc('r');putc('m');
            
            DS_Print_Clock_3by(DataL,1);
            putc(' ');putc(' ');
            DS_Print_Clock_3by(u8Alarm,0);
            putc(10);putc(13);
        }
        Menage_Alarma_3by(&DataL[0]);

        u16TaskTimeCounterL=0;
    }
}
////////////////////////////////////////////////////////////////////////////////
void Menage_Alarma_3by(uint8 *pDataP)
{
    uint8 u8IsAlarmTimeL = 0;
//    DS_Read_Clock(pDataP);
    u8IsAlarmTimeL = Comppare_Time_3by( pDataP, u8Alarm );

    if(1 == u8IsAlarmTimeL)
    {
        AddTimeToClock( u8Alarm, u8SnoozeDelay );
        u8Moove[5] = 1;
        u1StartFlagGlowAltL = 1;
        u8StaicByteFlags &= ~SleepFlagMask;
    }
}
////////////////////////////////////////////////////////////////////////////////
uint8 Comppare_Time_3by(uint8 *pDataP, uint8 *pAlarmP)
{
    uint8 u8ReturnL = 0;
    if( (pDataP[MasSec] == pAlarmP[MasSec]) &&
        (pDataP[MasMin] == pAlarmP[MasMin]) &&
        (pDataP[MasHour] == pAlarmP[MasHour]))
    {
        u8ReturnL = 1;
    }
    return u8ReturnL;
}
////////////////////////////////////////////////////////////////////////////////

#include "DS13XX.h"
#include "DS1307.h"
#include "ClockCalculationTransformation.h"

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

    uint8 u8SecL=59;
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
    uint8 u8AlarmTempL[7];
    uint8 u8SnoozeDelayTempL[7];
    
    putc(13);
    putc('O');putc('l');putc('d');putc('A');putc('l');putc('a');
    putc('r');putc('m');
    putc(' ');putc(' ');
    putc('S');putc('n');putc('o');putc('o');putc('z');putc('e');

    DS_Print_Clock_3by(u8Alarm, 1);
    putc(' ');putc(' ');
    DS_Print_Clock_3by(u8SnoozeDelay, 0);
    putc(10);putc(13);
            
//    DigitsToInt ( u8Alarm, u8AlarmTempL );
//    DigitsToInt ( u8SnoozeDelay, u8SnoozeDelayTempL );

    AddTimeToClock( u8Alarm, u8SnoozeDelay );
//    IntToDigits ( u8AlarmTempL, u8Alarm);

    putc('N');putc('e');putc('w');
    putc('A');putc('l');putc('a');putc('r');putc('m');
    DS_Print_Clock_3by(u8Alarm, 1);
}
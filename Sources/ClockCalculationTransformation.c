
#include "ClockCalculationTransformation.h"
#include "DS13XX.h"

////////////////////////////////////////////////////////////////////////////////
void TestClockInputs(uint8 *pDataP)
{
    
}
////////////////////////////////////////////////////////////////////////////////
#ifndef DS
void DS_Print_Clock(uint8 *pDataP)
{
    putc(10);
    putc(13);
    putc(pDataP[TenHour]+48);
    putc(pDataP[Hour]+48);
    putc(58);
    putc(pDataP[TenMin]+48);
    putc(pDataP[Min]+48);
    putc(58);
    putc(pDataP[TenSec]+48);
    putc(pDataP[Sec]+48);
}
#endif
////////////////////////////////////////////////////////////////////////////////
//void DS_Print_Clock_Mas(uint8 *pDataP)
//{
//    printf("\n\r%d:%d:%d",pDataP[MasHour]
//                         ,pDataP[MasMin]
//                         ,pDataP[MasSec]);
//}

////////////////////////////////////////////////////////////////////////////////
void DS_Print_Clock_Mas(uint8 *pDataP, uint8 u8NewLineP)
{
    DS_Print_Clock_3by(pDataP, u8NewLineP);
}
////////////////////////////////////////////////////////////////////////////////
/*Transform from u8InputP{2,3,5,9,5,9} to u8OutputP{23,59,59};*/
////////////////////////////////////////////////////////////////////////////////
void DigitsToInt (uint8 *u8InputP, uint8 *u8OutputP)
{
    u8OutputP[MasHour] = u8InputP[TenHourCCT] * 10;
    u8OutputP[MasHour] += u8InputP[HourCCT] ;
    
    u8OutputP[MasMin] = u8InputP[TenMinCCT] * 10;
    u8OutputP[MasMin] += u8InputP[MinCCT] ;
    
    u8OutputP[MasSec] = u8InputP[TenSecCCT] * 10;
    u8OutputP[MasSec] += u8InputP[SecCCT] ;
}
////////////////////////////////////////////////////////////////////////////////
//Transform from {23,59,59} to {2,3,5,9,5,9};
////////////////////////////////////////////////////////////////////////////////
void IntToDigits (uint8 *u8InputP, uint8 *u8OutputP)
{
    u8OutputP[TenHourCCT] = u8InputP[MasHour] / 10;
    u8OutputP[HourCCT]    = u8InputP[MasHour] % 10;
    
    u8OutputP[TenMinCCT] = u8InputP[MasMin] / 10;
    u8OutputP[MinCCT]    = u8InputP[MasMin] % 10;
    
    u8OutputP[TenSecCCT] = u8InputP[MasSec] / 10;
    u8OutputP[SecCCT]    = u8InputP[MasSec] % 10;
}
////////////////////////////////////////////////////////////////////////////////
/* u8ClockP     is in format {23,59,59} 
   u8AddedTimeP is in format {23,59,59} */
////////////////////////////////////////////////////////////////////////////////
void AddTimeToClock(uint8 *u8ClockP, uint8 *u8AddedTimeP)
{
    uint8 u8ClockTempL[3] = {0,0,0};
    uint8 u8CarryL = 0;
    uint8 u8ForCounterL = 2;
    
    for( u8ForCounterL = 0; (0 <= u8ForCounterL && 3 > u8ForCounterL); u8ForCounterL++ )
    {
        u8ClockTempL[u8ForCounterL] = u8ClockP[u8ForCounterL] + u8AddedTimeP[u8ForCounterL] + u8CarryL;
        if(2 != u8ForCounterL)
        {
            if( 60 <= u8ClockTempL[u8ForCounterL] ) 
            {
                u8ClockTempL[u8ForCounterL] = u8ClockTempL[u8ForCounterL] - 60;
                u8CarryL = 1;
            }
            else
            {
                u8CarryL = 0;
            }
        }
        else
        {
            if( 24 <= u8ClockTempL[u8ForCounterL] ) 
            {
                u8ClockTempL[u8ForCounterL] = u8ClockTempL[u8ForCounterL] - 24;
            }
        }
        u8ClockP[u8ForCounterL] = u8ClockTempL[u8ForCounterL];
    }
    
}
////////////////////////////////////////////////////////////////////////////////
void DigitsTest (void)
{
//    uint8 u8Alarm[6]= {2,3,5,9,5,9};
    uint8 u8Alarm2[3]= {23,58,49};
    uint8 u8Alarm1[3]= {0,1,11};//{23,59,59};
    uint8 u8ForCounterL = 0;

        
//    {
//        DS_Print_Clock(u8Alarm);
//        DS_Print_Clock_Mas(u8Alarm1);
//
//        DigitsToInt(u8Alarm,u8Alarm1);
//
//        DS_Print_Clock_Mas(u8Alarm1);
//
//        IntToDigits(u8Alarm1,u8Alarm);
//    }
    for( u8ForCounterL = 0; (59 >= u8ForCounterL ); u8ForCounterL++ )
    {
        u8Alarm1[2] = u8ForCounterL;
        
        DS_Print_Clock_Mas(u8Alarm1,1);
        AddTimeToClock(u8Alarm2,u8Alarm1);

        DS_Print_Clock_Mas(u8Alarm2,1);
    }
}
////////////////////////////////////////////////////////////////////////////////
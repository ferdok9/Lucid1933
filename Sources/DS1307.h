


#ifndef DS
#define	DS

#include "ClockCalculationTransformation.c"

//==============================================================================
// PRIVATE MACROS
//==============================================================================
#define     DS3231_WriteX    0xD0
#define     DS3231_ReadX     0xD1

/*          DaynamicFlags       */

#define     SetSnoozeDelayFlagMask      0x0001    //0b00000000 00000001
#define     SetInitialDelayFlagMask     0x0002    //0b00000000 00000010
#define     EffectIncrementFlagMask     0x0004    //0b00000000 00000100
#define     AddSnoozeDelayFlagMask      0x0008    //0b00000000 00001000
#define     SetAlarmFlagMask            0x0010    //0b00000000 00010000
#define     SetClockFlagMask            0x0020    //0b00000000 00100000
#define     OneSecondTaskFlagMask       0x0040    //0b00000000 01000000
#define     ReadeClockFlagMask          0x0080    //0b00000000 10000000
//u16ByteFlags2
#define     SleepDelayFlagMask          0x0001    //0b00000001 00000000

/*          StaticFlags       */
#define     DebugPrintFlagMask          0x01    //0b00000001
#define     SleepFlagMask               0x02    //0b00000010
//TBD
//#define     u1StartFlagGlowAltL         0x04    //0b00000100

#define     DebugPrintClock     0x01

#Bit    DS_Power_Pin_TRIS   =   TRISC.5
#Bit    DS_Power_Pin        =   PORTC.5

void ds1307_write_test(unsigned char rtcreg, unsigned int8 rtc_data);
uint8 DS_Write_Test(unsigned char rtcreg, unsigned int8 rtc_data);

void min_inc(void);
void hour_inc(void);

void DS_Task(void);

void DS_Init(void);
void DS_Init_Clock(uint8 u8FlagInitP, uint8 *pDataP);
void DS_Init_Data(uint8 u8FlagInitP, uint8 *pDataP);
void DS_Init_OutClock(void);

void DS_Read_Clock(uint8 *pDataP);
void DS_Read_Data(uint8 *pDataP);

void DS_Print_Clock(uint8 *pDataP);
void DS_Print_Data(uint8 *pDataP);

void DS_Write(uint8 u8AddresP, uint8 *pDataP, uint8 u8NumberOfBytesP);
void DS_Read(uint8 u8AddresP, uint8 *pDataP, uint8 u8NumberOfBytesP);

void DS_String_To_Massiv_Clock(uint8 * u8ClockSetP, char * cMsgClock);
void DS_String_To_Massiv_Data(uint8 * u8DateSetP, char * cMsgClock);

void SetSnoozeDelay(char * cMsgClockP);
void SetInitialDelay(char * cMsgClockP);

void DecodeClock(char * cMsgClock);
void DecodeData(char * cMsgClock);

uint8 DS_Reade_Pointer(uint8 u8AddresP, uint8 *pDataP, uint8 u8NumberOfBytesP);

uint8 Comppare_Time(uint8 *pDataP, uint8 *pAlarmP);
//uint8 Check_Sequences(uint8 *pDataP);
void Menage_Alarma(uint8 *pDataP);
void SetAlarm(void);
void AddTimeToAlarm(void);

//unsigned int8 rtensec;
//unsigned int8 rsec;
//unsigned int8 rtenmin;
//unsigned int8 rmin;
//unsigned int8 rtenhour;
//unsigned int8 rhour;
//unsigned int8 rday;
//unsigned int8 rtendate;
//unsigned int8 rdate;
//unsigned int8 rmonth;
//unsigned int8 rtenmonth;
//unsigned int8 rtenyear;
//unsigned int8 ryear;

unsigned int16 u16TaskTimeCounterL = 0;

unsigned int8 u8Alarm[6] = {23,59,59,59,0,0};

unsigned int8 u8InitialDelay[6] = {0,0,3,0,0,4};

unsigned int8 u8SnoozeDelay[6] = {0,5,0,9,2,3};


//unsigned int8 u8ReadeClockFlag = 0;
//unsigned int8 u8SetDataFlag = 0;
//unsigned int8 u8DebugPrintFlag = 0;

//unsigned int8 u8SetNapDelayFlag = 0;

unsigned int16 u16ByteFlags = 0;
unsigned int16 u16ByteFlags2 = 0;
unsigned int8 u8StaicByteFlags = (SleepFlagMask );//| DebugPrintFlagMask

#define TenSec      0
#define Sec         1
#define TenMin      2
#define Min         3
#define TenHour     4
#define Hour        5

#define AlarmTenSec      0
#define AlarmSec         1
#define AlarmTenMin      2
#define AlarmMin         3
#define AlarmTenHour     4
#define AlarmHour        5

#define Day         6

#define TenDate     0
#define Date        1
#define Month       2
#define TenMonth    3
#define TenYear     4
#define Year        5

//TBD 0b - 0x
#define Sec_Mask             0b00001111
#define TenSec_Mask          0b01110000
#define Min_Mask             0b00001111
#define TenMin_Mask          0b01110000
#define Hour_Mask            0b00001111
#define TenHour_Mask         0b00110000

#define Day_Mask             0b00000111
#define Date_Mask            0b00001111
#define TenDate_Mask         0b00110000
#define Month_Mask           0b00001111
#define TenMonth_Mask        0b00010000
#define Year_Mask            0b00001111
#define TenYear_Mask         0b11110000

#define Monday      1
#define Tuesday     2
#define Wednesday   3
#define Thursday    4
#define Friday      5
#define Saturday    6
#define Sunday      7

#endif	/* DS*/
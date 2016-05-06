#ifndef DS13XX
#define	DS13XX

//==============================================================================
// MACROS
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
//TBD
//#define     u1StartFlagGlowAltL         0x04    //0b00000100

//u16ByteFlags2
#define     SleepDelayFlagMask          0x0001    //0b00000001 00000000

/*          StaticFlags       */
#define     DebugPrintFlagMask          0x01    //0b00000001
#define     SleepFlagMask               0x02    //0b00000010

#define     DebugPrintClock     0x01

#Bit    DS_Power_Pin_TRIS   =   TRISC.5
#Bit    DS_Power_Pin        =   PORTC.5

//==============================================================================
// VARIABLES
//==============================================================================
unsigned int16 u16TaskTimeCounterL = 0;

 unsigned int8 u8Alarm[3] = {23,59,59};
 unsigned int8 u8InitialDelay[3] = {0,0,3};
 unsigned int8 u8SnoozeDelay[3] = {0,5,0};
 
unsigned int16 u16ByteFlags = 0;
unsigned int16 u16ByteFlags2 = 0;
unsigned int8 u8StaicByteFlags = (SleepFlagMask );//| DebugPrintFlagMask
//==============================================================================
// FUNCTIONS
//==============================================================================
void DS_Print_Clock_3by(uint8 *pDataP, uint8 u8NewLineP);
void DS_Read_Clock_3by(uint8 *pDataP);
void DS_String_To_Massiv_Clock_3by(uint8 * u8ClockSetP, char * cMsgClockP);
void DS_Init_Clock_3by(uint8 u8FlagInitP, uint8 *pDataP);
void DS_Init_3by(void);
void DS_Init_OutClock_3by(void);
void DecodeInitPrintClock_3by(char * cMsgClockP);

void SetSnoozeDelay_3by(char * cMsgClockP);
void SetInitialDelay_3by(char * cMsgClockP);

void AddTimeToAlarm_3by(void);

void DS_Task_Reade_Time_3by(void);
void Menage_Alarma_3by(uint8 *pDataP);
uint8 Comppare_Time_3by(uint8 *pDataP, uint8 *pAlarmP);

#endif
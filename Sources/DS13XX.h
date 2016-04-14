#ifndef DS13XX
#define	DS13XX

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
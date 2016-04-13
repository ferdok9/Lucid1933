#ifndef DS13XX
#define	DS13XX

void DS_Print_Clock_3by(uint8 *pDataP, uint8 u8NewLineP);
void DS_Read_Clock_3by(uint8 *pDataP);
void DS_String_To_Massiv_Clock_3by(uint8 * u8ClockSetP, char * cMsgClockP);
void DS_Init_Clock_3by(uint8 u8FlagInitP, uint8 *pDataP);
void DecodeInitPrintClock_3by(char * cMsgClockP);

void SetSnoozeDelay_3by(char * cMsgClockP);
void SetInitialDelay_3by(char * cMsgClockP);

#endif
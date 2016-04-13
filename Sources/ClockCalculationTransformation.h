
#ifndef ClockCalculationTransformation
#define ClockCalculationTransformation

////////////////////////////////////////////////////////////////////////////////
//CCT - Clock Calculation Transformation
#define TenSecCCT      4
#define SecCCT         5
#define TenMinCCT      2
#define MinCCT         3
#define TenHourCCT     0
#define HourCCT        1

#define MasHour        0
#define MasMin         1
#define MasSec         2
////////////////////////////////////////////////////////////////////////////////
void TestClockInputs(uint8 *pDataP);
#ifndef DS
void DS_Print_Clock(uint8 *pDataP);
#endif
void DS_Print_Clock_Mas(uint8 *pDataP, uint8 u8NewLineP);
void DigitsToInt (uint8 *u8InputP, uint8 *u8OutputP);
void IntToDigits (uint8 *u8InputP, uint8 *u8OutputP);
void AddTimeToClock(uint8 *u8ClockP, uint8 u8AddedTimeP);
void DigitsTest (void);
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

#endif //#define ClockCalculationTransformation
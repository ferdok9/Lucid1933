
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

#define TenSec      0
#define Sec         1
#define TenMin      2
#define Min         3
#define TenHour     4
#define Hour        5

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

#ifndef RTC_SOFT
#define	RTC_SOFT

#include "RTC_Typedefs.h"

#define Blink_Led_Green_TRIS    TRISB2
#define Blink_Led_Green         LATB2

#define Blink_Led_Red_TRIS      TRISB3
#define Blink_Led_Red           LATB3
// Number of days in a leap year
#define Leap-day 29

// Number of days in each month
enum 
{ January , February , March , April , May , June , July , August , September ,October , November , December };

// The month table used in day of week calculation
// January 6 for leap year, February 2 for leap year	
const unsigned char monthsTable [12] = {0,3,3,6,1,4,6,2,5,0,3,5};

// The number of days in each month
const unsigned char DaysInMonth [12]={31,28,31,30,31,30,31,31,30,31,30,31};
    
DATETIME rtcc;
DATETIME alarm;

void Timer1_On(void);
void Timer1_Init(void);
void RTC_INT_Ena();
void RTC_Task(void);
unsigned char DayOfWeek ( DATETIME getDayOfWeek );
void RTC_Print_Clock(DATETIME rtccP, uint8 u8NewLineP);
#endif//RTC_SOFT

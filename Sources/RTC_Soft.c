
#include "RTC_Soft.h"
//#include "RTC_Typedefs.h"


void Timer1_On(void)
{
    TMR1ON 		= 1;//Timer1 On bit 
}
////////////////////////////////////////////////////////////////////////////////
void Timer1_Init(void)
{
	TMR1CS1		= 1;
	TMR1CS0		= 0;
    
	T1CKPS1 	= 0;
	T1CKPS0 	= 0;
	T1SYNC  	= 1;
	T1OSCEN 	= 1;
	TMR1ON 		= 0;//Timer1 On bit 
    TMR1H = 0x80;
    TMR1L = 0;
    
    Blink_Led_Green_TRIS    = 0;
    Blink_Led_Red_TRIS      = 0;
    Blink_Led_Green         = 0;
    Blink_Led_Red           = 0;
    
    rtcc.time.RTChour       = 23;
    
    rtcc.time.RTCminutes    = 59;

    rtcc.time.RTCseconds    = 59;
    
}
////////////////////////////////////////////////////////////////////////////////
void RTC_INT_Ena()
{
//	PEIE  	= 1;		// enable peripheral interrupts
    TMR1IF  = 0;
	TMR1IE 	= 1;		// enable timer 1 interrupt
//	GIE 	= 1;		// enable general interrupts
}
////////////////////////////////////////////////////////////////////////////////
#INT_TIMER1
void Timer1_Interrupt(void)
{
    TMR1H = 0x80;
    TMR1L = 0;

    
    if(Blink_Led_Green == 0)
    {
        Blink_Led_Green = 1;
        Blink_Led_Red   = 0;
    }
    else
    {
        Blink_Led_Green = 0;
        Blink_Led_Red = 1;
    }
    RTC_Task();
    
    TMR1IF = 0;
}
////////////////////////////////////////////////////////////////////////////////
void RTC_Task(void)
{
// service the clock

    rtcc.time.RTCseconds++;
    if ( rtcc.time.RTCseconds >= 60 )
    {
        rtcc.time.RTCseconds = 0;
        rtcc.time.RTCminutes++;
        if ( rtcc.time.RTCminutes >= 60 )
        {
            rtcc.time.RTCminutes = 0;
            rtcc.time.RTChour++;
            if ( rtcc.time.RTChour >= 24 )
            {
                rtcc.time.RTChour = 0;

                // update calendar
                rtcc.date.RTCday++;
                if ( rtcc.date.RTCday > DaysInMonth [rtcc.date.RTCmonth]  )
                {
                    // see if this is a leap year
                    if ( rtcc.date.RTCmonth != February )
                    {
                        rtcc.date.RTCmonth++;
                        rtcc.date.RTCday = 1;
                    }
                    if ( (!(rtcc.date.RTCyear%4)) && (rtcc.date.RTCmonth == February) )  // leap years are multiple of 4
                    {
                        if ( rtcc.date.RTCday > 29)
                        {
                            rtcc.date.RTCmonth++;
                            rtcc.date.RTCday = 1;		
                        }
                    }

                    if  ( rtcc.date.RTCmonth > 12 )
                    {
                        rtcc.date.RTCyear++;
                        rtcc.date.RTCmonth=1;
                        rtcc.date.RTCday =1;
                    }
                }

                // end update calendar

                // get day of week						
                // no need to to day of week here
    //						rtcc.date.RTCdayofweek = DayOfWeek ( rtcc );
            }

        }
    } 
//			
//			// Simple alarm demo, uses an LED as alarm
//			
//			if ( rtcc.date.RTCyear      == alarm.date.RTCyear 	&&
//			     rtcc.date.RTCmonth 	== alarm.date.RTCmonth &&
//			     rtcc.date.RTCday 		== alarm.date.RTCday 	&&
//			     rtcc.time.RTChour      == alarm.time.RTChour 	&&
//			     rtcc.time.RTCminutes	== alarm.time.RTCminutes )
//			{
//				// trigger the alarm
////				LED_4 = 1;
//			}	
//			
//			else
//			{
////				LED_4 = 0;
//			}	
			
        
}
////////////////////////////////////////////////////////////////////////////////
// this function receives the current day as parameter
// and returns the day of week
unsigned char DayOfWeek ( DATETIME getDayOfWeek )
{
	int temp = 0;
	temp += 6;
	temp += getDayOfWeek.date.RTCyear;
	temp += (int) (getDayOfWeek.date.RTCyear>>2);
	temp += monthsTable[getDayOfWeek.date.RTCmonth-1];
	temp += getDayOfWeek.date.RTCday;
	// leap year correction
	if (!(getDayOfWeek.date.RTCyear%4))
	{
		if (getDayOfWeek.date.RTCmonth == January )
			temp += 6;
		if (getDayOfWeek.date.RTCmonth == February )
			temp -= 1;			
	}
	return (temp%7);
	
}	
////////////////////////////////////////////////////////////////////////////////
void RTC_Print_Clock(DATETIME rtccP, uint8 u8NewLineP)
{
    if(0 != u8NewLineP)
    {
       putc(10);
       putc(13);
    }
    putc((rtccP.time.RTChour / 10) + 48);
    putc((rtccP.time.RTChour % 10) + 48);
    putc(':');
    putc((rtccP.time.RTCminutes / 10) + 48);
    putc((rtccP.time.RTCminutes % 10) + 48);
    putc(':');
    putc((rtccP.time.RTCseconds  / 10) + 48);
    putc((rtccP.time.RTCseconds  % 10) + 48);
}

#ifndef _TYPEDEFS
#define _TYPEDEFS


typedef struct 
{
	struct 
	{
		unsigned char RTCSeconds;
		unsigned char RTCMinutes;		
		unsigned char RTCHour;		
	}time;
    
	struct
	{
		unsigned char RTCday;
		unsigned char RTCdayofweek;		
		unsigned char RTCmonth;		
		unsigned char RTCyear;	
 	}date;
    
}  DATETIME; 


#endif
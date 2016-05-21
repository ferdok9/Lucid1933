#include <Effects.h>
#include <DS13XX.h>
#include <PWMandTime.h>
#include "Effects.h"

#define MsgClockLengh 28

void DecodeClockData(char * cMsgClock);
void DC1307_String_To_Massiv_Clock(void);
void DecodeClock(char * cMsgClock);
void DecodeData(char * cMsgClock);
uint8 SurchForDelimiter(char * cMsgClock);

volatile char cMsgClock[MsgClockLengh];

uint8_t u8StateMashine = 0;
uint8_t u8MsgCount = 0;
uint8_t u8SleepCountL = 0;
//#013#010kotarak#013#010
//#013#01013:04:00#013#010
//#013#01018\07\15#013#010
#INT_RDA
void  RDA_isr(void)
{
    char c;
    c = fgetc();
    u8SleepCountL = 0;
//    fputc(c);
   switch(u8StateMashine)
   {
       case 0:
           if(c == 13)
           {
                u8StateMashine = 1;
           }
           else
           {
                u8StateMashine = 0;
           }
           break;

       case 1:
           if(c == 10)
           {
                u8StateMashine = 2;
                u8MsgCount=0;
           }
           else
           {
                u8StateMashine = 0;
           }
           break;

       case 2://loop
           if(c == 13)
           {
                u8StateMashine = 3;
           }
           else
           {
               cMsgClock[u8MsgCount] = c;
               u8MsgCount++;
               //fputc(c);
           }
           break;

       case 3:
            switch(c)
            {
                case 6:// set InitialDelay
                    //#013#01023:59:56#013#07

                    cMsgClock[u8MsgCount] = 0;
                    u8StateMashine = 0;
                    
                    u8ByteFlags |= SetInitialDelayFlagMask;
                    break;
                    
                case 7:// set NapDelay
                    //#013#01023:59:56#013#07

                    cMsgClock[u8MsgCount] = 0;
                    u8StateMashine = 0;
                    
                    u8ByteFlags |= SetSnoozeDelayFlagMask;
                    break;
                    
                case 8:// Alarm Test
                    //#013#010X#013#08
                    cMsgClock[u8MsgCount] = 0;
                    u8StateMashine = 0;
                    
                    u8ByteFlags |= AddSnoozeDelayFlagMask;
                    break;
                    
                case 9:// RESTART
                    //#013#010X#013#09
                    putc(13);putc('R');putc('E');putc('S');putc('T');putc('A');putc('R');putc('T');
                    cMsgClock[u8MsgCount] = 0;
                    u8StateMashine = 0;
                    
                    reset_cpu();
                    break;
                    
                case 10:// set CLOCK
                    //#013#01023:59:56#013#010
                    cMsgClock[u8MsgCount] = 0;
                    u8StateMashine = 0;
                    
                    u8ByteFlags |= SetClockFlagMask;
                    break;

                case 11:// set ALARM
                    //#013#01023:59:56#013#011
                    
                    cMsgClock[u8MsgCount] = 0;
                    u8StateMashine = 0;
                    
                    u8ByteFlags |= SetAlarmFlagMask;
                    break;

                case 12:// Reade Clock
                    //#013#010X#013#012
                    
                    cMsgClock[u8MsgCount] = 0;
                    u8StateMashine = 0;
                    
                    u8ByteFlags |= ReadeClockFlagMask;
                    break;

                case 13:// effect
                    //#013#010X#013#013
                    putc(13);putc('E');putc('f');putc('f');putc('e');putc('k');
                    cMsgClock[u8MsgCount] = 0;
                    u8StateMashine = 0;
                    TMR0IE = 1;
                    u8StaicByteFlags &= ~SleepFlagMask;
//                    TBD set SleepFlagMasks and TIMER
//					u8EffectFlags |= GlowAltLFlagMask;
                    u8EffectFlags |= AltBlinkFlagMask;
                    u8Moove[0] = 1;
                    
                    break;

                case 14:// Debug Print
                    //#013#010X#013#014
                    putc(13);putc('P');putc('r');putc('i');putc('n');putc('t');
                    cMsgClock[u8MsgCount] = 0;
                    u8StateMashine = 0;
                    
                    if( DebugPrintFlagMask == ( u8StaicByteFlags & DebugPrintFlagMask ) )
                    {
                        u8StaicByteFlags &= ~DebugPrintFlagMask;
                    }
                    else
                    {
                        u8StaicByteFlags |= DebugPrintFlagMask;
                    }
                    break;
                    
                case 15:// Sleep
                    //#013#010X#013#015
                    putc(13);putc('S');putc('l');putc('e');putc('e');putc('p');
                    cMsgClock[u8MsgCount] = 0;
                    u8StateMashine = 0;
					
					if( SleepFlagMask == ( u8StaicByteFlags & SleepFlagMask ) )
					{
						u8StaicByteFlags &= ~SleepFlagMask;
                    }
                    else
                    {
                        u8StaicByteFlags |= SleepFlagMask;
                    }
                    break; 
                    
                case 16:// 1s Interupt Enable/Disable
                    //#013#010X#013#016
                    putc(13);putc('I');putc('n');putc('t');putc('E');putc('n');putc('1');putc('6');
                    cMsgClock[u8MsgCount] = 0;
                    u8StateMashine = 0;
                    
                    if(0 == DS_Power_Pin)
                    {
                        DS_Power_Pin = 1;
                        IOCIE = 1;// Interrupt-on-Change Enable bit
                    }
                    else
                    {
                        DS_Power_Pin = 0;
                        IOCIE = 0;// Interrupt-on-Change Enable bit
                    }
                    break; 
                    
                case 17:// Reade Alarm
                    //#013#010X#013#017
                    cMsgClock[u8MsgCount] = 0;
                    u8StateMashine = 0;
                    u8ByteFlags2 |= ReadeAlarmFlagMask;
                    break;
                    
                case 18:// AddInitialDelay
                    //#013#010X#013#018
                    cMsgClock[u8MsgCount] = 0;
                    u8StateMashine = 0;
                    u8ByteFlags2 |= AddInitialDelayFlagMask;
                    break;
                    
                case 19:// Init u8Coeficient
                    //#013#010X#013#019
                    cMsgClock[u8MsgCount] = 0;
                    u8StateMashine = 0;
                    u8Coeficient = (cMsgClock[0] ^ '0');                    
                    break;
                    
                case 20:// Init u8MaxPWMCoef
                    //#013#010X#013#020
                    cMsgClock[u8MsgCount] = 0;
                    u8StateMashine = 0;
                    u8MaxPWMCoef[u8Coeficient] = (uint8_t)get_3ascii(cMsgClock);
                    break;
                    
                case 21:// Init u8EffeCountCoef
                    //#013#010X#013#021
                    cMsgClock[u8MsgCount] = 0;
                    u8StateMashine = 0;
                    u8EffeCountCoef[u8Coeficient] = (uint8_t)get_3ascii(cMsgClock);
                    break;
                    
                default:
                    u8StateMashine = 0;
                    break;
            }
        break;
   }
}
//////////////////////////////////////////////////////////////////
uint8_t get_ascii(uint8_t * p) 
{
   return ((*p) ^ '0');
}
//////////////////////////////////////////////////////////////////
uint8_t hex_to_ascii(uint8_t ch) 
{
   if (ch < 10) return '0' + ch;
   if (ch < 16) return 'A' - 10 + ch;
   return '?';
}
//////////////////////////////////////////////////////////////////
uint8_t get_2ascii(uint8_t * p) 
{
    uint8_t tmp, tmpl;

  tmp = get_ascii(p);
  if (tmp > 9) return 0xFF;
  p++;
  tmpl = get_ascii(p);
  if (tmpl > 9) return 0xFF;
  return tmpl + tmp*10;
}
//////////////////////////////////////////////////////////////////
uint16_t get_3ascii(uint8_t * p) 
{
  uint16_t tmp, tmp1;

  tmp = get_2ascii(p);
  if (tmp == 0xFF) return 0xFFFF;
  p += 2;
  tmp1 = get_ascii(p);
  if (tmp1 > 9) return 0xFFFF;
  return tmp1 + tmp*10;
}

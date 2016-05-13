
#include <Effects.h>

////////////////////////////////////////////////////////////////////////////////
void Effects_Task(void)
{
    u16EffectCounterL++;
    if((u8EffeCountCoef[u8Coeficient]) == u16EffectCounterL)//whit u16EffectCounterL = 19 u8EffeCountCoef[u8Coeficient])
                               //MaxPWM 255 and TIMER0 = 100us
                              //from min PWM to min PWM is 997ms
    {    
        u16EffectCounterL=0;
        if( GlowAltLFlagMask == ( u8EffectFlags & GlowAltLFlagMask ))
        {
            Glow_Alt();//Sweep_Left();//
        }
        if( AltBlinkFlagMask == ( u8EffectFlags & AltBlinkFlagMask ))
        {
            Alt_Blink();//Sweep_Left();//
        }
        if( ZeroingFlagsFlagMask == ( u8EffectFlags & ZeroingFlagsFlagMask ))
        {
            u8EffectFlags = 0;
            BlinkCount = 0;
    //      TBD set SleepFlagMasks
            u8StaicByteFlags |= SleepFlagMask;  
            u8ByteFlags2 |= SleepDelayFlagMask;
            u8SleepCountL = 0;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
void Set_Xth_Bit(uint8 *u8ByteP, uint8 u8XthBitP)
{
    *u8ByteP = 0b00000001;
    if(0 != u8XthBitP)
    {
        *u8ByteP <<= u8XthBitP;
    }
}
////////////////////////////////////////////////////////////////////////////////
void Glow_Alt(void)
{
    uint8 u8CurLEDL=0;
    
    for(u8CurLEDL=0;u8CurLEDL<6;u8CurLEDL++)
    {
        if(0 != u8Moove[u8CurLEDL])
        {
            Set_Xth_Bit(&u8IncMaskFlag, u8CurLEDL);
            if(u8IncMaskFlag == (u8IncPWMFlag & u8IncMaskFlag))//proveriava se koeficienta na
            {                                                  //zapylvane uvelichaca li se ili namaliava
                if(u8MaxPWMCoef[u8Coeficient] <= u8Duty[u8CurLEDL])
                {
                    u8IncPWMFlag &= (~u8IncMaskFlag);//PWM-a zapochva da namaliava
                    NextLed(u8CurLEDL);
                }
                else
                {
                    u8Duty[u8CurLEDL] += 1;
                }
            }
            else
            {
                if(0 == u8Duty[u8CurLEDL])
                {
                    u8Moove[u8CurLEDL] -= 1;//Flaga za dvijenie se nulira
                    u8IncPWMFlag |= u8IncMaskFlag;//Flaga za uvelichavane/namaliavane 
                                                  //se slaga otnovo na uvelichvane
                    if((1 == u8EndOfGlowL) & (MinLED == u8CurLEDL))
                    {
                        u8EndOfGlowL = 0;
                        u8EffectFlags |= ZeroingFlagsFlagMask;
                    }
                }                                 
                else
                {
                    u8Duty[u8CurLEDL] -= 1;
                }
            }
        }
    }

    PWMDC[Right][PWM1] = u8Duty[0];
    PWMDC[Right][PWM2] = u8Duty[1];
    PWMDC[Right][PWM3] = u8Duty[2];
    PWMDC[Left][PWM1]  = u8Duty[3];
    PWMDC[Left][PWM2]  = u8Duty[4];
    PWMDC[Left][PWM3]  = u8Duty[5];

}
////////////////////////////////////////////////////////////////////////////////
void NextLed(uint8 u8CurLEDP)
{
    uint8 u8NextLEDL = 0;
    uint8 u8TempXthBit = 0;

    if(0 != u8IncLEDFlag)//uvelichava li se nomera na LED-a ili namaliava
    {
        if( MaxLED == u8CurLEDP )
        {
            u8IncLEDFlag = 0;
            u8NextLEDL = MaxLED - 1;
        }
        else
        {
            u8NextLEDL = u8CurLEDP + 1;
        }
    }
    else
    {
        if( 0 == u8CurLEDP )
        {
            u8IncLEDFlag = 1;
            u8NextLEDL = 1;
            u8EndOfGlowL = 1;//THIS IS THE EDN
        }
        else
        {
            u8NextLEDL = u8CurLEDP - 1;
        }
    }
    if(0 == u8EndOfGlowL)
    {
        u8Moove[u8NextLEDL] = 1;
        u8Duty[u8NextLEDL] = 0;
        Set_Xth_Bit(&u8TempXthBit, u8NextLEDL);
        u8IncPWMFlag |= u8TempXthBit;
    }
    else
    {
        u8Duty[u8NextLEDL] = 0;
    }
}
////////////////////////////////////////////////////////////////////////////////
void Rotate_Left_Incrementation_Mask(void)
{
    if(0 == (u8IncMaskFlag & 0b11100000))
    {
        u8IncMaskFlag <<= 1;
    }
    else
    {
        u8IncMaskFlag = 0b00000001;
    }
}
////////////////////////////////////////////////////////////////////////////////
void Initial_Effect(void)
{
    u8Moove[0] = 1;
	u8EffectFlags |= GlowAltLFlagMask;
//    u1StartFlagGlowAltL = 1;
    u8StaicByteFlags &= ~SleepFlagMask;
}
////////////////////////////////////////////////////////////////////////////////
void Alt_Blink(void)
{
    u16AltBlinkTimeCounter++;
    if(100 <= u16AltBlinkTimeCounter)
    {
        u16AltBlinkTimeCounter = 0;
        switch(BlinkCount)
        {
            case 0:        
                PWMDC[Right][PWM1] = MaxPWM;
                PWMDC[Right][PWM3] = MaxPWM;

                PWMDC[Left][PWM1]  = MaxPWM;
                PWMDC[Left][PWM3]  = MaxPWM;
                break;
            case 1:        
                PWMDC[Right][PWM1] = 0;
                PWMDC[Right][PWM3] = 0;

                PWMDC[Left][PWM1]  = 0;
                PWMDC[Left][PWM3]  = 0;
                break;
            case 2:        
                PWMDC[Right][PWM1] = MaxPWM;
                PWMDC[Right][PWM3] = MaxPWM;

                PWMDC[Left][PWM1]  = MaxPWM;
                PWMDC[Left][PWM3]  = MaxPWM;
                break;
            case 3:        
                PWMDC[Right][PWM1] = 0;
                PWMDC[Right][PWM3] = 0;

                PWMDC[Left][PWM1]  = 0;
                PWMDC[Left][PWM3]  = 0;
                break;

            case 4:        
                PWMDC[Right][PWM2] = MaxPWM;
                PWMDC[Left][PWM2]  = MaxPWM;
                break;

            case 5:        
                PWMDC[Right][PWM2] = 0;
                PWMDC[Left][PWM2]  = 0;
                break;

            case 6:        
                PWMDC[Right][PWM2] = MaxPWM;
                PWMDC[Left][PWM2]  = MaxPWM;
                break;

            case 7:        
                PWMDC[Right][PWM2] = 0;
                PWMDC[Left][PWM2]  = 0;
                BlinkCount = 0;
                if(1 <= u8AltBlinkRepeetCounter)
                {
                    u8EffectFlags |= ZeroingFlagsFlagMask;
                    u8AltBlinkRepeetCounter = 0;
                }
                else
                {
                    u8AltBlinkRepeetCounter++;
                }
                return;
                break;
        }
        BlinkCount++;
    }
}
////////////////////////////////////////////////////////////////////////////////
//void Sweep_Left(void)
//{
//    if(1 == u1StartFlag1L)
//    {
//        u1StartFlag1L = 0;
//        u8Moove[0] = 1;
//        u8Duty[0] = 0;
//        u1Increment1L = 1;
//    }
//
//    ////////////////////////////////////////////
//    if((1 == u1Increment1L) && (MaxPWM < u8Duty[0]))
//    {
//        u1Increment1L = 0;
//        u8Moove[1] = 1;
//        u8Duty[1] = 0;
//    }
//    if((0 == u1Increment1L) && (0 == u8Duty[0]))
//    {
//        u8Moove[0]--;
//        u1Increment1L = 1;
//    }
//
//    if(0 != u8Moove[0])
//    {
//        if( 1 == u1Increment1L)
//        {
//            u8Duty[0] += 1;
//        }
//        else
//        {
//            u8Duty[0] -= 1;
//        }
//    }
//        ////////////////////////////////////////////
//    if((1 == u1Increment2L) && (MaxPWM < u8Duty[1]))
//    {
//        u1Increment2L = 0;
//        u8Moove[2] = 1;
//        u8Duty[2] = 0;
//    }
//    if((0 == u1Increment2L) && (0 == u8Duty[1]))
//    {
//        u8Moove[1]--;
//        u1Increment2L = 1;
//    }
//    if(0 != u8Moove[1])
//    {
//        if( 1 == u1Increment2L)
//        {
//            u8Duty[1] += 1;
//        }
//        else
//        {
//            u8Duty[1] -= 1;
//        }
//    }
//        ////////////////////////////////////////////
//    if((1 == u1Increment3L) && (MaxPWM < u8Duty[2]))
//    {
//        u1Increment3L = 0;
//        u8Moove[3] = 1;
//        u8Duty[3] = 0;
//    }
//    if((0 == u1Increment3L) && (0 == u8Duty[2]))
//    {
//        u8Moove[2]--;
//        u1Increment3L = 1;
//    }
//    if(0 != u8Moove[2])
//    {
//        if( 1 == u1Increment3L)
//        {
//            u8Duty[2] += 1;
//        }
//        else
//        {
//            u8Duty[2] -= 1;
//        }
//    }
//        ////////////////////////////////////////////
//    if((1 == u1Increment4L) && (MaxPWM < u8Duty[3]))
//    {
//        u1Increment4L = 0;
//        u8Moove[4] = 1;
//        u8Duty[4] = 0;
//    }
//    if((0 == u1Increment4L) && (0 == u8Duty[3]))
//    {
//        u8Moove[3]--;
//        u1Increment4L = 1;
//    }
//    if(0 != u8Moove[3])
//    {
//        if( 1 == u1Increment4L)
//        {
//            u8Duty[3] += 1;
//        }
//        else
//        {
//            u8Duty[3] -= 1;
//        }
//    }
//        ////////////////////////////////////////////
//    if((1 == u1Increment5L) && (MaxPWM < u8Duty[4]))
//    {
//        u1Increment5L = 0;
//        u8Duty[5] = 0;
//        u8Moove[5] = 1;
//    }
//    if((0 == u1Increment5L) && (0 == u8Duty[4]))
//    {
//        u8Moove[4]--;
//        u1Increment5L = 1;
//    }
//    if(0 != u8Moove[4])
//    {
//        if( 1 == u1Increment5L)
//        {
//            u8Duty[4] += 1;
//        }
//        else
//        {
//            u8Duty[4] -= 1;
//        }
//    }
//        ////////////////////////////////////////////
//    if((1 == u1Increment6L) && (MaxPWM < u8Duty[5]))
//    {
//        u1Increment6L = 0;
//        u8Duty[0] = 0;
//        u8Moove[0] = 0;//This whill Loop the effect
//         
//    }
//    if((0 == u1Increment6L) && (0 == u8Duty[5]))
//    {
//        u8Moove[5]--;
//        u1Increment6L = 1;
//        //This is the end of effect
//        //Terminate the effect
//        
//        TMR0IE = 0;
//        u8SleepFlag = 1;
//        //////////////////////////
//    }
//    if(0 != u8Moove[5])
//    {
//        if( 1 == u1Increment6L)
//        {
//            u8Duty[5] += 1;
//        }
//        else
//        {
//            u8Duty[5] -= 1;
//        }
//    }
//
//    PWMDC[Right][PWM1] = u8Duty[0];
//    PWMDC[Right][PWM2] = u8Duty[1];
//    PWMDC[Right][PWM3] = u8Duty[2];
//    PWMDC[Left][PWM1] = u8Duty[3];
//    PWMDC[Left][PWM2] = u8Duty[4];
//    PWMDC[Left][PWM3] = u8Duty[5];
//    
//}
///////////////////////////////////////////////////////////
//void Disable_Port(void)
//{
//    if(0 == u8Moove[0])
//    {
//        R_PWM1TRIS = 1;
//    }
//    if(0 == u8Moove[1])
//    {
//        R_PWM2TRIS = 1;
//    }
//    if(0 == u8Moove[2])
//    {
//        R_PWM3TRIS = 1;
//    }
//    if(0 == u8Moove[3])
//    {
//        L_PWM1TRIS = 1;
//    }
//    if(0 == u8Moove[4])
//    {
//        L_PWM2TRIS = 1;
//    }
//    if(0 == u8Moove[5])
//    {
//        L_PWM3TRIS = 1;
//    }
//    
//}
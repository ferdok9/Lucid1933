
#ifndef EFFECT
#define	EFFECT

#define	MaxLED 5
#define MaxPWM 255

int8 u8IncMaskFlag = 0;

int8 u8IncPWMFlag = 0b00111111;

int8 u8IncLEDFlag = 1;

//int1 u1Increment1L = 1;
//int1 u1Increment2L = 1;
//int1 u1Increment3L = 1;
//int1 u1Increment4L = 1;
//int1 u1Increment5L = 1;
//int1 u1Increment6L = 1;

uint8 u8Moove[6] = { 0, 0, 0, 0, 0, 0 };
//uint8 u8Moove[6] = { 0, 0, 0, 0, 0, 0 };

uint8 u8Duty[6] = { 0, 0, 0, 0, 0, 0 };

//int1 u1StartFlag1L = 0;
int1 u1StartFlagGlowAltL = 0;
//     int1 u1StartFlag2L = 1;
//     int1 u1StartFlag3L = 1;


unsigned int16 u16EffectCounterL = 0;

void Sweep_Left(void);
void Glow_Alt(void);
void Effects_Task(void);
void Rotate_Left_Incrementation_Mask(void);
void Set_Xth_Bit(uint8 *u8ByteP, uint8 u8XthBitP);
void Disable_Port(void);
void NextLed(uint8 u8CurLEDP);
#endif	/* EFFECT*/
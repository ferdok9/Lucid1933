
#ifndef EFFECT
#define	EFFECT

#include "stdint.h"

#define	MaxLED 5
#define MaxPWM 255

uint8_t u8IncMaskFlag = 0;

uint8_t u8IncPWMFlag = 0b00111111;

uint8_t u8IncLEDFlag = 1;

//int1 u1Increment1L = 1;
//int1 u1Increment2L = 1;
//int1 u1Increment3L = 1;
//int1 u1Increment4L = 1;
//int1 u1Increment5L = 1;
//int1 u1Increment6L = 1;

uint8_t u8Moove[6] = { 0, 0, 0, 0, 0, 0 };

uint8_t u8Duty[6] = { 0, 0, 0, 0, 0, 0 };

uint8_t u8MaxPWMCoef[10] =     {255,231,205,180,154,129,102,77,51,26};
uint8_t u8EffeCountCoef[10] = {19,21,25,28,32,39,50,66,99,195};
uint8_t u8Coeficient = 7;

int1 u1StartFlagGlowAltL = 0;

uint16_t u16EffectCounterL = 0;

void Sweep_Left(void);
void Glow_Alt(void);
void Effects_Task(void);
void Rotate_Left_Incrementation_Mask(void);
void Set_Xth_Bit(uint8_t *u8ByteP, uint8_t u8XthBitP);
void Disable_Port(void);
void NextLed(uint8_t u8CurLEDP);

uint8_t get_ascii(uint8_t * p);
uint8_t hex_to_ascii(uint8_t ch);
uint8_t get_2ascii(uint8_t * p);
uint16_t get_3ascii(uint8_t * p);

#endif	/* EFFECT*/
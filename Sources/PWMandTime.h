//#ifndef #define PORTA0 
//    #Bit    PORTA0      =   PORTA.0
//    #Bit    PORTA1      =   PORTA.1
//    #Bit    PORTA2      =   PORTA.2
//    #Bit    PORTA3      =   PORTA.3
//    #Bit    PORTA4      =   PORTA.4
//    #Bit    PORTA5      =   PORTA.5
//    #Bit    PORTA6      =   PORTA.6
//    #Bit    PORTA7      =   PORTA.7
//#endif
//
//#ifndef PORTC0
//    #Bit    PORTC0      =   PORTC.0
//    #Bit    PORTC1      =   PORTC.1
//    #Bit    PORTC2      =   PORTC.2
//    #Bit    PORTC3      =   PORTC.3
//    #Bit    PORTC4      =   PORTC.4
//    #Bit    PORTC5      =   PORTC.5
//    #Bit    PORTC6      =   PORTC.6
//    #Bit    PORTC7      =   PORTC.7
//#endif

#ifndef PWMandTimeFile
#define PWMandTimeFile

#define RTCInterruptPIN		0x01
#define UARTInterruptPIN	0x02

#define Right (0)   //pin_B
#define Left  (1)   //pin_B

#define PWM1    (0)
#define PWM2    (1)
#define PWM3    (2)

#define R_PWM1    PORTA3
#define R_PWM2    PORTA1
#define R_PWM3    PORTA2

#define L_PWM1    PORTA7
#define L_PWM2    PORTA6
#define L_PWM3    PORTA5

#define R_PWM1TRIS    TRISA1//ok
#define R_PWM2TRIS    TRISA2
#define R_PWM3TRIS    TRISA3

#define L_PWM1TRIS    TRISA5
#define L_PWM2TRIS    TRISA7
#define L_PWM3TRIS    TRISA6

void SetAllPWM( unsigned int16 u16PWM1P,
                unsigned int16 u16PWM2P,
                unsigned int16 u16PWM3P);

void Timer0_Init(void);
void Timer2_Init(void);

void PWM_Init(void);

void StartWakeUpFromUART(void);
void StopWakeUpFromUART(void);
void PWM_Incrementation(void);

unsigned int8 PWMDC[2][3] = {{ 0, 0, 0 },{ 0, 0, 0 }};
unsigned int8 u8PWMCounter = 1;

#endif //#define PWMandTimeFile
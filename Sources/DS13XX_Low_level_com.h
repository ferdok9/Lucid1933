
#ifndef DS13XX_LOW_LEVEL_COM
#define DS13XX_LOW_LEVEL_COM


uint8 DS_Write_Test(unsigned char rtcreg, unsigned int8 rtc_data);
uint8 DS_Reade_Pointer(uint8 u8AddresP, uint8 *pDataP, uint8 u8NumberOfBytesP);
void DS_Write(uint8 u8AddresP, uint8 *pDataP, uint8 u8NumberOfBytesP);
void DS_Read(uint8 u8AddresP, uint8 *pDataP, uint8 u8NumberOfBytesP);

#endif //DS13XX_LOW_LEVEL_COM
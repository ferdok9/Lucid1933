
#include "DS13XX_Low_level_com.h"

////////////////////////////////////////////////////////////////////////////////
uint8 DS_Write_Test(unsigned char rtcreg, unsigned int8 rtc_data)
{
//Example    ds1307_write_test(1,0b10000100);
    short ick[4]= {1,1,1,1};
    i2c_start();
    ick[0] = i2c_write(0xd0);
    ick[1] = i2c_write(rtcreg);
    ick[2] = i2c_write(rtc_data);
    i2c_stop();
    if(!(ick[0]||ick[1]||ick[2]))
    {
//        lcd_gotoxy(17,1);
        putc(13);putc('D');putc('S');putc(' ');putc('O');putc('k');
        //printf("\r\nCommunication is OK");
        return 1;
    }
    else 
    {
//      lcd_gotoxy(17,1);
        putc(13);putc('D');putc('S');putc(' ');putc('N');putc('O');putc('k');
        //printf("\r\nCommunication is NOT ok");
        return 0;
    }
}
////////////////////////////////////////////////////////////////////////////////
void DS_Write(uint8 u8AddresP, uint8 *pDataP, uint8 u8NumberOfBytesP)
{
    uint8 u8ByteCountL=1;
    i2c_start();
    i2c_write( DS3231_WriteX );

    i2c_write(u8AddresP);

    for( u8ByteCountL=1; u8ByteCountL <= u8NumberOfBytesP; u8ByteCountL++ )
    {
        i2c_write(pDataP[u8ByteCountL-1]);
    }

    i2c_stop();         // Restart
}
////////////////////////////////////////////////////////////////////////////////
uint8 DS_Reade_Pointer(uint8 u8AddresP, uint8 *pDataP, uint8 u8NumberOfBytesP)
{
    uint8 u8ByteCountL=1;
    uint8 pAckL[3];

    i2c_start();
    pAckL[0] = i2c_write( DS3231_WriteX );
    pAckL[1] = i2c_write( u8AddresP );
    pAckL[1] <<= 1;
    i2c_start();
    pAckL[2] = i2c_write( DS3231_ReadX );
    pAckL[2] <<= 2;

    for( u8ByteCountL=1; u8ByteCountL < u8NumberOfBytesP; u8ByteCountL++ )
    {
        pDataP[u8ByteCountL-1] = i2c_read(TRUE);  // Data to device
    }

    pDataP[u8NumberOfBytesP-1] = i2c_read(FALSE);
    i2c_stop();         // Restart

    return ( pAckL[0] | pAckL[1] | pAckL[2] );
}
////////////////////////////////////////////////////////////////////////////////

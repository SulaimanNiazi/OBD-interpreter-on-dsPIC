/*
 * File:   pps.c
 * Author: raza
 *
 * Created on 26 February 2025, 12:04 PM
 */


#include "xc.h"
#include "pps.h"
#include "mcc_generated_files/uart1.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

/*volatile PP_CONFIG g_PP_CONFIG;
#define FLASH_ADDRES 0x016000
void delay1_ms(uint16_t ms) {
    for (uint16_t i = 0; i < ms; i++) {
        for (uint16_t j = 0; j < 500; j++) {
            asm("nop"); // No-operation instruction
        }
    }
}
const uint8_t c_obd_config_preamble[9] = {0x49,0x52,0x51,0x64,0x65,0x73,0x69,0x67,0x6E};

void restoreDefaultProgrammableParametersOBD(void)
{
    uint16_t k = 0;
    for(k = 0; k <  OBD_ATPP_CONFIG_SIZE; k++)
    {
        WriteFlashWord(FLASH_ADDRES + (k*2),c_defualt_programmable_parameters[k][0]<<8 | c_defualt_programmable_parameters[k][1]);
    }
}
void updateProgrammableParametersOBD(void)
{
    uint16_t k = 0;
    for(k = 0; k <  OBD_ATPP_CONFIG_SIZE; k++)
    {
        WriteFlashWord(FLASH_ADDRES + (k*2),c_custom_programmable_parameters[k][0]<<8 | c_custom_programmable_parameters[k][1]);
    }
}
void printProgrammableParametersOBD(void)
{
    char TXbuf[10];
    uint16_t  k = 0;
    for( k = 0; k < OBD_ATPP_CONFIG_SIZE; k++)
    {
        sprintf(TXbuf,"%02X:%02X %c",k,(ReadFlashWord(FLASH_ADDRES + (k*2))>>8) & (0xFF),((ReadFlashWord(FLASH_ADDRES + (k*2))) & (0xFF)) == 0x00 ? 'N' : 'F');
        UART1_WriteString(TXbuf);
        UART1_WriteString("  ");
        if ((k+1)%4 == 0 && k!=0 )
        {
            UART1_WriteString("\r\n");
        }
    }
    UART1_WriteString("\r\n");
    
}

void TestProgrammableParametersOBD(void)
{
    char TXbuf[10];
    EraseFlashPP(FLASH_ADDRES);
    
    
    WriteFlashWord(FLASH_ADDRES+0,0x1111);
    delay1_ms(50);
    WriteFlashWord(FLASH_ADDRES+2,0x2222);
    delay1_ms(50);
    WriteFlashWord(FLASH_ADDRES+4,0x3333);
    delay1_ms(50);
    WriteFlashWord(FLASH_ADDRES+6,0x4444);
    delay1_ms(50);
    WriteFlashWord(FLASH_ADDRES+8,0x5555);
    
    sprintf(TXbuf,"%X \r\n",(ReadFlashWord(FLASH_ADDRES)));
    UART1_WriteString(TXbuf);
    sprintf(TXbuf,"%X \r\n",(ReadFlashWord(FLASH_ADDRES+2)));
    UART1_WriteString(TXbuf);
    sprintf(TXbuf,"%X \r\n",(ReadFlashWord(FLASH_ADDRES+4)));
    UART1_WriteString(TXbuf);
    sprintf(TXbuf,"%X \r\n",(ReadFlashWord(FLASH_ADDRES+6)));
    UART1_WriteString(TXbuf);
    sprintf(TXbuf,"%X \r\n",(ReadFlashWord(FLASH_ADDRES+8)));
    UART1_WriteString(TXbuf);
    
    EraseFlashPP(FLASH_ADDRES);
    
    
    WriteFlashWord(FLASH_ADDRES+0,0x6666);
    WriteFlashWord(FLASH_ADDRES+2,0x7777);
    WriteFlashWord(FLASH_ADDRES+4,0x8888);
    WriteFlashWord(FLASH_ADDRES+6,0x9999);
    WriteFlashWord(FLASH_ADDRES+8,0x0000);
    
    sprintf(TXbuf,"%X \r\n",(ReadFlashWord(FLASH_ADDRES)));
    UART1_WriteString(TXbuf);
    sprintf(TXbuf,"%X \r\n",(ReadFlashWord(FLASH_ADDRES+2)));
    UART1_WriteString(TXbuf);
    sprintf(TXbuf,"%X \r\n",(ReadFlashWord(FLASH_ADDRES+4)));
    UART1_WriteString(TXbuf);
    sprintf(TXbuf,"%X \r\n",(ReadFlashWord(FLASH_ADDRES+6)));
    UART1_WriteString(TXbuf);
    sprintf(TXbuf,"%X \r\n",(ReadFlashWord(FLASH_ADDRES+8)));
    UART1_WriteString(TXbuf);
    
    
}*/
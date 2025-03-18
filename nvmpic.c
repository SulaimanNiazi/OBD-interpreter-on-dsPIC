/**
* @file        nvmpic.c
* @author      Aqib D. Ace 
* @date        March 2025
* @version     0.0.0
*/

#include "nvmpic.h"
#include "mcc_generated_files/uart1.h"

void delay2_ms(uint16_t ms) {
    for (uint16_t i = 0; i < ms; i++) {
        for (uint16_t j = 0; j < 180; j++) {
            asm("nop"); // No-operation instruction
        }
    }
}
uint8_t read_PPdata[200];  //buffer holds the data after raed operation.
uint32_t flash_storage_address; //variable for storing NVM starting address
bool result;
uint32_t write_data[40]; //buffer that holde the data in 24bit word form before writing to NVM memory
// Allocate and reserve a page of flash for this test to use.  The compiler/linker will reserve this for data and not place any code here.
static __prog__  uint8_t flashTestPage[FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS] __attribute__((space(prog),aligned(FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS)));

// We have detected a flash hardware error of some type.
static void FlashError()
{
    while (1) 
    { }
}

static void MiscompareError()
{
    while (1) 
    { }
}
void getAddress(void)
{
    flash_storage_address = FLASH_GetErasePageAddress((uint32_t)&flashTestPage[0]);
}
void restoreDefaultPParameters(void)
{
    // Get flash page aligned address of flash reserved above for this test.
    getAddress();
   
    FLASH_Unlock(FLASH_UNLOCK_KEY);

    result = FLASH_ErasePage(flash_storage_address);
    if (result == false)
    {
        FlashError();
    }
    
    // Fill first 4 flash words with data
    // For this product we must write two adjacent words at a one time.
   
    // For this product we must write two adjacent words at a one time.
    for (int i = 0 ; i < 16; i++)
    {
        write_data[i*2]     = (((uint32_t)c_def_programmable_parameters[(i*3)+0][0]<<16) | ((uint32_t)c_def_programmable_parameters[(i*3)+0][1]<<8) | (c_def_programmable_parameters[(i*3)+1][0])); //0x00102030;
        write_data[(i*2)+1] = (((uint32_t)c_def_programmable_parameters[(i*3)+1][1]<<16) | ((uint32_t)c_def_programmable_parameters[(i*3)+2][0]<<8) | (c_def_programmable_parameters[(i*3)+2][1])); //0x00405060;
    
        //result  = 
        FLASH_WriteDoubleWord24(flash_storage_address + (i*4),  write_data[i*2], write_data[(i*2)+1]);
    }
   //if (result == false)
    //{
   //     FlashError();
    //}

    // Clear Key for NVM Commands so accidental call to flash routines will not corrupt flash
    FLASH_Lock();
}
void writeToNVM(char* command, char* argument)
{
    // Get flash page aligned address of flash reserved above for this test.
    getAddress();
   
    FLASH_Unlock(FLASH_UNLOCK_KEY);

    if (strcmp(command, "STWBR") == 0)
    {
        for (int i = 0 ; i < 2; i++)//4 words
        {
            write_data[i*2]     = (((uint32_t)argument[(i*6)+0]<<16) | ((uint32_t)argument[(i*6)+1]<<8) | (argument[(i*6)+2])); //0x00102030;
            write_data[(i*2)+1] = (((uint32_t)argument[(i*6)+3]<<16) | ((uint32_t)argument[(i*6)+4]<<8) | (argument[(i*6)+5])); //0x00405060; 
            
            FLASH_WriteDoubleWord24(flash_storage_address + (i*4) + 68,  write_data[i*2], write_data[(i*2)+1]);
        }
    }
    if (strcmp(command, "STSATI") == 0)
    {
        for (int i = 0 ; i < 6; i++)//12 words
        {
            write_data[i*2]     = (((uint32_t)argument[(i*6)+0]<<16) | ((uint32_t)argument[(i*6)+1]<<8) | (argument[(i*6)+2])); //0x00102030;
            write_data[(i*2)+1] = (((uint32_t)argument[(i*6)+3]<<16) | ((uint32_t)argument[(i*6)+4]<<8) | (argument[(i*6)+5])); //0x00405060; 
            
            FLASH_WriteDoubleWord24(flash_storage_address + (i*4) + 76,  write_data[i*2], write_data[(i*2)+1]);
        }
    }
    if (strcmp(command, "STSDI") == 0)
    {
        for (int i = 0 ; i < 8; i++)//16 words
        {
            write_data[i*2]     = (((uint32_t)argument[(i*6)+0]<<16) | ((uint32_t)argument[(i*6)+1]<<8) | (argument[(i*6)+2])); //0x00102030;
            write_data[(i*2)+1] = (((uint32_t)argument[(i*6)+3]<<16) | ((uint32_t)argument[(i*6)+4]<<8) | (argument[(i*6)+5])); //0x00405060; 
            
            FLASH_WriteDoubleWord24(flash_storage_address + (i*4) + 100,  write_data[i*2], write_data[(i*2)+1]);
        }
    }
    if (strcmp(command, "STS@1") == 0)
    {
        for (int i = 0 ; i < 8; i++)//16 words
        {
            write_data[i*2]     = (((uint32_t)argument[(i*6)+0]<<16) | ((uint32_t)argument[(i*6)+1]<<8) | (argument[(i*6)+2])); //0x00102030;
            write_data[(i*2)+1] = (((uint32_t)argument[(i*6)+3]<<16) | ((uint32_t)argument[(i*6)+4]<<8) | (argument[(i*6)+5])); //0x00405060; 
            
            FLASH_WriteDoubleWord24(flash_storage_address + (i*4) + 132,  write_data[i*2], write_data[(i*2)+1]);
        }
    }
    if (strcmp(command, "AT@3") == 0)
    {
        for (int i = 0 ; i < 2; i++)//12 bytes 4 words
        {
            write_data[i*2]     = (((uint32_t)argument[(i*6)+0]<<16) | ((uint32_t)argument[(i*6)+1]<<8) | (argument[(i*6)+2])); //0x00102030;
            write_data[(i*2)+1] = (((uint32_t)argument[(i*6)+3]<<16) | ((uint32_t)argument[(i*6)+4]<<8) | (argument[(i*6)+5])); //0x00405060; 
            
            FLASH_WriteDoubleWord24(flash_storage_address + (i*4) + 164,  write_data[i*2], write_data[(i*2)+1]);
        }
    }
    
    FLASH_Lock();
}
void printPParameters(void)
{
    getAddress();
    // read the flash words to verify the data
    for (int i = 0; i < 16*2 ; i++)
    {
        read_PPdata[(i*3)+0] = FLASH_ReadWord24(flash_storage_address + (i*2))>>16 & 0xFF;
        read_PPdata[(i*3)+1] = FLASH_ReadWord24(flash_storage_address + (i*2))>>8  & 0xFF;
        read_PPdata[(i*3)+2] = FLASH_ReadWord24(flash_storage_address + (i*2))     & 0xFF;
    }
    char TXbuf[30];
    for (int i = 0 ; i<48 ;i++)
    {
        sprintf(TXbuf,"%02X:%02X %c " ,i, read_PPdata[i*2],(read_PPdata[(i*2)+1]==0 ? 'N' : 'F')) ;
        delay2_ms(100);
        UART1_WriteString(TXbuf);   
        if ((i+1)%4==0)
        {
            UART1_Write('\r');
            if(getLFStatus())
            {
                UART1_Write('\n');
            }
        
        }
    }
    
    // Stop if the read data does not match the write data;
//    if ( (write_data[0] != read_data[0]) ||
//         (write_data[1] != read_data[1]) ||
//         (write_data[2] != read_data[2]) ||
//         (write_data[3] != read_data[3]) )
//    {
//        MiscompareError();    
//    }
}

//configute the PPs seperatelly using ATPP
void configurePPs(char* argument)
{
    uint8_t srNo,xx,yy;
    if ( strstr(argument, "ON") != NULL )
    {
        srNo = ((argument[0]-'0')*10)+(argument[1]-'0');
        enablePP(srNo);
    }
    else if(strstr(argument, "OFF") != NULL)
    {
        srNo = ((argument[0]-'0')*10)+(argument[1]-'0');
        disablePP(srNo);
    }
    else if(strstr(argument, "SV") != NULL)
    {
        xx = ((argument[0]-'0')*10)+(argument[1]-'0');
        yy = ((argument[4]-'0')*10)+(argument[5]-'0');
        changePP(xx,yy);
    }
    else
    {
        UART1_Write('?');
    }
}
void enablePP(uint8_t serial)
{
    getAddress();
   
    FLASH_Unlock(FLASH_UNLOCK_KEY);
    
    uint8_t  readPP[6];
    uint32_t writePP[2];
    uint8_t j = (serial*2)/6;
    
    uint8_t k = (serial*2)%6;
    
    for (int i = 0; i < 2 ; i++)
    {
        readPP[(i*3)+0] = FLASH_ReadWord24(flash_storage_address + (j*4) + (i*2) )>>16 & 0xFF;
        readPP[(i*3)+1] = FLASH_ReadWord24(flash_storage_address + (j*4) + (i*2) )>>8  & 0xFF;
        readPP[(i*3)+2] = FLASH_ReadWord24(flash_storage_address + (j*4) + (i*2) )     & 0xFF;
    }
    readPP[k+1]=0x00;
    
    writePP[0] = (((uint32_t)readPP[0]<<16) | ((uint32_t)readPP[1]<<8) | (readPP[2]));
    writePP[1] = (((uint32_t)readPP[3]<<16) | ((uint32_t)readPP[4]<<8) | (readPP[5])); 
    
    FLASH_WriteDoubleWord24(flash_storage_address + (j*4),  writePP[0], writePP[1]);
    
    FLASH_Lock();
     
}
void disablePP(uint8_t serial)
{
    getAddress();
   
    FLASH_Unlock(FLASH_UNLOCK_KEY);
    
    uint8_t  readPP[6];
    uint32_t writePP[2];
    uint8_t j = (serial*2)/6;
    
    uint8_t k = (serial*2)%6;
    
    for (int i = 0; i < 2 ; i++)
    {
        readPP[(i*3)+0] = FLASH_ReadWord24(flash_storage_address + (j*4) + (i*2) )>>16 & 0xFF;
        readPP[(i*3)+1] = FLASH_ReadWord24(flash_storage_address + (j*4) + (i*2) )>>8  & 0xFF;
        readPP[(i*3)+2] = FLASH_ReadWord24(flash_storage_address + (j*4) + (i*2) )     & 0xFF;
    }
    readPP[k+1] = 0xFF;
    
    writePP[0] = (((uint32_t)readPP[0]<<16) | ((uint32_t)readPP[1]<<8) | (readPP[2]));
    writePP[1] = (((uint32_t)readPP[3]<<16) | ((uint32_t)readPP[4]<<8) | (readPP[5])); 
    
    FLASH_WriteDoubleWord24(flash_storage_address + (j*4),  writePP[0], writePP[1]);
    
    FLASH_Lock();    
}
void changePP(uint8_t serial, uint8_t yy)
{
    getAddress();
   
    FLASH_Unlock(FLASH_UNLOCK_KEY);
    
    uint8_t  readPP[6];
    uint32_t writePP[2];
    uint8_t j = (serial*2)/6;
    
    uint8_t k = (serial*2)%6;
    
    for (int i = 0; i < 2 ; i++)
    {
        readPP[(i*3)+0] = FLASH_ReadWord24(flash_storage_address + (j*4) + (i*2) )>>16 & 0xFF;
        readPP[(i*3)+1] = FLASH_ReadWord24(flash_storage_address + (j*4) + (i*2) )>>8  & 0xFF;
        readPP[(i*3)+2] = FLASH_ReadWord24(flash_storage_address + (j*4) + (i*2) )     & 0xFF;
    }
    readPP[k] = yy;
    
    writePP[0] = (((uint32_t)readPP[0]<<16) | ((uint32_t)readPP[1]<<8) | (readPP[2]));
    writePP[1] = (((uint32_t)readPP[3]<<16) | ((uint32_t)readPP[4]<<8) | (readPP[5])); 
    
    FLASH_WriteDoubleWord24(flash_storage_address + (j*4),  writePP[0], writePP[1]);
    
    FLASH_Lock();
}
void printAllSaved(void)
{
    getAddress();
    // read the flash words to verify the data
    for (int i = 0; i < 52 ; i++)
    {
        read_PPdata[(i*3)+0] = FLASH_ReadWord24(flash_storage_address + (i*2) + 68)>>16 & 0xFF;
        read_PPdata[(i*3)+1] = FLASH_ReadWord24(flash_storage_address + (i*2) + 68)>>8  & 0xFF;
        read_PPdata[(i*3)+2] = FLASH_ReadWord24(flash_storage_address + (i*2) + 68)     & 0xFF;
    }
    char TXbuf[30];
    UART1_WriteString("\r\nUART1 baudrate: ");
    for (int i = 0 ; i<12 ;i++) //4 words 12 bytes
    {
        TXbuf[i]=read_PPdata[i];
    }
    UART1_WriteString(TXbuf); 
    UART1_WriteString("\r\nSaved ATI device ID String: ");
    for (int i = 12 ; i<48 ;i++)//12 words 36 bytes
    {
        TXbuf[i-12]=read_PPdata[i];
    }
    UART1_WriteString(TXbuf);
    UART1_WriteString("\r\nSaved Hardware ID device ID String: ");
    for (int i = 48 ; i<96 ;i++)//16 words 48 bytes
    {
        TXbuf[i-48]=read_PPdata[i];
    }
    UART1_WriteString(TXbuf);
    UART1_WriteString("\r\nSaved STS@1 description device ID String: ");
    for (int i = 96 ; i<144 ;i++)//16 words 48 bytes
    {
        TXbuf[i-96]=read_PPdata[i];
    }
    UART1_WriteString(TXbuf);
    UART1_WriteString("\r\nSaved AT@3 description device ID String: ");
    for (int i = 144 ; i<156 ;i++)//16 words 48 bytes
    {
        TXbuf[i-144]=read_PPdata[i];
    }
    UART1_WriteString(TXbuf);
    
}
void printSaved(char* command)
{
    getAddress();
    // read the flash words to verify the data
    for (int i = 0; i < 52 ; i++)
    {
        read_PPdata[(i*3)+0] = FLASH_ReadWord24(flash_storage_address + (i*2) + 68)>>16 & 0xFF;
        read_PPdata[(i*3)+1] = FLASH_ReadWord24(flash_storage_address + (i*2) + 68)>>8  & 0xFF;
        read_PPdata[(i*3)+2] = FLASH_ReadWord24(flash_storage_address + (i*2) + 68)     & 0xFF;
    }
    char TXbuf[30];
    if(strcmp(command,"baudrate") == 0)
    {
        for (int i = 0 ; i<12 ;i++) //4 words 12 bytes
        {
            TXbuf[i]=read_PPdata[i];
        }
        UART1_WriteString(TXbuf);
    }
    if(strcmp(command,"ATI") == 0)
    {
        for (int i = 12 ; i<48 ;i++)//12 words 36 bytes
        {
            TXbuf[i-12]=read_PPdata[i];
        }
        UART1_WriteString(TXbuf);
    }
    if(strcmp(command,"STSDI") == 0)
    {
        for (int i = 48 ; i<96 ;i++)//16 words 48 bytes
        {
            TXbuf[i-48]=read_PPdata[i];
        }
        UART1_WriteString(TXbuf);
    }
    if(strcmp(command,"STS@1") == 0)
    {
        for (int i = 96 ; i<144 ;i++)//16 words 48 bytes
        {
            TXbuf[i-96]=read_PPdata[i];
        }
        UART1_WriteString(TXbuf);
    }
    if(strcmp(command,"AT@3") == 0)
    {
        for (int i = 144 ; i<156 ;i++)//16 words 48 bytes
        {
            TXbuf[i-144]=read_PPdata[i];
        }
        UART1_WriteString(TXbuf);
    }
}
//uint32_t returnread(int i)
//{
    
//   return (uint32_t)read_data[i];
//}
    
static void PageWritexample()
{
    uint32_t flash_storage_address,flashOffset;
    bool result;
    uint32_t readData;

    // Get flash page aligned address of flash reserved above for this test.
    flash_storage_address = FLASH_GetErasePageAddress((uint32_t)&flashTestPage[0]);


    FLASH_Unlock(FLASH_UNLOCK_KEY);
    
    // ------------------------------------------
    // Fill a page of memory with data.  
    // ------------------------------------------
    
    // Erase the page of flash at this address
    result = FLASH_ErasePage(flash_storage_address);
    if (result == false)
    {
        FlashError();
    }
  
    // Program flash with a data pattern.  For the data pattern we will use the index 
    // into the flash as the data.
    for (flashOffset= 0U; flashOffset< FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS; flashOffset += 4U)
    {
        result = FLASH_WriteDoubleWord24(flash_storage_address+flashOffset, flashOffset, flashOffset+2U);
        if (result == false)
        {
            FlashError();
        }   
    }

    // Clear Key for NVM Commands so accidental call to flash routines will not corrupt flash
    FLASH_Lock();

    
    // Verify the flash data is correct.  If it's not branch to error loop.
    // The data in the flash is the offset into the flash page.
    for (flashOffset= 0U; flashOffset< FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS; flashOffset += 2U)
    {
        readData = FLASH_ReadWord24(flash_storage_address+flashOffset);
        if (readData != flashOffset )
        {
            MiscompareError();
        }   
    }

}


void FlashDemo()
{
    //WordWriteExample();
    //PageWritexample();
}

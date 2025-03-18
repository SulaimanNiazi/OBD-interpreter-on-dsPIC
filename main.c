//updated 05 march 1:19PM


#include "mcc_generated_files/system.h"
#include "mcc_generated_files/adc1.h"

char receivedCmd[20]; //buffer for commands

void delay_ms(uint16_t ms) { //temporary delay function
    for (uint16_t i = 0; i < ms; i++) {
        for (uint16_t j = 0; j < 180; j++){
            asm("nop"); // No-operation instruction
        }
    }
}

int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    delay_ms(200);
    UART1_Read(); //temporary startup
    UART1_Write_String("ELM327 v1.4b\r\n"); ///ATI string received on connection
    UART1_Write('>');
    while (1)
    {
        // wait for the command and process it
        
        ADC1_Enable();
        ADC1_ChannelSelect(channel_AN0);
        ADC1_SoftwareTriggerEnable();
        //Provide Delay
        delay_ms(200);
        ADC1_SoftwareTriggerDisable();
        while(!ADC1_IsConversionComplete(channel_AN0));
        printf("pot value: %d\r\n", ADC1_ConversionResultGet(channel_AN0));
        ADC1_Disable(); 
        UART1_Read();
        
        /*
        UART1_ReadString(receivedCmd, 20);
        processCommand(receivedCmd);
         */
    }
    return 1;
}

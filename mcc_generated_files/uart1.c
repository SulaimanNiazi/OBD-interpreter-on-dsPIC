#include <xc.h>
#include <string.h>
#include "uart1.h"

#define FCLK 7370000
char backupBuffer[100];

void UART1_Initialize(uint32_t baudrate)
{
/**    
     Set the UART1 module to the options selected in the user interface.
     Make sure to set LAT bit corresponding to TxPin as high before UART initialization
*/
    // STSEL 1; IREN disabled; PDSEL 8N; UARTEN enabled; RTSMD disabled; USIDL disabled; WAKE disabled; ABAUD disabled; LPBACK disabled; BRGH enabled; URXINV disabled; UEN TX_RX; 
    U1MODE = (0x8008 & ~(1<<15));  // disabling UARTEN bit   
    // UTXISEL0 TX_ONE_CHAR; UTXINV disabled; OERR NO_ERROR_cleared; URXISEL RX_ONE_CHAR; UTXBRK COMPLETED; UTXEN disabled; ADDEN disabled; 
    U1STA = 0x0;
    // BaudRate = 9600; Frequency = 7984164 Hz; BRG 207; 
    
    U1BRG = ((FCLK/baudrate)/16)-1;
    //0xCF;
    
    U1MODEbits.UARTEN = 1;  // enabling UARTEN bit
    U1STAbits.UTXEN = 1;   
}

uint32_t UART1_GetBRdiff(uint32_t brate)
{
    
    uint32_t brate_cal = ((FCLK/brate)/16)-1; //33 max
    uint32_t brate_act = FCLK/(16 * (brate_cal + 1));//14676 max baudrate with no error
    
    float Error;
    
    if(brate > brate_act)
    {
        Error = (brate - brate_act);
        Error = Error / brate;
    }
    else
    {
        Error = (brate_act - brate);
        Error = Error / brate;
    }
    Error = Error * 100;
    return (uint32_t)Error;
}

uint8_t UART1_Read(void)
{
    while(!(U1STAbits.URXDA == 1))
    {
    }

    if ((U1STAbits.OERR == 1))
    {
        U1STAbits.OERR = 0;
    }

    return U1RXREG;
}
void UART1_ReadString(char *buffer, uint16_t maxLength)
{
    uint16_t i = 0;
    while (i < (maxLength - 1)) // Leave space for null terminator
    {
        
        buffer[i] = UART1_Read(); // Read the character
        //check Echo status adjustable by ATE
        if(getEchoStatus())
        {
            UART1_Write(buffer[i]);
        }
        
        // <CR> command handling
        if (buffer[i] == '\r' && i == 0)
        {
            strcpy(buffer,backupBuffer);
            break;
        }
        //backspace handling
        if (buffer[i] == '\b' && i>0)
        {
            if(getEchoStatus())
            {
                UART1_Write(' ');
                UART1_Write(buffer[i]);
            }
            i--;   
        }
        
        // Check for enter key (assuming '\n' or '\r' as the delimiter)
        else if (buffer[i] == '\n' || buffer[i] == '\r')
        {
            buffer[i]='\0';
            strcpy(backupBuffer,buffer);
            break;
        }
        else
        {
            i++;
        }
    }
}

void UART1_Write(char txData)
{
    while(U1STAbits.UTXBF);
    U1TXREG = txData;    // Write the data byte to the USART.
}

void UART1_WriteString(const char *str)
{
    while (*str != '\0') // Loop until the null terminator
    {
        while (U1STAbits.UTXBF == 1) // Wait if the transmit buffer is full
        {
            // Optional: Add a timeout here if necessary
        }
        U1TXREG = *str; // Send the current character
        str++; // Move to the next character
    }
}

bool UART1_IsRxReady(void)
{
    return U1STAbits.URXDA;
}

bool UART1_IsTxReady(void)
{
    return (U1STAbits.TRMT && U1STAbits.UTXEN );
}

bool UART1_IsTxDone(void)
{
    return U1STAbits.TRMT;
}

/* !!! Deprecated API - This function may not be supported in a future release !!! */
UART1_STATUS __attribute__((deprecated)) UART1_StatusGet (void)
{
    return U1STA;
}

/* !!! Deprecated API - This function may not be supported in a future release !!! */
bool __attribute__((deprecated)) UART1_DataReady(void)
{
    return UART1_IsRxReady();
}

/* !!! Deprecated API - This function may not be supported in a future release !!! */
bool __attribute__((deprecated)) UART1_is_tx_ready(void)
{
    return UART1_IsTxReady();
}

/* !!! Deprecated API - This function may not be supported in a future release !!! */
bool __attribute__((deprecated)) UART1_is_rx_ready(void)
{
    return UART1_IsRxReady();
}

/* !!! Deprecated API - This function may not be supported in a future release !!! */
bool __attribute__((deprecated)) UART1_is_tx_done(void)
{
    return UART1_IsTxDone();
}


#include "xc.h"
#include "mcc_generated_files/system.h"
#include "PM.h"
#include <stdbool.h>
#include <stdlib.h>

char tempCommand[32];
unsigned long UART1_Timeout;
uint8_t UART1FlowCtrl = 1;
unsigned long baudrate;
unsigned long oldbaudrate = 9600;
char TXbuffer[20];
uint32_t BRError = 0;
char receivedChar;
//bool argErrFlag = false;

void processCommand(char *command)
{
    int j = 0;
    
    // Remove spaces from command
    for (int i = 0; command[i] != '\0' && j < sizeof(tempCommand) - 1; i++) {
    if (command[i] != ' ') {
        tempCommand[j++] = command[i];
    }
    }
    tempCommand[j] = '\0'; // Null-terminate the modified command

    for (int i = 0; tempCommand[i] != '\0'; i++)
    {
        if((tempCommand[i] >= 'a') && (tempCommand[i] <= 'z'))
        {
            tempCommand[i] = (tempCommand[i] - 'a') + 'A';
        }
    }

    if (tempCommand[0] == 'S' && tempCommand[1] == 'T')
    {
        processSTCommand(tempCommand);
    }
    else if (tempCommand[0] == 'A' && tempCommand[1] == 'T')
    {
        processATCommand(tempCommand);
    }
    else if (tempCommand[0] == '0')
    {
        processOBDRequest(tempCommand);
    }
    else
    {
        UART1_Write('?');
    }
    UART1_Write('\r');
    UART1_Write('>');
}

bool argErrCheck(char command[20])
{
    for (int i = 0; command[i] != '\0'; i++)
            {
                if((command[i] < '0') || (command[i] > '9'))
                {
                    return 1;
                    break;
                }
            }
}
void processSTCommand(char *command) {
    if  (strncmp(command, "STBRT", 5) == 0)
    {
        char *Uart1TimeoutStr = command + 5;
        if (argErrCheck(Uart1TimeoutStr))
        {
            UART1_Write("?");
        }
        else
        {
        unsigned long Uart1Timeout = strtoul(Uart1TimeoutStr, NULL, 10);
        if (Uart1Timeout > 0 && Uart1Timeout < 65536) 
        {
            UART1_Timeout = Uart1Timeout;
            UART1_Write('O');
            UART1_Write('K');
        }
        else
        {
            UART1_Write("?");
        }
        
        }
    }
    // Add more commands here
    else if(strcmp(command, "STSLCS") == 0){
        UART1_Write('O');
        UART1_Write('K');
        UART1_Write('\n');
        PM_LCS();
    }
    else if(strcmp(command, "STSLLT") == 0){
        UART1_Write('O');
        UART1_Write('K');
        UART1_Write('\n');
        PM_STSLLT();
    }
    else if(strncmp(command, "STSLEEP", 7) == 0){
        UART1_Write('O');
        UART1_Write('K');
        UART1_Write('\n');
        char val1[20];
        uint16_t index = 7, num;
        for(index; command[index] != '\0' && command[index] != ','; index++){
            val1[index-7] = command[index];
        }
        num = strtoul(val1, NULL, 10);
        PM_Sleep("CMD", 65536>num>=0?num:0);
    }
    else if(strncmp(command, "STSLXP", 6) == 0){
        if(command[6] == '0'||command[6] == '1'){
            UART1_Write('O');
            UART1_Write('K');
            UART1_Write('\n');
            PM_STSLXP(command[6]=='1'?true:false);
        }else{
            UART1_Write("?");
        }
    }
    else if(strncmp(command, "STSLPCP", 7) == 0){
        if(command[7]=='0'||command[7]=='1'){
            UART1_Write('O');
            UART1_Write('K');
            UART1_Write('\n');
            PM_STSLPCP(command[7]=='1'?true:false);
        }else{
            UART1_Write('?');
            UART1_Write('\n');
        }
    }
    else if(strncmp(command, "STSLUIT", 7) == 0){
        char val1[10]="";
        uint16_t index = 7;
        for(index; command[index] != '\0'; index++){
            val1[index-7] = command[index];
        }
        uint16_t num = strtoul(val1, NULL, 10);
        if(num>=0 || num<65536){
            UART1_Write('O');
            UART1_Write('K');
            UART1_Write('\n');
            PM_STSLVG(num);
        }else{
            UART1_Write('?');
            UART1_Write('\n');
        }
    }
    else if(strncmp(command, "STSLUWP", 7) == 0){
        char val1[10]="", val2[10]="";
        uint16_t index = 7;
        for(index; command[index] != '\0' && command[index] != ','; index++){
            val1[index-7] = command[index];
        }
        for(uint16_t index2 = index + 1; command[index2] != '\0'; index2++){
            val2[index2-index-1] = command[index2];
        }
        uint16_t num1 = strtoul(val1, NULL, 10), num2 = strtoul(val2, NULL, 10);
        if((num1 >= 0 && num1 < 65536)&&(num2 >= 0 && num2 < 65536)){
            UART1_Write('O');
            UART1_Write('K');
            UART1_Write('\n');
            PM_STSLUWP(num1, num2);
        }
        else{
            UART1_Write('?');
            UART1_Write('\n');
        }
    }
    else if(strncmp(command, "STSLVGW", 7) == 0){
        char val1[10]="", val2[10]="";
        uint16_t index = 7;
        for(index; command[index] != '\0' && command[index] != ','; index++){
            val1[index-7] = command[index];
        }
        for(uint16_t index2 = index + 1; command[index2] != '\0'; index2++){
            val2[index2-index-1] = command[index2];
        }
        float num1 = strtof(val1, NULL);
        uint16_t num2 = strtoul(val2, NULL, 10);
        if((num1 >= -10 && num1 < 10)&&(num2 >= 0 && num2 < 65536)){
            UART1_Write('O');
            UART1_Write('K');
            UART1_Write('\n');
            PM_STSLVGW(num1, num2);
        }
        else{
            UART1_Write('?');
            UART1_Write('\n');
        }
    }
    else if(strncmp(command, "STSLVG", 6) == 0){
        char val1[10]="";
        uint16_t index = 6;
        for(index; command[index] != '\0'; index++){
            val1[index-6] = command[index];
        }
        if(strncmp(val1, "ON", 2)==0||strncmp(val1, "OFF", 3)==0){
            UART1_Write('O');
            UART1_Write('K');
            UART1_Write('\n');
            PM_STSLVG(strncmp(val1, "ON", 2)==0?true:false);
        }
        else{
            UART1_Write('?');
            UART1_Write('\n');
        }
    }
    else if(strncmp(command, "STSLVLS", 7) == 0){
        char val1[10]="", val2[10]="";
        uint16_t index = 8;
        for(index; command[index] != '\0' && command[index] != ','; index++){
            val1[index-8] = command[index];
        }
        for(uint16_t index2 = index + 1; command[index2] != '\0'; index2++){
            val2[index2-index-1] = command[index2];
        }
        float volt = strtof(val1, NULL);
        uint16_t time = strtoul(val2, NULL, 10);
        if((volt >= 0 && volt < 4095)&&(time > 0 && time < 65536)&&(command[7] == '>' || command[7] == '<')){
            UART1_Write('O');
            UART1_Write('K');
            UART1_Write('\n');
            PM_STSLVLS(command[7], volt, time);
        }
        else{
            UART1_Write('?');
            UART1_Write('\n');
        }
    }
    else if(strncmp(command, "STSLVLW", 7) == 0){
        char val1[10]="", val2[10]="";
        uint16_t index = 8;
        for(index; command[index] != '\0' && command[index] != ','; index++){
            val1[index-8] = command[index];
        }
        for(uint16_t index2 = index + 1; command[index2] != '\0'; index2++){
            val2[index2-index-1] = command[index2];
        }
        float volt = strtof(val1, NULL);
        uint16_t time = strtoul(val2, NULL, 10);
        if((volt >= 0 && volt < 4095)&&(time > 0 && time < 65536)&&(command[7] == '>' || command[7] == '<')){
            UART1_Write('O');
            UART1_Write('K');
            UART1_Write('\n');
            PM_STSLVLW(command[7], volt, time);
        }
        else{
            UART1_Write('?');
            UART1_Write('\n');
        }
    }
    else if(strncmp(command, "STSLVL", 6) == 0){
        char val1[10]="", val2[10]="";
        uint16_t index = 6;
        for(index; command[index] != '\0' && command[index] != ','; index++){
            val1[index-6] = command[index];
        }
        for(uint16_t index2 = index + 1; command[index2] != '\0'; index2++){
            val2[index2-index-1] = command[index2];
        }
        if((strncmp(val1, "ON", 2)==0||strncmp(val1, "OFF", 3)==0)&&(strncmp(val2, "ON", 2)==0||strncmp(val2, "OFF", 3)==0)){
            UART1_Write('O');
            UART1_Write('K');
            UART1_Write('\n');
            PM_STSLVL(strncmp(val1, "ON", 2)==0?true:false, strncmp(val2, "ON", 2)==0?true:false);
        }
        else{
            UART1_Write('?');
            UART1_Write('\n');
        }
    }
    else if(strncmp(command, "STSLU", 5) == 0){
        char val1[10]="", val2[10]="";
        uint16_t index = 5;
        for(index; command[index] != '\0' && command[index] != ','; index++){
            val1[index-5] = command[index];
        }
        for(uint16_t index2 = index + 1; command[index2] != '\0'; index2++){
            val2[index2-index-1] = command[index2];
        }
        if((strncmp(val1, "ON", 2)==0||strncmp(val1, "OFF", 3)==0)&&(strncmp(val2, "ON", 2)==0||strncmp(val2, "OFF", 3)==0)){
            UART1_Write('O');
            UART1_Write('K');
            UART1_Write('\n');
            PM_STSLU(strncmp(val1, "ON", 2)==0?true:false, strncmp(val2, "ON", 2)==0?true:false);
        }else{
            UART1_Write('?');
            UART1_Write('\n');
        }
    }
    else if (strncmp(command, "STBR", 4) == 0 ) 
    {
        char *baudrateStr = command + 4; // Skip "STBR "
        if (argErrCheck(baudrateStr))
        {
            UART1_Write("?");
        }
        else
        {
            baudrate = strtoul(baudrateStr, NULL, 10);
            if (baudrate > 38 && baudrate <= 10000000) {
                BRError = UART1_GetBRdiff(baudrate);
                if (BRError <= 3)
                {
                    UART1_Write_String("OK\r");
                    delay_ms(200);
                    UART1_Initialize(baudrate);
                    delay_ms(200);
                    UART1_Write_String("STN2120 v0.0.0");
                    receivedChar = UART1_Read();
                    if (receivedChar == '\r')
                    {
                        UART1_Write_String("\rOK\n");
                        oldbaudrate = baudrate;
                    }
                    else
                    {
                        UART1_Initialize(oldbaudrate);
                    }
                }
                else
                {
                   UART1_Write('?');
                }
              }
            else
            {
                UART1_Write('?');
            }
        }
    }
    else if (strncmp(command, "STSBR", 5) == 0) {
        char *Uart1TFBaudStr = command + 5; // Skip "STBR "
        if (argErrCheck(Uart1TFBaudStr))
        {
            UART1_Write("?");
        }
        else
        {
            baudrate = strtoul(Uart1TFBaudStr, NULL, 10);
            if (baudrate >= 38 && baudrate <= 10000000) 
            {
                BRError = UART1_GetBRdiff(baudrate);
                if (BRError <= 3)
                {
                    UART1_Write('O');
                    UART1_Write('K');
                    UART1_Write('\r');
                    UART1_Write('\n');
                    delay_ms(200);
                    UART1_Initialize(baudrate);
                    delay_ms(200);
                }
                else
                {
                    UART1_Write('?');
                }       
            }
            else
            {
                UART1_Write('?');
            }
        }
    }
    else if (strncmp(command, "STUFC", 5) == 0) {
        char *stufcStr = command + 5; // Skip "STBR "
        if (argErrCheck(stufcStr))
        {
            UART1_Write("?");
        }
        else
        {
          uint32_t stufc = strtoul(stufcStr, NULL, 10);
          if (stufc == 0 || stufc == 1)
          {
            if (*(command+5) == '0') 
            {
                UART1FlowCtrl = 0;
                UART1_Write('O');
                UART1_Write('K');
            }
            else if (*(command+5) == '1')
            {
                UART1FlowCtrl = 1;
                UART1_Write('O');
                UART1_Write('K');
            }
          }
          else
          {
              UART1_Write('?');
          }
        }
    }
    else if (strncmp(command, "STWBR", 5) == 0) {
            UART1_Write('O');
            UART1_Write('K');
            
    }
    else if (strncmp(command, "STI", 3) == 0)
    {
            UART1_Write_String("STN2120 v0.0.0\n");
    }
    else 
    {
            UART1_Write('?');
    }
    
}
void processATCommand(char *command)
{
    if  (strncmp(command, "ATPPS", 5) == 0)
    {
        printPParameters();
    }
    else if (strncmp(command, "ATPPU", 5) == 0)
    {
       
    }
    else if (strncmp(command, "ATPPR", 5) == 0)
    {
        restoreDefaultPParameters();
        UART1_Write_String("\rOK");
    }
    else if (strncmp(command, "ATPPT", 5) == 0)
    {
       // WordWriteExample();
    }
    
    
}
void processOBDRequest(char *command)
{
    
}

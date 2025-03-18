/*
 * File:        PM.c
 * Author:      Muhammad Sulaiman (GitHub: SulaimanNiazi)
 * Comments:    This is the source file for Power Management
 * Created on   March 8, 2025, 4:41 PM
 */


#include "xc.h"
#include "PM.h"
#include "mcc_generated_files/uart1.h"
#include "mcc_generated_files/tmr1.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void PM_init(){
    strcpy(LAST_SLEEP_TRIG, "NONE");
    strcpy(LAST_WAKE_TRIG, "NONE");
    UART_SLEEP = EXT_INPUT = EXT_SLEEP = VL_SLEEP = VL_WAKE = VCHG_WAKE = STSLXP_VAL = SLEEP_MODE = false;
    UART_WAKE = EXT_WAKE = true;
    UART_SLEEP_TIM = 1200;
    UART_WAKE_TIM_L = 0;
    UART_WAKE_TIM_H = 30000;
    VL_SLEEP_TIM = 600;
    VL_WAKE_TIM = 1;
    VCHG_WAKE_TIM = 1000;
    EXT_SLEEP_VAL = 3000;
    EXT_WAKE_VAL = 2000;
    VL_SLEEP_VOLT = 13;
    VL_WAKE_VOLT = 13.2;
    VCHG_WAKE_VOLT = 0.2;
    VL_SLEEP_SYMBOL = '<';
    VL_WAKE_SYMBOL = '>';
    
    //Get Status of Master Enable pin, once code is written for this task, this line is no longer necessary.
    bool master_enable = false;
    
    if(master_enable){
        strcpy(CTRL_MODE, "ELM327");
        EXT_WAKE_DELAY = 5;
        LOW_POWER = true;
    }else{
        strcpy(CTRL_MODE, "NATIVE");
        EXT_WAKE_DELAY = 0;
        LOW_POWER = false;
    }
}

void PM_LCS(){
    char line[40], symbols[3]="";
    snprintf(line, 40, "CTRL MODE: %s\n", CTRL_MODE);
    UART1_Write_String(line);
    snprintf(line, 40, "PWR_CTRL: LOW POWER = %s\n", LOW_POWER?"HIGH":"LOW");
    UART1_Write_String(line);
    snprintf(line, 40, "UART SLEEP: %s, %d s\n", UART_SLEEP?"ON":"OFF", UART_SLEEP_TIM);
    UART1_Write_String(line);
    snprintf(line, 40, "UART WAKE: %s, %d-%d us\n", UART_WAKE?"ON":"OFF", UART_WAKE_TIM_L, UART_WAKE_TIM_H);
    UART1_Write_String(line);
    snprintf(line, 40, "EXT INPUT: %s = SLEEP\n", EXT_INPUT?"HIGH":"LOW");
    UART1_Write_String(line);
    snprintf(line, 40, "EXT SLEEP: %s, %s FOR %d ms\n", EXT_SLEEP? "ON": "OFF", STSLXP_VAL? "HIGH": "LOW", EXT_SLEEP_VAL);
    UART1_Write_String(line);
    snprintf(line, 40, "EXT WAKE: %s, %s FOR %d ms\n", EXT_WAKE? "ON": "OFF", STSLXP_VAL? "LOW": "HIGH", EXT_WAKE_VAL);
    UART1_Write_String(line);
    symbols[0] = VL_SLEEP_SYMBOL;
    symbols[1] = PM_Is_Voltage_Valid(VL_SLEEP_VOLT)?'\0':'!';
    snprintf(line, 40, "VL SLEEP: %s, %s%.2fV FOR %d s\n", VL_SLEEP?"ON":"OFF", symbols, VL_SLEEP_VOLT, VL_SLEEP_TIM);
    UART1_Write_String(line);
    symbols[0] = VL_WAKE_SYMBOL;
    symbols[1] = PM_Is_Voltage_Valid(VL_WAKE_VOLT)?'\0':'!';
    snprintf(line, 40, "VL WAKE: %s, %s%.2fV FOR %d s\n", VL_WAKE?"ON":"OFF", symbols, VL_WAKE_VOLT, VL_WAKE_TIM);
    UART1_Write_String(line);
    symbols[0] = PM_Is_Voltage_Valid(VCHG_WAKE_VOLT)?'\0':'!';
    symbols[1]='\0';
    snprintf(line, 40, "VCHG WAKE: %s, %s%.2fV IN %d ms\n", VCHG_WAKE?"ON":"OFF", symbols, VCHG_WAKE_VOLT, VCHG_WAKE_TIM);
    UART1_Write_String(line);
}

void PM_Sleep(char* cause, uint16_t delay){
    if(strcmp(cause, "CMD")==0 || strcmp(cause, "UART")==0 || strcmp(cause, "VL")==0 || strcmp(cause, "EXT")==0){
        TMR1_Start();
        while(TMR1_SoftwareCounterGet() < delay);
        TMR1_Stop();
        TMR1_SoftwareCounterClear();
        UART1_Write_String("\n\nELM327 v1.4b\n\n");
        __builtin_pwrsav(0);
        strcpy(LAST_SLEEP_TRIG, cause);
        SLEEP_MODE = true;
    }
}

void PM_STSLLT(){
    char line[30];
    sprintf(line, "SLEEP: %s\nWAKE: %s\n", LAST_SLEEP_TRIG, LAST_WAKE_TRIG);
    UART1_Write_String(line);
}

void PM_STSLU(bool sleep, bool wakeup){
    UART_SLEEP = sleep;
    UART_WAKE = wakeup;
}

void PM_STSLVG(bool trigger){
    VCHG_WAKE = PM_Is_Voltage_Valid(VCHG_WAKE_VOLT)? trigger: false;;
}

void PM_STSLVL(bool sleep, bool wakeup){
    VL_SLEEP = PM_Is_Voltage_Valid(VL_SLEEP_VOLT)? sleep: false;
    VL_WAKE = PM_Is_Voltage_Valid(VL_WAKE_VOLT)? wakeup: false;
}

void PM_STSLUIT(uint16_t time){
    UART_SLEEP_TIM = time;
}

void PM_STSLPCP(bool lowPower){
    LOW_POWER = lowPower;
}

void PM_STSLUWP(uint16_t min, uint16_t max){
    UART_WAKE_TIM_L = min;
    UART_WAKE_TIM_H = max;
}

void PM_STSLXP(bool STSLXP){
    STSLXP_VAL = STSLXP;
}

void PM_STSLVLS(char symbol, float volt, uint16_t time){
    VL_SLEEP_SYMBOL = symbol;
    VL_SLEEP_VOLT = volt;
    VL_SLEEP_TIM = time;
}

void PM_STSLVLW(char symbol, float volt, uint16_t time){
    VL_WAKE_SYMBOL = symbol;
    VL_WAKE_VOLT = volt;
    VL_WAKE_TIM = time;
}

void PM_STSLVGW(float volt, uint16_t time){
    uint16_t newTime = time/250;
    newTime*=250;
    if((newTime + 250 - time < time - newTime)||(newTime == 0)){
        newTime = newTime+250;
    }
    VCHG_WAKE_VOLT = volt;
    VCHG_WAKE_TIM = newTime;
}

bool PM_Is_Voltage_Valid(float volt){
    //WIP
    //awaiting ADC config. and Voltage calibration commands
    return false;
}

bool PM_Get_Inactivity_trig(){
    return UART_SLEEP;
}

uint16_t PM_Get_Inactivity_time(){
    return UART_SLEEP_TIM;
}

void PM_Set_Wake_Trig(char* trig){
    if(strcmp(trig, "UART")==0 || strcmp(trig, "EXT")==0 || strcmp(trig, "VCHG")==0 || strcmp(trig, "VL")==0){
        strcpy(LAST_WAKE_TRIG, trig);
    }
}

bool PM_Check_Reset_Recent_Sleep(){
    if(SLEEP_MODE){
        SLEEP_MODE = false;
        return true;
    }
    return false;
}

/*
 * File:        PM.c
 * Author:      Muhammad Sulaiman (GitHub: SulaimanNiazi)
 * Comments:    This is the source file for Power Management
 * Created on   March 8, 2025, 4:41 PM
 */

// Section of included header files

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <p33EP256GP506.h>
#include "xc.h"

#include "PM.h"
#include "mcc_generated_files/uart1.h"
#include "mcc_generated_files/tmr1.h"
#include "ADC1.h"
#include "at.h"

// Section of functions

void PM_Initialize(){
    TRISAbits.TRISA8 = 0;   // Set RA8 (PWR_CTRL) pin as output.
    TRISCbits.TRISC2 = 1;   // Set RC2 (EXT SLEEP) pin as input
    CNENCbits.CNIEC2 = 1;   // Enable CN for RC2
    IEC1bits.CNIE = 1;      // Enable CN interrupts
    IFS1bits.CNIF = 0;      // Clear CN interrupt flag
    
    CNPUCbits.CNPUC2 = 1;   // Enable internal pull-up for RC2
    INTCON2bits.GIE = 1;    // Enable global interrupts
    
    TRISBbits.TRISB14 = 0;  // Set RB14 (LED_STATUS) pin as output
    PORTBbits.RB14 = 1;     // Set LED_STATUS pin as high (by default)
    
    strcpy(LAST_SLEEP_TRIG, "NONE");
    strcpy(LAST_WAKE_TRIG, "NONE");
    UART_SLEEP = EXT_INPUT = EXT_SLEEP = VL_SLEEP = VL_WAKE = VCHG_WAKE = STSLXP_VAL = false;
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
        PM_STSLPCP(true);
    }else{
        strcpy(CTRL_MODE, "NATIVE");
        EXT_WAKE_DELAY = 0;
        PM_STSLPCP(false);
    }
}

void PM_LCS(){
    char line[40], symbols[3]="";
    snprintf(line, 40, "\r\nCTRL MODE: %s", CTRL_MODE);
    UART1_Write_String(line);
    snprintf(line, 40, "\r\nPWR_CTRL: LOW POWER = %s", PWR_CTRL?"HIGH":"LOW");
    UART1_Write_String(line);
    snprintf(line, 40, "\r\nUART SLEEP: %s, %d s", UART_SLEEP?"ON":"OFF", UART_SLEEP_TIM);
    UART1_Write_String(line);
    snprintf(line, 40, "\r\nUART WAKE: %s, %d-%d us", UART_WAKE?"ON":"OFF", UART_WAKE_TIM_L, UART_WAKE_TIM_H);
    UART1_Write_String(line);
    snprintf(line, 40, "\r\nEXT INPUT: %s = SLEEP", EXT_INPUT?"HIGH":"LOW");
    UART1_Write_String(line);
    snprintf(line, 40, "\r\nEXT SLEEP: %s, %s FOR %d ms", EXT_SLEEP? "ON": "OFF", STSLXP_VAL? "HIGH": "LOW", EXT_SLEEP_VAL);
    UART1_Write_String(line);
    snprintf(line, 40, "\r\nEXT WAKE: %s, %s FOR %d ms", EXT_WAKE? "ON": "OFF", STSLXP_VAL? "LOW": "HIGH", EXT_WAKE_VAL);
    UART1_Write_String(line);
    symbols[0] = VL_SLEEP_SYMBOL;
    symbols[1] = PM_Is_Voltage_Valid(VL_SLEEP_VOLT)?'\0':'!';
    snprintf(line, 40, "\r\nVL SLEEP: %s, %s%.2fV FOR %d s", VL_SLEEP?"ON":"OFF", symbols, VL_SLEEP_VOLT, VL_SLEEP_TIM);
    UART1_Write_String(line);
    symbols[0] = VL_WAKE_SYMBOL;
    symbols[1] = PM_Is_Voltage_Valid(VL_WAKE_VOLT)?'\0':'!';
    snprintf(line, 40, "\r\nVL WAKE: %s, %s%.2fV FOR %d s", VL_WAKE?"ON":"OFF", symbols, VL_WAKE_VOLT, VL_WAKE_TIM);
    UART1_Write_String(line);
    symbols[0] = PM_Is_Voltage_Valid(VCHG_WAKE_VOLT)?'\0':'!';
    symbols[1]='\0';
    snprintf(line, 40, "\r\nVCHG WAKE: %s, %s%.2fV IN %d ms", VCHG_WAKE?"ON":"OFF", symbols, VCHG_WAKE_VOLT, VCHG_WAKE_TIM);
    UART1_Write_String(line);
}

void PM_Manage_Power(){
    PORTBbits.RB14 = 1; //Set LED_STATUS pin as high
    
    if(EXT_SLEEP){
        while(PORTCbits.RC2 == PWR_CTRL?1:0){
            TMR1_Start();
            if(TMR1_SoftwareCounterGet()>=EXT_SLEEP_VAL){
                TMR1_Stop();
                PM_Sleep("EXT", 0);
            }
        }
        TMR1_Stop();
        TMR1_SoftwareCounterClear();
    }
    
    if(VL_SLEEP){
        if(PM_Check_VL()){
            PM_Sleep("VL", 0);
        }
    }
    
}

void PM_Sleep(char* cause, uint16_t delay){
    if(strcmp(cause, "CMD")==0 || strcmp(cause, "UART")==0 || strcmp(cause, "VL")==0 || strcmp(cause, "EXT")==0){
        TMR1_Start();
        while(TMR1_SoftwareCounterGet() < delay);
        TMR1_Stop();
        TMR1_SoftwareCounterClear();
        UART1_Write_String("\r\n\nELM327 v1.4b\n\n");
        PORTBbits.RB14 = 0; // Set LED_STATUS pin as low. 
        __builtin_pwrsav(0);
        strcpy(LAST_SLEEP_TRIG, cause);
    }
}

void PM_STSLLT(){
    char line[30];
    sprintf(line, "\r\nSLEEP: %s\r\nWAKE: %s", LAST_SLEEP_TRIG, LAST_WAKE_TRIG);
    UART1_Write_String(line);
}

void PM_STSLU(bool sleep, bool wakeup){
    UART_SLEEP = sleep;
    UART_WAKE = wakeup;
}

void PM_STSLVG(bool trig){
    if(PM_Is_Voltage_Valid(VCHG_WAKE_VOLT) && trig){
        VCHG_WAKE = true;
        ADC1_Enable();
    }else{
        VCHG_WAKE = false;
    }
}

void PM_STSLVL(bool sleep, bool wakeup){
    VL_SLEEP = PM_Is_Voltage_Valid(VL_SLEEP_VOLT)? sleep: false;
    VL_WAKE = PM_Is_Voltage_Valid(VL_WAKE_VOLT)? wakeup: false;
    if(VL_SLEEP || VL_WAKE){
        ADC1_Enable();
    }
}

void PM_STSLUIT(uint16_t time){
    UART_SLEEP_TIM = time;
}

void PM_STSLPCP(bool lowPower){
    PWR_CTRL = lowPower;
    PORTAbits.RA8 = PWR_CTRL?1:0;
}

void PM_STSLUWP(uint16_t min, uint16_t max){
    UART_WAKE_TIM_L = min;
    UART_WAKE_TIM_H = max;
}

void PM_STSLXP(bool STSLXP){
    STSLXP_VAL = STSLXP;
}

void PM_STSLX(bool sleep, bool wake){
    EXT_SLEEP = sleep;
    EXT_WAKE = wake;
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

bool PM_Check_Reset_Recent_Sleep(){;
    if(RCONbits.SLEEP){
        if(UART_WAKE && (UART_WAKE_TIM_L <= (64/GetBR())*1000 <= UART_WAKE_TIM_H)){
            RCONbits.SLEEP = 0;
            return true;
        }
        else{
            __builtin_pwrsav(0);
        }
    }
    return false;
}

bool PM_Check_VL(void){
    bool trig = false;
    if(VL_WAKE_SYMBOL == '>'){
        ADC1_Start();
        delay_ms(1000);
        if(ADC1_Get_Voltage() > VL_WAKE_VOLT){
            TMR1_Start();
            while(ADC1_Get_Voltage() > VL_WAKE_VOLT){
                ADC1_Start();
                if(TMR1_SoftwareCounterGet() >= VL_WAKE_TIM){
                    trig = true;
                }
            }
            TMR1_Stop();
            TMR1_SoftwareCounterClear();
        }
    }else{
        ADC1_Start();
        delay_ms(1000);
        if(ADC1_Get_Voltage() < VL_WAKE_VOLT){
            TMR1_Start();
            while(ADC1_Get_Voltage() < VL_WAKE_VOLT){
                ADC1_Start();
                if(TMR1_SoftwareCounterGet() >= VL_WAKE_TIM){
                    trig = true;
                }
            }
            TMR1_Stop();
            TMR1_SoftwareCounterClear();
        }
    }
    return trig;
}

bool PM_Check_VCHG(void){
    ADC1_Start();
    delay_ms(1000);
    float initial = ADC1_Get_Voltage(), final;
    TMR1_Start();
    while(TMR1_SoftwareCounterGet() < VCHG_WAKE_TIM);
    TMR1_Stop();
    TMR1_SoftwareCounterClear();
    ADC1_Start();
    delay_ms(1000);
    final = ADC1_Get_Voltage();
    if(VCHG_WAKE_VOLT > 0){
        if(final - initial > VCHG_WAKE_VOLT){
            return true;
        }
    }else{
        if(final - initial < VCHG_WAKE_VOLT){
            return true;
        }
    }
    return false;
}

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{
    // Clear CN interrupt flag
    IFS1bits.CNIF = 0;

    // Check the state of RC2
    if (PORTCbits.RC2 == 0)
    {
        PM_Sleep("EXT", 0);
    }
    else
    {
        PM_Set_Wake_Trig("EXT");
        UART1_Write('>');
    }
}

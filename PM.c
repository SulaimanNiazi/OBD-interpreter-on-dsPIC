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
#include "mcc_generated_files/adc1.h"
#include "parameters.h"

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
    UART_SLEEP = EXT_INPUT = EXT_SLEEP = VL_SLEEP = VL_WAKE = VCHG_WAKE = STSLXP_VAL = UART_ALERT = false;
    UART_WAKE = EXT_WAKE = VL_WAKE_CONVERT = VCHG_CONVERT = VL_SLEEP_CONVERT = true;
    UART_SLEEP_TIM = 1200;
    UART_WAKE_TIM_L = 0;
    UART_WAKE_TIM_H = 30000;
    VL_SLEEP_TIM = 600;
    VL_WAKE_TIM = 1;
    VCHG_WAKE_TIM = 1000;
    EXT_SLEEP_TIM = 3000;
    EXT_WAKE_TIM = 2000;
    VL_SLEEP_VOLT = 13;
    VL_WAKE_VOLT = 13.2;
    VCHG_WAKE_VOLT = 0.2;
    VL_SLEEP_SYMBOL = '<';
    VL_WAKE_SYMBOL = '>';
    
    strcpy(CTRL_MODE, "NATIVE");
    EXT_WAKE_TIM = 0;
    PM_STSLPCP(false);
    
    //g_obdConfig.atppConfig[0x0E][0] = 0x5A; //default value
    
    uint8_t ELM327_config = g_obdConfig.atppConfig[0x0E][0];
    
    if((ELM327_config & 0x7F) != 0){
        strcpy(CTRL_MODE, "ELM327");
        if((ELM327_config & 0x40) != 0){
            PM_STSLPCP(true);
        }
        if((ELM327_config & 0x20) == 0){
            UART_SLEEP = true;
        }
        if((ELM327_config & 0x10) != 0){
            UART_SLEEP_TIM = 20*60;
        }else{
            UART_SLEEP_TIM = 5*60;
        }
        if((ELM327_config & 0x04) != 0){
            UART_ALERT = true;
        }
        if((ELM327_config & 0x02) != 0){
            EXT_WAKE_TIM = 5000;
        }else{
            EXT_WAKE_TIM = 1000;
        }
    }
}

void PM_LCS(){
    char line[40], subline[5], symbols[3]="";
    snprintf(line, 40, "\nCTRL MODE: %s", CTRL_MODE);
    UART1_Write_String(line);
    snprintf(line, 40, "\nPWR_CTRL: LOW POWER = %s", PWR_CTRL?"HIGH":"LOW");
    UART1_Write_String(line);
    snprintf(line, 40, "\nUART SLEEP: %s, %d s", UART_SLEEP?"ON":"OFF", UART_SLEEP_TIM);
    UART1_Write_String(line);
    snprintf(line, 40, "\nUART WAKE: %s, %d-%d us", UART_WAKE?"ON":"OFF", UART_WAKE_TIM_L, UART_WAKE_TIM_H);
    UART1_Write_String(line);
    snprintf(line, 40, "\nEXT INPUT: %s = SLEEP", EXT_INPUT?"HIGH":"LOW");
    UART1_Write_String(line);
    snprintf(line, 40, "\nEXT SLEEP: %s, %s FOR %d ms", EXT_SLEEP? "ON": "OFF", STSLXP_VAL? "HIGH": "LOW", EXT_SLEEP_TIM);
    UART1_Write_String(line);
    snprintf(line, 40, "\nEXT WAKE: %s, %s FOR %d ms", EXT_WAKE? "ON": "OFF", STSLXP_VAL? "LOW": "HIGH", EXT_WAKE_TIM);
    UART1_Write_String(line);
    if(VL_SLEEP_CONVERT){
        sprintf(subline, "%.2f", VL_SLEEP_VOLT);
        symbols[0] = VL_SLEEP_SYMBOL;
        symbols[1] = PM_Is_Voltage_Valid(VL_SLEEP_VOLT)?'\0':'!';
    }else{
        sprintf(subline, "%#X", VL_SLEEP_STEPS);
        symbols[0]='\0';
    }
    snprintf(line, 40, "\nVL SLEEP: %s, %s%sV FOR %d s", VL_SLEEP?"ON":"OFF", symbols, subline, VL_SLEEP_TIM);
    UART1_Write_String(line);
    if(VL_WAKE_CONVERT){
        sprintf(subline, "%.2f", VL_WAKE_VOLT);
        symbols[0] = VL_WAKE_SYMBOL;
        symbols[1] = PM_Is_Voltage_Valid(VL_WAKE_VOLT)?'\0':'!';
    }else{
        sprintf(subline, "%#X", VL_WAKE_STEPS);
        symbols[0]='\0';
    }
    snprintf(line, 40, "\nVL WAKE: %s, %s%sV FOR %d s", VL_WAKE?"ON":"OFF", symbols, subline, VL_WAKE_TIM);
    UART1_Write_String(line);
    if(VCHG_CONVERT){
        sprintf(subline, "%.2f", VCHG_WAKE_VOLT);
        symbols[0] = PM_Is_Voltage_Valid(VCHG_WAKE_VOLT)?'\0':'!';
        symbols[1]='\0';
    }else{
        sprintf(subline, "%#X", VCHG_WAKE_STEPS);
        symbols[0] = '\0';
    }
    snprintf(line, 40, "\nVCHG WAKE: %s, %s%sV IN %d ms", VCHG_WAKE?"ON":"OFF", symbols, subline, VCHG_WAKE_TIM);
    UART1_Write_String(line);
}

void PM_Manage_Power(){
    PORTBbits.RB14 = 1; //Set LED_STATUS pin as high
    
    if(VL_SLEEP){
        if(PM_Check_VL()){
            PM_Set_Sleep("VL", 0);
        }
    }
    
}

void PM_Set_Sleep(char* cause, uint16_t delay){
    if(strcmp(cause, "CMD")==0 || strcmp(cause, "UART")==0 || strcmp(cause, "VL")==0 || strcmp(cause, "EXT")==0){
        TMR1_Start();
        while(TMR1_SoftwareCounterGet() < delay);
        UART1_Write_String("\r\nELM327 v1.4b");
        delay_ms(1000);
        PORTBbits.RB14 = 0; // Set LED_STATUS pin as low. 
        PM_Sleep();
        strcpy(LAST_SLEEP_TRIG, cause);
    }
}

void PM_Sleep(){
    TMR1_Start();
    if(VL_WAKE || VCHG_WAKE)ADC1_Begin_Converting();   //begin an ADC conversion 
    if(UART_WAKE)CNENBbits.CNIEB6 = 1;              //set CNEN interrupt on RX pin if UART Wake trigger is enabled.
    __builtin_pwrsav(0);
    TMR1_Stop();
    TMR1_SoftwareCounterClear();
}

void PM_STSLLT(){
    char line[30];
    sprintf(line, "\nSLEEP: %s\nWAKE: %s", LAST_SLEEP_TRIG, LAST_WAKE_TRIG);
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

void PM_STSLVLS_Volts(char symbol, float volt, uint16_t time){
    VL_SLEEP_SYMBOL = symbol;
    VL_SLEEP_VOLT = volt;
    VL_SLEEP_TIM = time;
    VL_SLEEP_CONVERT = true;
}

void PM_STSLVLW_Volts(char symbol, float volt, uint16_t time){
    VL_WAKE_SYMBOL = symbol;
    VL_WAKE_VOLT = volt;
    VL_WAKE_TIM = time;
    VL_WAKE_CONVERT = true;
}

void PM_STSLVLS_Steps(char symbol, int steps, uint16_t time){
    VL_SLEEP_SYMBOL = symbol;
    VL_SLEEP_STEPS = steps;
    VL_SLEEP_TIM = time;
    VL_SLEEP_CONVERT = false;
}

void PM_STSLVLW_Steps(char symbol, int steps, uint16_t time){
    VL_WAKE_SYMBOL = symbol;
    VL_WAKE_STEPS = steps;
    VL_WAKE_TIM = time;
    VL_WAKE_CONVERT = false;
}

void PM_STSLVGW_Steps(int steps, uint16_t time){
    VCHG_CONVERT = false;
    uint16_t newTime = time/250;
    newTime*=250;
    if((newTime + 250 - time < time - newTime)||(newTime == 0)){
        newTime = newTime+250;
    }
    VCHG_WAKE_STEPS = steps;
    VCHG_WAKE_TIM = newTime;
}

void PM_STSLVGW_Volts(float volt, uint16_t time){
    VCHG_CONVERT = true;
    uint16_t newTime = time/250;
    newTime*=250;
    if((newTime + 250 - time < time - newTime)||(newTime == 0)){
        newTime = newTime+250;
    }
    VCHG_WAKE_VOLT = volt;
    VCHG_WAKE_TIM = newTime;
}

bool PM_Is_Voltage_Valid(float volt){
    return 0 <= volt < ((float)g_obdInfo.voltageCalibration)/1000;
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
    if(RCONbits.SLEEP == 1){
        if(UART_WAKE && (UART_WAKE_TIM_L <= (64/g_obdInfo.uartBaudrate)*1000 <= UART_WAKE_TIM_H)){
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
        if(VL_WAKE_CONVERT){
            ADC1_Get_Measurement();
            if(ADC1_Get_Voltage() > VL_WAKE_VOLT){
                TMR1_Start();
                while(ADC1_Get_Voltage() > VL_WAKE_VOLT){
                    ADC1_Get_Measurement();
                    if(TMR1_SoftwareCounterGet() >= VL_WAKE_TIM){
                        trig = true;
                    }
                }
                TMR1_Stop();
                TMR1_SoftwareCounterClear();
            }
        }else{
            if(ADC1_Get_Measurement() > VL_WAKE_STEPS){
                TMR1_Start();
                while(ADC1_Get_Measurement() > VL_WAKE_STEPS){
                    if(TMR1_SoftwareCounterGet() >= VL_WAKE_TIM){
                        trig = true;
                    }
                }
                TMR1_Stop();
                TMR1_SoftwareCounterClear();
            }
        }
    }else{
        if(VL_WAKE_CONVERT){
            ADC1_Get_Measurement();
            if(ADC1_Get_Voltage() < VL_WAKE_VOLT){
                TMR1_Start();
                while(ADC1_Get_Voltage() < VL_WAKE_VOLT){
                    ADC1_Get_Measurement();
                    if(TMR1_SoftwareCounterGet() >= VL_WAKE_TIM){
                        trig = true;
                    }
                }
                TMR1_Stop();
                TMR1_SoftwareCounterClear();
            }
        }else{
            if(ADC1_Get_Measurement() < VL_WAKE_STEPS){
                TMR1_Start();
                while(ADC1_Get_Measurement() < VL_WAKE_STEPS){
                    if(TMR1_SoftwareCounterGet() >= VL_WAKE_TIM){
                        trig = true;
                    }
                }
                TMR1_Stop();
                TMR1_SoftwareCounterClear();
            }
        }
    }
    return trig;
}

bool PM_Check_VCHG(void){
    if(VCHG_CONVERT){
        ADC1_Get_Measurement();
        float initial = ADC1_Get_Voltage(), final;
        TMR1_Start();
        while(TMR1_SoftwareCounterGet() < VCHG_WAKE_TIM);
        TMR1_Stop();
        TMR1_SoftwareCounterClear();
        ADC1_Get_Measurement();
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
    }else{
        float initial = ADC1_Get_Measurement(), final;
        TMR1_Start();
        while(TMR1_SoftwareCounterGet() < VCHG_WAKE_TIM);
        TMR1_Stop();
        TMR1_SoftwareCounterClear();
        final = ADC1_Get_Measurement();;
        if(VCHG_WAKE_STEPS > 0){
            if(final - initial > VCHG_WAKE_STEPS){
                return true;
            }
        }else{
            if(final - initial < VCHG_WAKE_STEPS){
                return true;
            }
        }
    }
    return false;
}

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{
    // Clear CN interrupt flag
    IFS1bits.CNIF = 0;

    // Check the state of RC2
    if(EXT_SLEEP){
        while(PORTCbits.RC2 == PWR_CTRL?1:0){
            TMR1_Start();
            if(TMR1_SoftwareCounterGet()>=EXT_SLEEP_TIM){
                TMR1_Stop();
                PM_Set_Sleep("EXT", 0);
            }
        }
        TMR1_Stop();
        TMR1_SoftwareCounterClear();
    }
    
    if(PORTCbits.RC2 == 1){
        if(PM_Check_Reset_Recent_Sleep()){
            if(EXT_WAKE){
                PM_Set_Wake_Trig("EXT");
                UART1_Write('>');
            }else{
                PM_Sleep();
            }
        }
    }
    if(PM_Check_Reset_Recent_Sleep()){
        if(PORTCbits.RC2 == PWR_CTRL?0:1 && EXT_WAKE){
            while(PORTCbits.RC2 == PWR_CTRL?0:1){
                TMR1_Start();
                if(TMR1_SoftwareCounterGet()>=EXT_WAKE_TIM){
                    TMR1_Stop();
                    PM_Set_Wake_Trig("EXT");
                    UART1_Write('>');
                }
            }
            TMR1_Stop();
            TMR1_SoftwareCounterClear();
        }
        else if(PORTBbits.RB6 == 1 && UART_WAKE && (UART_WAKE_TIM_L < 8000000/g_obdInfo.uartBaudrate < UART_WAKE_TIM_H)){
            PM_Set_Wake_Trig("UART");
            CNENBbits.CNIEB6 = 0;
        }else{
            PM_Sleep();
        }
    }
}

/*
 * File:        ADC1.c
 * Author:      Muhammad Sulaiman (GitHub: SulaimanNiazi)
 * Comments:    This is the source file for ADC1 module.
 * Created on   March 20, 2025
*/

#include <xc.h>
#include <stdint.h>
#include "ADC1.h"
#include "PM.h"

/*
// FGS
#pragma config GWRP = OFF           // General Segment Write-Protect bit (General Segment may be written)
//#pragma config GSS = OFF          // General Segment Code-Protect bit (General Segment Code protect is disabled)
//#pragma config GSSK = OFF         // General Segment Key bits (General Segment Write Protection and Code Protection is Disabled)

// FOSCSEL
#pragma config FNOSC = FRC          // Initial Oscillator Source Selection Bits (Internal Fast RC (FRC))
#pragma config IESO = OFF           // Two-speed Oscillator Start-up Enable bit (Start up with user-selected oscillator source)

// FOSC
#pragma config POSCMD = XT          // Primary Oscillator Mode Select bits (XT Crystal Oscillator Mode)
#pragma config OSCIOFNC = OFF       // OSC2 Pin Function bit (OSC2 is clock output)
#pragma config IOL1WAY = OFF        // Peripheral pin select configuration (Allow multiple reconfigurations)
#pragma config FCKSM = CSECMD       // Clock Switching Mode bits (Clock switching is enabled,Fail-safe Clock Monitor is disabled)

// FWDT
#pragma config WDTPOST = PS32768    // Watchdog Timer Postscaler Bits (1:32,768)
#pragma config WDTPRE = PR128       // Watchdog Timer Prescaler bit (1:128)
#pragma config PLLKEN = ON          // PLL Lock Wait Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.)
#pragma config WINDIS = OFF         // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF         // Watchdog Timer Enable bit (Watchdog timer enabled/disabled by user software)

// FPOR
//#pragma config FPWRT = PWR128     // Power-on Reset Timer Value Select bits (128ms)
//#pragma config BOREN = ON         // Brown-out Reset (BOR) Detection Enable bit (BOR is enabled)
#pragma config ALTI2C1 = OFF        // Alternate I2C pins for I2C1 (SDA1/SCK1 pins are selected as the I/O pins for I2C1)
#pragma config ALTI2C2 = OFF        // Alternate I2C pins for I2C2 (SDA2/SCK2 pins are selected as the I/O pins for I2C2)

// FICD
#pragma config ICS = PGD1           // ICD Communication Channel Select bits (Communicate on PGEC1 and PGED1)
//#pragma config RSTPRI = PF        // Reset Target Vector Select bit (Device will obtain reset instruction from Primary flash)
#pragma config JTAGEN = OFF         // JTAG Enable bit (JTAG is disabled)

// FAS
//#pragma config AWRP = OFF         // Auxiliary Segment Write-protect bit (Auxiliary program memory is not write-protected)
//#pragma config APL = OFF          // Auxiliary Segment Code-protect bit (Aux Flash Code protect is disabled)
//#pragma config APLK = OFF         // Auxiliary Segment Key bits (Aux Flash Write Protection and Code Protection is Disabled)
*/

void ADC1_Initialize ( void )
{
    TRISAbits.TRISA0 = 1;           // Set RA0 pin as input

    AD1CON1bits.FORM = 0;           // Data Output Format: Unsigned Integer Decimal Value
    AD1CON1bits.SSRC = 0;           // Clear SAMP bit ends sampling and starts conversion
    AD1CON1bits.SSRCG = 0;
    AD1CON1bits.ASAM = 1;           // ADC Sample Control: Sampling begins immediately after conversion
    AD1CON1bits.AD12B = 0;          // 10-bit ADC operation
    AD1CON2bits.CHPS = 0;           // Converts CH0

    // Set up A/D conversion clock to ADC Internal RC so that it will work in SLEEP mode
    AD1CON3 = ( AD1CON3 | 0x8000 );
    AD1CON3bits.SAMC = 1;           // Auto Sample Time = 1*Tad
    AD1CON3bits.ADCS = 0;           // ADC Conversion Clock Tad=Trc
    AD1CON2bits.SMPI = 0;           // SMPI must be 0

    //AD1CHS0: A/D Input Select Register
    AD1CHS0bits.CH0SA = 0;          // MUXA +ve input selection (AIN5) for CH0
    AD1CHS0bits.CH0NA = 0;          // MUXA -ve input selection (Vref-) for CH0

    //All inputs are analog by default
    ANSELBbits.ANSB0 = 1;           // AN0 as Analog Input
    IFS0bits.AD1IF = 0;             // Clear the A/D interrupt flag bit
    IEC0bits.AD1IE = 1;             // Enable A/D interrupt
    AD1CON1bits.ADON = 1;           // Turn on the A/D converter
    
    Steps_per_Volt = 315;           //Set default Calibration value
}

void ADC1_Calib(uint16_t value){
    Steps_per_Volt = value;
}

uint16_t ADC1_Get_Voltage(void){
    return (uint16_t)(ADC1BUF0/Steps_per_Volt);
}

uint16_t ADC1_Get_Steps(void){
    return (uint16_t)ADC1BUF0;
}

void ADC1_Enable(void){
    AD1CON1bits.SAMP = 1;
    for(uint16_t i = 0; i < 50; i++ );
}

void ADC1_Start(void){
    AD1CON1bits.SAMP = 0;           // Generate Start of Conversion
}

void __attribute__ ( (interrupt, no_auto_psv) ) _AD1Interrupt( void )
{
    IFS0bits.AD1IF = 0;             // Clear the ADC1 Interrupt Flag
    if(PM_Check_Reset_Recent_Sleep()){
        if(VL_WAKE){
            if(!PM_Check_VL()){
                __builtin_pwrsav(0);
            }
        }
        else if(VCHG_WAKE){
            if(!PM_Check_VCHG()){
                __builtin_pwrsav(0);
            }
        }
    }
}

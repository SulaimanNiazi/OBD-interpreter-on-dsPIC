/**
* @file        at.c
* @author      Aqib D. Ace 
* @date        March 2025
* @version     0.0.0
*/

#include "xc.h"
#include "at.h"
#include "ADC1.h"


bool uartEcho = true;
bool uartLineFeed = false;
uint32_t voltage_calibration = 12345;
uint32_t voltage_offset = 670;
uint32_t analog_in_voltage;
uint32_t analog_in_steps;
bool stvcalrun = false;
bool stsavcalrun = false;


char ELM327_VERSION[32];

void initDefaultAT(void)
{
    strcpy(ELM327_VERSION, "ELM327 v1.4b");
}


void disableEcho(void)
{
    uartEcho = false;
}
void enableEcho(void)
{
    uartEcho = true;
}
bool getEchoStatus(void)
{
    return uartEcho;
}
void disableLF(void)
{
    uartLineFeed = false;
}
void enableLF(void)
{
    uartLineFeed = true;
}
bool getLFStatus(void)
{
    return uartLineFeed;
}
void calibrateVoltage(char* argument)
{
    stvcalrun = true;
    uint32_t volts;
    uint32_t offset;
    sscanf(argument, "%lf,%lf", &volts, &offset);
    voltage_calibration = volts;
    voltage_offset = offset;
}
void readADCvolts(char* adcVolts, char* precession)
{
    //read the ANALOG_IN pin here and return the actual  volts
    float analog_in_voltage = ADC1_Get_Voltage();
    uint8_t precess = strtoul(precession, '\0', 10);
    sprintf(adcVolts,"%u.",analog_in_voltage);
}
uint16_t readADCSteps(void)
{
    //read the ANALOG_IN pin here and return the ADC steps
    uint16_t steps = ADC1_Get_Steps();
    return steps;
}

void saveCallibratedValues(void)
{
    stsavcalrun = true;
}
uint8_t readSavedCalibrationStatus(void)
{
    if (stvcalrun == true && stsavcalrun == true)
    {
        return 1;
    }
    else if (stvcalrun == false)
    {
        return 2;
    }
    else if (stvcalrun == true && stsavcalrun == false)
    {
        return 3;
    }
    else
    {
        return 0;
    }
}
void setAllSettoDefault(void)
{
    
}

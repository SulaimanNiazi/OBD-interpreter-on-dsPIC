/**
* @file        at.h
* @author      Aqib D. Ace and Muhammad Sulaiman
* @date        March 2025
* @version     0.0.0
*/
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> 
#include "stdbool.h"
void disableEcho(void);
void enableEcho(void);
bool getEchoStatus(void);
void disableLF(void);
void enableLF(void);
bool getLFStatus(void);
void initDefaultAT(void);
void calibrateVoltage(char* argument);
void readADCvolts(char* adcVolts, char* precession);
uint16_t readADCSteps(void);
void saveCallibratedValues(void);
uint8_t readSavedCalibrationStatus(void);
void setAllSettoDefault(void);
uint32_t getCalibration(void);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */



#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */


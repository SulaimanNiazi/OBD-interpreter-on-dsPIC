/* 
 * File: PM.h
 * Author: Muhammad Sulaiman (GitHub: SulaimanNiazi)
 * Comments: This is the header file for Power Management.
 * Revision history: Created on March 8, 2025, Last modified on March 14, 2025.
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

// TODO Insert appropriate #include <>

#include <stdbool.h>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

static char CTRL_MODE[20], LAST_SLEEP_TRIG[5], LAST_WAKE_TRIG[5], VL_SLEEP_SYMBOL, VL_WAKE_SYMBOL;
static uint16_t UART_SLEEP_TIM, UART_WAKE_TIM_L, UART_WAKE_TIM_H, VL_SLEEP_TIM, VL_WAKE_TIM, VCHG_WAKE_TIM, EXT_SLEEP_VAL, EXT_WAKE_VAL, EXT_WAKE_DELAY;
static float VL_SLEEP_VOLT, VL_WAKE_VOLT, VCHG_WAKE_VOLT;
static bool LOW_POWER, UART_SLEEP, UART_WAKE, EXT_INPUT, EXT_SLEEP, SLEEP_MODE, EXT_WAKE, VL_SLEEP, VL_WAKE, VCHG_WAKE, STSLXP_VAL, UART_INACTIVITY_ALERT;

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 

/**
  @Summary
    Initialization of PM (Power Management).

  @Description
    This routine sets all PM parameters to their default values.
  
  @Preconditions
    Set Master Enable pin for ELM327 mode, else reset it for Native mode .

  @Param
    None

  @Returns
    None
 
  @Example 
    <code>    
        PM_init();
    </code>
*/
void PM_init();

/**
  @Summary
    LCS command of PM (Power Management).

  @Description
    This routine displays all PM parameters through UART1.
  
  @Preconditions
    Having initialized PM (using PM_init() function) and UART1.

  @Param
    None

  @Returns
    None
 
  @Example 
    <code>    
        PM_LCS();
    </code>
*/
void PM_LCS();

/**
  @Summary
    Puts device to Sleep.

  @Description
    This routine puts the device to sleep after the specified delay, and stores the latest sleep trigger.
  
  @Preconditions
    Having initialized Timer1 and UART1.

  @Param
    String stating cause, cause must me entered in capitals, if an allowed cause is not entered, the function will do nothing, allowed causes are:
 * CMD:     STSLEEP or ATLP command
 * UART:    UART inactivity timeout
 * EXT:     External sleep control input
 * VL:      Voltage level
  
    And Unsigned integer stating delay time in seconds.

  @Returns
    None
 
  @Example 
    <code>    
        PM_Sleep("CMD", 0);
    </code>
*/
void PM_Sleep(char*, uint16_t);

/**
  @Summary
    STSLLT command of PM (Power Management).

  @Description
    This routine displays last sleep and wake triggers through UART1.
  
  @Preconditions
    Having initialized PM (using PM_init() function) and UART1.

  @Param
    None

  @Returns
    None
 
  @Example 
    <code>    
        PM_STSLLT();
    </code>
*/
void PM_STSLLT();

/**
  @Summary
    STSLU command of PM (Power Management).

  @Description
    This routine sets UART sleep and wake triggers.
  
  @Preconditions
    None

  @Param
    Boolean value of UART sleep and wake triggers respectively.

  @Returns
    None
 
  @Example 
    <code>    
        PM_STSLU(true, true);
    </code>
*/
void PM_STSLU(bool, bool);

/**
  @Summary
    STSLVG command of PM (Power Management).

  @Description
    This routine sets voltage change wake trigger.
  
  @Preconditions
 * Having initialized PM (using PM_init() function) and ADC1. 
 * Recommended: Setting VCHG wake trigger voltage (using PM_STSLVGW() function).

  @Param
    Boolean value of voltage change wake trigger.

  @Returns
    None
 
  @Example 
    <code>    
        PM_STSLVG(true);
    </code>
*/
void PM_STSLVG(bool);

/**
  @Summary
    STSLVL command of PM (Power Management).

  @Description
    This routine sets voltage level sleep and wake triggers.
  
  @Preconditions
 * Having initialized PM (using PM_init() function) and ADC1.
 * Recommended: Configure voltage level sleep and wake triggers (using PM_STSLVLS() and PM_STSLVLW() functions respectively).

  @Param
    Boolean value of voltage level sleep and wake triggers respectively.

  @Returns
    None
 
  @Example 
    <code>    
        PM_STSLVL(false, true);
    </code>
*/
void PM_STSLVL(bool, bool);

/**
  @Summary
    STSLUIT command of PM (Power Management).

  @Description
    This routine sets the time in seconds for UART inactivity timeout.
  
  @Preconditions
    None

  @Param
    Unsigned integer value of time in seconds for UART inactivity timeout.

  @Returns
    None
 
  @Example 
    <code>    
        PM_STSLUIT(2000);
    </code>
*/
void PM_STSLUIT(uint16_t);

/**
  @Summary
    STSLPCP command of PM (Power Management).

  @Description
    This routine sets polarity of the PWR_CTRL output.
  
  @Preconditions
    None

  @Param
    Boolean value of PWR_CTRL output.
 * false:   Normal power = HIGH,    Low power mode = LOW
 * true:    Normal power = LOW,     Low power mode = HIGH

  @Returns
    None
 
  @Example 
    <code>    
        PM_STSLUIT(true);
    </code>
*/
void PM_STSLPCP(bool);

/**
  @Summary
    STSLUWP command of PM (Power Management).

  @Description
    This routine sets the UART wake up pulse timing in microseconds.
  
  @Preconditions
    None

  @Param
    Unsigned integer value of minimum and maximum pulse timing in microseconds respectively.

  @Returns
    None
 
  @Example 
    <code>    
        PM_STSLUWP(10000, 50000);
    </code>
*/
void PM_STSLUWP(uint16_t, uint16_t);

/**
  @Summary
    STSLXP command of PM (Power Management).

  @Description
    This routine configures the polarity of the SLEEP input.
 * false:   LOW = sleep,    HIGH = wake up
 * true:    LOW = wake up,  HIGH = sleep
  
  @Preconditions
    None

  @Param
    Boolean value of the SLEEP input polarity.

  @Returns
    None
 
  @Example 
    <code>    
        PM_STSLXP(true);
    </code>
*/
void PM_STSLXP(bool);

/**
  @Summary
    STSLVLS command of PM (Power Management).

  @Description
    This routine configures the voltage level sleep trigger.
  
  @Preconditions
    None

  @Param
 * Character specifying above or below the voltage threshold (?<? = below, ?>? = above).
 * float value specifying the voltage.
 * Unsigned integer value of time in seconds specifying how long the voltage must remain above or below the threshold.

  @Returns
    None
 
  @Example 
    <code>    
        PM_STSLVLS('<', 12.85, 60);
    </code>
*/
void PM_STSLVLS(char, float, uint16_t);

/**
  @Summary
    STSLVLW command of PM (Power Management).

  @Description
    This routine configures the voltage level wake trigger.
  
  @Preconditions
    None

  @Param
 * Character specifying above or below the voltage threshold (?<? = below, ?>? = above).
 * float value specifying the voltage.
 * Unsigned integer value of time in seconds specifying how long the voltage must remain above or below the threshold.

  @Returns
    None
 
  @Example 
    <code>    
        PM_STSLVLW('>', 12.85, 60);
    </code>
*/
void PM_STSLVLW(char, float, uint16_t);

/**
  @Summary
    STSLVGW command of PM (Power Management).

  @Description
    This routine configures the voltage change wake up trigger.
  
  @Preconditions
    None

  @Param
 * Float value specifying the voltage change.
 * Unsigned integer specifying the time in milliseconds for the voltage change to take place in.

  @Returns
    None
 
  @Example 
    <code>    
        PM_STSLVGW(-0.15, 750);
    </code>
*/
void PM_STSLVGW(float, uint16_t);

/**
  @Summary
    Sets Wake trigger.

  @Description
    This routine sets the cause of the Last Wake up trigger.
  
  @Preconditions
    None

  @Param
    Character array/string specifying the cause, the cause must be specified in capitals, if an invalid cause is provided the function does nothing, allowed causes are:
 * UART:    UART activity, or UART Rx pulse.
 * EXT:     External sleep control input.
 * VL:      voltage level wake trigger.
 * VCHG:    voltage change wake trigger.

  @Returns
    None
 
  @Example 
    <code>    
        PM_Set_Wake_Trig("UART");
    </code>
*/
void PM_Set_Wake_Trig(char*);

/**
  @Summary
    Checks if voltage is valid.

  @Description
    This routine checks if any specified voltage, for a trigger, is valid according to the ADC calibrations.
  
  @Preconditions
    Initializing and calibrating ADC.

  @Param
    Float value specifying the voltage.

  @Returns
    Boolean value specifying if the voltage is valid.
 
  @Example 
    <code>    
        bool valid = PM_Is_Voltage_Valid(13.14);
    </code>
*/
bool PM_Is_Voltage_Valid(float);

/**
  @Summary
    Get UART inactivity trigger.

  @Description
    This routine only returns the UART inactivity trigger status.
  
  @Preconditions
    Initializing PM (using PM_init() function)
    Recommended: Configuring UART sleep trigger (using PM_STSLU() function)

  @Param
    None

  @Returns
    Boolean value specifying if the UART inactivity trigger is set or not.
 
  @Example 
    <code>    
        bool trigger = PM_Get_Inactivity_trig();
    </code>
*/
bool PM_Get_Inactivity_trig();

/**
  @Summary
    Checks if the device was recently sleeping.

  @Description
    This routine checks if the device was sleeping and resets the SLEEP_MODE variable that determines this.
  
  @Preconditions
    Initialize PM (using PM_init() function).

  @Param
    None

  @Returns
    Boolean value specifying if the device was recently sleeping or not.
 
  @Example 
    <code>    
        bool was_sleeping = PM_Check_Reset_Recent_Sleep();
    </code>
*/
bool PM_Check_Reset_Recent_Sleep();

/**
  @Summary
    Get the time value of UART inactivity timeout.

  @Description
    This routine simply gets the time, in seconds, set for the UART inactivity timeout sleep trigger.
  
  @Preconditions
 * Initialize PM (using PM_init() function)
 * Recommended: Set the time for UART inactivity timeout (using PM_STSLUIT() function)

  @Param
    None

  @Returns
    Unsigned integer value specifying the time in seconds of the UART inactivity timeout.
 
  @Example 
    <code>    
        uint16_t inactivity_time = PM_Get_Inactivity_time();
    </code>
*/
uint16_t PM_Get_Inactivity_time();

// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */


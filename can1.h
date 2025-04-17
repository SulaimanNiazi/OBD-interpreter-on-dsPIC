 
#ifndef __CAN_H
#define	__CAN_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define FCAN    20267500UL
#define BITRATE 250000UL  
#define NTQ     10  // 10 Time Quanta in a Bit Time
#define BRP_VAL ( (FCAN / (2 * NTQ * BITRATE)) - 1  )

#define CAN_BUFFER_CNT              16                  // must be power of 2 

#define OPTION_NOP                  0x0
#define OPTION_IS_FLOW_CONTROL      0x01

#define DEFAULT_SID_CAN_FILTER_MASK 0xfff
// TODO: check if this is the correct filter value 0x1fffffff for EID
#define DEFAULT_EID_CAN_FILTER_MASK 0x00000000
//#define SID_FLOW_CONTROL_EXT_EQUIP  0x7E0

#define DEFAULT_SID_CAN_FILTER 0x07E8
#define DEFAULT_EID_CAN_FILTER 0x18DAF111





#define CAN_BAUDRATE_HIGH_SPEED_500K    500000
#define CAN_BAUDRATE_LOW_SPEED_250K     250000

#define COUNT_BYTES_8_DLC           8
#define MAX_CAN_FRAMES_SIZE         8
#define MAX_CAN_MESSAGE_DATA_LENGTH 16

#define CAN_FIXED_DLC                  0x80
#define CAN_EXTENDED_ID_FORMAT         0x01

typedef
enum {
    CAN_TP_SINGLE_FRAME = 0,
    CAN_TP_FIRST_FRAME = 1,
    CAN_TP_CONSECUTIVE_FRAME = 2,
    CAN_TP_FLOW_CONTROL_FRAME = 3,
} CanTPFrameType_t;

enum CAN_MODE {
    CAN_MODE_STANDARD_DATA_FRAME_VAR_DLC = 0x00,
    CAN_MODE_EXTENDED_DATA_FRAME_VAR_DLC = CAN_EXTENDED_ID_FORMAT,
    CAN_MODE_STANDARD_DATA_FRAME_8_DLC = CAN_FIXED_DLC ,
    CAN_MODE_EXTENDED_DATA_FRAME_8_DLC = CAN_FIXED_DLC | CAN_EXTENDED_ID_FORMAT,
};

//enum CAN_MODE {
//    CAN_MODE_STANDARD_DATA_FRAME_VAR_DLC = 0x00,
//    CAN_MODE_EXTENDED_DATA_FRAME_VAR_DLC = 0x01,
//    CAN_MODE_STANDARD_DATA_FRAME_8_DLC = 0x80,
//    CAN_MODE_EXTENDED_DATA_FRAME_8_DLC = 0x81,
//};

typedef struct {
    uint8_t data[CAN_BUFFER_CNT][16];
    uint8_t ridx;
    uint8_t widx;
} canRingBufferInfo_t;

extern volatile canRingBufferInfo_t g_canRingBufferInfo;


#define GET_CAN_SID_X____(DATA_ARR) \
    (((uint16_t)((((uint16_t)(DATA_ARR)[1]) << 2) | (((uint16_t)(DATA_ARR)[0] >> 6) & 0x03)) & 0xFFF) << 4) | (((uint16_t)(DATA_ARR)[0] >> 2) & 0x0F)

#define GET_CAN_SID___V1(DATA_ARR) \
    (((uint16_t)((((uint16_t)(DATA_ARR)[1]) << 2)) & 0xFFF) << 4) | (((uint16_t)(DATA_ARR)[0] >> 2) & 0x7F)  

#define GET_CAN_SID(DATA_ARR) \
    (((uint16_t)((((uint16_t)(DATA_ARR)[1]) << 6)) & 0xFC0)) | (((uint16_t)(DATA_ARR)[0] >> 2) & 0x7F)  


#define GET_CAN_EID__OLD(myeid) ( \
    ((uint32_t)(myeid[1]) << 24) | \
    ((uint32_t)(myeid[0] & 0xFE) << 16) | \
    ((uint32_t) ((((uint32_t) myeid[3] >> 2) & 0xC0) |  (  (  ((uint32_t)myeid[2]) >> 2) & 0x3F)) << 8) | \
    ((uint32_t)(((myeid[5] >> 2) & 0x3F) | ((myeid[2] & 0x03) << 6))) \
)

#define GET_CAN_EID_31(myeid) ( \
    ((uint32_t)(myeid[1]) << 24) | \
    ((uint32_t)(myeid[0] & 0xFE) << 16) | \
    ((uint32_t) ((((uint32_t)myeid[3]>>2)&0xC0) |  (((uint32_t)myeid[2]>>2)&0xFf)) << 8) | \
    ((uint32_t)(((myeid[5] >> 2) & 0x3F) | ((myeid[2] & 0x03) << 6))) \
)
#define GET_CAN_EID(myeid) ( \
    ((uint32_t)(myeid[1]) << 24) | \
    ((uint32_t)(myeid[0] & 0xFE) << 16) | \
    (((uint32_t) ((((myeid[3])<<8)&0xff00) | ((myeid[2])&0xff)) << 6)&0xff00 ) | \
    ((uint32_t)(((myeid[5] >> 2) & 0x3F) | ((myeid[2] & 0x03) << 6))) \
)

// Request to switch the CAN controller to configuration mode and wait until it is done
#define SWITCH_TO_CAN_CONFIG_MODUS  if(C1CTRL1bits.OPMODE != 4) {C1CTRL1bits.REQOP = 4; while (C1CTRL1bits.OPMODE != 4);  }
// Request Normal Operation mode and Wait for CAN module to switch to Normal Operation mode
#define EXIT_CAN_CONFIG_MODUS  if(C1CTRL1bits.OPMODE != 0) {C1CTRL1bits.REQOP = 0; while (C1CTRL1bits.OPMODE != 0);  }



#define CAN_FUNCTIONS_INTERFACE

#ifdef CAN_FUNCTIONS_INTERFACE
void initCAN(void);
uint8_t txMessageCAN(uint32_t sid, uint32_t eid, uint8_t mode, uint8_t * data, uint8_t size, uint16_t timeout);
void setBaudrateCAN(uint32_t baudrate);
uint32_t getBaudrateCAN(void);
uint8_t getMessageCAN(uint8_t * data);
uint8_t dataAvailableCAN(void);
void flushBufferCAN(void);
void printMessageCAN(uint8_t buffer);
void configureFilterCAN(uint8_t mode, uint32_t sid, uint32_t eid, uint32_t sid_mask, uint32_t eid_mask);
void getErrorCountCAN(uint8_t * rx, uint8_t * tx);
#endif

void setCANTimConReg(uint32_t timeValue);
uint32_t getCANTimConReg(void);


#endif	/* XC_HEADER_TEMPLATE_H */

// https://en.wikipedia.org/wiki/ISO_15765-2

	

/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

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


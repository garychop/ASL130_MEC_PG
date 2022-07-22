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
 * File:   AnalogInput.h
 * Author: G. Chopcinski
 * Comments: Defines the usable external calls.
 * Revision history: June 21, 2022
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ANALOG_INPUT_H
#define	ANALOG_INPUT_H

//#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <stdbool.h>

enum JOYSTICK_CHANNEL_ENUM {SPEED_ARRAY, DIRECTION_ARRAY, NUM_JS_POTS};

// This holds the Joystick information 
typedef struct 
{
    uint16_t m_rawInput;
    uint16_t m_rawNeutral;
    uint16_t m_rawMinNeutral;
    uint16_t m_rawMaxNuetral;
    uint16_t m_rawMinimum;
    uint16_t m_rawMaximum;
    uint16_t m_PositiveScale;   // This is used to scale from neutral to Most Positive
    uint16_t m_NegativeScale;   // This is used to scale from neutral to Most Negative
} JOYSTICK_STRUCT;

extern JOYSTICK_STRUCT Joystick_Data[NUM_JS_POTS];

#define NEUTRAL_JOYSTICK_INPUT (0x202)

#ifdef BUILD_FOR_ASL133_ASL134
// The ASL133 and ASL134 joysticks require a much smaller Neutral Area than
// ... ASL128, ASL130 and ASL138 joysticks.
#define NEUTRAL_ERROR_MARGIN (0x18)     // The amount of deviation from the Neutral
#else
#define NEUTRAL_ERROR_MARGIN (0x40)     // The amount of deviation from the Neutral
#endif

#define JOYSTICK_RAW_MAX_DEFLECTION (220)   // This is the max that the joystick 
                                        // .. input can deviate from neutral.

void AnalogInputInit(void);
uint16_t ReadSpeed (void);
uint16_t ReadDirection (void);
void GetSpeedAndDirection (uint16_t *speed, uint16_t *direction);
bool IsJoystickInNeutral (void);
    
#endif	/* ANALOG_INPUT_H */


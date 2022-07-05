///////////////////////////////////////////////////////////////////////////////
// File Name: AnalogInput.c
// Project:  Prop ASL130 with Bluetooth Module
//      Supports PC Board version PROP_BT_V2/3
//
// Date: 6/21/22
//
///////////////////////////////////////////////////////////////////////////////

// NOTE: This must ALWAYS be the first include in a file.
#include "device_xc8.h"

#include "bsp.h"
#include "AnalogInput.h"

JOYSTICK_STRUCT Joystick_Data[NUM_JS_POTS];

void AnalogInputInit(void)
{
    TRISAbits.RA0 = GPIO_BIT_INPUT;
    TRISAbits.RA1 = GPIO_BIT_INPUT;
    
    ADCON1bits.VCFG0 = 0;   // Use Vdd as voltage reference
    ADCON1bits.VCFG1 = 0;   // Use Fxx as voltage Reference
	ADCON1bits.VCFG01 = 0; // VSS negative voltage reference
	ADCON1bits.VCFG11 = 0; // VDD positive voltage reference
//	ADCON1bits.PCFG = 0x0e; // Channels AN0 is enabled
	ADCON1bits.PCFG = 0x0d; // Channels AN0 and AN1 are enabled
    
    // NOTE: Time to capture is 6.4 us.  This should be fine for any operational environment as
	// NOTE: See Equation 21-3 of the PIC18F4550's datasheet.  Also, Table 21-1
	ADCON2bits.ADCS = 0x05; // FOSC / 16 = 625 kHz
	ADCON2bits.ACQT = 0x02; // 4 AD clock cycles per conversion.

	ADCON2bits.ADFM = 1; // Results right justified

	ADCON0bits.ADON = 1; // Enable ADC
            
}

//------------------------------------------------------------------------------
// Function reads the Speed Pot in the Joystick on Ain Ch 0
//------------------------------------------------------------------------------
uint16_t ReadSpeed (void)
{
    // Select Channel 0. CHS0, CHS1, CHS2 and CHS3
    ADCON0bits.CHS0 = 0;
    ADCON0bits.CHS1 = 0;
    ADCON0bits.CHS2 = 0;
    ADCON0bits.CHS3 = 0;
    
	// Need to wait at least Tad * 3. Clock is FOSC/16, which gets us: 3/(625,000) = ~4.8 us.  Our delay resolution is not
	// great, so we just delay for the min time.
	bspDelayUs (US_DELAY_50_us);
	
	// Kick off the conversion
	ADCON0bits.GO_nDONE = 1;
	while (ADCON0bits.GO_nDONE == 1)
	{
		(void)0;
	}
    
	return ((uint16_t)ADRESL + ((uint16_t)(ADRESH & 0x3) << 8));
}

//------------------------------------------------------------------------------
// Function reads the Direction Pot in the Joystick on Ain Ch 1
//------------------------------------------------------------------------------
uint16_t ReadDirection (void)
{
    // Select Channel 1. CHS0, CHS1, CHS2 and CHS3
    ADCON0bits.CHS0 = 1;
    ADCON0bits.CHS1 = 0;
    ADCON0bits.CHS2 = 0;
    ADCON0bits.CHS3 = 0;
    
	// Need to wait at least Tad * 3. Clock is FOSC/16, which gets us: 3/(625,000) = ~4.8 us.  Our delay resolution is not
	// great, so we just delay for the min time.
	bspDelayUs (US_DELAY_100_us);
	
	// Kick off the conversion
	ADCON0bits.GO_nDONE = 1;
	while (ADCON0bits.GO_nDONE == 1)
	{
		(void)0;
	}
    
	return ((uint16_t)ADRESL + ((uint16_t)(ADRESH & 0x3) << 8));
}

void GetSpeedAndDirection (uint16_t *speed, uint16_t *direction)
{
    int i;
    uint16_t speedTotal, directionTotal;
    
    speedTotal = 0;
    directionTotal = 0;
    
    for (i = 0; i<10; ++i)
    {
        bspDelayUs (US_DELAY_20_us);
        speedTotal += ReadSpeed();
        //bspDelayUs (US_DELAY_20_us);
        directionTotal += ReadDirection();
    }
    *speed = speedTotal / 10;
    *direction = directionTotal / 10;
}

//------------------------------------------------------------------------------
// This function returns "true" if the joystick signals are within the Neutral
// window else returns "false".
//------------------------------------------------------------------------------
bool IsJoystickInNeutral ()
{
    uint16_t rawSpeed, rawDirection; 
    
    GetSpeedAndDirection (&rawSpeed, &rawDirection);
    
    if ((rawSpeed < (NEUTRAL_JOYSTICK_INPUT + NEUTRAL_ERROR_MARGIN))
    && (rawSpeed > (NEUTRAL_JOYSTICK_INPUT - NEUTRAL_ERROR_MARGIN))
    && (rawDirection < (NEUTRAL_JOYSTICK_INPUT + NEUTRAL_ERROR_MARGIN))
    && (rawDirection > (NEUTRAL_JOYSTICK_INPUT - NEUTRAL_ERROR_MARGIN)))
    {
        return true;
    }
    else
    {
        return false;
    }
}


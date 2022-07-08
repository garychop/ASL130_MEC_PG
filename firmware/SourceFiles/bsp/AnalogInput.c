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
#ifdef _18F46K40
    ANSELAbits.ANSELA0 = 1;     // Configure A0 as Analog Input
    ANSELAbits.ANSELA1 = 1;     // Configure A1 as Analog Input
    
    ADCON0bits.ADCS = 0; // Fosc is the clock source for ADC clock
    ADCON0bits.ADFM = 1; // Results are right justified
    ADCON0bits.ADCONT = 0; // Not continuous conversion: one-shot
    
    // ADCON1bits is a don't care because we don't use pre-charge.
    
    ADCON2bits.ADMD = 0x00; // No filtering or averaging on ADC samples.
    
    // ADCON3bits are not required to be set.
    
    ADREFbits.ADPREF = 0x00; // VSS negative voltage reference
    ADREFbits.ADNREF = 0x00; // VDD positive voltage reference
    
    // Set clock to Fosc/(2*(ADCLKbits.ADCS+1)) = Fosc / 16 = 625 kHz
    ADCLKbits.ADCS = 7;
    
    ADPREbits.ADPRE = 0; // No pre-charge before taking an ADC sample.
    ADACQbits.ADACQ = 4; // 4 AD clock cycles per conversion.
    ADCAPbits.ADCAP = 0; // No external capacitance attached to the signal path.
    
    ADRPTbits.ADRPT = 0; // Repeat threshold: don't care since not filtering or averaging.
    ADCNTbits.ADCNT = 0; // Don't care since not filtering or averaging.
    ADFLTRHbits.ADFLTRH = 0; // Don't care since not filtering or averaging.
    ADFLTRLbits.ADFLTRL = 0; // Don't care since not filtering or averaging.
    
    ADCON0bits.ADON = 1; // Enable ADC
#else
	ADCON1bits.VCFG01 = 0; // VSS negative voltage reference
	ADCON1bits.VCFG11 = 0; // VDD positive voltage reference
	ADCON1bits.PCFG = 0x0C; // Channels AN0->2 are enabled

	// NOTE: Time to capture is 6.4 us.  This should be fine for any operational environment as
	// NOTE: See Equation 21-3 of the PIC18F4550's datasheet.  Also, Table 21-1
	ADCON2bits.ADCS = 0x05; // FOSC / 16 = 625 kHz
	ADCON2bits.ACQT = 0x02; // 4 AD clock cycles per conversion.

	ADCON2bits.ADFM = 1; // Results right justified

	ADCON0bits.ADON = 1; // Enable ADC
#endif
    
}

//------------------------------------------------------------------------------
// Function reads the Speed Pot in the Joystick on Ain Ch 0
//------------------------------------------------------------------------------
uint16_t ReadSpeed (void)
{
    ADPCHbits.ADPCH = 0x00; // Channel 0, RA0
    
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
    ADPCHbits.ADPCH = 0x01; // Channel 1, RA1
    
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


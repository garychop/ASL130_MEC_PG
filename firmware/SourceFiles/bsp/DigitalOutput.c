///////////////////////////////////////////////////////////////////////////////
// File Name: DigitalOutput.c
// Project:  Prop ASL130 with Bluetooth Module
//      Supports PC Board version PROP_BT_V2/3
//
// Date: June 24, 2022
//
///////////////////////////////////////////////////////////////////////////////

// NOTE: This must ALWAYS be the first include in a file.
#include "device_xc8.h"

#include "bsp.h"
#include "DigitalOutput.h"

void DigitalOutputInit(void)
{
    TRISEbits.TRISE0 = GPIO_BIT_OUTPUT;
    ANSELEbits.ANSELE0 = 0; // Allow Digital control to operate correctly.
    LATEbits.LATE0 = GPIO_LOW;
}

//------------------------------------------------------------------------------

void SetResetOutput (bool state)
{
    LATEbits.LATE0 = state;
}

// end of file
//------------------------------------------------------------------------------


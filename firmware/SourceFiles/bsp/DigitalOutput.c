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
    LATEbits.LATE0 = GPIO_LOW;

#ifndef DEBUG    
    // This is shared with some debugging pin
//    TRISAbits.TRISA5 = GPIO_BIT_OUTPUT; // This is the LEFT CLICK to BT
//    LATEbits.LATE0 = GPIO_HIGH;
#endif
}

//------------------------------------------------------------------------------

void SetResetOutput (bool state)
{
    LATEbits.LATE0 = state;
}

//------------------------------------------------------------------------------

void SetLeftClickOutput (bool state)
{
#ifndef DEBUG    
    // This is shared with some debugging pin
//    LATAbits.LATA5 = state;
#endif
}

//------------------------------------------------------------------------------

// end of file
//------------------------------------------------------------------------------


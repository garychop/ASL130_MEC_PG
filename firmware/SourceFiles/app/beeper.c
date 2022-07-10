//////////////////////////////////////////////////////////////////////////////
//
// Filename: beeper.c
//
// Description: Provides beeper control to the application.
//
// Author(s): Trevor Parsh (Embedded Wizardry, LLC)
//
// Modified for ASL on Date: 
//
//////////////////////////////////////////////////////////////////////////////


/* **************************   Header Files   *************************** */

// NOTE: This must ALWAYS be the first include in a file.
#include "device_xc8.h"

#include <stdint.h>
#include <stdbool.h>

// from local
#include "bsp.h"
#include "beeper.h"

/* ******************************   Types   ******************************* */

//-------------------------------
// Function: beeperInit
//
// Description: Initializes this module.
//
//-------------------------------
void beeperInit(void)
{
    TRISDbits.TRISD0 = GPIO_BIT_OUTPUT;
    ANSELDbits.ANSELD0 = 0;     // Disable Analog feature to allow digital to operate correctly.
    LATDbits.LATD0 = GPIO_HIGH;     // Turn the beeper off.
}

//-------------------------------------------------------------------------
void TurnBeeper (BEEP_CONTROL_ENUM onoff)
{
    LATDbits.LATD0 = onoff;
}
// end of file.
//-------------------------------------------------------------------------

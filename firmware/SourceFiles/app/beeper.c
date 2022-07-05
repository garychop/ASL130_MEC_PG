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
    //TurnBeeper(BEEPER_ON);
    TurnBeeper(BEEPER_OFF);
}

//-------------------------------------------------------------------------
void TurnBeeper (BEEP_CONTROL_ENUM onoff)
{
    LATDbits.LATD0 = onoff;
}
// end of file.
//-------------------------------------------------------------------------

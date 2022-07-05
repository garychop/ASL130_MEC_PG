//////////////////////////////////////////////////////////////////////////////
//
// Filename: beeper.h
//
// Description: Provides beeper control to the application.
//
// Author(s): Trevor Parsh (Embedded Wizardry, LLC)
//
// Modified for ASL on Date: 
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BEEPER_H
#define BEEPER_H

/* ***************************    Includes     **************************** */

// from stdlib
#include <stdint.h>
#include <stdbool.h>

/* ******************************   Types   ******************************* */

typedef enum  {
    BEEPER_ON = 0,
    BEEPER_OFF
} BEEP_CONTROL_ENUM;

/* ***********************   Function Prototypes   ************************ */

void beeperInit(void);
void TurnBeeper (BEEP_CONTROL_ENUM);

#endif // BEEPER_H

// end of file.
//-------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////
//
// Filename: BluetoothControl.h
//
// Description: Control driver for the Bluetooth Module
//
// Author(s): G. Chopcinski (Kg Solutins, LLC)
//
// Modified for ASL on Date: June 23, 2022
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BLUETOOTH_CONTROL_H
#define BLUETOOTH_CONTROL_H

/* ***************************    Includes     **************************** */

// from stdlib
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    NO_BT = 0,
    FWD_BT, REV_BT, LEFT_BT, RIGHT_BT
} BT_DIRECTIONS;

/* ***********************   Function Prototypes   ************************ */

void BluetoothControlInit(void);
void EnableBluetooth (void);
void DisableBluetooth (void);
void SendBlueToothSignal (bool, BT_DIRECTIONS);

#endif // BLUETOOTH_CONTROL_H

// end of file.
//-------------------------------------------------------------------------

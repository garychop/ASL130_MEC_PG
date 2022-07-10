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
    FWD_BT, REV_BT, LEFT_BT, RIGHT_BT, RIGHT_CLICK_OUT, LEFT_CLICK_OUT
} BT_DIRECTIONS;

/* ***********************   Function Prototypes   ************************ */

void BluetoothControlInit(void);
void EnableBluetooth (void);
void DisableBluetooth (void);
void SendBlueToothSignal (BT_DIRECTIONS, bool);
bool IsMouseRightClickActive (void);
void GetMouseClickInputs(void);

#endif // BLUETOOTH_CONTROL_H

// end of file.
//-------------------------------------------------------------------------

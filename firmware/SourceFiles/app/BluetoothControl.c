//////////////////////////////////////////////////////////////////////////////
//
// Filename: BluetoothControl.c
//
// Description: Provides control of the Bluetooth Module.
//
// Author(s): G. Chopcinski (Kg Solutions, LLC)
//
// Modified for ASL on Date: 6/23/2022
//
//////////////////////////////////////////////////////////////////////////////


/* **************************   Header Files   *************************** */

// NOTE: This must ALWAYS be the first include in a file.
#include "device_xc8.h"

#include "bsp.h"
#include <stdint.h>
#include <stdbool.h>

#include "BluetoothControl.h"

//------------------------------------------------------------------------------
// Initialize the pins used during Bluetooth operation.
//------------------------------------------------------------------------------
void BluetoothControlInit(void)
{
    TRISEbits.TRISE1 = GPIO_BIT_OUTPUT;    // D1
    TRISDbits.TRISD3 = GPIO_BIT_OUTPUT;    // D2
    TRISCbits.TRISC2 = GPIO_BIT_OUTPUT;    // D3
    TRISCbits.TRISC1 = GPIO_BIT_OUTPUT;    // D4
    TRISAbits.TRISA5 = GPIO_BIT_OUTPUT;    // R_CLK_OUT

    // Set all Bluetooth control lines high
    LATEbits.LATE1 = GPIO_HIGH;      
    LATDbits.LATD3 = GPIO_HIGH;      
    LATCbits.LATC2 = GPIO_HIGH;      
    LATCbits.LATC1 = GPIO_HIGH;      
    LATAbits.LATA5 = GPIO_HIGH;      
}

//-------------------------------------------------------------------------

void SendBlueToothSignal (bool active, BT_DIRECTIONS whichOne)
{
    switch (whichOne)
    {
        case FWD_BT:
            LATDbits.LATD3 = active;
            break;
        case REV_BT:
            LATCbits.LATC2 = active;
            break;
        case LEFT_BT:
            LATEbits.LATE1 = active;
            break;
        case RIGHT_BT:
            LATCbits.LATC1 = active;
            break;
        default:
            break;
    } // end switch
}

//-------------------------------------------------------------------------

void wait10msec (void)
{
    uint8_t i;
    
    // 10 milliseconds, actually 9.74 msecs
    for (i = 0; i < 20; ++i)
        bspDelayUs (US_DELAY_500_us);
}

//-------------------------------------------------------------------------

void wait50msec (void)
{
    uint8_t i;
    
    // 10 milliseconds, actually 9.74 * 5 msecs
    for (i = 0; i < 100; ++i)
        bspDelayUs (US_DELAY_500_us);
}

//-------------------------------------------------------------------------

void SetOutputs (bool value)
{
    LATEbits.LATE1 = value;      
    LATDbits.LATD3 = value;      
    LATCbits.LATC2 = value;      
    LATCbits.LATC1 = value;      
}
//-------------------------------------------------------------------------
void EnableBluetooth (void)
{
    SetOutputs (GPIO_LOW);
    wait10msec();
    SetOutputs (GPIO_HIGH);
    wait10msec();
    SetOutputs (GPIO_LOW);
    wait10msec();
    SetOutputs (GPIO_HIGH);
    wait10msec();
    SetOutputs (GPIO_LOW);
    wait50msec();
    SetOutputs (GPIO_HIGH);
    wait50msec();
    SetOutputs (GPIO_LOW);
    wait50msec();
    SetOutputs (GPIO_HIGH);
    wait50msec();
    wait50msec();
    wait50msec();
}

//-------------------------------------------------------------------------

void DisableBluetooth (void)
{
    SetOutputs (GPIO_HIGH);
    wait10msec();
    SetOutputs (GPIO_LOW);
    wait10msec();
    SetOutputs (GPIO_HIGH);
    wait10msec();
    SetOutputs (GPIO_LOW);
    wait10msec();
    SetOutputs (GPIO_HIGH);
    wait10msec();
    SetOutputs (GPIO_LOW);
    wait50msec();
    SetOutputs (GPIO_HIGH);
    wait50msec();
    wait50msec();
    wait50msec();
}

// end of file.
//-------------------------------------------------------------------------

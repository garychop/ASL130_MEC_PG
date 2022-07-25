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
// File Global variables.

static uint8_t g_MouseClick_DebounceCounter;
static bool g_MouseClick_State;
static bool g_MouseClicksEnabled;

//------------------------------------------------------------------------------
// Forward Declarations
void wait10msec (void);

//------------------------------------------------------------------------------
// Initialize the pins used during Bluetooth operation.
//------------------------------------------------------------------------------
void BluetoothControlInit(void)
{
    TRISEbits.TRISE1 = GPIO_BIT_OUTPUT;    // D1
    ANSELEbits.ANSELE1 = 0;
    TRISDbits.TRISD3 = GPIO_BIT_OUTPUT;    // D2
    ANSELDbits.ANSELD3 = 0;
    TRISCbits.TRISC2 = GPIO_BIT_OUTPUT;    // D3
    ANSELCbits.ANSELC2 = 0;
    TRISCbits.TRISC1 = GPIO_BIT_OUTPUT;    // D4
    ANSELCbits.ANSELC1 = 0;
    TRISAbits.TRISA5 = GPIO_BIT_OUTPUT;    // LEFT_CLK_OUT
    ANSELAbits.ANSELA5 = 0;

    // Set all Bluetooth control lines high
    LATEbits.LATE1 = GPIO_HIGH;      
    LATDbits.LATD3 = GPIO_HIGH;      
    LATCbits.LATC2 = GPIO_HIGH;      
    LATCbits.LATC1 = GPIO_HIGH;      
    LATAbits.LATA5 = GPIO_HIGH;    // Left Click control

#ifndef DEBUG
    // Left Click to BT module. This is shared with some debugging pin
    TRISAbits.TRISA4 = GPIO_BIT_INPUT;
    ANSELAbits.ANSELA4 = 0; // Allow Digital control to operate correctly.
    WPUAbits.WPUA4 = 1;     // Enable a weak pull-up
    //LATAbits.LATA4 = GPIO_HIGH;
    
    // Right Click input, shared with PGC
    TRISBbits.TRISB6 = GPIO_BIT_INPUT;
    ANSELBbits.ANSELB6 = 0;         // Ensure it is setup for Digital
    WPUBbits.WPUB6 = 1;             // Enable a weak pully-up
    g_MouseClick_DebounceCounter = 0;
    g_MouseClick_State = PORTBbits.RB6;
    g_MouseClicksEnabled = (PORTBbits.RB6 ? true : false);   // If low then RT MOUSE CLICKS are disabled.
                                    // .. because the input is held low via a single mono
                                    // .. switch plugged into the Right/Left Mouse Click
                                    // If no switch is plugged in, this input is high.
                                    // If a stereo adapter is plugged and both switches
                                    // .. are open, this input is high and goes low when
                                    // .. the switch is closed.
    
#else
    g_MouseClick_State = true;

#endif    

}

//------------------------------------------------------------------------------

void GetMouseClickInputs(void)
{
    g_MouseClick_State = PORTBbits.RB6;
}

//------------------------------------------------------------------------------

bool IsMouseRightClickActive (void)
{
    if (g_MouseClicksEnabled)
    {
        return (g_MouseClick_State ? false : true);
    }
    else
        return false;
}

//-------------------------------------------------------------------------

void SendBlueToothSignal (BT_DIRECTIONS whichOne, bool active)
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
        case LEFT_CLICK_OUT:
            LATAbits.LATA5 = active;
            break;
        case RIGHT_CLICK_OUT:
#ifndef DEBUG
            LATAbits.LATA4 = active;
#endif
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

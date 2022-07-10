//------------------------------------------------------------------------------
// Filename: UserButton.c
//
// Description: BSP level and application level functions for processing
//      the User Buttons.
//
// Author(s): G. Chopcinski (Kg Solutions, LLC)
//
// Created for ASL on Date: 6/22/2020
//
//------------------------------------------------------------------------------
// NOTE: This must ALWAYS be the first include in a file.
#include "device_xc8.h"

#include "bsp.h"
#include "AnalogInput.h"
#include "UserButton.h"
#include "BluetoothControl.h"

//------------------------------------------------------------------------------
// Macros and defines


#define USE_JOYSTICK_MODE_SWITCH (1)

#ifdef FUTURE_WORK
typedef enum {
    CALIBRATION_BUTTON,
    USER_PORT_BUTTON,
    MODE_PORT_BUTTON,
    MAX_BUTTONS
} BUTTON_ENUM;

typedef struct {
    uint16_t m_PortBit;
    uint8_t m_DebounceCounter;
    bool  m_ButtonState;
} BUTTON_STRUCT;

#endif // FUTURE_WORK

//------------------------------------------------------------------------------
// Local Variables

//BUTTON_STRUCT g_ButtonInfo [MAX_BUTTONS];

static uint8_t g_CalButton_DebounceCounter;
static bool g_CalButtonState;

static uint8_t g_UserPort_DebounceCounter;
static bool g_UserPort_State;

static uint8_t g_ModeButton_DebounceCounter;
static bool g_ModeButton_State;

//------------------------------------------------------------------------------
// Forward Prototype Declarations

//------------------------------------------------------------------------------
// Initialize the User Button inputs as Digital Inputs.
//------------------------------------------------------------------------------

void UserButtonInit(void)
{
    // Initialize the Calibration Button input
    TRISBbits.TRISB2 = GPIO_BIT_INPUT;
    ANSELBbits.ANSELB2 = 0;         // Ensure it's a digital input.
    WPUBbits.WPUB2 = 1;             // Enable weak pullup.
    g_CalButton_DebounceCounter = 0;
    g_CalButtonState = PORTBbits.RB2;

#ifdef USE_JOYSTICK_MODE_SWITCH    
    //Initialize the Mode Button on the Joystick
    TRISBbits.TRISB0 = GPIO_BIT_INPUT;
    ANSELBbits.ANSELB0 = 0;         // Ensure the analog process does not interfere.
    WPUBbits.WPUB0 = 1;             // Enable weak pullup.
    g_ModeButton_DebounceCounter = 0;
    g_ModeButton_State = PORTBbits.RB0;
#endif
    
    // Initialize the SW2 DIP Switches, No need to debounce DIP switches
    TRISBbits.TRISB1 = GPIO_BIT_INPUT;  // SW2-1
    ANSELBbits.ANSELB1 = 0;         // Ensure it's a digital input.
    WPUBbits.WPUB1 = 1;             // Enable weak pullup.

    TRISBbits.TRISB5 = GPIO_BIT_INPUT;  // SW2-2
    ANSELBbits.ANSELB5 = 0;         // Ensure it's a digital input.
    WPUBbits.WPUB5 = 1;             // Enable weak pullup.

#ifndef DEBUG
    // Setup USER PORT as input.
    // Unfortunately, PIN RB6 is shared with PGD programming pin
    TRISBbits.TRISB7 = GPIO_BIT_INPUT;  // USER PORT
    ANSELBbits.ANSELB7 = 0;         // Ensure it is setup for Digital
    WPUBbits.WPUB7 = 1;             // Enable a weak pully-up.
    g_UserPort_State = PORTBbits.RB7;
    g_UserPort_DebounceCounter = 0;
#else
    g_UserPort_State = true;    // 1=Switch is Open, 0=Switch is closed
    g_MouseClick_State = true;

#endif    
}

//------------------------------------------------------------------------------
// Functions returns true if the Calibration Button is pressed.
bool IsCalibrationButtonActive (void)
{
    return (g_CalButtonState ? false : true);   // Closed is active low.
}

//------------------------------------------------------------------------------

bool IsUserPortButtonActive (void)
{
    return (g_UserPort_State ? false : true);   // Closed is active low.
}

//------------------------------------------------------------------------------

bool IsModeButtonActive (void)
{
#ifdef USE_JOYSTICK_MODE_SWITCH    
    uint16_t rawSpeed, rawDirection;
    uint16_t deflection;
    bool reverseIsActive;
   
    reverseIsActive = false;
    if (IsSW2_1_Closed())  // Are we using Reverse as a Mode Switch? YES!
    {
        GetSpeedAndDirection (&rawSpeed, &rawDirection);
        // Don't process Reverse if Direction is active.
        if ((rawDirection > (Joystick_Data[DIRECTION_ARRAY].m_rawNeutral - (NEUTRAL_ERROR_MARGIN * 2)))
        && (rawDirection < (Joystick_Data[DIRECTION_ARRAY].m_rawNeutral + (NEUTRAL_ERROR_MARGIN * 2))))
        {
            if (rawSpeed < Joystick_Data[SPEED_ARRAY].m_rawMinNeutral)
            {
                if (rawSpeed < (Joystick_Data[SPEED_ARRAY].m_rawNeutral - Joystick_Data[SPEED_ARRAY].m_NegativeScale))
                    rawSpeed = Joystick_Data[SPEED_ARRAY].m_rawNeutral - Joystick_Data[SPEED_ARRAY].m_NegativeScale;
                deflection =  Joystick_Data[SPEED_ARRAY].m_rawNeutral - rawSpeed;
                if (deflection > (Joystick_Data[SPEED_ARRAY].m_NegativeScale / 2))
                {
                    reverseIsActive = true;
                }
            }
        }
    }
    if (reverseIsActive)
        return true;
#endif
    
    // Otherwise return the Joystick's Mode Button state.
    return (g_ModeButton_State ? false : true);     // Closed is active low
}

//------------------------------------------------------------------------------

bool IsSW2_1_Closed (void)
{
    return (PORTBbits.RB1 ? false : true);
}

//------------------------------------------------------------------------------

bool IsSW2_2_Closed (void)
{
    return (PORTBbits.RB5 ? false : true);
}

//------------------------------------------------------------------------------

void Read_User_Buttons (void)
{
    // Read and debounce the Calibration Button
    if (PORTBbits.RB2 == g_CalButtonState)
    {
        // Nothing to do, the signal is stable.
        g_CalButton_DebounceCounter = 0;
    }
    else
    {
        ++g_CalButton_DebounceCounter;
        if (g_CalButton_DebounceCounter > MAX_DEBOUNCE)
        {
            g_CalButtonState = PORTBbits.RB2;
            g_CalButton_DebounceCounter = 0;
        }
    }

#ifdef USE_JOYSTICK_MODE_SWITCH    
    
    // Read and debounce the Mode Button on the Joystick.
    if (IsSW2_2_Closed() == false)  // Are we disabling the Joystick's Mode Switch
    {
        if (PORTBbits.RB0 == g_ModeButton_State)
        {
            // Nothing to do, the signal is stable.
            g_ModeButton_DebounceCounter = 0;
        }
        else
        {
            ++g_ModeButton_DebounceCounter;
            if (g_ModeButton_DebounceCounter > 0) // MAX_DEBOUNCE)
            {
                g_ModeButton_State = PORTBbits.RB0;
                g_ModeButton_DebounceCounter = 0;
            }
        }
    }
    else
    {
        g_ModeButton_State = GPIO_HIGH; // Indicate OPEN, Not Active
    }
#else
    g_ModeButton_State = GPIO_HIGH;
#endif
#ifndef DEBUG
    // Process USER PORT switch input
    if (PORTBbits.RB7 == g_UserPort_State)
    {
        // Nothing to do, the signal is stable.
        g_UserPort_DebounceCounter = 0;
    }
    else
    {
        ++g_UserPort_DebounceCounter;
        if (g_UserPort_DebounceCounter > MAX_DEBOUNCE)
        {
            g_UserPort_State = PORTBbits.RB7;
            g_UserPort_DebounceCounter = 0;
        }
    }

#endif
    // Read and debounce the Left and Right Click inputs
    GetMouseClickInputs();
   
}


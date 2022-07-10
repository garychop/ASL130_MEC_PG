/* 
 * File:   DigitalOutput.h
 * Author: G. Chopcinski
 * Comments: Defines the usable external calls.
 * Revision history: June 24, 2022
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef DIGITAL_OUTPUT_H
#define	DIGITAL_OUTPUT_H

//#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <stdbool.h>

void DigitalOutputInit(void);
void SetResetOutput (bool state);

#endif	/* DIGITAL_OUTPUT_H */


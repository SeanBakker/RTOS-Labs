/*----------------------------------------------------------------------------
 * Name: _threadsCore.h
 * Purpose: Library used to create the stacks for multi-threading
 *----------------------------------------------------------------------------
*/
 
//Include guards for _threadsCore
#ifndef _threadsCore
//This file contains relevant pin and other settings, such as register access functions
#include <LPC17xx.h>

//This file is for printf and other IO functions (used for debugging when needed)
#include "stdio.h"

//This file is for integer definitions
#include "stdint.h" 
#endif

//Obtains the initial location of MSP by looking it up in the vector table
uint32_t* getMSPInitialLocation(void);

//Returns the address of a new PSP with offset of "offset" bytes from MSP
uint32_t* getNewThreadStack(uint32_t offset);

//Sets the value of PSP to threadStack and ensures that the microcontroller is using that value by changing the CONTROL register
void setThreadingWithPSP(uint32_t* threadStack);

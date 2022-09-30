/*----------------------------------------------------------------------------
 * Name: _threadsCore.h
 * Purpose: Library used to create the stacks for multi-threading
 *----------------------------------------------------------------------------
*/
 
//Include guards for _threadsCore
#ifndef _threadsCore
#define _threadsCore

#include "osDefs.h"

//Obtains the initial location of MSP by looking it up in the vector table
uint32_t* getMSPInitialLocation(void);

//Returns the address of a new PSP with offset of "offset" bytes from MSP
uint32_t* getNewThreadStack(uint32_t offset);

//Sets the value of PSP to threadStack and ensures that the microcontroller is using that value by changing the CONTROL register
void setThreadingWithPSP(uint32_t* threadStack);

//Create a new thread
void create_thread(void (*func)(void* args));

//Thread function
typedef void *threadFunc(void);

#endif

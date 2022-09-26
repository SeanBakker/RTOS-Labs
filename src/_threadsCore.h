/*----------------------------------------------------------------------------
 * Name: _threadsCore.h
 * Purpose: Library used to create the stacks for multi-threading
 *----------------------------------------------------------------------------
*/
 
//Include guards for _threadsCore
#ifndef osDefs
#include "osDefs.h"
#endif

//Obtains the initial location of MSP by looking it up in the vector table
uint32_t* getMSPInitialLocation(void);

//Returns the address of a new PSP with offset of "offset" bytes from MSP
uint32_t* getNewThreadStack(uint32_t offset);

//Sets the value of PSP to threadStack and ensures that the microcontroller is using that value by changing the CONTROL register
void setThreadingWithPSP(uint32_t* threadStack);

//
void create_thread(void);

//
typedef void *threadFunc(void);

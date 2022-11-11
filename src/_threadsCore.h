/*----------------------------------------------------------------------------
 * Name: _threadsCore.h
 * Purpose: Library used to create the stacks for multi-threading
 *----------------------------------------------------------------------------
*/
 
//Include guards for _threadsCore
#ifndef _threadsCore
#define _threadsCore

#include "osDefs.h"

//Define stack alignment constants
#define EIGHT_BYTE_ALIGN 8
#define FOUR_BYTE_ALIGN 4

//Obtains the initial location of MSP by looking it up in the vector table
uint32_t* getMSPInitialLocation(void);

//Returns the address of a new PSP with offset of "offset" bytes from MSP
uint32_t* getNewThreadStack(uint32_t offset);

//Creates one single thread, returns the thread ID or -1 if the thread cannot be created
int create_thread(void (*func)(void* args), int deadline);

//Creates one single thread, returns the thread ID or -1 if the thread cannot be created
int create_periodic(void (*func)(void* args), int deadline, int period);

//Thread function type
typedef void *threadFunc(void);

#endif

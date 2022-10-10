/*----------------------------------------------------------------------------
 * Name: _kernelCore.h
 * Purpose: Stores any functions that the kernel oversees
 *----------------------------------------------------------------------------
*/

//Include guards for _kernelCore
#ifndef _kernelCore
#define _kernelCore

#include "osDefs.h"

//Initializes memory structures and interrupts necessary to run the kernel
void kernelInit(void);

//Called by the kernel to schedule which threads to run
void osSched(void);

//Sets the value of PSP to threadStack and ensures that the microcontroller is using that value by changing the CONTROL register
void setThreadingWithPSP(uint32_t* threadStack);

//Start the kernel, returns false if no threads have been created
bool kernel_start(void);

//Helper function to switch threads and switch the PSP instead of using assembly
int thread_switch(void);

#endif

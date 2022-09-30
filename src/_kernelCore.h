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

//Initialize the kernel
void kernel_Initialize(void);

//Start the kernel
void kernel_start(void);

//Switch tasks
int task_switch(void);

#endif

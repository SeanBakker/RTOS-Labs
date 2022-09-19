/*----------------------------------------------------------------------------
 * Name: _kernelCore.h
 * Purpose: Stores any functions that the kernel oversees
 *----------------------------------------------------------------------------
*/

//Include guards for _kernelCore
#ifndef _kernelCore
//This file contains relevant pin and other settings, such as register access functions
#include <LPC17xx.h>

//This file is for integer definitions
#include "stdint.h" 
#endif

//Define the System Handler Priority Register 3 for the location of the PendSV priority register
#define SHPR3 *(uint32_t*)0xE000ED20

//Define the Interrupt Control and State Register memory location
#define ICSR *(uint32_t*)0xE000ED04

//Initializes memory structures and interrupts necessary to run the kernel
void kernelInit(void);

//Called by the kernel to schedule which threads to run
void osSched(void);
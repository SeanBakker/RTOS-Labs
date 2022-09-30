/*----------------------------------------------------------------------------
 * Name: osDefs.h
 * Purpose: Definitions for Global OS Constants
 *----------------------------------------------------------------------------
*/

//Include guards for osDefs
#ifndef _osDefs
#define _osDefs

//This file contains relevant pin and other settings, such as register access functions
#include <LPC17xx.h>

//This file is for printf and other IO functions (used for debugging when needed)
#include "stdio.h"

//This file is for integer definitions
#include "stdint.h" 

//Define the System Handler Priority Register 3 for the location of the PendSV priority register
#define SHPR3 *(uint32_t*)0xE000ED20

//Define the Interrupt Control and State Register memory location
#define ICSR *(uint32_t*)0xE000ED04

//Define stack alignment constants
#define EIGHT_BYTE_ALIGN 8
#define FOUR_BYTE_ALIGN 4

//Define maximum number of threads
#define MAX_THREADS 10

//Define thread struct for each thread stored
typedef struct thread_struct
{
	//Thread stack pointer
	uint32_t* threadStack;
	
	//Thread function pointer
	void (*threadFunc)(void* args);
	
	//Store other information in a thread struct?
	//possibly store threadIndex for the array?
	//any other registers?
	
}rtosThread;

#endif

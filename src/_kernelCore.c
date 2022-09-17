/*----------------------------------------------------------------------------
 * Name: _kernelCore.c
 * Purpose: Stores any functions that the kernel oversees
 *----------------------------------------------------------------------------
*/

//Include guards for _kernelCore
#ifndef _kernelCore
#include "_kernelCore.h"
#endif

//Initializes memory structures and interrupts necessary to run the kernel
void kernelInit(void)
{
	//Set bits 23-16 to 0xFF by bit shifting and bitwise OR SHPR3 with this value 
	SHPR3	|= 0xFF << 16;	
}

//Called by the kernel to schedule which threads to run
void osSched(void)
{
	//Set PendSV exception state to pending to run the interrupt when all other interrupts are done
	//Bit 28 of this register controls the behaviour of PendSV 
	ICSR |= 1<<28;
	
	//Clear the pipeline before triggering an interrupt
	__asm("isb");
}

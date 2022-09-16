/*----------------------------------------------------------------------------
 * Name:    _threadsCore.c
 * Purpose: 
 * Version: V1.00
 *----------------------------------------------------------------------------
*/
 
 //Include guards for _threadsCore.h file
#ifndef _threadsCore
//This file contains relevant pin and other settings, such as register access functions
#include <LPC17xx.h>
#include <stdio.h>

//This file is for integer definitions
#include "stdint.h" 
#endif

//Obtains the initial location of MSP by looking it up in the vector table
uint32_t* getMSPInitialLocation(void)
{
	//MSP is stored at 0x0 in the vector table
	uint32_t* msp = 0x00;
	return (uint32_t *) *msp;
}

//Returns the address of a new PSP with offset of "offset" bytes from MSP
uint32_t* getNewThreadStack(uint32_t offset)
{
	uint32_t newThreadStack = (uint32_t)getMSPInitialLocation();
	newThreadStack -= offset;
	
	if (newThreadStack % 8 != 0)
	{
		//Decrement the thread stack pointer by 4 (size of uint32_t)
		newThreadStack -= sizeof(uint32_t);
	}
	
	//Check that the size of the thread stack is not larger than the maximum stack size (0x2000 = 8192 in decimal)
	if (offset <= 8192)
	{
		//Return the thread stack pointer
		return (uint32_t *)newThreadStack;
	}
	else 
	{
		printf("Error: Size of the stack is larger than the maximum");
		return 0;
	}
}

//Sets the value of PSP to threadStack and ensures that the microcontroller is using that value by changing the CONTROL register
void setThreadingWithPSP(uint32_t* threadStack)
{
	//Set PSP
	__set_PSP((uint32_t)threadStack); 
	
	//Set Control
	__set_CONTROL(1<<1);
}

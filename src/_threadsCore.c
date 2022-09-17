/*----------------------------------------------------------------------------
 * Name: _threadsCore.c
 * Purpose: Library used to create the stacks for multi-threading
 *----------------------------------------------------------------------------
*/
 
//Include guards for _threadsCore
#ifndef _threadsCore
#include "_threadsCore.h"
#endif

//Obtains the initial location of MSP by looking it up in the vector table
uint32_t* getMSPInitialLocation(void)
{
	//MSP location is stored at address 0x00 in the vector table
	uint32_t* msp = 0x00;
	
	//Return address of where MSP is stored
	return (uint32_t *) *msp;
}

//Returns the address of a new PSP with an offset of "offset" bytes from MSP
uint32_t* getNewThreadStack(uint32_t offset)
{
	//Set the newThreadStack address to initially be the MSP location
	uint32_t newThreadStack = (uint32_t)getMSPInitialLocation();
	
	//Decrement the address by the offset
	newThreadStack -= offset;
	
	//Check if the stack pointer is not divisible by 8 (internal ARM specification)
	//The stack pointer must lie on an 8-byte boundary
	if (newThreadStack % 8 != 0)
	{
		//Decrement the thread stack pointer by 4 (size of uint32_t)
		newThreadStack -= sizeof(uint32_t);
	}
	
	//Confirm the size of the thread stack is not larger than the maximum stack size (0x2000 = 8192 in decimal)
	if (offset <= 8192)
	{
		//Return the thread stack pointer
		return (uint32_t *)newThreadStack;
	}
	else 
	{
		//If the stack exceeds the allowable size an error is displayed
		printf("Error: Size of the stack is larger than the maximum");
		return 0;
	}
}

//Sets the value of PSP to threadStack and ensures that the microcontroller is using that value by changing the CONTROL register
void setThreadingWithPSP(uint32_t* threadStack)
{
	//Set PSP to the new thread stack address
	__set_PSP((uint32_t)threadStack); 
	
	//Switch to threading mode by setting the CONTROL, which involves shifting a 1 into its 1st bit
	__set_CONTROL(1<<1);
}

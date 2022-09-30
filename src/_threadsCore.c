/*----------------------------------------------------------------------------
 * Name: _threadsCore.c
 * Purpose: Library used to create the stacks for multi-threading
 *----------------------------------------------------------------------------
*/
 
//Include header file for _threadsCore
#include "_threadsCore.h"

//Thread struct array
extern struct thread_struct osThreads[MAX_THREADS];

//Set the number of stacks created to 0 initially
uint32_t numStacks = 0;

//Set the index of the current thread to 0 initially
uint32_t threadIndex = 0;

//Set the number of total threads
uint32_t num_threads = 0;

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
	if (newThreadStack % EIGHT_BYTE_ALIGN != 0)
	{
		//Decrement the thread stack pointer by 4 (FOUR_BYTE_ALIGN)
		newThreadStack -= FOUR_BYTE_ALIGN;
	}
	
	//Confirm the size of all thread stacks will not be larger than the maximum stack size (0x2000 = 8192 in decimal)
	uint32_t maxStackSize = 8192;
	if (maxStackSize >= (numStacks + 1)*offset)
	{
		//Increment the number of stacks allocated
		numStacks++;
		
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

//Creates one single thread
void create_thread(void (*func)(void* args))
{
	//Check to make sure there is room to create the thread
	//max stack size is the offset 512, we need to make sure the thread size will fit into this offset
	//thread size we set?
	
	
	
	//Set 24th bit of the SP
	*--(osThreads[threadIndex].threadStack) = 1<<24;
	
	//Store the PC
	*--(osThreads[threadIndex].threadStack) = (uint32_t)osThreads[threadIndex].threadFunc; //threadFunc is a function pointer
	
	//Store the next registers LR, R12, R3, R2, R1, R0
	for (uint32_t i = 0xE; i > 0x8; i--) 
	{
		printf("%x \n", i);
		*--(osThreads[threadIndex].threadStack) = i;
	}
	
	//Store the next registers R11 through R4
	for (uint32_t i = 0xB; i > 0x3; i--) 
	{
		printf("%x \n", i);
		*--(osThreads[threadIndex].threadStack) = i;
	}
	
	//Store the function pointer for the thread
	osThreads[threadIndex].threadFunc = func;
	
	//Increment the thread index and number of threads
	threadIndex++;
	num_threads++;
}


//Run the next thread in sequential order (index+1)?

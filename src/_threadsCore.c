/*----------------------------------------------------------------------------
 * Name: _threadsCore.c
 * Purpose: Library used to create the stacks for multi-threading
 *----------------------------------------------------------------------------
*/
 
//Include header file for _threadsCore
#include "_threadsCore.h"

int numStacks = 0; //Set the number of stacks created to 0 initially
int num_threads = 0; //Set the number of total threads

extern rtosThread osThreads[MAX_THREADS]; //Thread struct array
extern int runningThread; //Current running thread index
extern uint32_t mspAddress; //Store the address of the MSP location

//Obtains the initial location of MSP by looking it up in the vector table
uint32_t* getMSPInitialLocation(void)
{
	uint32_t* msp = 0; //MSP location is stored at address 0x00 in the vector table
	return (uint32_t *) *msp; //Return address of where MSP is stored
}

//Returns the address of a new PSP with an offset of "offset" bytes from MSP
uint32_t* getNewThreadStack(uint32_t offset)
{
	//Set the newThreadStack address to initially be the MSP location
	uint32_t newThreadStack = (uint32_t)getMSPInitialLocation();
	
	newThreadStack -= offset; //Decrement the address by the offset
	
	//Check if the stack pointer is not divisible by 8 (internal ARM specification)
	//The stack pointer must lie on an 8-byte boundary
	if (newThreadStack % EIGHT_BYTE_ALIGN != 0)
	{
		//Decrement the thread stack pointer by 4 (FOUR_BYTE_ALIGN)
		newThreadStack -= FOUR_BYTE_ALIGN;
	}
	
	//Confirm the size of all thread stacks will not be larger than the maximum stack size
	if (MAX_STACK_SIZE >= (numStacks + 1)*THREAD_STACK_SIZE + MSR_STACK_SIZE)
	{
		numStacks++; //Increment the number of stacks allocated
		return (uint32_t *)newThreadStack; //Return the thread stack pointer
	}
	else 
	{
		//If the stack exceeds the allowable size an error is displayed
		printf("Error: Size of the stack is larger than the maximum");
		return 0; //Return 0 when there is an error
	}
}

//Creates one single thread, returns the thread ID or -1 if the thread cannot be created
int create_thread(void (*func)(void* args))
{
	//Get the new thread stack pointer location
	uint32_t* newThreadStack = getNewThreadStack(MSR_STACK_SIZE + (num_threads*THREAD_STACK_SIZE));
	
	//Check the number of threads does not exceed the maximum
	if (num_threads < MAX_THREADS && newThreadStack != 0)
	{		
		osThreads[num_threads].status = CREATED; //Set the status of the thread
		osThreads[num_threads].threadFunc = func; //Store the function pointer for the thread
		osThreads[num_threads].threadStack = newThreadStack; //Store the stack pointer location for this thread stack pointer
		osThreads[num_threads].timer = TIMESLICE; //Set the timeslice for the thread
		
		//Setup the stack for the new thread
		//Set 24th bit of the SP, this sets xpsr (status register)
		*(--osThreads[num_threads].threadStack) = 1<<24;
		
		//Store the PC as the function we will be running
		*(--osThreads[num_threads].threadStack) = (uint32_t)func;	
		
		//Store the next registers LR, R12, R3, R2, R1, R0
		//LR=0xE, R12=0xD, R3=0xC, R2=0xB, R1=0xA, R0=0x9
		for (uint32_t i = 0xE; i > 0x8; i--) 
		{
			*(--osThreads[num_threads].threadStack) = i;
		}
		
		//Store the next registers R11 through R4
		//R11=0xB, R10=0xA, R9=0x9, R8=0x8, R7=0x7, R6=0x6, R5=0x5, R4=0x4
		for (uint32_t i = 0xB; i > 0x3; i--) 
		{
			*(--osThreads[num_threads].threadStack) = i;
		}
		
		num_threads++; //Increment the number of threads
		return num_threads - 1; //Return the thread index (position of the thread in the array)
	}
	return -1; //Return -1 if the thread cannot be created
}

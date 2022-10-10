/*----------------------------------------------------------------------------
 * Name: _threadsCore.c
 * Purpose: Library used to create the stacks for multi-threading
 *----------------------------------------------------------------------------
*/
 
//Include header file for _threadsCore
#include "_threadsCore.h"

int numStacks = 0; //Set the number of stacks created to 0 initially
int num_threads = 0; //Set the number of total threads
uint32_t maxStackSize = 8192; //Set the maximum stack size (0x2000 = 8192 in decimal)

extern rtosThread osThreads[MAX_THREADS]; //Thread struct array
extern int runningThread; //Current running thread index
extern uint32_t mspAddress; //Store the address of the MSP location

//Obtains the initial location of MSP by looking it up in the vector table
uint32_t* getMSPInitialLocation(void)
{
	//MSP location is stored at address 0x00 in the vector table
	uint32_t* msp = 0;
	//*msp = 0x10004150;
	//printf("MSP: %x\n", *msp);
	//__set_MSP((uint32_t)0x10004150);
	//printf("MSP: %x\n", *msp);
	
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
	
	//Confirm the size of all thread stacks will not be larger than the maximum stack size
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

//Creates one single thread, returns the thread ID or -1 if the thread cannot be created
int create_thread(void (*func)(void* args))
{	
	//Check the number of threads does not exceed the maximum
	if (num_threads < MAX_THREADS)
	{		
		osThreads[num_threads].status = CREATED; //Set the status of the thread
		osThreads[num_threads].threadFunc = func; //Store the function pointer for the thread
		osThreads[num_threads].threadStack = getNewThreadStack(MSR_STACK_SIZE + (num_threads*THREAD_STACK_SIZE)); //Store the stack pointer location for this thread stack pointer
		
		printf("%x \n", (uint32_t)osThreads[num_threads].threadStack);
		
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
		
		//Increment the number of threads
		num_threads++;
		
		//Return the thread index (position of the thread in the array)
		return num_threads - 1;
	}
	//Return -1 if the thread cannot be created
	return -1;
}

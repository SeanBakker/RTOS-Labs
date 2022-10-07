/*----------------------------------------------------------------------------
 * Name: _threadsCore.c
 * Purpose: Library used to create the stacks for multi-threading
 *----------------------------------------------------------------------------
*/
 
//Include header file for _threadsCore
#include "_threadsCore.h"

extern rtosThread osThreads[MAX_THREADS]; //Thread struct array
extern int runningThread; //Current running thread index

int numStacks = 0; //Set the number of stacks created to 0 initially
int num_threads = 0; //Set the number of total threads
uint32_t maxStackSize = 8192; //Set the maximum stack size (0x2000 = 8192 in decimal)
extern uint32_t mspAddress;

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
		
		printf(" HERE 1 \n");
		printf("%x \n", num_threads);
		printf("%x \n", *osThreads[num_threads].threadStack);
		
		//First is xpsr, the status register. If bit 24 is not set and we are in thread mode we get a hard fault, so we just make sure it's set
		*(--osThreads[num_threads].threadStack) = 1<<24;
		
		printf(" HERE 2 \n");
		
		//Next is the program counter, which is set to whatever the function we are running will be
		*(--osThreads[num_threads].threadStack) = (uint32_t)func;
		
		//Setup the stack for the new thread
		//Set 24th bit of the SP, this sets xpsr (status register)
		//*(--osThreads[num_threads].threadStack) = 1<<24;
		

		
		//Store the PC as the function we will be running
		//*(--osThreads[num_threads].threadStack) = (uint32_t)func;
		
		printf(" HERE 3 \n");
		
/*		
		//Next is a set of important registers. These values are meaningless but we are setting them to be nonzero so that the 
		//compiler doesn't optimize out these lines
		*(--osThreads[num_threads].threadStack) = 0xE; //LR
		*(--osThreads[num_threads].threadStack) = 0xC; //R12
		*(--osThreads[num_threads].threadStack) = 0x3; //R3
		*(--osThreads[num_threads].threadStack) = 0x2; //R2
		*(--osThreads[num_threads].threadStack) = 0x1; //R1
		*(--osThreads[num_threads].threadStack) = 0x0; // R0
		
		
		//Now we have registers R11 to R4, which again are just set to random values so that we know for sure that they exist
		*(--osThreads[num_threads].threadStack) = 0xB; //R11
		*(--osThreads[num_threads].threadStack) = 0xA; //R10
		*(--osThreads[num_threads].threadStack) = 0x9; //R9
		*(--osThreads[num_threads].threadStack) = 0x8; //R8
		*(--osThreads[num_threads].threadStack) = 0x7; //R7
		*(--osThreads[num_threads].threadStack) = 0x6; //R6
		*(--osThreads[num_threads].threadStack) = 0x5; //R5
		*(--osThreads[num_threads].threadStack) = 0x4; //R4
*/		
		
		//Store the next registers LR, R12, R3, R2, R1, R0
		//LR=0xE, R12=0xD, R3=0xC, R2=0xB, R1=0xA, R0=0x9
		for (uint32_t i = 0xE; i > 0x8; i--) 
		{
			printf("%x \n", i);
			*(--osThreads[num_threads].threadStack) = i;
		}
		
		//Store the next registers R11 through R4
		//R11=0xB, R10=0xA, R9=0x9, R8=0x8, R7=0x7, R6=0x6, R5=0x5, R4=0x4
		for (uint32_t i = 0xB; i > 0x3; i--) 
		{
			printf("%x \n", i);
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

/*----------------------------------------------------------------------------
 * Name: _kernelCore.c
 * Purpose: Stores any functions that the kernel oversees
 *----------------------------------------------------------------------------
*/

//Include header file for _kernelCore
#include "_kernelCore.h"

#include "_threadsCore.h"

//Thread struct array
struct thread_struct osThreads[MAX_THREADS];

//Global variable for number of stacks created
extern uint32_t numStacks;

//Global variable for index of current running thread
extern uint32_t threadIndex;

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

//Start the kernel
void kernel_start(void)
{
	//Set the first thread as running (index 0 in the struct array)
	threadIndex = 0;
	
	//This function’s purpose is to initialize anything that the first thread needs before it gets going
	
	
	
	//Set thread mode and SP to PSP by calling setThreadingWithPSP function
	setThreadingWithPSP((uint32_t*) osThreads[threadIndex].threadStack);
	
	//Call the scheduling function
	osSched();
}

//Switch tasks
int task_switch(void){
	//Increment the current thread index | where do we actually increment this???
	threadIndex++;
	
	//Decrement the PSP by 16*4=64 bytes after PendSV executes
	*osThreads[threadIndex].threadStack -= 64;
	
	//Set the new PSP
	__set_PSP((uint32_t)osThreads[threadIndex].threadStack);
	
	return 1; //You are free to use this return value in your
	//assembly eventually. It will be placed in r0, so be sure to
	//access it before overwriting r0
}

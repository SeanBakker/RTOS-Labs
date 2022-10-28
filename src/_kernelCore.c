/*----------------------------------------------------------------------------
 * Name: _kernelCore.c
 * Purpose: Stores any functions that the kernel oversees
 *----------------------------------------------------------------------------
*/

//Include header file for _kernelCore and _threadsCore
#include "_kernelCore.h" 
#include "_threadsCore.h"

rtosThread osThreads[MAX_THREADS]; //Static thread struct array
int runningThread = 0; //Current running thread index
uint32_t mspAddress; //Store the address of the MSP location

extern int num_threads; //Number of threads created

//Initializes memory structures and interrupts necessary to run the kernel
void kernelInit(void)
{
	//Set bits 23-16 to 0xFF by bit shifting and bitwise OR SHPR3 with this value 
	SHPR3	|= 0xFF << 16;	
	
	//Store the initial address of the MSP
	mspAddress = *getMSPInitialLocation();
}

//Called by the kernel to schedule which threads to run
void osSched(uint32_t PSP_Offset)
{
	//Check to make sure there is a thread currently running
	if (runningThread >= 0)
	{
		if (osThreads[runningThread].status != SLEEPING)
		{
			osThreads[runningThread].status = WAITING; //Set the current thread as ready but not running yet
		}
		
		//Set the thread stack pointer to the PSP
		//The thread stack pointer must be restored to its location after all registers are pushed
		//This is 16*4 bytes lower than its location before PendSV executes because we are using a uint32_t
		osThreads[runningThread].threadStack = (uint32_t*)(__get_PSP() - PSP_Offset);
	}
	
	int previousActiveThread = runningThread; //Save the previous active thread index
	runningThread++; //Loop through the threads 
	
	//Reset the runningThread back to the first thread (0) after it has looped through all of the threads
	if (runningThread >= num_threads - 1)
	{
		runningThread = 0;
	}
	
	while(osThreads[runningThread].status == SLEEPING)
	{
		runningThread++; //Loop through the threads 
		
		//Reset the runningThread back to the first thread (0) after it has looped through all of the threads
		if (runningThread >= num_threads - 1)
		{
			runningThread = 0;
		}
		
		if (runningThread == previousActiveThread + 1)
		{
			runningThread = num_threads - 1; //Set the running thread to be the idle thread
		}
	}
	
	//Set the thread to be in the running state
	osThreads[runningThread].status = RUNNING;
	
	//Context switch
	osYield();
}

//Call PendSV interrupt to context switch
void osYield(void)
{
	//Set PendSV exception state to pending to run the interrupt when all other interrupts are done
	//Bit 28 of this register controls the behaviour of PendSV 
	ICSR |= 1<<28;
	
	//Clear the pipeline before triggering an interrupt
	__asm("isb");
}

//Sets the value of PSP to threadStack and ensures that the microcontroller is using that value by changing the CONTROL register
void setThreadingWithPSP(uint32_t* threadStack)
{
	//Set PSP to the new thread stack address
	__set_PSP((uint32_t)threadStack); 
	
	//Switch to threading mode by setting the CONTROL, which involves shifting a 1 into its 1st bit
	__set_CONTROL(1<<1);
}

//Sleep function to put a thread to sleep
void osSleep(int sleepTime)
{
	osThreads[runningThread].timer = sleepTime; //Set the timer for the thread
	osThreads[runningThread].status = SLEEPING; //Set the status of the thread to sleeping
	
	osSched(SIXTEEN_BYTE_OFFSET); //Call the scheduler
}

//Start the kernel
bool kernel_start(void)
{
	//Create the idle thread
	create_thread(osIdleThread);
	
	//Configure the SysTick timer
	SysTick_Config(SystemCoreClock/1000);
	
	//Check if any threads have been created
	if (num_threads > 0)
	{
		//Initialization for the first thread before it starts running
		runningThread = -1; //No threads currently running, the thread at index 0 will run first when running the scheduler 
		setThreadingWithPSP(osThreads[0].threadStack); //Set thread mode and SP to PSP by calling setThreadingWithPSP function
		osSched(SIXTEEN_BYTE_OFFSET);
	}
	return 0; //Return false when no threads have been created, or an error occurred
}

//Switch between threads
int thread_switch(void)
{
	//Set the new PSP for the context switch
	__set_PSP((uint32_t)osThreads[runningThread].threadStack);
	return 1; //Return value can be used in assembly in r0
}

//SysTick handler function
void SysTick_Handler(void)
{
	//Decrement the timer for all sleeping threads
	for (int i = 0; i < num_threads - 1; i++)
	{
		//Only decrement the timer for sleeping threads
		if (osThreads[i].status == SLEEPING)
		{
			osThreads[i].timer--;
			//printf("THREAD: %d, SLEEPING TIME: %d\n", i+1, osThreads[i].timer);
			
			//Check that the timer is up for sleeping threads
			if (osThreads[i].timer == 0)
			{
				osThreads[i].status = WAITING;
				osThreads[i].timer = TIMESLICE;
			}
		}
	}
	
	//Decrement the timer of the current running thread
	osThreads[runningThread].timer--;
	
	//Check that the timer is up for running threads
	if (osThreads[runningThread].timer == 0)
	{
		//Check for a running thread
		if (osThreads[runningThread].status == RUNNING)
		{
			osThreads[runningThread].timer = TIMESLICE; //Reset the timeslice for the thread
			
			//Call the scheduler function
			//Offset by only 8x4 bytes when using SysTick to keep the stack aligned
			//In tail-chained interrupts, the 8 hardware saved registers do not need to be pushed again
			osSched(EIGHT_BYTE_OFFSET);
		}
	}
}

//Idle thread when no other thread is running
void osIdleThread(void* args)
{
	//Infinite loop for the thread
	while (1)
	{
		printf("In thread 0\n");
		osSched(SIXTEEN_BYTE_OFFSET); //Call the scheduler 
		//SHOULD THE IDLE THREAD CALL THE SCHEDULER??
		//Maybe our current error is due to the race conditions and not this line?
	}
}

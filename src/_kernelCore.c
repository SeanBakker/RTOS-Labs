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

extern int num_threads; //Number of threads created

//Initializes memory structures and interrupts necessary to run the kernel
void kernelInit(void)
{
	//Define the priorities of the PendSV, SysTick, and SVC interrupts
	SHPR3 |= 0xFE << 16; //Set the priority of PendSV to almost the weakest (0xFE)
	SHPR3 |= 0xFFU << 24; //Set the priority of SysTick to be the weakest (0xFFU)
	SHPR2 |= 0xFDU << 24; //Set the priority of SVC the be the strongest (0xFDU)
}

//Called by the kernel to schedule which threads to run
void osSched(uint32_t PSP_Offset)
{
	//Check to make sure there is a thread currently running
	if (runningThread >= 0)
	{
		//Sleeping threads are handled by the SysTick handler so their state should not be changed here after a context switch
		if (osThreads[runningThread].status != SLEEPING)
		{
			osThreads[runningThread].status = WAITING; //Set the current thread as waiting but not running yet
		}
		
		//Set the thread stack pointer to the PSP
		//The thread stack pointer must be restored to its location after all registers are pushed
		//This is the specified PSP_Offset number of bytes lower than its location before PendSV executes
		osThreads[runningThread].threadStack = (uint32_t*)(__get_PSP() - PSP_Offset);
	}
	
	int previousActiveThread = runningThread; //Save the previous active thread index
	runningThread++; //Loop through the threads 
	
	//Reset the runningThread back to the first thread (0) after it has looped through all of the threads
	if (runningThread >= num_threads - 1)
	{
		runningThread = 0;
	}
	
	//Loop through the thread struct array when the next runningThread index is a sleeping thread
	while(osThreads[runningThread].status == SLEEPING || osThreads[runningThread].status == BLOCKED)
	{
		runningThread++; //Increment the index to loop through the threads 
		
		//Reset the runningThread back to the first thread (0) after it has looped through all of the threads
		if (runningThread >= num_threads - 1)
		{
			runningThread = 0;
		}
		
		//After looping through all of the threads, if they are all sleeping then run the idle thread
		if (runningThread == previousActiveThread + 1)
		{
			runningThread = num_threads - 1; //Set the running thread to be the idle thread
		}
	}
	
	//Set the thread to be in the running state
	osThreads[runningThread].status = RUNNING;
}

//Call PendSV interrupt to context switch
void osYield(void)
{
	//Trigger the SVC handler
	__asm("SVC #0");
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
	osYield(); //Yield
}

//Start the kernel
bool kernel_start(void)
{
	create_thread(osIdleThread); //Create the idle thread
	
	SysTick_Config(SystemCoreClock/1000); //Configure the SysTick timer
	
	//Check if any threads have been created
	if (num_threads > 0)
	{
		//Initialization for the first thread before it starts running
		runningThread = -1; //No threads currently running, the thread at index 0 will run first when running the scheduler 
		setThreadingWithPSP(osThreads[0].threadStack); //Set thread mode and SP to PSP by calling setThreadingWithPSP function
		osYield(); //Yield
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

//SysTick handler function to handle timers
void SysTick_Handler(void)
{
	//Decrement the timer for all sleeping threads
	for (int i = 0; i < num_threads - 1; i++)
	{
		//Only decrement the timer for sleeping threads
		if (osThreads[i].status == SLEEPING)
		{
			osThreads[i].timer--; //Decrement timer
			
			//Check that the timer is up for sleeping threads
			if (osThreads[i].timer == 0)
			{
				osThreads[i].status = WAITING; //Set status from sleeping to waiting
				osThreads[i].timer = TIMESLICE; //Reset the timer to the default timeslice
			}
		}
	}
	
	osThreads[runningThread].timer--; //Decrement the timer of the running thread
	
	//Check that the timer is up for running threads
	if (osThreads[runningThread].timer == 0)
	{
		osThreads[runningThread].timer = TIMESLICE; //Reset the timeslice for the thread
		
		//Call the scheduler function
		//Offset by only 8x4 bytes when using SysTick to keep the stack aligned
		//In tail-chained interrupts, the 8 hardware saved registers do not need to be pushed again
		osSched(EIGHT_BYTE_OFFSET);
		
		//Yield manually so that we don't have to pass the offset into osYield()
		//Set PendSV exception state to pending to run the interrupt when all other interrupts are done
		//Bit 28 of this register controls the behaviour of PendSV 
		ICSR |= 1<<28;
		
		//Clear the pipeline before triggering an interrupt
		__asm("isb");
	}
}

//SVC handler function
void SVC_Handler_Main(uint32_t *svc_args)
{
	//Get the argument from the stack
	char call = ((char*)svc_args[6])[-2];
	
	//Yield Switch
	if(call == YIELD_SWITCH)
	{	
		//Run the scheduler
		osSched(EIGHT_BYTE_OFFSET);
		
		//Set PendSV exception state to pending to run the interrupt when all other interrupts are done
		//Bit 28 of this register controls the behaviour of PendSV 
		ICSR |= 1<<28;
		
		//Clear the pipeline before triggering an interrupt
		__asm("isb");
	}
}

//Idle thread when no other thread is running
//This thread could call the scheduler after running 
//However, for this lab it was chosen to run for the complete time slice of 5ms before context switching
void osIdleThread(void* args)
{
	//Infinite loop for the thread
	while (1)
	{
		//Only print the first time the idle thread loops
		if (osThreads[runningThread].timer == TIMESLICE)
		{
			printf("Running idle thread\n");
		}
	}
}

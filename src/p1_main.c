//This file sets up the UART
#include "uart.h"

//Include header file for _threadsCore
#include "_threadsCore.h"

//Include header file for _kernelCore
#include "_kernelCore.h"

//Variables for threads to test that they are working
int x = 0;
int y = 0;
int z = 0;

//Thread 1
void thread1(void* args)
{
	//Infinite loop for the thread
	while (1)
	{
		x++;
		printf("In thread 1. x is: %d\n", x);
		osSched(); //Call the scheduler 
	}
}

//Thread 2
void thread2(void* args)
{
	//Infinite loop for the thread
	while (1)
	{
		y++;
		printf("In thread 2. y is: %d\n", y);
		osSched(); //Call the scheduler 
	}
}

//Thread 3
void thread3(void* args)
{
	//Infinite loop for the thread
	while (1)
	{
		z++;
		printf("In thread 3. z is: %d\n", x);
		osSched(); //Call the scheduler 
	}
}

//Idle thread when no other thread is running
void osIdleThread(void* args)
{
	//Infinite loop for the thread
	while (1)
	{
		printf("In thread 0\n");
		osSched(); //Call the scheduler 
	}
}

//This is C. The expected function heading is int main(void)
int main(void) 
{
	//Always call this function at the start. It sets up various peripherals, the clock etc. If you don't call this
	//you may see some weird behaviour
	SystemInit();
	
	//Test the _threadsCore library
	//Print the value of the initial MSP location
	uint32_t* msp = getMSPInitialLocation();
	printf("\nInitial MSP Location: %x\n", (uint32_t)msp);
	
	//Test the interrupt code
	//Call kernelInit before the infinite while loop
	kernelInit();
	
	//Setup threads
	create_thread(osIdleThread);
	create_thread(thread1);
	create_thread(thread2);
	create_thread(thread3);
	
	//Start the kernel
	kernel_start();
	
	//Your code should always terminate in an endless loop if it is done. If you don't
	//the processor will enter a hardfault and will be weird
	while(1);
}

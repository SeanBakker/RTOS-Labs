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


//Test Cases #1
/*
//Thread 1 (yields after running)
void thread1(void* args)
{
	//Infinite loop for the thread
	while (1)
	{
		x++;
		printf("In thread 1. x is: %d\n", x);
		osSched(SIXTEEN_BYTE_OFFSET); //Call the scheduler 
	}
}

//Thread 2 (sleeps after running)
void thread2(void* args)
{
	//Infinite loop for the thread
	while (1)
	{
		y++;
		printf("In thread 2. y is: %d\n", y);
		osSleep(100); //Sleep for 100ms
	}
}

//Thread 3 (doesn't yield or sleep)
void thread3(void* args)
{
	//Infinite loop for the thread
	while (1)
	{
		z++;
		printf("In thread 3. z is: %d\n", x);
	}
}
*/

//Test Cases #2
//Thread 1 (yields after running)
void thread1(void* args)
{
	//Infinite loop for the thread
	while (1)
	{
		x++;
		printf("In thread 1. x is: %d\n", x);
		osSleep(3); //Sleep for 3ms
	}
}

//Thread 2 (sleeps after running)
void thread2(void* args)
{
	//Infinite loop for the thread
	while (1)
	{
		y++;
		printf("In thread 2. y is: %d\n", y);
		osSleep(7); //Sleep for 7ms
	}
}

//Thread 3 (doesn't yield or sleep)
void thread3(void* args)
{
	//Infinite loop for the thread
	while (1)
	{
		z++;
		printf("In thread 3. z is: %d\n", x);
		osSleep(17); //Sleep for 17ms
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
	create_thread(thread1);
	create_thread(thread2);
	create_thread(thread3);
	
	//Start the kernel
	kernel_start();
	
	//Your code should always terminate in an endless loop if it is done. If you don't
	//the processor will enter a hardfault and will be weird
	while(1);
}

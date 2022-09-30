//This file sets up the UART
#include "uart.h"

//Include header file for _threadsCore
#include "_threadsCore.h"

//Include header file for _kernelCore
#include "_kernelCore.h"

int x = 0;
int y = 0;

void task1(void* args)
{
	while (1)
	{
		x++;
		printf("In task 1. x is: %d\n", x);
		osSched();
	}
}

void task2(void* args)
{
	while (1)
	{
		y++;
		printf("In task 2. y is: %d\n", y);
		osSched();
	}
}

void osIdleTask(void* args)
{
	while (1)
	{
		printf("In task 0\n");
		osSched();
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
	printf("%x\n", (uint32_t)msp);
	
	//Create a new thread stack 512 bytes below the value of the MSP and set the processor to use it
	setThreadingWithPSP(getNewThreadStack(512));
	
	//Test the interrupt code
	//Call kernelInit before the infinite while loop
	kernelInit();
	
	//Setup threads
	create_thread(osIdleTask);
	create_thread(task1);
	create_thread(task2);
	
	//Start the kernel
	kernel_start();
	
	//Your code should always terminate in an endless loop if it is done. If you don't
	//the processor will enter a hardfault and will be weird
	while(1);
}

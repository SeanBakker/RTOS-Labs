//This file sets up the UART
#include "uart.h"

//Include header file for _threadsCore
#include "_threadsCore.h"

//Include header file for _kernelCore
#include "_kernelCore.h"

//Include header file for _mutexAPI
#include "_mutexAPI.h"

//Variables for threads to test that they are working
int x = 0;
int y = 0;
int z = 0;

//Define created mutexes
int mutex_1;
int mutex_2;

//Define the threads
int thread_1;
int thread_2;
int thread_3;

//Test Cases #1
//Thread 1 (yields after running)
void thread1(void* args)
{
	//Infinite loop for the thread
	while (1)
	{
		//Acquire the mutex for uart to print
		if(osAcquireMutex(thread_1, mutex_1))
		{
			printf("Thread 1\n"); //print
			printf("Extra Printing -----------------------------------------------------------------------\n");
		}
		
		//Release the mutex after printing
		osReleaseMutex(thread_1, mutex_1);  
		osYield(); //Yield 
	}
}

//Thread 2 (yields after running)
void thread2(void* args)
{
	//Infinite loop for the thread
	while (1)
	{
		//Acquire the mutex for uart to print
		if(osAcquireMutex(thread_2, mutex_1))
		{
			printf("Thread 2\n"); //print
		}
		
		//Release the mutex after printing
		osReleaseMutex(thread_2, mutex_1);
		osYield(); //Yield 
	}
}

//Thread 3 (yields after running)
void thread3(void* args)
{
	//Infinite loop for the thread
	while (1)
	{
		//Acquire the mutex for uart to print
		if(osAcquireMutex(thread_3, mutex_1))
		{
			printf("Thread 3\n"); //print
		}
		
		//Release the mutex after printing
		osReleaseMutex(thread_3, mutex_1);
		osYield(); //Yield 
	}
}

/*
//Test Cases #2
//Thread 1 (sleeps 3ms after running)
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

//Thread 2 (sleeps 7ms after running)
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

//Thread 3 (sleeps 17ms after running)
void thread3(void* args)
{
	//Infinite loop for the thread
	while (1)
	{
		z++;
		printf("In thread 3. z is: %d\n", z);
		osSleep(17); //Sleep for 17ms
	}
}
*/

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
	thread_1 = create_thread(thread1);
	thread_2 = create_thread(thread2);
	thread_3 = create_thread(thread3);
	
	//Setup mutexes
	
	//Test case #1
	mutex_1 = osCreateMutex();
	
	//Test case #2
	/*
	mutex_1 = osCreateMutex();
	mutex_2 = osCreateMutex();
	*/
	
	//Start the kernel
	kernel_start();
	
	//Your code should always terminate in an endless loop if it is done. If you don't
	//the processor will enter a hardfault and will be weird
	while(1);
}

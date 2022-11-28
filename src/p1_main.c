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
/*
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
			printf("Extra Printing -----------------------\n");
			
			//Release the mutex after printing
			osReleaseMutex(thread_1, mutex_1);  
		}
		
		osYield(); //Yield 
	}
}

//Thread 2 (yields after running)
void thread2(void* args)
{
	//Infinite loop for the thread
	while (1)
	{
		x++; //Increment x
		
		//Acquire the mutex for uart to print
		if(osAcquireMutex(thread_2, mutex_1))
		{
			printf("Thread 2, x = %d\n", x); //print
			
			//Release the mutex after printing
			osReleaseMutex(thread_2, mutex_1);
		}
		
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
			
			//Release the mutex after printing
			osReleaseMutex(thread_3, mutex_1);
		}
		
		osYield(); //Yield 
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
		//Acquire the mutex for the global variable
		if (osAcquireMutex(thread_1, mutex_1))
		{
			x++; //Increment x
			printf("Thread 1, x is: %d\n", x);
			
			//Release the mutex after incrementing
			osReleaseMutex(thread_1, mutex_1);
		}
		
		osYield(); //Yield 
	}
}

//Thread 2 (yields after running)
void thread2(void* args)
{
	//Infinite loop for the thread
	while (1)
	{
		//Acquire both mutexes for the global variable and the LEDs
		if(osAcquireMutex(thread_2, mutex_1) && osAcquireMutex(thread_2, mutex_2))
		{
			printf("Thread 2, x mod 47 is: %d\n", x % 47);
			
			//Set the LEDs
			
			
			//Release the mutexes
			osReleaseMutex(thread_2, mutex_1);
			osReleaseMutex(thread_2, mutex_2);
		}
		
		osYield(); //Yield 
	}
}

//Thread 3 (yields after running)
void thread3(void* args)
{
	//Infinite loop for the thread
	while (1)
	{
		//Acquire mutex for the LEDs
		if(osAcquireMutex(thread_3, mutex_2))
		{
			printf("Thread 3\n");
			
			//Set the LEDs
			
			
			//Release the mutexes
			osReleaseMutex(thread_3, mutex_2);
		}
		
		osYield(); //Yield  
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
	thread_1 = create_thread(thread1);
	thread_2 = create_thread(thread2);
	thread_3 = create_thread(thread3);
	
	//Setup mutexes
	//Test case #1 & #2
	mutex_1 = osCreateMutex();
	mutex_2 = osCreateMutex();
	
	//Start the kernel
	kernel_start();
	
	//Your code should always terminate in an endless loop if it is done. If you don't
	//the processor will enter a hardfault and will be weird
	while(1);
}

//This file sets up the UART
#include "uart.h"

//This file contains relevant pin and other settings 
#include <LPC17xx.h>

//This file is for printf and other IO functions
#include "stdio.h"

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
		bool acquireMutex1 = osAcquireMutex(thread_2, mutex_1);
		bool acquireMutex2 = osAcquireMutex(thread_2, mutex_2);
		
		//Acquire both mutexes for the global variable and the LEDs
		if(acquireMutex1 && acquireMutex2)
		{
			printf("Thread 2, x mod 47 is: %d\n", x % 47);
			
			//Set the LEDs to x%47
			//Convert x from decimal to unsigned 8-bit binary
			unsigned int binary_x = (x % 47);
			
			//Check each bit for each LED
			for(int i = 0; i < 8; i++)
			{
				//Check the bit of the integer x
				if((binary_x >> i) & 1)
				{
					//Set the LED
					if (i < 2)
					{
						//Set the first 2 LEDs
						LPC_GPIO1->FIOSET |= 1<<(28 + i);
					}
					else if (i == 2)
					{
						//Set the third LED
						LPC_GPIO1->FIOSET |= 1<<(31);
					}
					else 
					{
						//Set the last 5 LEDs
						LPC_GPIO2->FIOSET |= 1<<(i - 1);
					}
				}
				else 
				{
					//Clear the LED
					if (i < 2)
					{
						//Clear the first 2 LEDs
						LPC_GPIO1->FIOCLR |= 1<<(28 + i);
					}
					else if (i == 2)
					{
						//Clear the third LED
						LPC_GPIO1->FIOCLR |= 1<<(31);
					}
					else 
					{
						//Clear the last 5 LEDs
						LPC_GPIO2->FIOCLR |= 1<<(i - 1);
					}
				}
			}
			
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
			
			//Set the LEDs to 0x71 - 01110001 in binary
			LPC_GPIO1->FIOCLR |= 1<<28;
			LPC_GPIO1->FIOSET |= 1<<29;
			LPC_GPIO1->FIOSET |= 1<<31;	
			LPC_GPIO2->FIOSET |= 1<<2;	
			LPC_GPIO2->FIOCLR |= 1<<3;
			LPC_GPIO2->FIOCLR |= 1<<4;
			LPC_GPIO2->FIOCLR |= 1<<5; 
			LPC_GPIO2->FIOSET |= 1<<6;
			
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
	
	//Test Case #2 - Setup the LEDs
	LPC_GPIO1->FIODIR |= 1<<28;
	LPC_GPIO1->FIODIR |= 1<<29;
	LPC_GPIO1->FIODIR |= 1<<31;	
	LPC_GPIO2->FIODIR |= 1<<2;	
	LPC_GPIO2->FIODIR |= 1<<3;
	LPC_GPIO2->FIODIR |= 1<<4;
	LPC_GPIO2->FIODIR |= 1<<5; 
	LPC_GPIO2->FIODIR |= 1<<6;
	
	//Start the kernel
	kernel_start();
	
	//Your code should always terminate in an endless loop if it is done. If you don't
	//the processor will enter a hardfault and will be weird
	while(1);
}

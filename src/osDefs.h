/*----------------------------------------------------------------------------
 * Name: osDefs.h
 * Purpose: Definitions for Global OS Constants to be used with the kernel and threading
 *----------------------------------------------------------------------------
*/

//Include guards for osDefs
#ifndef _osDefs
#define _osDefs

#include "LPC17xx.h" //This file contains relevant pin and other settings, such as register access functions
#include "stdio.h" //This file is for printf and other IO functions (used for debugging when needed)
#include "stdint.h" //This file is for integer definitions
#include "stddef.h" //This file is for standard definitions
#include "stdbool.h" //This file is for using bool keyword

//Define the System Handler Priority Register 3 for the location of the PendSV priority register
#define SHPR3 *(uint32_t*)0xE000ED20

//Define the System Handler Priority Register 2 for the location of the SVC priority register
#define SHPR2 *(uint32_t*)0xE000ED1C

//Define the Interrupt Control and State Register memory location
#define ICSR *(uint32_t*)0xE000ED04

//Define stack sizes
#define MSR_STACK_SIZE 0x400 //Size of the model-specific registers (MSR) reserved memory
#define THREAD_STACK_SIZE 0x200 //Max thread size offset is 512 = 0x200
#define MAX_STACK_SIZE 0x2000 //Set the maximum stack size (0x2000)

//Stack alignment constants for context switching
#define EIGHT_BYTE_OFFSET 8*4 //Stack PSP offset for tail-chained interrupts

//Define maximum number of threads
//10 threads for the user + the idle thread
#define MAX_THREADS 11

//Thread states
#define RUNNING 1 //Active thread is running
#define WAITING 2 //Thread is waiting to be run
#define SLEEPING 3 //Thread is sleeping for a specified time after running

//Timeslice for how long a thread will run (5ms)
#define TIMESLICE 5 

//Define the interrupt numbers for SVC
#define YIELD_SWITCH 0

//Define the idle thread deadline
#define IDLE_DEADLINE 100000

//Define thread struct for each thread stored
typedef struct thread_struct
{
	uint32_t* threadStack; //Thread stack pointer
	void (*threadFunc)(void* args); //Thread function pointer
	int status; //Status of the thread (Running/Waiting/Blocked)
	int timer; //Timer for the thread
	int timeToDeadline; //Timer for the deadline
	int period; //Stores the thread period (periodic thread)
	int deadline; //Stores the thread deadline
}rtosThread;

#endif

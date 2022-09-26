/*----------------------------------------------------------------------------
 * Name: osDefs.h
 * Purpose: Definitions for Global OS Constants
 *----------------------------------------------------------------------------
*/

//This file contains relevant pin and other settings, such as register access functions
#include <LPC17xx.h>

//This file is for printf and other IO functions (used for debugging when needed)
#include "stdio.h"

//This file is for integer definitions
#include "stdint.h" 

//Define the System Handler Priority Register 3 for the location of the PendSV priority register
#define SHPR3 *(uint32_t*)0xE000ED20

//Define the Interrupt Control and State Register memory location
#define ICSR *(uint32_t*)0xE000ED04

//Define maximum number of threads
#define MAX_THREADS 10

//Global variable for number of stacks created
extern uint32_t numStacks;

//Global variable for index of current thread
extern uint32_t threadIndex;

//Define typedef of thread struct
typedef struct rtosThread thread_struct;

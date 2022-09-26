//This file sets up the UART
#include "uart.h"

//Include guards for _threadsCore
#ifndef _threadsCore
#include "_threadsCore.h"
#endif

//Include guards for _kernelCore
#ifndef _kernelCore
#include "_kernelCore.h"
#endif

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
	//Call kernelInit and osSched before the infinite while loop
	kernelInit();
	osSched();
	
	//Your code should always terminate in an endless loop if it is done. If you don't
	//the processor will enter a hardfault and will be weird
	while(1);
}

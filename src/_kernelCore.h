/*----------------------------------------------------------------------------
 * Name:    _kernelCore.h
 * Purpose: 
 * Version: V1.00
 *----------------------------------------------------------------------------
*/

//Define System Handler Priority Register 3 for the location of the PendSV priority register
#define SHPR3 *(uint32_t*)0xE000ED20

//Define Interrupt Control and State Register memory location
#define ICSR *(uint32_t*)0xE000ED04

//Initializes memory structures and interrupts necessary to run the kernel
void kernelInit(void);

//Called by the kernel to schedule which threads to run
void osSched(void);

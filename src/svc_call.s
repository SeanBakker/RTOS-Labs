	AREA handle_pend,CODE,READONLY ;Define new area which needs a contiguous block of space to hold code that is readonly
	GLOBAL PendSV_Handler ;Declare global function to be used by other files to handle the PendSV interrupt
	PRESERVE8 ;Stack will lie on 8 byte boundary
PendSV_Handler ;Define PendSV_Handler function
	MOV LR,#0xFFFFFFFD ;Moves constant address into the link register
	BX LR ;Return from function by branching to link register address
	END
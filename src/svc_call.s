	AREA	handle_pend,CODE,READONLY ;Define new area which needs a contiguous block of space to hold code that is readonly
	EXTERN task_switch ;I am going to call a C function to handle the switching
	GLOBAL PendSV_Handler ;Declare global function to be used by other files to handle the PendSV interrupt
	PRESERVE8 ;Stack will lie on 8 byte boundary

PendSV_Handler ;Define PendSV_Handler function
	MRS r0,PSP
	;Store the registers
	STMDB r0!,{r4-r11}
	;call kernel task switch
	BL task_switch
	MRS r0,PSP ;this is the new task stack
	MOV LR,#0xFFFFFFFD ;Moves constant address into the link register to go back to Thread mode
	;LoaD Multiple Increment After, basically undo the stack pushes we did before
	LDMIA r0!,{r4-r11}
	;Reload PSP. Now that we've popped a bunch, PSP has to be updated
	MSR PSP,r0
	;Return from function by branching to link register address
	BX LR
	END
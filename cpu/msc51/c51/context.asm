
NAME	CONTEXT

?PR?Timer0ISR?CONTEXT                    SEGMENT CODE 
?PR?_ThreadSwitchTo?CONTEXT              SEGMENT CODE 
?XD?_ThreadSwitchTo?CONTEXT              SEGMENT XDATA OVERLAYABLE 
?ID?CONTEXT          SEGMENT IDATA 
	EXTRN	XDATA (OS_ThreadHandlerIndex)
	EXTRN	CODE (OS_ENTER_CRITICAL)
	EXTRN	CODE (Timer_Reload)
	EXTRN	XDATA (OS_RunningThreadID)
	EXTRN	CODE (ThreadScan)
	EXTRN	CODE (OS_EXIT_CRITICAL)
	EXTRN	CODE (?C?PSTOPTR)
	EXTRN	CODE (?C?PLDOPTR)
	EXTRN	CODE (?C?CSTOPTR)
	EXTRN	CODE (?C?PLDPTR)
	EXTRN	CODE (?C?CLDOPTR)
	EXTRN	CODE (?C?ICALL)
	PUBLIC	OSStack
	PUBLIC	_ThreadSwitchTo
	PUBLIC	Timer0ISR

	RSEG  ?XD?_ThreadSwitchTo?CONTEXT
?_ThreadSwitchTo?BYTE:
     thread?140:   DS   3

	RSEG  ?ID?CONTEXT
        OSStack:   DS   10

CSEG	AT	0000BH
	LJMP	Timer0ISR


	RSEG  ?PR?Timer0ISR?CONTEXT
	USING	0
Timer0ISR:
	PUSH 	ACC
	PUSH 	B
	PUSH 	DPH
	PUSH 	DPL
	PUSH 	PSW
	MOV  	PSW,#00H
	PUSH 	AR0
	PUSH 	AR1
	PUSH 	AR2
	PUSH 	AR3
	PUSH 	AR4
	PUSH 	AR5
	PUSH 	AR6
	PUSH 	AR7
	USING	0
	LCALL	OS_ENTER_CRITICAL
	MOV  	R1,SP
	MOV  	R2,#00H
	MOV  	R3,#00H
	PUSH 	AR3
	PUSH 	AR2
	PUSH 	AR1
	MOV  	DPTR,#OS_RunningThreadID
	MOVX 	A,@DPTR
	MOV  	B,#03H
	MUL  	AB
	ADD  	A,#LOW (OS_ThreadHandlerIndex)
	MOV  	DPL,A
	MOV  	A,B
	ADDC 	A,#HIGH (OS_ThreadHandlerIndex)
	MOV  	DPH,A
	MOVX 	A,@DPTR
	MOV  	R3,A
	INC  	DPTR
	MOVX 	A,@DPTR
	MOV  	R2,A
	INC  	DPTR
	MOVX 	A,@DPTR
	MOV  	R1,A
	MOV  	DPTR,#03H
	LCALL	?C?PSTOPTR
	MOV  	A,#LOW (OSStack)
	DEC  	A
	MOV  	SP,A
	LCALL	Timer_Reload
	LCALL	ThreadScan
	MOV  	DPTR,#OS_RunningThreadID
	MOVX 	A,@DPTR
	MOV  	B,#03H
	MUL  	AB
	ADD  	A,#LOW (OS_ThreadHandlerIndex)
	MOV  	DPL,A
	MOV  	A,B
	ADDC 	A,#HIGH (OS_ThreadHandlerIndex)
	MOV  	DPH,A
	MOVX 	A,@DPTR
	MOV  	R3,A
	INC  	DPTR
	MOVX 	A,@DPTR
	MOV  	R2,A
	INC  	DPTR
	MOVX 	A,@DPTR
	MOV  	R1,A
	MOV  	DPTR,#03H
	LCALL	?C?PLDOPTR
	MOV  	SP,R1
	LCALL	OS_EXIT_CRITICAL
	POP  	AR7
	POP  	AR6
	POP  	AR5
	POP  	AR4
	POP  	AR3
	POP  	AR2
	POP  	AR1
	POP  	AR0
	POP  	PSW
	POP  	DPL
	POP  	DPH
	POP  	B
	POP  	ACC
	RETI 	


	RSEG  ?PR?_ThreadSwitchTo?CONTEXT
_ThreadSwitchTo:
	USING	0
	MOV  	DPTR,#thread?140
	MOV  	A,R3
	MOVX 	@DPTR,A
	INC  	DPTR
	MOV  	A,R2
	MOVX 	@DPTR,A
	INC  	DPTR
	MOV  	A,R1
	MOVX 	@DPTR,A
	MOV  	DPTR,#thread?140
	MOVX 	A,@DPTR
	MOV  	R3,A
	INC  	DPTR
	MOVX 	A,@DPTR
	MOV  	R2,A
	INC  	DPTR
	MOVX 	A,@DPTR
	MOV  	R1,A
	MOV  	DPTR,#07H
	MOV  	A,#03H
	LCALL	?C?CSTOPTR
	LCALL	?C?PLDPTR
	MOV  	A,R1
	ADD  	A,#0FFH
	MOV  	R1,A
	MOV  	A,R2
	ADDC 	A,#0FFH
	MOV  	R2,A
	PUSH 	AR3
	PUSH 	AR2
	PUSH 	AR1
	MOV  	DPTR,#thread?140
	MOVX 	A,@DPTR
	MOV  	R3,A
	INC  	DPTR
	MOVX 	A,@DPTR
	MOV  	R2,A
	INC  	DPTR
	MOVX 	A,@DPTR
	MOV  	R1,A
	MOV  	DPTR,#03H
	LCALL	?C?PSTOPTR
	MOV  	DPTR,#thread?140
	MOVX 	A,@DPTR
	MOV  	R3,A
	INC  	DPTR
	MOVX 	A,@DPTR
	MOV  	R2,A
	INC  	DPTR
	MOVX 	A,@DPTR
	MOV  	R1,A
	MOV  	DPTR,#0AH
	LCALL	?C?CLDOPTR
	MOV  	DPTR,#OS_RunningThreadID
	MOVX 	@DPTR,A
	LCALL	?C?PLDPTR
	MOV  	A,R1
	DEC  	A
	MOV  	SP,A
	MOV  	DPTR,#thread?140
	MOVX 	A,@DPTR
	MOV  	R3,A
	INC  	DPTR
	MOVX 	A,@DPTR
	MOV  	R2,A
	INC  	DPTR
	MOVX 	A,@DPTR
	MOV  	R1,A
	MOV  	DPTR,#0CH
	LCALL	?C?PLDOPTR
	LJMP 	?C?ICALL

	END

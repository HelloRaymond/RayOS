PUBLIC	SaveContext
PUBLIC	RecoveryContext

EXTRN	DATA(ContextStack)

Context  SEGMENT  CODE
RSEG  Context

SaveContext:
	MOV	ContextStack, PSW
	MOV	ContextStack+1, A
	MOV	ContextStack+2, B
	MOV	ContextStack+3, DPL
	MOV	ContextStack+4, DPH
	MOV	ContextStack+5, R0
	MOV	ContextStack+6, R1
	MOV	ContextStack+7, R2
	MOV	ContextStack+8, R3
	MOV	ContextStack+9, R4
	MOV	ContextStack+10, R5
	MOV	ContextStack+11, R6
	MOV	ContextStack+12, R7
RET

RecoveryContext:
	MOV	R7, ContextStack+12
	MOV	R6, ContextStack+11
	MOV	R5, ContextStack+10
	MOV	R4, ContextStack+9
	MOV	R3, ContextStack+8
	MOV	R2, ContextStack+7
	MOV	R1, ContextStack+6
	MOV	R0, ContextStack+5
	MOV	DPH, ContextStack+4
	MOV	DPL, ContextStack+3
	MOV	B, ContextStack+2
	MOV	A, ContextStack+1
	MOV	PSW, ContextStack
RET

END

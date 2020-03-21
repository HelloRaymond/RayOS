PUBLIC	SaveContext
PUBLIC	RecoveryContext

EXTRN	DATA(GPRStack,SFRStack)

Context  SEGMENT  CODE
RSEG  Context

SaveContext:
	MOV	SFRStack, PSW
	MOV	SFRStack+1, A
	MOV	SFRStack+2, B
	MOV	SFRStack+3, DPL
	MOV	SFRStack+4, DPH
	MOV	GPRStack, R0
	MOV	GPRStack+1, R1
	MOV	GPRStack+2, R2
	MOV	GPRStack+3, R3
	MOV	GPRStack+4, R4
	MOV	GPRStack+5, R5
	MOV	GPRStack+6, R6
	MOV	GPRStack+7, R7
RET

RecoveryContext:
	MOV	PSW, SFRStack
	MOV	A, SFRStack+1
	MOV	B, SFRStack+2
	MOV	DPL, SFRStack+3
	MOV	DPH, SFRStack+4
	MOV	R0, GPRStack
	MOV	R1, GPRStack+1
	MOV	R2, GPRStack+2
	MOV	R3, GPRStack+3
	MOV	R4, GPRStack+4
	MOV	R5, GPRStack+5
	MOV	R6, GPRStack+6
	MOV	R7, GPRStack+7
RET

END

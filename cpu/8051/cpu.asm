;{************************************************************************
;* FileName      : cpu.asm
;* Author        : Raymond Hsu (Xu Rui)
;* Description   : OS Startup File
;{************************************************************************
;* -------HISTORY_LOG--------
;* Version  	 : V1.1.0
;* Date          : 2020.3.21
;* Modify        : Separate context switching code
;*************************************************************************}
;{************************************************************************
;* -------HISTORY_LOG--------
;* Version  	 : V1.0.0
;* Date          : 2018.5.6
;* Modify        : Added for the first time to implement task scheduler
;                  context switching; Initial initialization function, 
;                  interrupt service function call function
;*************************************************************************}
IDATALEN EQU 80H
XDATASTART EQU 0 
XDATALEN EQU 0 
PDATASTART EQU 0H
PDATALEN EQU 0H
IBPSTACK EQU 0 ; set to 1 if small reentrant is used.
IBPSTACKTOP EQU 0xFF +1 ; default 0FFH+1 
XBPSTACK EQU 0 ; set to 1 if large reentrant is used.
XBPSTACKTOP EQU 0xFFFF +1 ; default 0FFFFH+1 
PBPSTACK EQU 0 ; set to 1 if compact reentrant is used.
PBPSTACKTOP EQU 0xFF +1 ; default 0FFH+1 
PPAGEENABLE EQU 0 ; set to 1 if pdata object are used.
PPAGE EQU 0
PPAGE_SFR DATA 0A0H

PUBLIC	?C_STARTUP

EXTRN	IDATA(OSStack)
EXTRN	IDATA(TaskStack)
EXTRN	CODE(?C_START)
EXTRN	CODE(SaveContext,RecoveryContext)
EXTRN	CODE(ThreadScan,ThreadSwitch)
EXTRN	DATA(StackPointer)

TIMER0_VECTOR EQU 000BH
	
?C_STARTUP :
	;System Reset
	CSEG	AT	0000H
	JMP SystemInit
	;Timer0 interrupt vector
	CSEG	AT	TIMER0_VECTOR
	JMP	Timer0ISR
	CSEG	AT	0100H

Timer0ISR:
	CLR	EA
	MOV	StackPointer, SP
	MOV	SP, #(OSStack - 1)
	CALL	SaveContext
	CALL	ThreadScan
	CALL	ThreadSwitch
	CALL	RecoveryContext
	MOV	SP, StackPointer
	SETB	EA
RETI

SystemInit:
	CLR EA
	IF IDATALEN <> 0
	 MOV R0,#IDATALEN - 1
	 CLR A
	IDATALOOP: MOV @R0,A
	 DJNZ R0,IDATALOOP
	ENDIF
	
	IF XDATALEN <> 0
	 MOV DPTR,#XDATASTART
	 MOV R7,#LOW (XDATALEN)
	 IF (LOW (XDATALEN)) <> 0
	 MOV R6,#(HIGH (XDATALEN)) +1
	 ELSE
	 MOV R6,#HIGH (XDATALEN)
	 ENDIF
	 CLR A
	XDATALOOP: MOVX @DPTR,A
	 INC DPTR
	 DJNZ R7,XDATALOOP
	 DJNZ R6,XDATALOOP
	ENDIF
	
	IF PPAGEENABLE <> 0
	 MOV PPAGE_SFR,#PPAGE
	ENDIF
	
	IF PDATALEN <> 0
	 MOV R0,#LOW (PDATASTART)
	 MOV R7,#LOW (PDATALEN)
	 CLR A
	PDATALOOP: MOVX @R0,A
	 INC R0
	 DJNZ R7,PDATALOOP
	ENDIF
	
	IF IBPSTACK <> 0
	EXTRN DATA (?C_IBP)
	
	 MOV ?C_IBP,#LOW IBPSTACKTOP
	ENDIF
	
	IF XBPSTACK <> 0
	EXTRN DATA (?C_XBP)
	
	 MOV ?C_XBP,#HIGH XBPSTACKTOP
	 MOV ?C_XBP+1,#LOW XBPSTACKTOP
	ENDIF
	
	IF PBPSTACK <> 0
	EXTRN DATA (?C_PBP)
	 MOV ?C_PBP,#LOW PBPSTACKTOP
	ENDIF
	MOV	SP, #(TaskStack - 1)
	JMP		?C_START
RET

END

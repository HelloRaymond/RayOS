;{************************************************************************
;* -------参考Keil自带STARTUP.A51文件--------
;* FileName      : RaySTARTUP.asm
;* FileName      : RaySTARTUP.asm
;* Author        : 徐睿
;* Description   : OS Startup File
;{************************************************************************
;* -------HISTORY_LOG--------
;* Version  	 : V1.0.0
;* Date          : 2018.5.6
;* Modify        : 首次添加，实现任务调度器上下文切换功能
;                  、启动初期初始化功能、中断服务函数调用功能
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
EXTRN	CODE(ThreadScan,ThreadSwitch)
EXTRN	CODE(INT0_ISR,INT1_ISR,Timer1_ISR,Uart1_ISR,ADC_ISR,LVD_ISR,PCA_ISR,Uart2_ISR,SPI_ISR,INT2_ISR,INT3_ISR,Timer2_ISR,INT4_ISR,Uart3_ISR,Uart4_ISR,Timer3_ISR,Timer4_ISR,Comparator_ISR,PWM_ISR,PWMFD_ISR)
EXTRN	DATA(StackPointer,GPRStack,SFRStack)

RESET_VECTOR	EQU 0000H
INT0_VECTOR	EQU 0003H
TIMER0_VECTOR EQU 000BH
INT1_VECTOR	EQU 0013H
TIMER1_VECTOR	EQU 001BH
UART1_VECTOR	EQU 0023H
ADC_VECTOR	EQU 002BH
LVD_VECTOR	EQU 0033H
PCA_VECTOR	EQU 003BH
UART2_VECTOR	EQU 0043H
SPI_VECTOR	EQU 004BH
INT2_VECTOR	EQU 0053H
INT3_VECTOR	EQU 005BH
TIMER2_VECTOR	EQU 0063H
INT4_VECTOR	EQU 0083H
UART3_VECTOR	EQU 008BH
UART4_VECTOR	EQU 0093H
TIMER3_VECTOR	EQU 009BH
TIMER4_VECTOR	EQU 00A3H
COMPARATOR_VECTOR	EQU 00ABH
PWM_VECTOR	EQU 00B3H
PWMFD_VECTOR	EQU 00BBH

	
?C_STARTUP :
	;系统复位
	CSEG	AT	RESET_VECTOR
	JMP SystemInit
	;INT0 中断向量
	CSEG	AT	INT0_VECTOR
	JMP	INT0ISR
	;Timer0 中断向量
	CSEG	AT	TIMER0_VECTOR
	JMP	Timer0ISR
	;INT1中断向量	
	CSEG	AT	INT1_VECTOR
	JMP	INT1ISR
	;Timer1 中断向量
	CSEG	AT	TIMER1_VECTOR
	JMP	Timer1ISR
	;Uart1 中断向量
	CSEG	AT	UART1_VECTOR
	JMP	Uart1ISR
	;ADC 中断向量
	CSEG	AT	ADC_VECTOR
	JMP	ADCISR
	;LVD中断向量
	CSEG	AT	LVD_VECTOR
	JMP	LVDISR
	;PCA中断向量
	CSEG	AT	PCA_VECTOR
	JMP	PCAISR
	;Uart2中断向量
	CSEG	AT	UART2_VECTOR
	JMP	Uart2ISR
	;SPI 中断向量
	CSEG	AT	SPI_VECTOR
	JMP	SPIISR
	;INT2中断向量
	CSEG	AT	INT2_VECTOR
	JMP	INT2ISR
	;INT3中断向量
	CSEG	AT	INT3_VECTOR
	JMP	INT3ISR
	;Timer2中断向量
	CSEG	AT	TIMER2_VECTOR
	JMP	Timer2ISR
	;INT4中断向量
	CSEG	AT	INT4_VECTOR
	JMP	INT4ISR
	;Uart3中断向量
	CSEG	AT	Uart3_VECTOR
	JMP	Uart3ISR
	;Uart4中断向量
	CSEG	AT	Uart4_VECTOR
	JMP	Uart4ISR
	;Timer3中断向量
	CSEG	AT	TIMER3_VECTOR
	JMP	Timer3ISR
	;Timer4中断向量
	CSEG	AT	TIMER4_VECTOR
	JMP	Timer4ISR
	;Comparator中断向量
	CSEG	AT	COMPARATOR_VECTOR
	JMP	ComparatorISR
	;PWM中断向量
	CSEG	AT	PWM_VECTOR
	JMP	PWMISR
	;PWMFD中断向量
	CSEG	AT	PWMFD_VECTOR
	JMP	PWMFDISR

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

INT0ISR:
	MOV	StackPointer, SP
	MOV	SP, #(OSStack - 1)
	LCALL	SaveContext
	LCALL	INT0_ISR
	LCALL	RecoveryContext
	MOV	SP, StackPointer
RETI

Timer0ISR:
	CLR	EA
	MOV	StackPointer, SP
	MOV	SP, #(OSStack - 1)
	LCALL	SaveContext
	LCALL	ThreadScan
	LCALL	ThreadSwitch
	LCALL	RecoveryContext
	MOV	SP, StackPointer
	SETB	EA
RETI

INT1ISR:
	MOV	StackPointer, SP
	MOV	SP, #(OSStack - 1)
	LCALL	SaveContext
	LCALL	INT1_ISR
	LCALL	RecoveryContext
	MOV	SP, StackPointer
RETI

Timer1ISR:
	MOV	StackPointer, SP
	MOV	SP, #(OSStack - 1)
	LCALL	SaveContext
	LCALL	Timer1_ISR
	LCALL	RecoveryContext
	MOV	SP, StackPointer
RETI

Uart1ISR:
	MOV	StackPointer, SP
	MOV	SP, #(OSStack - 1)
	LCALL	SaveContext
	LCALL	Uart1_ISR
	LCALL	RecoveryContext
	MOV	SP, StackPointer
RETI

ADCISR:
	MOV	StackPointer, SP
	MOV	SP, #(OSStack - 1)
	LCALL	SaveContext
	LCALL	ADC_ISR
	LCALL	RecoveryContext
	MOV	SP, StackPointer
RETI

LVDISR:
	MOV	StackPointer, SP
	MOV	SP, #(OSStack - 1)
	LCALL	SaveContext
	LCALL	LVD_ISR
	LCALL	RecoveryContext
	MOV	SP, StackPointer
RETI

PCAISR:
	MOV	StackPointer, SP
	MOV	SP, #(OSStack - 1)
	LCALL	SaveContext
	LCALL	PCA_ISR
	LCALL	RecoveryContext
	MOV	SP, StackPointer
RETI

Uart2ISR:
	MOV	StackPointer, SP
	MOV	SP, #(OSStack - 1)
	LCALL	SaveContext
	LCALL	Uart2_ISR
	LCALL	RecoveryContext
	MOV	SP, StackPointer
RETI

SPIISR:
	MOV	StackPointer, SP
	MOV	SP, #(OSStack - 1)
	LCALL	SaveContext
	LCALL	SPI_ISR
	LCALL	RecoveryContext
	MOV	SP, StackPointer
RETI

INT2ISR:
	MOV	StackPointer, SP
	MOV	SP, #(OSStack - 1)
	LCALL	SaveContext
	LCALL	INT2_ISR
	LCALL	RecoveryContext
	MOV	SP, StackPointer
RETI

INT3ISR:
	MOV	StackPointer, SP
	MOV	SP, #(OSStack - 1)
	LCALL	SaveContext
	LCALL	INT3_ISR
	LCALL	RecoveryContext
	MOV	SP, StackPointer
RETI

Timer2ISR:
	MOV	StackPointer, SP
	MOV	SP, #(OSStack - 1)
	LCALL	SaveContext
	LCALL	Timer2_ISR
	LCALL	RecoveryContext
	MOV	SP, StackPointer
RETI

INT4ISR:
	MOV	StackPointer, SP
	MOV	SP, #(OSStack - 1)
	LCALL	SaveContext
	LCALL	INT4_ISR
	LCALL	RecoveryContext
	MOV	SP, StackPointer
RETI

Uart3ISR:
	MOV	StackPointer, SP
	MOV	SP, #(OSStack - 1)
	LCALL	SaveContext
	LCALL	Uart3_ISR
	LCALL	RecoveryContext
	MOV	SP, StackPointer
RETI

Uart4ISR:
	MOV	StackPointer, SP
	MOV	SP, #(OSStack - 1)
	LCALL	SaveContext
	LCALL	Uart4_ISR
	LCALL	RecoveryContext
	MOV	SP, StackPointer
RETI

Timer3ISR:
	MOV	StackPointer, SP
	MOV	SP, #(OSStack - 1)
	LCALL	SaveContext
	LCALL	Timer3_ISR
	LCALL	RecoveryContext
	MOV	SP, StackPointer
RETI

Timer4ISR:
	MOV	StackPointer, SP
	MOV	SP, #(OSStack - 1)
	LCALL	SaveContext
	LCALL	Timer4_ISR
	LCALL	RecoveryContext
	MOV	SP, StackPointer
RETI

ComparatorISR:
	MOV	StackPointer, SP
	MOV	SP, #(OSStack - 1)
	LCALL	SaveContext
	LCALL	Comparator_ISR
	LCALL	RecoveryContext
	MOV	SP, StackPointer
RETI

PWMISR:
	MOV	StackPointer, SP
	MOV	SP, #(OSStack - 1)
	LCALL	SaveContext
	LCALL	PWM_ISR
	LCALL	RecoveryContext
	MOV	SP, StackPointer
RETI

PWMFDISR:
	MOV	StackPointer, SP
	MOV	SP, #(OSStack - 1)
	LCALL	SaveContext
	LCALL	PWMFD_ISR
	LCALL	RecoveryContext
	MOV	SP, StackPointer
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

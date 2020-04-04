	.module context
	.optsdcc -mmcs51 --model-large
	
	.globl _Timer0ISR
	.globl _Timer_Reload
	.globl _OS_EXIT_CRITICAL
	.globl _OS_ENTER_CRITICAL
	.globl _ThreadScan
	.globl _SP_REG
	.globl _OSStack
	.globl _ThreadSwitchTo

	.area RSEG    (ABS,DATA)
	.org 0x0000
_SP_REG	=	0x0081

	.area RSEG    (ABS,DATA)
	.org 0x0000

	.area REG_BANK_0	(REL,OVR,DATA)
	.ds 8

	.area BIT_BANK	(REL,OVR,DATA)
bits:
	.ds 1
	b0 = bits[0]
	b1 = bits[1]
	b2 = bits[2]
	b3 = bits[3]
	b4 = bits[4]
	b5 = bits[5]
	b6 = bits[6]
	b7 = bits[7]

	.area DSEG    (DATA)
_ThreadSwitchTo_sloc0_1_0:
	.ds 3

	.area ISEG    (DATA)
_OSStack::
	.ds 10

	.area IABS    (ABS,DATA)
	.area IABS    (ABS,DATA)

	.area BSEG    (BIT)

	.area PSEG    (PAG,XDATA)

	.area XSEG    (XDATA)
_ThreadSwitchTo_thread_65536_22:
	.ds 3

	.area XABS    (ABS,XDATA)

	.area XISEG   (XDATA)
	.area HOME    (CODE)
	.area GSINIT0 (CODE)
	.area GSINIT1 (CODE)
	.area GSINIT2 (CODE)
	.area GSINIT3 (CODE)
	.area GSINIT4 (CODE)
	.area GSINIT5 (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area CSEG    (CODE)

	.area HOME    (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area GSINIT  (CODE)

	.area HOME    (CODE)
	.area HOME    (CODE)

	.area CSEG    (CODE)

_Timer0ISR:
	ar7 = 0x07
	ar6 = 0x06
	ar5 = 0x05
	ar4 = 0x04
	ar3 = 0x03
	ar2 = 0x02
	ar1 = 0x01
	ar0 = 0x00
	push	bits
	push	acc
	push	b
	push	dpl
	push	dph
	push	(0+7)
	push	(0+6)
	push	(0+5)
	push	(0+4)
	push	(0+3)
	push	(0+2)
	push	(0+1)
	push	(0+0)
	push	psw
	mov	psw,#0x00
	lcall	_OS_ENTER_CRITICAL
	mov	dptr,#_OS_RunningThreadID
	movx	a,@dptr
	mov	b,#0x03
	mul	ab
	add	a,#_OS_ThreadHandlerIndex
	mov	dpl,a
	mov	a,#(_OS_ThreadHandlerIndex >> 8)
	addc	a,b
	mov	dph,a
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x03
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	r2,_SP_REG
	mov	r3,#0x00
	mov	r4,#0x00
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	mov	a,r2
	lcall	__gptrput
	inc	dptr
	mov	a,r3
	lcall	__gptrput
	inc	dptr
	mov	a,r4
	lcall	__gptrput
	mov	a,#_OSStack
	dec	a
	mov	_SP_REG,a
	lcall	_Timer_Reload
	lcall	_ThreadScan
	mov	dptr,#_OS_RunningThreadID
	movx	a,@dptr
	mov	b,#0x03
	mul	ab
	add	a,#_OS_ThreadHandlerIndex
	mov	dpl,a
	mov	a,#(_OS_ThreadHandlerIndex >> 8)
	addc	a,b
	mov	dph,a
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x03
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	_SP_REG,a
	lcall	_OS_EXIT_CRITICAL
	pop	psw
	pop	(0+0)
	pop	(0+1)
	pop	(0+2)
	pop	(0+3)
	pop	(0+4)
	pop	(0+5)
	pop	(0+6)
	pop	(0+7)
	pop	dph
	pop	dpl
	pop	b
	pop	acc
	pop	bits
	reti

_ThreadSwitchTo:
	mov	r7,b
	mov	r6,dph
	mov	a,dpl
	mov	dptr,#_ThreadSwitchTo_thread_65536_22
	movx	@dptr,a
	mov	a,r6
	inc	dptr
	movx	@dptr,a
	mov	a,r7
	inc	dptr
	movx	@dptr,a
	mov	dptr,#_ThreadSwitchTo_thread_65536_22
	movx	a,@dptr
	mov	r5,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	a,#0x07
	add	a,r5
	mov	r2,a
	clr	a
	addc	a,r6
	mov	r3,a
	mov	ar4,r7
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,#0x03
	lcall	__gptrput
	add	a,r5
	mov	_ThreadSwitchTo_sloc0_1_0,a
	clr	a
	addc	a,r6
	mov	(_ThreadSwitchTo_sloc0_1_0 + 1),a
	mov	(_ThreadSwitchTo_sloc0_1_0 + 2),r7
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	r0,a
	inc	dptr
	lcall	__gptrget
	mov	r1,a
	inc	dptr
	lcall	__gptrget
	mov	r4,a
	dec	r0
	cjne	r0,#0xff,00103$
	dec	r1
00103$:
	mov	dpl,_ThreadSwitchTo_sloc0_1_0
	mov	dph,(_ThreadSwitchTo_sloc0_1_0 + 1)
	mov	b,(_ThreadSwitchTo_sloc0_1_0 + 2)
	mov	a,r0
	lcall	__gptrput
	inc	dptr
	mov	a,r1
	lcall	__gptrput
	inc	dptr
	mov	a,r4
	lcall	__gptrput
	mov	a,#0x0a
	add	a,r5
	mov	r2,a
	clr	a
	addc	a,r6
	mov	r3,a
	mov	ar4,r7
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
	mov	dptr,#_OS_RunningThreadID
	movx	@dptr,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	dec	a
	mov	_SP_REG,a
	mov	a,#0x0c
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	r5,a
	inc	dptr
	lcall	__gptrget
	mov	r6,a
	push	ar6
	push	ar5
	mov	dpl,r5
	mov	dph,r6
	lcall	__sdcc_call_dptr
	pop	ar5
	pop	ar6
	ret
	.area CSEG    (CODE)
	.area CONST   (CODE)
	.area XINIT   (CODE)
	.area CABS    (ABS,CODE)

CODE		SEGMENT PUBLIC 'CODE'

		ASSUME	CS:CODE

		ORG	0		;start code at loc 0

userRoutine	PROC	FAR		;must be a FAR process

begin:		jmp	doIt

		ORG	4		;improve execution speed to
					;align at word boundary

resultSave	DW	?		;location to return result
ssSave		DW	?		;stack segment save loc
spSave		DW	?		;stack point save loc
argumentCnt	DW	?		;argument count
argument1	DW	?		;declare as many as needed
argument2	DW	?

stackBottom	DW	128 DUP (?)	;good practice to use your own stack
stackTop	EQU	$

doIt:		push	ax		;push ax in old stack

		mov	cs:[ssSave],ss	;save old stack segment
		mov	cs:[spSave],sp	;save old stack pointer

		mov	ax,[bp]
		mov	cs:[argumentCnt],ax	;[bp] = argument count
		mov	ax,[bp+2]
		mov	cs:[argument1],ax	;[bp+2] = the last argument
		mov	ax,[bp+4]
		mov	cs:[argument2],ax	;[bp+4] = the next to last argument

		cli			;disable interrupt when changing stack

		mov	ax,cs
		mov	ss,ax		;our new stack is in the code segment
		mov	ax,OFFSET stackTop
		mov	sp,ax		;now we have new stack
		sti			;re-enable interrupt after changing stack

		push	bx		;save all registers that you use
		push	cx

		;do work here, as an example, raise argument 2 to
		;the power of argument 1

		mov	ax,cs:[argument2]
		mov	bx,ax
		mov	cx,cs:[argument1]
		dec	cx

multiLoop:	mul	bx
		loop	multiLoop

		mov	cs:[resultSave],ax	;save result

		pop	cx			;restore registers
		pop	bx

		cli				;disable interrupt when changing stack

		mov	sp,cs:[spSave]		;restore old stack pointer
		mov	ss,cs:[ssSave]		;restore old stack segment

		sti				;re-enable interrupt after changing stack

		pop	ax			;restore reg ax from old stack
		ret

userRoutine	ENDP

CODE		ENDS
		END    BEGIN

;
; INSTALL.ASM   created 06/09/89  by Drew Gislason
; Copyright (C) 1989 by Datalight
;
; C CALLABLE FUNCTIONS SUPPORTED HERE:
;
;    _getDS()
;    _getCS()
;    _cli()
;    _sti()
;    _install(vector,(*func)())
;
INCLUDE MMACROS.INC

CBegCode

c_func  _getDS
	MOV     AX,DS
	RET
c_endp  _getDS

c_func  _getCS
	MOV     AX,CS
	RET
c_endp  _getCS

	PUBLIC  __cli,__sti
IF largeCODE
__cli   PROC    FAR
ELSE
__cli   PROC    NEAR
ENDIF
	CLI
	RET
__cli   ENDP

IF largeCODE
__sti   PROC    FAR
ELSE
__sti   PROC    NEAR
ENDIF
	STI
	RET
__sti   ENDP


; install(vector,(*func)())
c_func  _install
	PUSH    BP
	MOV     BP,SP
	PUSH    DS

	MOV     BX,P[BP]        ; vector #
	SHL     BX,1
	SHL     BX,1            ; multiply by four to get offset
	XOR     AX,AX
	MOV     DS,AX           ; DS:[BX] pts to vector

	MOV     AX,P+2[BP]      ; AX =offset of function
IF largeCODE
	MOV     CX,P+4[BP]
ELSE
	MOV     CX,CS           ; CX = seg of function
ENDIF

	CLI
	MOV     DS:[BX],AX      ; set the vector
	MOV     DS:2[BX],CX
	STI

	POP     DS
	POP     BP
	RET
c_endp  _install


IFNDEF  ??version
;       This fulfills the external call from _output (for
;       printf and sprintf).
;
	extrn _exit:near
c_func  _flsbuf
	mov     AX, 10
	push    AX
	call    _exit
c_endp  _flsbuf
ENDIF


CEndCode
	end

COMMENT ^ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  MMACROS.INC  created Apr 3, 1989  by Drew Gislason
  Copyright (C) 1989 by Datalight, All Rights Reserved
  Version  1.50

  General macros for interfacing assembly with C Code

  Modified 06/01/89  DG  Made generic for BOTH Turbo C and Microsoft C

  MACRO DESCRIPTIONS:
  ------------------

  CBegCode		; open the code segment (CODE)
  CEndCode
  CBegData		; open the data segment (DATA)
  CEndData
  CBegUdata		; open the uninitialized data segment (BSS)
  CEndUdata
  c_func <name>		; declares a function callable from C
  c_endp <name>		; declare the ending of a function
  c_callm <name>	; calls a C function
  c_public <name>	; declares a c public
  c_extrn <name>,<type>	; declares a "C" external
  c_extrnP <name>	; declares an external function
  largeCODE		; TRUE if M, L or H model
  largeDATA		; TRUE if C, L or H model
  P			; offset of first parameter on stack
  GOFFSET		; DGROUP:xxxx (offset of a variable within DGROUP)
  COFFSET		; CS:xxxx (offset within the current CODE segment)


EXAMPLE:

CBegCode
	c_extrnP a_routine
	c_extrn  value,word

c_func	name			; C function declaration name()
	PUSH	BP
	MOV	BP,SP
if largeDATA
	MOV	ES,P+4[BP]	; some code dependant on large data
endif
	.
	.
	MOV	AX,P[BP]	; get first argument (word)
	MOV	BX,P+2[BP]	; get next argument (word)
	.
	callm	a_routine	; call a C routine from assembly
	MOV	value,AX	; return value (if any) will be in AX
	.
	POP	BP
	RET
c_endp	name

CEndCode
	END


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ^

; <<<<< if no model chosen, default to SMALL >>>>>
IFNDEF memS
IFNDEF memM
IFNDEF memL
IFNDEF memC
IFNDEF memH
    memS = 1
ENDIF
ENDIF
ENDIF
ENDIF
ENDIF


; <<<<< Print Message on Pass 1 >>>>>
out1 MACRO msg
    IF1
	%out msg
    ENDIF
ENDM

; <<<<< Model setup macro >>>>>
outif MACRO name,msg,codesize,datasize
    IFDEF name
    IF name
	out1 <* &msg>
	largeCODE = codesize
	largeDATA = datasize
    ENDIF
    ENDIF
ENDM

; <<<<< show version and model during assembly >>>>>
out1	<>
out1	<Mmacros Version 1.50>

outif memS,<Small Model>,	0,0
outif memM,<Medium Model>,	1,0
outif memC,<Compact Model>,	0,1
outif memL,<Large Model>,	1,1
outif memH,<Huge Model>,	1,1


; <<<<< CODE segment declaration >>>>>
CBegCode	MACRO
_TEXT	SEGMENT	WORD PUBLIC 'CODE'
	ASSUME	CS:_TEXT
ENDM

CEndCode	MACRO
_TEXT	ENDS
	ENDM

; <<<<< DATA segment declarations >>>>>
CBegData	MACRO
_DATA	SEGMENT WORD PUBLIC 'DATA'
_DATA	ENDS
IFNDEF	??version
    CONST	SEGMENT WORD PUBLIC 'CONST'
    CONST	ENDS
    MSG		SEGMENT WORD PUBLIC 'MSG'
    MSG		ENDS
ENDIF
_BSS	SEGMENT WORD PUBLIC 'BSS'
_BSS	ENDS
DGROUP	GROUP	_DATA,_BSS
IFNDEF 	??version
    DGROUP	GROUP	CONST,MSG
ENDIF
_DATA	SEGMENT
	ASSUME DS:DGROUP
	ENDM

CEndData	MACRO
_DATA	ends
	ENDM

CBegUdata	MACRO
_DATA	SEGMENT WORD PUBLIC 'DATA'
_DATA	ENDS
IFNDEF	??version
    CONST	SEGMENT WORD PUBLIC 'CONST'
    CONST	ENDS
    MSG		SEGMENT WORD PUBLIC 'MSG'
    MSG		ENDS
ENDIF
_BSS	SEGMENT WORD PUBLIC 'BSS'
_BSS	ENDS
DGROUP	GROUP	_DATA,_BSS
IFNDEF 	??version
    DGROUP	GROUP	CONST,MSG
ENDIF
_BSS	SEGMENT
	ASSUME DS:DGROUP
	ENDM

CEndUdata	MACRO
_BSS	ENDS
	ENDM


IF largeCODE
	P	equ	6
ELSE
	P	equ	4
ENDIF

c_func	MACRO	name
	public	_&name
IF largeCODE
_&name	PROC	FAR
ELSE
_&name	PROC	NEAR
ENDIF
	name	EQU	_&name
	ENDM

c_endp	MACRO	name
_&name	ENDP
	ENDM

c_callm	MACRO	name
    IF largeCODE
	CALL	FAR PTR	_&name
    ELSE
	CALL	NEAR PTR _&name
    ENDIF
    name	EQU	_&name
	ENDM

c_public MACRO	a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z
    ifnb <a>			; recursive check for blank argument
	public	_&a
	a EQU _&a
      ifnb <b>
	c_public b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z
      endif
    endif
	ENDM

c_extrn	MACRO	a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z
    ifnb <a>			; recursive check for blank argument
	extrn	_&a:b
	a EQU _&a
      ifnb <c>
	c_extrn	c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z
      endif
    endif
	ENDM


c_extrnP MACRO	name
    ifnb <name>
      IF largeCODE
	extrn _&name:far
      ELSE
	extrn _&name:near
      ENDIF
      name	EQU	_&name
    endif
	ENDM

c_label	MACRO	name,xxx
    ifnb <name>
	PUBLIC	_&name
	_&name	LABEL	xxx
	name	EQU	_&name
    endif
	ENDM


c_labelP MACRO	name
    ifnb <name>
      PUBLIC	_&name
      IF largeCODE
	_&name LABEL FAR
      ELSE
	_&name LABEL NEAR
      ENDIF
      name	EQU	_&name
    endif
	ENDM


; <<<<< offset macros (relative to DGROUP not segment) >>>>>
GOFFSET EQU <OFFSET DGROUP:>
COFFSET EQU <OFFSET CS:>

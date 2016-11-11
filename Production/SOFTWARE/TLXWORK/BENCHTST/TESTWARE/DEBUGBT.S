    LIST
    TITLE   DEBUG TERMINAL I/O ROUTINES
    SUBTTL  (C)1984,85,86,87,88 HAYES INTERNATIONAL CORPORTION
    SECTION DEBUG,$D800
;************************************
;
; TERMINAL I/O ROUTINES FOR 1805
;
; TLX-1 VERSION 1 FOR 2 MEGHZ CLOCK
;
; 8 ,16 AND 32 BIT OPERATIONS
:
; MODIFIED FOR 300 BAUD OPERATION
;	   NEW BASE ADDRESS AND
;          RELOCATED RAM BASE ADDRESS
;
; REGISTER ASSIGNMENTS
;
; RF   CHAR OUTPUT/INPUT
; RE   TIMER
; RD   TEMPORAY STORAGE
; RC   DELAY PROGRAM COUNTER
; RB   WORD COUNTER
; RA   DATA STACK POINTER
; R9   LOW 16 BITS OF 32 BIT
; R6   SUBROUTINE LINK
; R3   PROGRAM COUNTER
; R2   PROGRAM STACK POINTER
;
;*************************************
; REGISTER EQUATES
;*************************************
ORGIN  EQU   $d800
RAM    EQU   $3000
SP     EQU   R2
PC     EQU   R3
LINK   EQU   R6
STORE  EQU   R9
TOP    EQU   RA
NUM    EQU   RB
DELAY  EQU   RC
TEMP   EQU   RD
TIMER  EQU   RE
CHAR   EQU   RF
;*************************************
        ORG ORGIN
;*************************************
;
; SUBROUTINE NAME: OSTRNG
;
; THIS ROUTINE PRINTS A STRING PASSED
; IN-LINE WITH THE SUBROUTINE CALL
;
; EXAMPLE:
;
;      SCAL R6 OSTRNG
;      ,$0A0D
;      ,T'THIS IS A TEST'
;      ,$00
;
;*************************************
OSTRN1: SCAL LINK,RCA1	    ; SET UP DELAY TIMER
OSTRNG: LDA LINK	    ; GET INLINE CHARACTER
	BZ 01$		    ; IF NULL EXIT
	PHI CHAR	    ; LOAD INTO CHARACTER REGISTER
	SCAL LINK,TYPED     ; CALL SCREEN TYPE ROUTINE
	BR OSTRNG	    ; LOOP FOR NEXT CHARACTER
01$:	SRET LINK	    ; RETURN
;*************************************
;
; SUBROUTINE NAME: TYPEHD
;
; THIS ROUTINE PRINTS THE HEX
; BYTE PASSED IN RF.1
;
;*************************************
TYPEH1: SCAL LINK,RCA1	    ; SET UP DELAY TIMER
TYPEHD: SEP DELAY	    ; DELAY
	DB	$17	    ; DELAY CONSTANT
TYPEH:	GHI CHAR
	LBR	AAHEX		; PACTH FOR DEBUGGER RUN I/O FUNCTION
;       SHR			; NOP OUT 1 BYTE
;	SHR			; NOP OUT 1 MORE BYTE
;	SHR			; NOP OUT 1 MORE BYTE FOR A TOTAL OF 3 BYTES
	SHR
        ADI $F6
        LSNF
        ADI $07
        SMI $C6
        PLO TIMER
        LDI $1B
	BR TYPEH0
;*************************************
;
; SUBROUTINE NAME: TYPE
;
; THIS ROUTINE PRINTS THE ASCII CHAR
; PASSED IN RF.1
;
;*************************************
TYPED1:	SCAL LINK,RCA1
TYPED:	SEP DELAY
	DB $17
TYPER:	GHI CHAR
	LBR	AAWRITE		; PATCH FOR DEBUGGER I/O FUNCTION
;        PLO TIMER		; NOP OUT 1 BYTE
;        XRI $0A		; NOP OUT 2 MORE BYTES FOR A TOTAL OF 3
	BNZ 01$
        LDI $8B
        LSKP
01$:	LDI $0B
TYPEH0: PLO CHAR
        GLO TEMP
        STR SP
;******************
        SEQ
        GLO TIMER
        PLO TEMP
        SEP DELAY
	DB $07
        DEC CHAR
        SD
        GLO TEMP
        SHRC
        PLO TEMP
        LSDF
        SEQ
        LSKP
        REQ
	REQ
        GLO CHAR
        ANI $0F
        NOP
	NOP
	NOP
	NOP
        BNZ *-$13
;******************
        GLO CHAR
        ADI $FB
        PLO CHAR
        BNF *+$19
        SMI $1B
        BZ *+$15
        BNF *+$06
        LDI $00
        BR *+$0C
;******************
        GHI CHAR
        ANI $0F
        ADI $F6
        LSNF
        ADI $07
        SMI $C6
        PLO TIMER
        BR *-$33
;*******************
        LDX
        PLO TEMP
        SRET LINK
;*************************************
;
; SUBROUTINE NAME: RCA1
;
; THIS ROUTINE SELECTS SCREEN AND 
; KEYBORAD I/O AND SETS BAUD RATE
; IN RE.1
;
; VALUES FOR RCA DEVELOPMENT SYSTEM
;
;	110 BAUD	7000
;	300 BAUD	2800
;	1200 BAUD	0800
;
; VALUES FOR AMERICAN AUTOMATION SYSTEM RUNNING AT 300 BAUD IS 6600
;
;******************************************************************************
RCA1:	SEX PC
        OUT 1
	DB $01
        SEX SP
        RLDI DELAY,DELAY1
        RLDI TIMER,$2800	; PARAMETER FOR BAUD RATE
        SEP DELAY
	DB $17
        SRET LINK
;*************************************
;
; SUBROUTINE NAME: DELAY1
;
; THIS ROUTINE PROVIDES A DELAY
; AND SHOULD BE USED BETWEEN INPUT
; AND OUTPUT TO INSURE THAT THE
; LAST OPERATION IS FINISHED.
; THE DELAY CONSTANT IS PASSED INLINE
;
;*************************************
DELAY0:	SEP DELAY
        SEP DELAY
        SEP DELAY
        SEP DELAY
        SEP PC
DELAY1:	GHI TIMER
        SHR
        PLO TIMER
DELAY2:	DEC TIMER
        LDA PC
DELAY3:	SMI $01
        BNZ DELAY3
        GLO TIMER
        BZ DELAY0
        DEC PC
        BR DELAY2
;*************************************
;
; SUBROUTINE NAME: READHX
;
; THIS ROUTINE IS USED TO INPUT A
; 8 OR 16 BIT HEX WORD INTO RD.
; RD.0 IS THE LSB AND RD.1 IS THE
; MSB. IF A NONE HEX CHAR IS ENTERED
; AND ERROR MESSAGE IS GIVEN AND
; INPUT MAY BE GIVEN AGAIN.
; INPUT IS TERMINATED BY A RETURN.
;
;*************************************
READX1:	SCAL LINK,RCA1
READHX:	SCAL LINK,READHD
        BDF READHX
        GHI CHAR
        XRI $0D
        BNZ *+$04
        SRET LINK
        SCAL LINK,OSTRNG
        DB $0A,$0D
	DB 'ERR'
        DB $0A,$0D
        DB $2A
        DB $00
        BR READHX
;*************************************
;
; SUBROUTINE NAME: READH
;
; THIS ROUTINE IS USED TO ENTER A
; HEX CHARACTER INTO RD.0
;
;*************************************
READH1:	SCAL LINK,RCA1
READHD:	SEP DELAY
	DB $17
READH:	SCAL LINK,READ
;*******************
CKHXE:	GHI CHAR
        SMI $41
        BNF *+$25
        SMI $06
        BDF *+$29
        ADI $10
;*******************
        PLO TIMER
        GHI TEMP
        SHL
	SHL
	SHL
	SHL
        STR SP
        GLO TEMP
        SHR
	SHR
	SHR
	SHR
        OR
        PHI TEMP
        GLO TEMP
        SHL
	SHL
	SHL
	SHL
        STR SP
        GLO TIMER
        ANI $0F
        OR
        PLO TEMP
        SMI $00
        SRET LINK
;*******************
        ADI $07
        BDF *+$06
	ADI $0A
        BDF *-$23
        ADI $00
        SRET LINK
;*************************************
        ORG ORGIN+$0100
;*************************************
;
; SUBROUTINE NAME: READ
;
; THIS ROUTINE ALLOWS THE INPUT OF
; ANY ASCII CHARACTER INTO RF.1
;
;*************************************
READ1:	SCAL LINK,RCA1
READD:	SEP DELAY
	DB $17
READ:	LBR AAREAD	; AMERICAN AUTOMATION READ
;	LDI $80		; NOP OUT 2 BYTES
;       PHI CHAR	; NOP OUT ONE MORE BYTE FOR A TOTAL OF 3
        BN4 *
        B4 *
	SEP DELAY
	DB $02
;*******************
	SEX SP
	SEX SP
        GHI TIMER
        SHR
        BDF *+$09
        B4 *+$05
        SEQ
        BR *+$06
        REQ
        LSKP
        NOP
	NOP
        SEP DELAY
	DB $07
        NOP
	NOP
        GHI CHAR
        SHR
        PHI CHAR
        BDF *+$09
        ORI $80
        BN4 *-$1A
        PHI CHAR
        BR *-$1B
        REQ
        BZ READ
        SRET LINK
;*************************************
;
; SUBROUTINE NAME: TYPE3
;
; THIS ROUTINE WILL PRINT THE 32 BIT
; HEX WORD, WITH MSB IN RD.1,RD.0
; AND THE LSB IN R9.1,R9.0
; THE OPTION FOR TERMNATION IS
; PASSED IN-LINE
;
;*************************************
TYPE31: SCAL LINK,RCA1
TYPE3D: SEP DELAY
	DB $17
TYPE3:	SCAL LINK,TYPE6
        DB $00
        GHI R9
        PHI TEMP
        GLO R9
        PLO TEMP
	SCAL LINK,TYPE6
        DB $00
        LBR TEST
;*************************************
;
; SUBROUTINE NAME: TYPE6
;
; THIS ROUTINE WILL PRINT THE 16 BIT
; HEX WORD, WITH MSB IN RD.1 AND
; THE LSB IN RD.0
; THE OPTION FOR TERMNATION IS
; PASSED IN-LINE
;
;*************************************
TYPE61:	SCAL LINK,RCA1
TYPE6D:	SEP DELAY
	DB $17
TYPE6:	GHI TEMP
        PHI CHAR
        SCAL LINK,TYPEH
        GLO TEMP
        PHI CHAR
        SCAL LINK,TYPEH
        LBR TEST
;*************************************
;
; SUBROUTINE NAME: TYPE8
;
; THIS ROUTINE WILL PRINT THE 8 BIT
; HEX WORD IN RD.1
; THE OPTION FOR TERMNATION IS
; PASSED IN-LINE
;
;*************************************
TYPE81:	SCAL LINK,RCA1
TYPE8D:	SEP DELAY
	DB $17
TYPE8:	GHI TEMP
        PHI CHAR
        SCAL LINK,TYPEH
        LBR TEST
;*************************************
WORD31:	SCAL LINK,RCA1
WORD3D:	SEP DELAY
	DB $17
WORD3:	GLO TEMP
        STXD
        SCAL LINK,WORD
        DB $00
        IRX
        LDX
        PHI TEMP
        SCAL LINK,WORD
        DB $00
        GHI R9
        PHI TEMP
        SCAL LINK,WORD
        DB $00
        GLO R9
        PHI TEMP
        SCAL LINK,WORD
        DB $00
        LBR TEST
;*************************************
WORD61:	SCAL LINK,RCA1
WORD6D:	SEP DELAY
	DB $17
WORD6:	GLO TEMP
        STXD
        SCAL LINK,WORD
        DB $00
        IRX
        LDX
        PHI TEMP
        SCAL LINK,WORD
        DB $00
        LBR TEST
;*************************************
WORD81:	SCAL LINK,RCA1
WORD8D:	SEP DELAY
	DB $17
WORD:	LDI $08
        PLO TEMP
WORDLOOP:        GHI TEMP
        SHL
        PHI TEMP
        LDI $00
        ADCI $30
        PHI CHAR
        SCAL LINK,TYPER
        DEC TEMP
        GLO TEMP
        BNZ WORDLOOP
        LBR TEST
;*************************************
TOUT31:	SCAL LINK,RCA1
TOUT3D:	SEP DELAY
	DB $17
TOUT3:	RSXD RA
        RSXD RB
;*******************
        SEX TEMP
        RNX RA
        SEX SP
        SCAL LINK,MOUT3
        DB $02
        DB $01
        DEC RA
        DEC RA
        DEC RA
        DEC RA
	DBNZ RB,*-$0E
        IRX
        RLXA RB
        RLXA RA
        DEC SP
        LDA LINK
        LBR MLFCRT
;*******************
MOUT31: SCAL LINK,RCA1
MOUT3D: SEP DELAY
	DB $17
MOUT3:	LDN TEMP
        STXD
        DEC TEMP
        LDN TEMP
        STXD
        DEC TEMP
        LDN TEMP
        STXD
        DEC TEMP
        LDN TEMP
        STXD
        GHI TEMP
        PHI CHAR
	SCAL LINK,TYPEH
        GLO TEMP
        PHI CHAR
	SCAL LINK,TYPEH
	SCAL LINK,OSTRNG
	DB $20,$20
	DB $20,$20
	DB $00
        IRX
        LDXA
        PHI TEMP
        LDXA
        PLO TEMP
        LDXA
        PHI R9
        LDX
        PLO R9
        DEC SP
        DEC SP
        DEC SP
        DEC SP
	SCAL LINK,WORD3
	DB $03
	DB $04
        IRX
        LDXA
        PHI TEMP
        LDXA
        PLO TEMP
        LDXA
        PHI R9
        LDX
        PLO R9
        GHI TEMP
        PHI CHAR
	SCAL LINK,TYPEH
        GLO TEMP
        PHI CHAR
	SCAL LINK,TYPEH
        GHI R9
        PHI CHAR
	SCAL LINK,TYPEH
        GLO R9
        PHI CHAR
	SCAL LINK,TYPEH
        LBR TEST
;*************************************
TOUT61: SCAL LINK,RCA1
TOUT6D: SEP DELAY
	DB $17
TOUT6:	GHI RA
        STXD
        GLO RA
        STXD
        GHI RB
        STXD
        GLO RB
        STXD
;********************
        SEX TEMP
        RNX RA
        SEX SP
	SCAL LINK,MOUT
	DB $02
	DB $01
        DEC RA
        DEC RA
	DBNZ RB,*-$0C
        IRX
        LDXA
        PLO RB
        LDXA
        PHI RB
        LDXA
        PLO RA
        LDX
        PHI RA
        LDA LINK
        LBR MLFCRT
;*************************************
MOUT1:	SCAL LINK,RCA1
MOUTD:	SEP DELAY
	DB $17
MOUT:	LDN TEMP
        STXD
        DEC TEMP
        LDN TEMP
        STXD
        GHI TEMP
        PHI CHAR
	SCAL LINK,TYPEH
        GLO TEMP
        PHI CHAR
	SCAL LINK,TYPEH
        IRX
        LDXA
        PHI TEMP
        LDX
        PLO TEMP
        DEC SP
        DEC SP
	SCAL LINK,OSTRNG
	DB $20,$20
	DB $20,$20
	DB $00
	SCAL LINK,WORD6
	DB $03
	DB $04
        IRX
        LDXA
        PHI CHAR
        LDX
        PLO TEMP
	SCAL LINK,TYPEH
        GLO TEMP
        PHI CHAR
	SCAL LINK,TYPEH
        LBR TEST
;*************************************
T8OUT1: SCAL LINK,RCA1
T8OUTD: SEP DELAY
	DB $17
T8OUT:	RSXD RA
        RSXD RB
;********************
        SEX TEMP
        RNX RA
        SEX SP
	SCAL LINK,M8OUT
	DB $02
	DB $01
        DEC RA
	DBNZ RB,*-$0B
        IRX
        RLXA RB
        RLXA RA
        DEC SP
        LBR MLFCRT
;*******************
M8OUT1: SCAL LINK,RCA1
M8OUTD: SEP DELAY
	DB $17
M8OUT:	LDN TEMP
        STXD
        GHI TEMP
        PHI CHAR
	SCAL LINK,TYPEH
        GLO TEMP
        PHI CHAR
	SCAL LINK,TYPEH
	SCAL LINK,OSTRNG
	DB $20,$20
	DB $20,$20
	DB $00
        IRX
        LDX
        PHI TEMP
        DEC SP
	SCAL LINK,WORD
	DB $03
	DB $04
        IRX
        LDX
        PHI CHAR
	SCAL LINK,TYPEH
        LBR TEST
;*************************************
TEST:	LDA LINK
        BZ WEXIT
	SMI $01
        BNZ CRLFT
CR:	SCAL LINK,OSTRNG
	DB $0D
	DB $00
        BR WEXIT
CRLFT:	SMI $01
        BNZ SPACET
CRLF:	LDA LINK
        PLO TEMP
	SCAL LINK,OSTRNG
	DB $0A,$0D
	DB $00
        DEC TEMP
        GLO TEMP
        BNZ *-$09
        BR WEXIT
SPACET: SMI $01
        BNZ MLFCRT
SPACE:	LDA LINK
        PLO TEMP
	SCAL LINK,OSTRNG
	DB $20
	DB $00
        DEC TEMP
        GLO TEMP
        BNZ *-$08
        BR WEXIT
MLFCRT: SMI $01
        BNZ BAR
MLFCR:	LDA LINK
        PLO TEMP
	SCAL LINK,OSTRNG
	DB $0D
	DB $1B,$41
	DB $00
        DEC TEMP
        GLO TEMP
        BNZ *-$0A
        BR WEXIT
BAR:	LDI $28
        PLO TEMP
	SCAL LINK,OSTRNG
	DB $2D
	DB $00
        DEC TEMP
        GLO TEMP
        BNZ *-$08
	SCAL LINK,OSTRNG
	DB $0A,$0D
	DB $00
;*******************
WEXIT:	SRET LINK
;*************************************
;
; SPECIAL TERMINAL FUNCTIONS 1
;
;*************************************
;
; HOME CURSOR
;
;*************************************
HOME:  SCAL LINK,OSTRN1
	DB $1B,$48
	DB $00
        SRET LINK
;*************************************
;
; CLEAR SCREEN
;
;*************************************
CLEAR:	SCAL LINK,OSTRN1
	DB $1B,$6A
	DB $00
        SRET LINK
;*************************************
;
; HIGH RESOLUTION
;
;*************************************
REHIGH: SCAL LINK,OSTRN1
	DB $1B,$1B
	DB $52,$31
	DB $00
        SRET LINK
;*************************************
;
; LOW RESOLUTION
;
;*************************************
RELOW:SCAL LINK,OSTRN1
	DB $1B,$1B
	DB $52,$30
	DB $00
        SRET LINK
;*************************************
;
; REVERSE VIDEO ON
;
;*************************************
RVON:	SCAL LINK,OSTRN1
	DB $1B,$1B
	DB $53,$31
	DB $00
        SRET LINK
;*************************************
;
; REVERSE VIDEO OFF
;
;*************************************
RVOFF:	SCAL LINK,OSTRN1
	DB $1B,$1B
	DB $53,$30
	DB $00
        SRET LINK
;*************************************
;
; RUN PROGRAM ROUTINE
;
;*************************************
	DIS
	DB $00
	RLDI R2,RAM+$0FFF
        SEX R2
	RLDI R3,RUN
        SEP R3
;*************************************
RUN:	 SCAL R6,OSTRN1
	DB $0A,$0D
	DB 'ENTER RUN ADDRESS'
	DB $20,$20
	DB $00
	SCAL R6,READHX
        SEX R0
        RNX RD
        SEP R0
;*************************************
;
; DISPLAY ON
;
;*************************************
DISON:	SCAL R6,OSTRN1
	DB $1B,$1B
	DB $43,$30
	DB $00
        SRET R6
;*************************************
;
; DISPLAY OFF
;
;*************************************
DISOFF: SCAL R6,OSTRN1
	DB $1B,$1B
	DB $43,$31
	DB $00
        SRET R6
;*************************************
;
; KYEBOARD ON
;
;*************************************
KYEON:	SCAL R6,OSTRN1
	DB $1B,$1B
	DB $4B,$31
	DB $00
        SRET R6
;*************************************
;
; KYEBOARD OFF
;
;*************************************
KYEOFF: SCAL R6,OSTRN1
	DB $1B,$1B
	DB $4B,$30
	DB $00
        SRET R6
;*************************************
;
; ENGLISH CHARACTER SET
;
;*************************************
ENG:	SCAL R6,OSTRN1
	DB $1B,$1B
	DB $45
	DB $00
        SRET R6
;*************************************
;
; DEFINE COMMAND DELIMITER (INLINE)
;
;*************************************
DCD:	SCAL R6,OSTRN1
	DB $1B,$1B
	DB $58
	DB $00
        LDA R6
        PHI RF
	SCAL R6,TYPED
        SRET R6
;*************************************
;
; CURSOR ON
;
;*************************************
CURON:	SCAL R6,OSTRN1
	DB $1B,$1B
	DB $43,$31
	DB $00
        SRET R6
;*************************************
;
; CURSOR OFF
;
;*************************************
CUROFF: SCAL R6,OSTRN1
	DB $1B,$1B
	DB $43,$30
	DB $00
        SRET R6
;*************************************
;
; CURSOR NO BLINK
;
;*************************************
CURNB:	SCAL R6,OSTRN1
	DB $1B,$1B
	DB $43,$32
	DB $00
        SRET R6
;*************************************
;
; ADDRESS CURSOR PASSED IN RD
;
;*************************************
ADCUR:	SCAL R6,OSTRN1
	DB $1B,$59
	DB $00
        GHI RD
        PHI RF
	SCAL R6,TYPER
        GLO RD
        PHI RF
	SCAL R6,TYPER
        SRET R6
;*************************************
;
; FORE SPACE (NUMBER INLINE)
;
;*************************************
FORE:	LDA R6
        PLO RD
	SCAL R6,OSTRN1
	DB $1B,$43
	DB $00
        DEC RD
        GLO RD
        BNZ *-$09
        SRET R6
;*************************************
;
; BACK SPACE (NUMBER INLINE)
;
;*************************************
BACK:	LDA R6
        PLO RD
	SCAL R6,OSTRN1
	DB $1B,$44
	DB $00
        DEC RD
        GLO RD
        BNZ *-$09
        SRET R6
;*************************************
        ORG ORGIN+$0500
;*************************************
;
; TEST OF TERMINAL I/O ROUTINES
;
;*************************************
DEBUGT:	DIS
	DB $00

	RLDI R2,RAM+$0FFF
        SEX R2

	RLDI R3,ITEST
        SEP R3
;*************************************
ITEST:	SCAL R6,OSTRN1
	DB $0A,$0D
	DB 'TIME '
	DB $00

;	SCAL R6,TIMALC
	NOP
	NOP
	NOP
	NOP

        GHI RE
        PHI RF
	SCAL R6,TYPEHD

	SCAL R6,OSTRNG
	DB $0A,$0D
	DB $2A
	DB $00

	SCAL R6,READHX

	SCAL R6,OSTRNG
	DB $0A,$0D
	DB $23
	DB $00

        GLO RD
        PHI RF
	SCAL R6,TYPEHD

        SCAL R6,CRLF
	DB $02

        RLDI RA,$0EFF
        RLDI RB,$0010

	SCAL R6,TOUT3
	DB $02

	SCAL R6,TOUT6
	DB $02

	SCAL R6,T8OUT
	DB $02

        RLDI RD,$0F0F
        RLDI R9,$0F0F
	SCAL R6,TYPE3
	DB $02,$02

        RLDI RD,$0F0F
        SCAL R6,TYPE6
        DB $02,$02

        RLDI RD,$0F00
	SCAL R6,TYPE8
	DB $02,$02

        LBR RUN

;************************************
        ORG ORGIN+$0600
;*************************************
;
; SUBROUTINE NAME: TIMALC
;
; THIS ROUTINE DETECTS A KEYSTROKE
; AND COMPUTES THE BAUD RATE OF
; THE USER TERMINAL. IF THE CHAR
; IS $0D (A CR) ECHO IS ENABLED. IF
; IT IS A $0A (A LF) ECHO IS DISABLED
;
;************************************
TIMLC1: SCAL R6,RCA1
TIMALC: LDI $00
        PLO RE
        PLO RF
        B4 *
        BN4 *
	LDI $03
	SMI $01
        BNZ *-$02
        GLO RF
        BNZ *+$05
        B4 *+$05
        INC RF
        B4 *+$07
        INC RE
	LDI $07
        BR *-$0F
        DEC RE
        DEC RE
        GLO RE
        ORI $01
        PHI RE
	SEP RC
	DB $0C
        BN4 *+$06
        GHI RE
	ANI $FE
        PHI RE
	SEP RC
	DB $26
        SRET R6
;*************************************
;
;	A/D-D/A TEST ROUTINES
;
;*************************************
ADTST:	RLDI	R2,RAM+$0FFF
	SEX	R2
	RLDI	R3,ADTST0
	SEP	R3
;*************************************
ADTST0:	SCAL	R6,CLEAR
	SCAL	R6,OSTRN1
	DB	$0A,$0D
	DB	'A/D-D/A TEST'
	DB	$0A,$0D,$00
ADTST1:	SEX	R3
	OUT	1
	DB	$30
	OUT	6
	DB	01
	OUT	5
	DB	00
	SEX	R2
	RLDI	R2,RAM+$0800
	RLDI	R4,$0008
;
ADTST2:	BN1	*
	INP	4
	DEC	R2
	DBNZ	R4,ADTST2
;
	RLDI	R2,RAM+$07FF
	OUT	4
	OUT	3
;
	RLDI	R2,RAM+$0FFF
	RLDI	RA,RAM+$0800
	RLDI	RB,$0008
	SCAL	R6,T8OUT1
	DB	$02
;
	LBR	ADTST1
;************************************
;
;	PIO TEST
;
;************************************
POTST:	RLDI	R2,RAM+$0FFF
	SEX	R2
	RLDI	R3,POTST0
	SEP	R3
;************************************
POTST0:	SCAL	R6,CLEAR
	SCAL	R6,OSTRN1
	DB	$0A,$0D
	DB	'PIO TEST'
	DB	$0A,$0D,$00
;
POTST1:	SEX	R3
	OUT	1
	DB	$08
	OUT	2
	DB	$0F
	OUT	2
	DB	$F7
	OUT	2
	DB	$F0
	SEX	R2
	RLDI	R2,RAM+$0800
	INP	4
	DEC	R2
	INP	6
	ANI	$0F
	STR	R2
	SHL
	SHL
	SHL
	SHL
	OUT	6
;
	RLDI	R2,RAM+$0FFF
	RLDI	RA,RAM+$0800
	RLDI	RB,$0002
	SCAL	R6,T8OUT1
	DB	$02
	LBR	EFTST
;**************************************
	ORG	ORGIN+$0700
;**************************************
;
;	TEST STATUS FLAGS
;
;**************************************
EFTST:	B3	EF3
	SCAL	R6,OSTRN1
	DB	$0A,$0D
	DB	'NOT EF3'
	DB	$0A,$0A,$0D,$00
	BR	POTST2
EF3:	SCAL	R6,OSTRN1
	DB	$0A,$0D
	DB	'EF3'
	DB	$0A,$0A,$0D,$00
;****************************************
POTST2:	B2	EF2
	SCAL	R6,OSTRN1
	DB	$0A,$0D
	DB	'NOT EF2'
	DB	$0A,$0A,$0D,$00
	BR	POTST3
EF2:	SCAL	R6,OSTRN1
	DB	$0A,$0D
	DB	'EF2'
	DB	$0A,$0A,$0D,$00
;******************************************
POTST3:	LBR	POTST1
;******************************************************************************
;
;	AMERICAN AUTOMATION RUN I/O ROUTINES
;
;******************************************************************************
;
;	AMERICAN AUTOMATION INPUT ROUTINE
;
;******************************************************************************
AAREAD:	NOP
	RLDI	R5,IODATA
	LDI	$01
	STR	R5
	SCAL	R6,IOCALL
	RLDI	R5,FETCH
	LDN	R5
	PHI	RF
	SRET	R6
;******************************************************************************
;
;	AMERICAN AUTOMATION OUTPUT ROUTINE
;
;******************************************************************************
AAWRITE:NOP
	RLDI	R5,IODATA
	LDI	$00
	STR	R5
	RLDI	R5,FETCH
	GHI	RF		; check character
				; check for ascii
	STR	R5
	SCAL	R6,IOCALL
	SRET	R6
;******************************************************************************
;
;	AMERICAN AUTOMATION HEX BYTE OUTPUT ROUTINE
;
;******************************************************************************
AAHEX:	GHI	RF
	PLO	RF
	SHR
	SHR
	SHR
	SHR
	ADI	$F6
	LSNF
	ADI	$07
	SMI	$C6
	PHI	RF
	SCAL	R6,AAWRITE
	GLO	RF
	ANI	$0F
	ADI	$F6
	LSNF
	ADI	$07
	SMI	$C6
	PHI	RF
	SCAL	R6,AAWRITE
	SRET	R6
;******************************************************************************
;
;	AMERICAN AUTOMATION I/O ROUTINE
;
;******************************************************************************
IOCALL:	NOP
	SRET	R6
;******************************************************************************
;
;	AMERICAN AUTOMATION I/O BUFFER
;
;******************************************************************************
IODATA:	DB	$00
FETCH:	DB	$00
;******************************************************************************
	END

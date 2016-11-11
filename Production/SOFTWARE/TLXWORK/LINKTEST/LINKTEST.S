	LIST
	TITLE	1805 CPU / A/D-D/A TEST
	SUBTTL	(C)1984,85,86,87,88 HAYES INTERNATIONAL CORPORATION
	SECTION	LINKTEST,$0000
;*************************************
;
; TLX-1 COMMAND LINK SOFTWARE TEST
;
;
; DATA VALID : EF2
; AND GATE   : EF3
;
; (C) 1987 HAYES INTERNATIONAL CORP.
;
; BY DENTON MARLOWE
;
;*************************************
ORGIN	EQU	$0000
RAM	EQU	$4000
DEBUG	EQU	$D000	; changed form 1000
;*************************************
READHX	EQU	DEBUG+$0898
READD	EQU	DEBUG+$0904
OSTRN1	EQU	DEBUG+$0800
TYPE8	EQU	DEBUG+$0965
T8OUT1	EQU	DEBUG+$0AB5
WORD	EQU	DEBUG+$09AC
;*************************************
	ORG	ORGIN
;*************************************
        DIS
	DB	$00
        RLDI 	R2,RAM+$0FFF
        SEX 	R2
        RLDI 	R3,HELLO
        SEP 	R3
;*************************************
COMM	EQU	RAM+$0EFF
;*************************************
; SET-UP FOR PIO PORTS
;
; PORT A FOR DATA INPUT
; PORT B LOW NIBBLE ADDRESS INPUT
; PORT B HIGH NIBBLE OUTPUT ECHO
;*************************************
HELLO:	RLDI 	R2,COMM
        SEX 	R3
        OUT 	1
	DB	$08 ; I/O GROUP 8
        OUT 	2
	DB	$0F ; A IS INPUT
        OUT 	2
	DB	$F7 ; B IS BIT PROG
        OUT 	2
	DB	$F0 ; LOW IN,HIGH OUT
        SEX 	R2
        SCAL 	R6,OSTRN1
        DB	$0A,$0D
        DB	'TLX TM TEST'
        DB	$0A,$0D
        DB	'DATA VALID : EF2'
        DB	$0A,$0D
        DB	'AND GATE  :EF3'
        DB	$0A,$0D
        DB	'CHANNEL 8 DRIVES D/A CH1 & CH 2'
        DB	$0A,$0D
        DB	'CHANNEL 9 IS ECHOED BACK'
        DB	$0A,$0D
        DB	'CHANNEL A FOR COUNTER TEST'
        DB	$0A,$0D
        DB	'CHANNEL B FOR SERVO TEST'
        DB	$0A,$0D
        DB	'CHANNEL C FOR A/D-D/A TEST'
        DB	$0A,$0A,$0D
        DB	'CHANNEL $?'
        DB	$00
        RLDI 	RD,$0000
        SCAL 	R6,READHX
        GLO 	RD
        LBNZ 	CH
        LDI 	$01
        PLO 	R1
        RLDI 	R0,$FFFF
        SCAL 	R6,OSTRN1
        DB	$0A,$0D
        DB	'SCAN MODE'
        DB	$00
        LBR 	GATED
;************************************
CH:	PLO 	R1
        SMI 	$0A
        LBZ 	COUNT
        GLO 	R1
        SMI 	$0B
        LBZ 	SERVO
        GLO 	R1
        SMI 	$0C
        LBZ 	ADTST
        RLDI 	R0,$0000
;*************************************
;
; AND GATE TEST OPTION
;
;*************************************
GATED:  SCAL 	R6,OSTRN1
        DB	$0A,$0A,$0D
        DB	'AND GATE TEST (HALT) '
        DB	'(Y/N) '
        DB	$00
        SCAL 	R6,READD
        GHI 	RF
        SMI 	$59
        BNZ 	NOAND
        RLDI	R8,$FFFF
        LBR 	RCOM
NOAND:	RLDI 	R8,$0000
        LBR 	RCOM
;*************************************
        ORG	ORGIN+$0200
;*************************************
; COMMAND LINK
;*************************************
RCOM:	RLDI 	R2,COMM
        SEX 	R3
        OUT 	1
	DB	$08 	;I/O GROUP 8
        SEX 	R2
;*************************************
; INPUT PIO DATA
;*************************************
PIO:	GHI 	R8
        BZ 	PIO2
PIO1:	B3 	PIO1
;*************************************
PIO2:	B2 	PIO2
        SEX 	R3
        OUT 	1
	DB	$08
        SEX 	R2
WAIT:	B2 	WAIT
;*************************************
        INP 	6    ; ADDRESS
        IRX
        INP 	4    ; DATA
        DEC 	R2
        LDX
        ANI 	$0F
        STR 	R2
        PHI 	R7
;*************************************
        GLO 	R1
        SM
        BNZ 	WAIT
;*************************************
        IRX
        LDX
        PLO 	R7
        DEC 	R2
;*************************************
; CHECK FOR CHANNEL 9
;*************************************
        GLO 	R1
        SMI 	$09
        BNZ 	NEXT
        GLO 	R7
        SHL
        SHL
        SHL
        SHL
        STR 	R2
        OUT 	6
        DEC 	R2
;*************************************
; CHECK FOR CHANNEL 8
;*************************************
NEXT:	GLO 	R1
        SMI 	$08
        LBNZ 	NEXT1
        SEX 	R3
        OUT 	1
	DB	$30
        SEX 	R2
        GLO 	R7
        STR 	R2
        OUT 	3
        DEC 	R2
        OUT 	4
        DEC 	R2
;*************************************
NEXT1:	GLO 	R0
        BZ 	OUT
        INC 	R1
        GLO 	R1
        SMI	$0A
        BNZ 	OUT
        RLDI 	R1,$0001
;*************************************
OUT:	NOP
;*************************************
        SCAL 	R6,OSTRN1
        DB	$0A,$0D
        DB	'CH '
        DB	$00
        GHI 	R7
	PHI 	RD
        SCAL 	R6,TYPE8
        DB	$03,$02
        GLO 	R7
	PHI 	RD
        SCAL 	R6,WORD
        DB	$03,$02
        GLO 	R7
	PHI 	RD
        SCAL 	R6,TYPE8
        DB	$02,$02
;*************************************
        LBR 	PIO
;*************************************
        ORG	ORGIN+$0300
;*************************************
; COUNTER TEST
;*************************************
COUNT:  SCAL 	R6,OSTRN1
        DB	$0A,$0D
        DB	'COUNTER TEST'
        DB	$0A,$0D
        DB	'ENTER COUNTER VALUE '
        DB	$00
        RLDI 	RD,$0000
        SCAL 	R6,READHX
        STPC
        GLO 	RD
        LDC
        ETQ
        SCM2
COUNT1: BR 	COUNT1
;*************************************
;*************************************
;
; TLX-1 SERVO CALIBRATION
;
; (C) 1987 HAYES INTERNATIONAL CORP.
;
; BY DENTON MARLOWE
;
;*************************************
SERVO:  SCAL 	R6,OSTRN1
        DB	$0A,$0D
        DB	'TLX SERVO CALIBRATION'
        DB	$0A,$0D,$00
;*************************************
SERVO1: SCAL 	R6,OSTRN1
        DB	$0A,$0D
        DB	'HEX WING COMMAND $?'
        DB	$00
        RLDI 	RD,$0000
        SCAL 	R6,READHX
;*************************************
        SEX 	R3
        OUT 	1
	DB	$30
        SEX 	R2
        GLO 	RD
        STR 	R2
        OUT 	3
        DEC 	R2
        OUT 	4
        DEC 	R2
;*************************************
        RLDI 	RF,$07FF
LOOP:   RLDI 	RE,$000F
LOOP1:  NOP
        DBNZ 	RE,LOOP1
        DBNZ 	RF,LOOP
;*************************************
        SEX 	R3
        OUT 	1
	DB	$30
        OUT 	6
	DB	$00
        OUT 	5
	DB	$03
        SEX 	R2
        BN1 	*
        INP 	3
        PLO 	R7
        SCAL 	R6,OSTRN1
        DB	$0A,$0D
        DB	'FEEDBACK '
        DB	$00
        GLO 	R7
	PHI 	RD
        SCAL 	R6,TYPE8
        DB	$02,$02
        LBR 	SERVO1
;*************************************
;
; A/D-D/A TEST ROUTINE
;
;*************************************
ADTST:  SCAL 	R6,OSTRN1
        DB	$0A,$0D
        DB	'A/D-D/A TEST'
        DB	$0A,$0D,$00
ADTST1: SEX 	R3
        OUT 	1
	DB	$30
        OUT 	6
	DB	$01
        OUT 	5
	DB	$00
        SEX 	R2
        RLDI 	R2,RAM+$0800
        RLDI 	R4,$0008
ADTST2: BN1 	*
        INP 	4
        DEC 	R2
        DBNZ 	R4,ADTST2
;
        RLDI 	R2,RAM+$07FD
        OUT 	4
        OUT 	3
;
        RLDI 	R2,RAM+$0FFF
        RLDI 	RA,RAM+$0800
        RLDI 	RB,$0008
        SCAL 	R6,T8OUT1
        DB	$02
;
        LBR 	ADTST1
;*************************************
	END

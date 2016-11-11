	LIST
	TITLE   LOW LEVEL OPERATING SYSTEM
	SUBTTL  (C)1984,85,86,87,88,89,90,91 HAYES INTERNATION CORPORATION
	SECTION TLX,$0000
;****************************************************************************
;
;       LOW LEVEL OPERATING SYSTEM
;
;       FIRST PRODUCTION VERSION 1.0/07 JUL84
;
;       WRITTEN BY DENTON MARLOWE
;
;       COPYRIGHT HAYES INTERNATIONAL CORPORATION
;
;****************************************************************************
;	EQUATES FROM TLXPROD.S
;****************************************************************************
	INCLUDE TLXEQU.S
;****************************************************************************
;       VERSION DEFINTIONS
;****************************************************************************
VER_0   EQU     0               ; PROTOTYPE CONFIGURATION
VER_1   EQU     1               ; STANDARD HAYES CONFIGURATION (SERIAL NO. 1-19)
VER_2   EQU     2               ; STANDARD HAYES CONFIGURATION (SERIAL NO. 20-?)
VER_3   EQU     3               ; SPECIAL FLIGHT INTERNATION VERSION (SERIAL NO. 12)
VER_4   EQU     4               ; STANDARD FLIGHT INTERNATION VERSION (BEFORE FEB 7,1992)
VER_5   EQU     5               ; STANDARD HAYES CONFIGURATION TO MATCH NEW ALTIMETER

;VERSION EQU    VER_0           ; SET VERSION
;VERSION EQU    VER_1           ; SET VERSION
VERSION EQU    VER_2		   ; SET VERSION
;VERSION EQU    VER_3           ; SET VERSION
;VERSION EQU	VER_4			; SET VERSION
;VERSION EQU	VER_5			; SET VERSION

	IF      (VERSION EQ VER_0)
	MSG     'PROTOTYPE CONFIGURATION - VERSION 0 - !!!WARNING DO NOT USE!!!'
	ENDIF

	IF      (VERSION EQ VER_1)
	MSG     'STANDARD HAYES CONFIGURATION - VERSION 1'
	ENDIF

	IF      (VERSION EQ VER_2)
	MSG     'STANDARD HAYES CONFIGURATION - VERSION 2'
	ENDIF

	IF      (VERSION EQ VER_3)
	MSG     'SPECIAL FLIGHT INTERNATIONAL CONFIGURATION - VERSION 3'
	ENDIF

	IF      (VERSION EQ VER_4)
	MSG     'STANDARD HAYES CONFIGURATION - VERSION 4'
	ENDIF

	IF      (VERSION EQ VER_5)
	MSG     'STANDARD HAYES CONFIGURATION - VERSION 5'
	ENDIF

	IF      (VERSION GT VER_2)
	MSG     'VERSION 3 PATCHES ADDED TO CODE'
	ENDIF

;****************************************************************************
;
;
;       TLX ACCEL CONTOLLER SOFTWARE
;
;
;****************************************************************************
	ORG 	$0C00		   ; EPROM NUMBER ONE
;****************************************************************************
;
;       WING LIMITS
;
;       HARDWARE WING POSITION
;       BASED ON FOLLOWING CALIBRATION
;
;       ANGLE  ANGLE     HEX  DEC  VOLTS
;
;       +1      +10      FF   255   2.5
;        0       +9      E5   229   2.25
;       -1       +8      CC   204   2.0
;       -1.5     +7.5    BF   191   1.875
;       -2       +7      B2   178   1.75
;       -3       +6      99   153   1.5
;       -4       +5      7F   127   1.25
;       -4.5     +4.5    73   115   1.125
;       -5       +4      66   102   1.0
;       -6       +3      4C    76   0.75
;       -7       +2      33    51   0.50
;       -8       +1      19    25   0.25
;       -8.5     +0.5    0C    12   0.125
;       -9        0      00     0   0.0
;
;       SOFTWARE WING POSITION
;       BASED ON THE FOLLOWING CONVENTION
;
;       ANGLE    HEX
;
;       +8       00CC + 00E5 = 01B1
;        0       0000 + 00E5 = 00E5
;       -8       FF34 + 00E5 = 0019
;
;****************************************************************************
;       WING POSITON EQUATES
;****************************************************************************
WNGPOS  EQU     $E5             ; MAX POSITIVE (0 DEG)
WNGZER  EQU     $E5             ; ZERO ANGLE (0 DEG)
WNGREC  EQU     $BF             ; RECOVERY (-1.5 DEG)


	IF		((VERSION EQ VER_0) OR (VERSION EQ VER_1) OR (VERSION EQ VER_2) OR (VERSION EQ VER_4) OR (VERSION EQ VER_5))

WNGNEG	EQU 	$19 			; MAX NEGATIVE (-8.0 DEG)

	MSG     'MAXIMUM NEGATIVE WING ANGLE IS -8 DEGREES'
	ENDIF

	IF		(VERSION EQ VER_3)

WNGNEG	EQU 	$0C 			; MAX NEGATIVE (-8.5 DEG)

	MSG     'MAXIMUM NEGATIVE WING ANGLE IS -8.5 DEGREES'
	ENDIF

	IF		((VERSION EQ VER_1) OR (VERSION EQ VER_2) OR (VERSION EQ VER_4) OR (VERSION EQ VER_5))

WNGRET	EQU 	$7F 			; NORMAL MANUAL RESET (4.0 DEG - 9DEG = -5.0 DEG)

	MSG     'MANUAL RESET OF WINGS SET AT -4.0 DEGREES'
	ENDIF

	IF		(VERSION EQ VER_3)

WNGRET	EQU 	$7F-$0C 		; NORMAL MANUAL RESET (4.5 DEG - 9DEG = -4.5 DEG)

	MSG     'MANUAL RESET OF WINGS SET AT -4.5 DEGREES'
	ENDIF

	IF		(VERSION GT VER_2)

WNGWIN  EQU     $0C             ; WING WINDOW +/- 0.5 DEG
WNGRL   EQU     WNGRET-WNGWIN   ; WNGRET - 0.5 DEG
WNGRH	EQU 	WNGRET+WNGWIN	; WNGRET + 0.5 DEG

	ENDIF
;
WNGMAX  EQU     $00CC           ; ABS POS LIMIT (+8)
WNGXRI  EQU     $FF34           ; ABS NEG LIMIT (-8)
;****************************************************************************
;
;       ACCELEROMETER SCALING
;
;       G'S    HEX    DEC    VOLTS
;
;       +1.5   FF     255     2.5
;       +1.25  BF     191     1.875
;       +1     7F     127     1.25
;       +0.75  3F      63     0.625
;       +0.5   00       0     0.0
;
;****************************************************************************
;
;       TLX ACCEL CONTROLLER
;
;****************************************************************************
TLXACC: NOP
;****************************************************************************
;       ZERO ACCEL STACK
;****************************************************************************
ACCZER: RLDI    RD,CTOP         ; TOP OF ACCEL STACK
		RLDI	RC,CBYTE		; NUMBER BYTES
		SEX 	RD				; SET STACK
		LDI 	$00 			; ZERO G'S
;
ZEROC1: STXD                    ; STORE BYTE
;
		DBNZ	RC,ZEROC1		; ZERO ALL BYTES
;****************************************************************************
;       SET COUNTER FOR FOUR CYCLES
;****************************************************************************
		RLDI	RD,COUNT
		LDI 	$04
		STR 	RD
;****************************************************************************
;       INITIALIZE PRESET FLAG
;****************************************************************************
	IF		((VERSION GT VER_2) AND (VERSION LT VER_5))

		RLDI	RD,PFLAG
		LDI 	$00
		STR 	RD

	MSG 'GAIN SWITCH CODE'
	ENDIF
;****************************************************************************
;
;       DELAY LOOP SECTION
;
;       DELAY LOOP FOR PROGRAM EXECUTION
;       OF 0.25 SECONDS PER CYCLE.
;       POLL THE HALT SWITCH WHILE
;       WAITING, USING THE EF3 FLAG .
;
;****************************************************************************
LOWALT: RLDI    RF,$0167        ; DELAY COUNTER
LOWHLT: BN3     HALT
		BN3 	HALT
		BN3 	HALT
		BN3 	HALT
;
		BN3 	HALT
		BN3 	HALT
		BN3 	HALT
		BN3 	HALT
;
		DBNZ	RF,LOWHLT		; DECREMENT DELAY COUNT
		BR		TLXFLY
;****************************************************************************
;
;       DEBUG:CYCLE TIMER
;
;       NOP OUT ABOVE BRANCH AND STOP
;       CLOCK COUNTER. USES Q OUTPUT
;       TO INDICATE CONTROL LOOP
;
;****************************************************************************
		LSQ 					; SKIP NEXT 2 IF Q=1
;
		SEQ 					; SET Q=1
		LSKP					; SKIP NEXT 2
;
		REQ 					; SET Q=0
		REQ 					; EVEN OUT LOOP
;
		BR		CLTSHF
;****************************************************************************
;       JUMP TO COMMAND LINK
;****************************************************************************
	IF		(VERSION GT VER_3)

HALT:	LBR 	HALTPAT ;		 BRANCH TO PATCH AREA

	ELSE

HALT:	LBR 	ABORTP			; ABORT PROCESSOR

	ENDIF
;****************************************************************************
; CONTROL LOOP
;****************************************************************************
TLXFLY: NOP                     ; NO OPERATION
		NOP 					; NO OPERATION
;****************************************************************************
;       SHIFT ALT STACK DOWN ONE ENTRY
;****************************************************************************
CLTSHF: RLDI    RA,ABOT+$02     ; BOTTOM OF STACK+2
		RLDI	RB,ABOT 		; BOTTOM STACK
		RLDI	RC,ABYTE-$02	; NUMBER BYTES-2
		SEX 	RA				; RA IS SP
;
CAHFT:	LDXA					; GET BYTE
		STR 	RB				; MOVE DOWN
		INC 	RB				; INCREMENT POINTER
;
		DBNZ	RC,CAHFT		; LOOP FOR ALL BYTES
		INC 	RB				; POINT TO LOW BYTE
;****************************************************************************
;       SHIFT ACCEL STACK DOWN ONE ENTRY
;****************************************************************************
CELSFT: RLDI    R7,CBOT+$02     ; BOTTOM OF STACK+2
		RLDI	R8,CBOT 		; BOTTOM STACK
		RLDI	R9,CBYTE-$02	; NUMBER BYTES-2
		SEX 	R7				; R7 IS SP
;
CSHFT:	LDXA					; GET BYTE
		STR 	R8				; MOVE DOWN
		INC 	R8				; INCREMENT POINTER
;
		DBNZ	R9,CSHFT		; LOOP FOR ALL BYTES
		INC 	R8				; POINT TO LOW BYTE
;****************************************************************************
;
;       A/D INPUT SECTION
;
;****************************************************************************
ADSET:  RLDI    R2,INPUT        ; INPUT STACK
;
		SEX 	R3				; MAKE PC SP
		OUT 	1				; SELECT I/O
		DB		$30
		OUT 	6				; SCAN MODE
		DB		$01
		OUT 	5				; START AT CH 0
		DB		$00
		SEX 	R2				; RESET SP
;
		BN1 	*				; WAIT FOR FLAG
;
		INP 	4				; INPUT RELIBILITY
		SMI 	$80
		LBNF	ABORTR
;****************************************************************************
;       RELIBILITY OK
;****************************************************************************
RELOK:  BN1     *               ; WAIT FOR FLAG
;
		INP 	4				; 100FT RANGE
		PLO 	RF				; STASH
;
		BN1 	*				; WAIT FOR FLAG
;
		INP 	4				; 1000FT RANGE
		PLO 	R7				; STASH
;****************************************************************************
		BN1 	*				; WAIT FOR FLAG
		INP 	4				; WING POSITION

	IF		(VERSION GT VER_2)

		PLO 	R0				; STASH

	ENDIF
;****************************************************************************
		BN1 	*				; WAIT
		INP 	3				; ACCEL
;****************************************************************************
;
;       SUBTRACT +1 G BIAS ($007F)
;
;****************************************************************************
BIAS:   SMI     $7F             ; BIAS
		STR 	R8				; STACK IT
		DEC 	R8				; HIGH BYTE
		LDI 	$00 			; ZERO BYTE
		SMBI	$00 			; BIAS
		STR 	R8				; STACK IT
;****************************************************************************
;       AUTO-RANGE ALT INPUTS
;****************************************************************************
AUTO:   LDI     $00             ; ZERO BYTE
		PHI 	RF				; HIGH BYTE OF ALT
;
		GLO 	RF				; LOW RANGE ALT
		SDI 	$FF 			; SUBTRACT MAX VALVE
		BNZ 	STACKA			; IF NOT MAX USE LOW RANGE
;****************************************************************************
;       FULL SCALE RANGE
;****************************************************************************
		GLO 	R7				; HIGH RANGE ALT
		PLO 	RF				; SET LOW BYTE OF ALT
;
		RLDI	RD,UTIL 		; SET MATH STACK
		SEX 	RD				; MAKE RD SP
		LDI 	$05 			; LOW BYTE OF 5
		STXD					; PUT ON STACK
		LDI 	$00 			; HIGH BYTE OF 5
		STR 	RD				; PUT ON STACK
;
		RLDI	R6,MULT 		; ADDRESS OF MULT
		SEP 	R6				; CALL MULT
;****************************************************************************
;       PUT CURRENT ALT ON STACK
;****************************************************************************
STACKA: GLO     RF              ;
		STR 	RB				;
		DEC 	RB				;
		GHI 	RF				;
		STR 	RB				;
;****************************************************************************
;       CHECK FOR ALT < 8 FT
;****************************************************************************
		GLO 	R7				; HIGH RANGE
		SMI 	$04 			; SUB 8FT
		LBNF	ABORTL			; IF BORROW ABORT
;****************************************************************************
;       AVERAGE 4 ALT SAMPLES
;****************************************************************************
		RLDI	RD,ATOP 		; TOP OF ALT STACK
		RLDI	RC,$0004		; NUMBER OF SAMPLES
		RLDI	RF,$0000		; ZERO SUM
		SEX 	RD				; SET STACK
;
AVG:    GLO     RF              ; GET LOW BYTE
		ADD 					; ADD TO ALT
		PLO 	RF				; RESET SUM
;
		DEC 	RD				; POINT TO HIGH BYTE
;
		GHI 	RF				; GET HIGH BYTE
		ADC 					; ADD WITH DF
		PHI 	RF				; RESET SUM
;
		DEC 	RD				; POINT TO NEXT LOW BYTE
;
		DBNZ	RC,AVG			; ADD ALL WORDS
;
		SEX 	R2				; RESET STACK
		RLDI	R6,USHR 		; ADDRESS OF USHR
		SEP 	R6				; DIVIDE SUM BY 4
		DB		$02
;****************************************************************************
;       RESET CURRENT INPUT WITH AVERAGE
;****************************************************************************
		RLDI	RD,ATOP
		SEX 	RD				; RESET SP
		GLO 	RF				; LOW BYTE
		STXD					; STACK IT
		GHI 	RF				; HIGH BYTE
		STR 	RD				; STACK IT
;****************************************************************************
;       CALCULATE ALT ERROR
;****************************************************************************
CALERR: RLDI    RD,PRESET       ; POINTER TO PRESET
		SEX 	RD				; SET STACK POINTER
;
		GLO 	RF				; GET LOW BYTE OF ALT
		SD						;
;
		PLO 	RF				;
		PLO 	R9				;
;
		DEC 	RD				;
;
		GHI 	RF				; GET HIGH BYTE OF ALT
		SDB 					;
;
		PHI 	RF				;
		PHI 	R9				;
;****************************************************************************
;       DETERMINE +/- 50 FT ALT DEVIATION
;****************************************************************************
DETDEV: GHI     R9              ; CHECK SIGN OF ERROR
		SHL 					;
		LBDF	AHIGH			;
;****************************************************************************
;       LOW ON PRESET
;****************************************************************************
	IF		((VERSION GT VER_2) AND (VERSION LT VER_5))

		RLDI	RD,PFLAG		; PRESET FLAG
		LDN 	RD				; LOAD FLAG
		BNZ 	01$ 			; IF FLAG SET SKIP SETTING GAIN

		RLDI	RD,WFLAG		; WING FLAG
		GLO 	R0				; GET WING
		STR 	RD				; STORE
		RLDI	R6,GAINEW		; SET NEW GAIN FUNCTION
		SEP 	R6				; CALL FUNCTION

		RLDI	RD,PFLAG		; PRESET FLAG
		LDN 	RD				; LOAD FLAG
		BZ		01$ 			; IF FLAG RESET SKIP SETTING FAST OK

		RLDI	R6,OKFAST		; SET FAST OK LITE
		SEP 	R6				; CALL FUNCTION

	MSG 'GAIN SWITCH CODE'
	ENDIF
;****************************************************************************
;       LOW ON PRESET
;****************************************************************************
01$:    GHI     R9              ;
		BNZ 	MAXL			;
;
		GLO 	R9				;
		ADI 	$80 			;
		BDF 	MAXL			;
;
		LSKP					;
;
MAXL:   LDI     $FF             ;
		BR		OUT 			;
;****************************************************************************
;       HIGH ON PRESET
;****************************************************************************
AHIGH:  GHI     R9              ;
		SMI 	$FF 			;
		BNZ 	MAXH			;
;
		GLO 	R9				;
		SMI 	$80 			;
		BNF 	MAXH			;
;
	IF	(VERSION EQ VER_4)

		BR		OUT				; BRANCH
;
MAXH:	LBR		GAINRT			; BRANCH TO PATCH AREA
		LDI		$00				; THIS CODE IS NOT REQUIRED

	MSG 'GAIN SWITCH CODE'
	ELSE

		LSKP					; SKIP NEXT INSTRUNCTION
MAXH:	LDI		$00

	ENDIF
;****************************************************************************
;       OUTPUT ALT DEVIATION TO D/A CH 2
;****************************************************************************
OUT:    XRI     $FF             ; INVERT ALT DEVIATION
		STR 	R2				; STACK IT
		SEX 	R2				; MAKE R2 STACK
		OUT 	4				; OUTPUT ALT DEVIATION FROM STACK
		DEC 	R2				; RESET STACK POINTER
;****************************************************************************
;       GAIN ALT ERROR (KP*ERROR)
;****************************************************************************
		RLDI	RD,EGAIN-1		; POINT TO HIGH BYTE
		RLDI	R6,MULT 		; ADDRESS OF MULT
		SEX 	RD				; RD IS SP
		SEP 	R6				; CALL MULT
;****************************************************************************
;       STORE RESULT (RE,RF) IN R0,R1
;****************************************************************************
		GHI 	RE				;
		PHI 	R0				;
;
		GLO 	RE				;
		PLO 	R0				;
;
		GHI 	RF				;
		PHI 	R1				;
;
		GLO 	RF				;
		PLO 	R1				;
;****************************************************************************
;       DETERMINE ALT(N)-ALT(1)
;
;       THIS IS REVERSED SO THAT AN UPWARD
;       ALT CHANGE WITH YIELD A NEGATIVE
;       WING ANGLE
;****************************************************************************
ARATE:  RLDI    RD,ATOP         ; ALT(1)
		RLDI	RC,ABOT+$01 	; ALT(N)
		SEX 	RD
;
		LDN 	RC
		SM						; (N)-(1)
		PLO 	RF				; STORE
;
		DEC 	RD				; POINT TO HIGH BYTE
		DEC 	RC				; POINT TO HIGH BYTE
;
		LDN 	RC				; HIGH BYTE
		SMB 					; HIGH BYTE
		PHI 	RF				; STORE
;****************************************************************************
;       DIVIDE ALT DIFFERENCE BY N*0.0625
;       WITH N=15 THEN IS APPROXIMATELY
;       EQUAL TO 1, THEREFORE NO OPERATION
;****************************************************************************
		NOP
		NOP
		NOP
;****************************************************************************
;       GAIN ALT DIFF (KV* DIFF)
;****************************************************************************
		RLDI	RD,RGAIN-1		; POINT TO HIGH BYTE
		RLDI	R6,MULT 		; ADDRESS OF MULT
		SEX 	RD				; MAKE RD STACK
		SEP 	R6				; CALL MULT
;****************************************************************************
;       ADD KP*ERROR + KV*ALT DIFF
;****************************************************************************
		RLDI	R6,ADDER		; ADDRESS OF ADDER
		SEX 	R2				; RESET STACK
		SEP 	R6				; CALL ADDER
;****************************************************************************
; AVERAGE ACCEL INPUT
;****************************************************************************
ACCEL:  RLDI    RB,CTOP         ; ACCEL STACK
		RLDI	RC,$0004		; 4 AVERAGES
		RLDI	RF,$0000		; ZERO SUM
		SEX 	RB				; SET STACK
;
CELAVG: GLO     RF              ; LOW BYTE
		ADD 					; ADD LOW BYTES
		PLO 	RF				; RESET SUM
;
		DEC 	RB				; POINT TO HIGH BYTE
;
		GHI 	RF				; HIGH BYTE
		ADC 					; ADD HIGH BYTES
		PHI 	RF				; RESET SUM
;
		DEC 	RB				; POINT TO NEXT LOW
;
		DBNZ	RC,CELAVG		; ADD UP FOUR INPUTS
;
		SEX 	R2				; RESET SP
		RLDI	R6,ASHR 		; ADDRESS OF ASHR
		SEP 	R6
		DB		$02
;
		RLDI	RB,CTOP
		GLO 	RF				; LOW BYTE
		STR 	RB				; STACK IT
		DEC 	RB				; POINT TO HIGH
		GHI 	RF				; HIGH BYTE
		STR 	RB				; STACK IT
;****************************************************************************
;       DOUBLE INTEGRATE THE ACCEL
;****************************************************************************
DOUBLE: RLDI    RD,COUNT        ; POINT TO COUNT
		LDN 	RD				; LOAD COUNT
		SMI 	$01 			; COUNT-1
		STR 	RD				; RESET COUNT
		LBNZ	LOWALT			; LOOP TILL ZERO
		LDI 	$04 			; 4 CYCLES
		STR 	RD				; RESET COUNT
;****************************************************************************
;       INTEGRATE ACCEL
;****************************************************************************
		RLDI	RB,CTOP 		; ACCEL STACK
		RLDI	RA,CBYTE2		; NUMBER WORBS
		RLDI	R9,$0000		; DOUBLE SUM
		SEX 	RD				; SET MATH STACK
;
CCEL:	LDN 	RB				; GET LOW BYTE
		PLO 	RF				; STORE
		DEC 	RB				; POINT TO HIGH BYTE
		LDN 	RB				; GET HIGH BYTE
		PHI 	RF				; STORE
		DEC 	RB				; NEXT LOW BYTE
;
		RLDI	RD,UTIL 		; SET STACK
		GLO 	RA				; LOW COUNT
		STXD					; STACK IT
		GHI 	RA				; HIGH COUNT
		STR 	RD				; STACK IT
		RLDI	R6,MULT 		; ADDRESS OF MULT
		SEP 	R6				; MULT
;
		GLO 	RF				; LOW BYTE
		STR 	RD				; STACK IT
		GLO 	R9				; LOW BYTE
		ADD 					; ADD LOW BYTES
		PLO 	R9				; RESET SUM
;
		GHI 	RF				; HIGH BYTE
		STR 	RD				; STACK IT
		GHI 	R9				; HIGH BYTE
		ADC 					; ADD HIGH BYTES
		PHI 	R9				; RESET SUM
;
		DBNZ	RA,CCEL 		; SUM STACK
;
		GLO 	R9				; LOW BYTE
		PLO 	RF				;
		GHI 	R9				; HIGH BYTE
		PHI 	RF				;
;****************************************************************************
;       MULT ACCEL INTEGRAL BY 0.0625**2
;       AND DIVIDE BY 15*0.0625
;       WHICH IS APPROXIMATELY 2**(-8)
;****************************************************************************
		RLDI	R6,ASHR 		; ADDRESS OF ASHR
		SEP 	R6				; CALL ASHR
		DB		$08 			; NUMBER OF RIGHT SHIFTS
;****************************************************************************
;       GAIN ACCEL INTEGRAL (KV* ACCEL)
;****************************************************************************
		RLDI	RD,IGAIN-1		; POINT TO HIGH BYTE
		RLDI	R6,MULT 		; ADDRESS OF MULT
		SEX 	RD				; MAKE RD STACK
		SEP 	R6				; CALL MULT
;****************************************************************************
;       ADD KP*ERROR + KV*ACCEL INTEGRAL
;****************************************************************************
		RLDI	R6,ADDER		; ADDRESS OF ADDER
		SEX 	R2				; RESET STACK
		SEP 	R6				; CALL ADDER
;****************************************************************************
;       WING COMMAND SECTION
;****************************************************************************
WING:   RLDI    R2,UTIL         ; MATH STACK
		SEX 	R2				; SETUP STACK
;
		RLDI	RD,SGAIN		; SCALE FACTOR
		LDN 	RD				; GET SCALE
		SHR 					; DIVIDE BY 16
		SHR
		SHR
		SHR
		PLO 	RD				; STORE
;****************************************************************************
;       DIVIDE BY 2**SCALE
;****************************************************************************
		RLDI	R6,SCALE		; ADDRESS OF SCALE
		SEP 	R6				; CALL SCALE
;****************************************************************************
;       CHECK WING COMMAND FOR OVERFLOW
;****************************************************************************
OVERFL: GHI     R0              ; GET MSB
		SHL 					; PUT SIGN IN DF

	IF		(VERSION LT VER_3) OR (VERSION EQ VER_5)

		BDF 	NEGWNG			; IF NEG BRANCH

	ELSE

		LBDF	NEGWNG			; IF NEG BRANCH

	ENDIF

		GHI 	R1				; GET HIGH BYTE
		BNZ 	POSOVF			; IF NOT ZERO OVERFLOW
		GLO 	R1				; GET LOW BYTE
		SHL 					; PUT BIT 7 IN DF
		BNF 	_OFFSET 		; IF ZERO OK
POSOVF: RLDI    R1,WNGMAX       ; MAXIMUM (+)
		BR		_OFFSET 		; ADD IN OFFSET
;
NEGWNG: GHI     R1              ; GET HIGH BYTE
		SDI 	$FF 			; SUBTRACK NEG BITS
		BNZ 	NEGOVF			; IF NOT ZERO OVERFLOW
		GLO 	R1				; GET LOW BYTE
		SHL 					; PUT SIGN IN DF
		BDF 	_OFFSET 		; IF SET OK
NEGOVF: RLDI    R1,WNGXRI       ; MAXIMUM (-)
;****************************************************************************
;       ADD IN HARDWARE OFFSET
;****************************************************************************
_OFFSET:
		GLO 	R1				; GET WING COMMAND
		ADI 	LOW(WNGZER) 	; ADD OFFSET
		PLO 	R1				; RESET COMMAND
		GHI 	R1				; GET HIGH BYET
		ADCI	HIGH(WNGZER)	; ADD OFFSET HIGH BYTE
		PHI 	R1				; RESET HIGH BYTE
;****************************************************************************
;       SUBTRACT MANUAL RESET
;****************************************************************************
SUBMAN: RLDI    RD,RESET        ; MANUAL RESET
		LDN 	RD				; GET RESET
		STR 	R2				; STACK IT
;
		GLO 	R1				; GET WING POSITION
		SM						; SUBTRACT MR
		PLO 	R1				; RESET WING POSITION
		GHI 	R1				; HIGH BYTE
		SMBI	$00 			; HIGH BYTE OF LIMIT
		PHI 	R1				; RESET HIGH BYTE
;****************************************************************************
;       SOFTWARE LIMIT SWITCHES
;****************************************************************************
SOFT:   GHI     R1              ; CHECK HIGH BYTE
		BZ		SOFT1			; IF ZERO IN LIMITS
;
		SHL 					; PUT SIGN IN DF
		BDF 	STOP			; NEG OVERFLOW
		BR		LIMIT			; POS OVERFLOW
;
SOFT1:  GLO     R1              ; GET COMMAND
		SMI 	LOW(WNGNEG) 	; - LIMIT
		BNF 	STOP			; IF DF=0 LIMIT
;
		GLO 	R1				; GET COMMAND
		SMI 	LOW(WNGPOS) 	; + LIMIT
		BDF 	LIMIT			; IF DF=1 LIMIT
;
		GLO 	R1				; GET COMMAND
;
		LSKP					; SKIP NEXT
;
LIMIT:  LDI     LOW(WNGPOS)     ; MAXIMUM + LIMIT
;
		LSKP					; SKIP NEXT
;
STOP:   LDI     LOW(WNGNEG)     ; MAXIMUM - LIMIT
;****************************************************************************
; OUTPUT TO SERVO
;****************************************************************************
SERVO:  STR     R2              ; STACK IT
		OUT 	3				; OUT D/A CH 1
		DEC 	R2				; RESET STACK
;****************************************************************************
; CYCLE CONTROLLER
;****************************************************************************
		LBR 	LOWALT
;****************************************************************************
;
; END OF CONTROLLER SECTION
;
;****************************************************************************
;
;       ABORT CONTROLLER
;
;****************************************************************************
;       ABORT ON HALT SWITCH
;****************************************************************************
ABORTP: LDI 	$00 	; HALT FLAG
		PLO 	RC		; SET ABORT FLAG

	IF		(VERSION GT VER_2)

		BR		ABORT	; BRANCH

	MSG 'ABORT PROCESSOR PATCH'
	ELSE

		BR		PREC	; BRANCH

	ENDIF
;****************************************************************************
;       ABORT IF ALT LESS THAN 8 FT
;****************************************************************************
ABORTL: LDI     $01     ; ABOTRL FLAG
		PLO 	RC		; SET ABORT FLAG
		BR		ABORT	; BRANCH
;****************************************************************************
;       ABORT IF ALT RELIBILITY IS LOST
;****************************************************************************
ABORTR: LDI     $02     ; ABORTR FLAG
		PLO 	RC		; SET ABORT FLAG
		BR		ABORT	; BRANCH
;****************************************************************************
;       ABORT PROCESSOR
;****************************************************************************
ABORT:
	IF		(VERSION GT VER_2)

		RLDI	R6,OKSLOW	; SET SLOW OK LITE
		SEP 	R6			; CALL FUNCTION

	ENDIF

		RLDI	R2,INPUT	; I/O STACK
		SEX 	R3			; SET SP TO PC
		OUT 	1			; I/O GROUP 30
		DB		$30
		OUT 	6			; SCAN MODE
		DB		$01
		OUT 	5			; REL
		DB		$00
		SEX 	R2			; RESET SP
;
		BN1 	*			; WAIT
		INP 	4			; REL
;
		BN1 	*			; WAIT
		INP 	4			; 100FT ALT
;
		BN1 	*			; WAIT
		INP 	4			; 500FT ALT
;
		BN1 	*			; WAIT
		INP 	3			; WING
;
		SMI 	LOW(WNGREC) ; SUBTRACK RECOVERY
		BNF 	PREC		; DF=0 WING BELOW RECOVERY
;****************************************************************************
;       DO NOT PUT WINGS IN RECOVERY
;****************************************************************************
		LDX 				; GET POSITION
;
		LSKP				; SKIP NEXT INSTRUCTION
;****************************************************************************
;       PUT WINGS INTO RECOVERY
;****************************************************************************
PREC:	LDI 	LOW(WNGREC) ; RECOVERY
;
		STR 	R2			; PUT ON STACK
		OUT 	3			; OUPUT D/A CH1
		DEC 	R2			; RESET STACK
;****************************************************************************
;       EXIT TO COMMAND LINK ERROR ROUTINE
;****************************************************************************
	IF		(VERSION GT VER_2)

		LDX 				; GET WING POSITION
		SDI 	WNGREC		; CHECK ON RECOVERY POSITION
		LBZ 	LINK		; BRANCH IF AT RECOVERY
EXIT:	LBR 	CMWAIT		; BRANCH IF NOT AT RECOVERY

	ELSE

EXIT:	LBR 	LINK		; BRANCH

	ENDIF
;****************************************************************************
;
;       END OF ABORT PROCESSOR
;
;****************************************************************************
	END

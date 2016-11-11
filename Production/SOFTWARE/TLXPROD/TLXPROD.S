	LIST
	TITLE   LOW LEVEL OPERATING SYSTEM
	SUBTTL	(C)1984,85,86,87,88,89,90,91,92 HAYES INTERNATION CORPORATION
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
;
;                       REVISION DATE
;
;                       SEPTEMBER 14 1989
;
;                       PRODUCTION VERSION 1
;
; CHANGED PROPORTIONAL GAIN FROM 1 VOLT TO 2 VOLTS
;
;                       JANUARY 31 1989
;
;                       PRODUCTION VERSION 2
;
; THIS RELEASE REPLACES THE SOFTWARE INSTALLED IN THE PROTOTYPE TARGETS
; REVISIONS ARE IN THE FIXED GAIN SETTINGS AND ARE NOTED
;
;                       JUNE 02 1991
;
;                       FLIGHT INTERNATIONAL TEST VERSION (VERSION 3)
;
; THIS RELEASE INCORPORATES EXTENSIVE MODIFICATION TO THE SOFTWARE. THE
; TARGET CANNOT BE COMMAND INTO RECOVERY IF THE WINGS ARE ABOVE THE MINUS
; 1.5 DEGREE POSITION. IF DONE SO, THE RECOVERY/FLY LIGHTS FLASH AND THE
; TLX MUST BE COMMANDED BACK INTO FLY AND THEN BACK TO RECOVERY TO POSITION
; THE WINGS AT THE -1.5 DEGREE POSITION. ALSO THE GAIN SETTING HARDCODED IN
; THE AUTOPILOT USE THE VERSION 1 GAINS (25 FEET) UNTIL THE TLX PASSES
; BELOW PRESET. ONCE BELOW PRESET ( AND WITH THE WINGS LESS THAN +/- 0.5 DEG
; FROM THE MANUAL RESET POSITION, THE GAINS SWITCH TO THE VERSION 2 GAINS
; (12.5 FEET). AT THIS TIME THE OK LIGHT BLINK RATE IS DOUBLED TO INDICATE
; THE SWITCH. THE OK LIGHT RETURNS TO NORMAL APON A RECOVERY COMMAND.
;
; THE CHANGES WERE EXTENSIVE ENOUGH THAT A SECOND EPROM IS NOW REQUIRED IN
; THE SYSTEM AT ADDRESS $0800.
;
;                       JUNE 16 1991
;
;                       PRODUCTION VERSION 4
;
; THIS RELEASE ADDS A FEW FEATURES TO THE FLIGHT INTERNATION VERSION
; TEST FLOWN ON JUNE 13, 1991. A CHECK IS MADE BEFORE ENTERING "FLY' MODE
; TO INSURE THAT FOUR CONSECUTIVE FLY COMMANDS ARE ISSUED. THIS IS TO PREVENT
; AN COMMAND LINK GLITCH FROM COMMANDING THE TLX INTO FLY. ALSO, THE EXIT
; FROM THE FLY MODE TO RECOVERY MODE, MADE BY THE DETECTION OF THE RECOVERY
; COMMAND FROM THE CONTROL STATION IS PUT THROUGH A SECOND WAIT STATE TO
; INSURE THE COMMAND IS VALID AND THAT A GLITCH IN THE COMMAND LINK WILL NOT
; PLACE THE TLX INTO RECOVERY ONCE IN THE FLY MODE. ALSO, ONCE THE GAINS
; ARE RESET BY THE TLX ENTERING "CAPTURE MODE", THE ALTITUDE DEVIATION IS
; CHECK FOR THE TARGET GOING 50 FEET ABOVE PRESET. IF THIS OCCURS, THE GAINS
; ARE SET BACK TO THE VERSION 1 VALUES (25 FEET) UNTILL SUCH TIME AS THE
; TLX CAN ONCE AGAIN CATURE THE PRESET ALTITUDE. ONCE THE GAINS ARE SET BACK
; TO THE VERSION 1 VALUES, THE OK LIGHT IS RESET TO ITS NORMAL BLINK RATE.
;
;                       FEB 27 1992
;
;                       PRODUCTION VERSION 5
;
; THIS RELEASE REMOVES THE BREAK OUT OF CAPTURE MODE OF VERSION 4. THIS
; PROVED TO BE UNSTABLE IN ROUGH CONDITIONS. ALSO A FIX WAS MADE IN THE
; CMDPAT AREA THAT RE-ENABLES MANUAL MODE (WHICH WAS LOST DUE TO A LOGIC
; CONFLICT). ALSO ADDED IN ALTERNATE PROGRAM INTERFACE AT $0C00.
;
;****************************************************************************
;
;       EQUATES
;
;****************************************************************************
	ORG     $0000           ; EPROM NUMBER ONE
;
RAM     EQU     $4000           ; CPU CONFIGURATION
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
;VERSION EQU    VER_2           ; SET VERSION
;VERSION EQU    VER_3           ; SET VERSION
;VERSION EQU	VER_4			; SET VERSION
VERSION EQU    VER_5		   ; SET VERSION

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
;       ACCEL CONTROLLER ADDRESS
;****************************************************************************
; TLXACC        EQU     $0400    ; (EPROM ONE)
;****************************************************************************
;       ALTERNATE CONTROLLER ADDRESS
;****************************************************************************
; TLXALT        EQU     $0800   ; (EPROM TWO)
;****************************************************************************
;
;       DATA STACK EQUATES
;
;****************************************************************************
ATOP    EQU     RAM+$01FF       ; TOP OF ALT STACK
ABOT    EQU     RAM+$01E0       ; BOTTOM OF ALT STACK
ABYTE   EQU     $0020           ; NUMBER BYTES
;
CTOP    EQU     RAM+$02FF       ; TOP OF ACCEL STACK
CBOT    EQU     RAM+$02E0       ; BOTTOM OF ACCEL STACK
CBYTE   EQU     $0020           ; NUMBER BYTES
CBYTE2  EQU     $0010           ; NUMBER WORDS
;
COUNT   EQU     RAM+$03FF       ; LOOP COUNTER
;
ALTREL  EQU     RAM+$037F       ; RELIBILITY
RELFLG  EQU     RAM+$037D       ; REL FLAG
RELCNT  EQU     RAM+$037B       ; REL COUNTER
;
ALTLOW  EQU     RAM+$036F       ; 100FT ALT
ALTHIH  EQU     RAM+$035F       ; 1000FT ALT
FEDPOT  EQU     RAM+$034F       ; FEEDBACK POT
GLOADS  EQU     RAM+$033F       ; ACCEL
;
MATH    EQU     RAM+$043F       ; MATH STACK
INPUT   EQU     RAM+$047F       ; INPUT STACK
UTIL    EQU     RAM+$04FF       ; UTILLITY STACK
;
PRESET  EQU     RAM+$050F       ; PRESET ALT
PRESE1  EQU     RAM+$050B       ; PRESET ONE
PRESE2  EQU     RAM+$0507       ; PRESET TEN
PRESE3  EQU     RAM+$0503       ; PRESET 100
;
EGAIN2  EQU     RAM+$0517       ; PROPORTIONAL GAIN TWO
EGAIN   EQU     RAM+$051F       ; PROPORTIONAL GAIN
RGAIN   EQU     RAM+$052F       ; DERIVATIVE GAIN
IGAIN   EQU     RAM+$053F       ; INTEGRAL GAIN
SGAIN   EQU     RAM+$054F       ; SCALE FACTOR
RESET   EQU     RAM+$055F       ; MANUAL RESET
;****************************************************************************
;       TEST REGISTERS
;****************************************************************************
PREONE  EQU     RAM+$063F       ; ONE DIGIT
PRETEN  EQU     RAM+$063B       ; TEN DIGIT
PREHUN  EQU     RAM+$0637       ; HUN DIGIT
;****************************************************************************
;       STATUS LIGHTS AND BITS EQUATES
;****************************************************************************
MANLIT  EQU     $0000           ; MANUAL CONTROL LIGHT
RECLIT  EQU     $0010           ; RECOVERY LIGHT
PRGLIT  EQU     $0020           ; PRG CONTROLLER LIGHT
ALTLIT  EQU     $0040           ; ALT PROGRAM LIGHT
ACCLIT  EQU     $0060           ; ACC CONTROLLER LIGHTS
;****************************************************************************
ACCBIT  EQU     $0000           ; ACC BITS
RECBIT  EQU     $0001           ; RECOVERY BIT
PRGBIT  EQU     $0002           ; PRG BIT
ALTBIT  EQU     $0004           ; ALT BITS
MANBIT  EQU     $0006           ; MANUAL BIT
;****************************************************************************
;       OK LIGHT COUNTER
;****************************************************************************
OKLITE  EQU     $40
;****************************************************************************
;       COMMAND WORD
;****************************************************************************
COMWRD  EQU     RAM+$06FF       ; COMMAND WORD
TIMER   EQU     RAM+$067F       ; TIMER STACK
;****************************************************************************
;       GAIN SWITCH FLAGS
;****************************************************************************
PFLAG   EQU     RAM+$07FF       ; PRESET FLAG
WFLAG   EQU     RAM+$077F       ; WING FLAG
;****************************************************************************
;       COMMAND WORD CHECKSUM
;****************************************************************************
COMCHK  EQU     RAM+$073F       ; LAST COMMAND
COMCNT  EQU     RAM+$073D       ; COMMAND CHECK
COMFLG  EQU     RAM+$073B       ; COMMAND FLAG
;****************************************************************************
;
;       POWER ON RESET ENTRY
;
;****************************************************************************
		DIS 					; DISABLE INTERRUPTS
		DB		$00
;****************************************************************************
;       SET WINGS TO RECOVERY POSITION
;****************************************************************************
		OUT 	1				; I/O GROUP 30
		DB		$30
		OUT 	3				; RECOVERY
		DB		WNGREC
;****************************************************************************
;       TEST FOR ACTIVE DECODER
;****************************************************************************
	IF		(VERSION GT VER_2)

		LBR 	RTD1			; BRANCH TO PATCH

	MSG 	'RTD1 PATCH'
	ENDIF
;****************************************************************************
;
;       SETUP COUNTER MODE 2
;
;       1805 DOWN COUNTER WITH DOWN COUNT ON
;       EACH TRANSITION OF EF2 (DATA VALID)
;       THE INITIAL COUNTER VALUE IS SET TO
;       UNDERFLOW EVERY 64 DEC COUNTS
;       (0.25 SECONDS) AND TO FLIP/FLOP THE
;       Q TOGGLE.
;       THE COUNTER VALUE IS COMPUTED BY:
;
;       2048 BITS/SEC FROM REMTRON DECODER
;       256 FRAMES/SEC FROM DECODER
;       0.25 SEC / 256 SEC/FRAME = 64
;
;****************************************************************************
OKSET:  STPC                    ; STOP COUNTER
		LDI 	OKLITE			; 64 COUNTS
		LDC 					; LOAD COUNTER
		ETQ 					; ENABLE Q
		SCM2					; COUNTER MODE 2
;****************************************************************************
;       SETUP PROGRAM COUNTER IN R3
;****************************************************************************
TMINIT: RLDI    R3,TMWAIT
		SEP 	R3
;****************************************************************************
;
;       COMMAND LINK SOFTWARE
;
;****************************************************************************
;
;       WAIT FOR RECOVERY/FLY SWITCH TO REC
;       BLINK RECOVERY LIGHT DURING WAIT
;
;****************************************************************************
;****************************************************************************
;
;       SETUP PIO PORTS FOR COMMAND/TM LINK
;
;       I/O GROUP NUMBER 8
;       CHANNEL A INPUT (DATA WORD)
;       CHANNEL B BIT PROGRAMABLE
;       LOW NIBBLE INPUT (ADDRESS)
;       HIGH NIBBLE OUTPUT (STATUS)
;
;****************************************************************************
TMWAIT: SEX     R3              ; SET PC TO SP
		OUT 	1				; I/O GROUP 8
		DB		$08
		OUT 	2
		DB		$0F
		OUT 	2
		DB		$F7
		OUT 	2
		DB		$F0
;
		RLDI	R2,INPUT		; INPUT STACK
		SEX 	R2				; SET SP
		LDI 	RECLIT			; RECOVERY LIGHT
		PLO 	RC				; STORE WORD
;****************************************************************************
VAL:    B2      VAL             ; WAIT FOR DATA VALID
;****************************************************************************
		INP 	6				; INPUT ADDRESS
		IRX 					; INCREMENT SP
		INP 	4				; INPUT ADDRESS
		DEC 	R2				; RESET SP
		LDX 					; GET ADDRESS
		ANI 	$0F 			; MASK BITS
;
		SMI 	$09 			; CHANNEL NINE
		BNZ 	VAL 			; IF NOT NEXT INPUT
;****************************************************************************
;       CHECK REC/FLY BIT FOR REC
;****************************************************************************
		IRX 					; INCREMENT TO DATA
		LDX 					; GET DATA
		PHI 	RC				; STORE WORD
		DEC 	R2				; RESET SP
;
		GLO 	RC				; GET LIGHT
		STR 	R2				; STACK IT
		XRI 	$FF 			; INVERT BITS
		ANI 	RECLIT			; ONLY REC LIGHT
		PLO 	RC				; RESET LIGHT
		OUT 	6				; SEND STATUS
		DEC 	R2				; RESET STACK
;
		RLDI	RF,$3FFF		; COUNTER DELAY
DELAY1: NOP                     ; NO OPERATION
		DBNZ	RF,DELAY1		; DELAY LOOP
;
		GHI 	RC				; GET WORD
		ANI 	RECBIT			; MASK REC BIT
		BZ		VAL 			; IF NOT REC, WAIT
;****************************************************************************
;
;       COMMAND LINK GRABER ROUTINE
;
;****************************************************************************
GRABER: SEX     R3              ; SET PC TO SP
		OUT 	1				; I/O GROUP 8
		DB		$08
		RLDI	R2,INPUT		; INPUT STACK
		SEX 	R2				; SET SP
;****************************************************************************
;       INITIALIZE CHANNEL NUMBER TO ONE
;****************************************************************************
		RLDI	R7,$0001
;****************************************************************************
;       WAIT ON DATA VALID
;****************************************************************************
VALID:  B2      VALID
;****************************************************************************
;       INPUT BYTES FROM 1851 INPUT PORTS
;****************************************************************************
		INP 	6				; INPUT ADDRESS
		IRX 					; INCREMENT SP
		INP 	4				; INPUT DATA
		DEC 	R2				; RESET SP
		LDX 					; GET ADDRESS
		ANI 	$0F 			; MASK BITS
		STR 	R2				; REPLACE ON STACK
;****************************************************************************
;       CHECK FOR CORRECT ADDRESS
;****************************************************************************
		GLO 	R7				; GET CHANNEL
		SM						; COMPARE INPUT
		BNZ 	VALID			; IF NOT SAME, NEXT
;****************************************************************************
;       RETREIVE DATA
;****************************************************************************
		IRX 					; INCREMENT TO DATA
		LDX 					; GET DATA
		DEC 	R2				; RESET SP
		STR 	R2				; REPLACE ON STACK
;****************************************************************************
;       DETERMINE DATA CHANNEL
;****************************************************************************
WHICH:  GLO     R7              ; GET CHANNEL
		INC 	R7				; INC FOR NEXT
;
		SMI 	$01
		BZ		CH1
;
		SMI 	$01
		BZ		CH2
;
		SMI 	$01
		BZ		CH3
;
		SMI		$01
		BZ		CH4
;
		SMI 	$01
		BZ		CH5
;
		SMI 	$01
		BZ		CH6
;
		SMI 	$01
		BZ		CH7
;
		SMI 	$01
		BZ		CH8
;
		BR		CH9
;****************************************************************************
;
;       LOAD DATA AND STORE IN M(RD)
;       AND STORE ZERO IN HIGH BYTE M(RD)-1
;
;****************************************************************************
CH0:    LDX                     ; GET DATA
		STR 	RD				; STACK IT
;
		DEC 	RD				; DECREMENT TO HIGH BYTE
		LDI 	$00 			; ZERO BYTE
		STR 	RD				; STACK IT
;
		BR		VALID			; NEXT CHANNEL
;****************************************************************************
;       CHANNEL ONE (PRESET ONE'S)
;****************************************************************************
CH1:    RLDI    RD,PRESE1
		BR		CH0
;****************************************************************************
;       CHANNEL TWO (PRESET TEN'S)
;****************************************************************************
CH2:    RLDI    RD,PRESE2
		BR		CH0
;****************************************************************************
;       CHANNEL THREE (PRESET 100'S)
;****************************************************************************
CH3:    RLDI    RD,PRESE3
		BR		CH0
;****************************************************************************
;       CHANNEL FOUR (PROPORTIONAL GAIN)
;****************************************************************************
CH4:    RLDI    RD,EGAIN
		BR		CH0
;****************************************************************************
;       CHANNEL FIVE (HEIGHT RATE GAIN)
;****************************************************************************
CH5:    RLDI    RD,RGAIN
		BR		CH0
;****************************************************************************
;       CHANNEL SIX (ACCEL INTEGRAL GAIN)
;****************************************************************************
CH6:    RLDI    RD,IGAIN
		BR		CH0
;****************************************************************************
;       CHANNEL SEVEN (OUTPUT SCALE FACTOR)
;****************************************************************************
CH7:    RLDI    RD,SGAIN
		BR		CH0
;****************************************************************************
;       CHANNEL EIGHT (MANUAL RESET)
;****************************************************************************
CH8:    RLDI    RD,RESET
		BR		CH0
;****************************************************************************
;
;       CHANNEL NINE: COMMAND
;
;****************************************************************************
CH9:    RLDI    RC,COMWRD       ; COMMAND WORD
		LDX 					; GET DATA
		STR 	RC				; STASH WORD
;
		PHI 	RC				; STORE
;****************************************************************************
;       JUMP TO PATCH AREA
;****************************************************************************
	IF		(VERSION GT VER_3)

		LBR 	CMDPAT			; JUMP TO PATCH AREA

	MSG 	'CMDPAT PATCH'
	ENDIF
;****************************************************************************
;       ECHO COMMAND WORD TO STATUS LIGHTS
;****************************************************************************
ECHO:   RLDI    R2,UTIL         ; UTIL STACK
		SEX 	R2				; SET SP
		GHI 	RC				; GET COMMAND
;
		SHL 					; LEFT JUSTIFY
		SHL 					;
		SHL 					;
		SHL 					;
;
		STR 	R2				; STACK IT
		OUT 	6				; OUTPUT
		DEC 	R2				; RESET STACK
;****************************************************************************
;
;       EXECUTE COMMAND INSTRUCTIONS
;               BIT 0 REC/FLY (1=REC/0=FLY)
;               BIT 1 PRG1
;               BIT 2 PRG2
;               BIT 3 FIRE ONE
;
;       WHEN BIT 0 = 1 (FLY)
;
;               PRG1=0,PRG2=0 ACCEL CONTROLLER
;               PRG1=1,PRG2=0 PROGRAMABLE CONTROLLER
;               PRG1=0,PRG2=1 ALTERNATE CONTROL MODE
;               PRG1=1,PRG2=1 MANUAL CONTROL MODE
;
;****************************************************************************
		GHI 	RC				; GET COMMAND
		ANI 	RECBIT			; RECOVERY BIT
		LBNZ	PULLIT			; RECOVERY MODE
;
		GHI 	RC				; GET COMMAND
		ANI 	$07 			; MASK BIT 3
		LBZ 	ACC 			; ACCEL CONTROLLER
;
		GHI 	RC				; GET COMMAND
		ANI 	$07 			; MASK BIT 3
		SMI 	PRGBIT			; PRG BIT
		LBZ 	PRG 			; PROGRAM MODE
;
		GHI 	RC				; GET COMMAND
		ANI 	$07 			; MASK BIT 3
		SMI 	ALTBIT			; ALTERNATE BIT
		LBZ 	ALT 			; ALTERNATE MODE
;
		GHI 	RC				; GET COMMAND
		ANI 	$07 			; MASK BIT 3
		SMI 	MANBIT			; MANUAL BIT
		LBZ 	MANUAL			; MANUAL MODE
;****************************************************************************
;
;       RECOVERY MODE
;
;****************************************************************************
PULLIT: SEX     R3              ; SET R3 TO SP
		OUT 	1				; I/O GROUP
		DB		$30
		OUT 	3				; WINGS AT -1.5 DEG
		DB		WNGREC
		SEX 	R2				; RESET SP
;
		LBR 	DETDEC
;****************************************************************************
;
;       MANUAL WING CONTROL
;
;****************************************************************************
MANUAL: SEX     R3              ; SET R3 TO SP
		OUT 	1				; I/O GROUP
		DB		$30
		SEX 	R2				; RESET SP
;
		RLDI	RD,RESET		; POINT TO WING
		LDN 	RD				; GET WING
		PLO 	R1				; STASH
;****************************************************************************
;       SOFTWARE LIMIT SWITCHES
;****************************************************************************
		GLO 	R1				; GET COMMAND
		SMI 	WNGNEG			; - LIMIT
		BNF 	STOPC			; IF DF=1 OK
		GLO 	R1				; GET COMMAND
		SMI 	WNGPOS			; + LIMIT
		BDF 	LIMITC			; IF DF=1 LIMIT
;
		GLO 	R1				; GET COMMAND
;
		LSKP					; SKIP NEXT
;
LIMITC: LDI     WNGPOS          ; MAXIMUM + LIMIT
;
		LSKP					; SKIP NEXT
;
STOPC:  LDI     WNGNEG          ; MAXIMUM - LIMIT
;****************************************************************************
;       OUTPUT TO SERVO
;****************************************************************************
SERVOC: STR     R2              ; STACK IT
		OUT 	3				; OUT D/A CH 1
		DEC 	R2				; RESET STACK
;
		LBR 	DETDEC
;****************************************************************************
;
;       ACCEL CONTROLLER MODE
;
;****************************************************************************
ACC:    RLDI    RE,$0004        ; 4 BLINKS
		LDI 	ACCLIT			; ACC LIGHT
		PLO 	RC				; STORE
;
ACCBNK: GLO     RC              ; GET LIGHT
		STR 	R2				; STACK IT
		XRI 	$FF 			; INVERT BITS
		ANI 	ACCLIT			; MASK BITS
		PLO 	RC				; RESET LIGHT
		OUT 	6				; SEND STATUS
		DEC 	R2				; RESET STACK
;
		RLDI	RF,$1FFF		; COUNTER DELAY
DELAY2: NOP                     ; NO OPERATION
		DBNZ	RF,DELAY2		; DELAY LOOP
		DBNZ RE,ACCBNK			; BLINK
;
		GHI 	RC				; COMMAND
		SHL
		SHL
		SHL
		SHL
		STR 	R2
		OUT 	6
		DEC 	R2
;****************************************************************************
;
;       SET FIXED GAINS
;
;****************************************************************************
GAINER: SEX     RD              ; SET SP TO RD
;
		RLDI	RD,EGAIN

	IF		(VERSION EQ VER_1)

		LDI 	$33 			; 1.000 VOLTS

	MSG 	'VER 1 GAIN KP'
	ELSE

		LDI 	$66 			; 2.000 VOLTS

	MSG 	'VER 2 GAIN KP'
	ENDIF

		STXD
		LDI 	$00
		STR 	RD
;
		RLDI	RD,RGAIN
		LDI 	$4C 			; 1.500 VOLTS
		STXD
		LDI 	$00
		STR 	RD
;
		RLDI	RD,IGAIN
		LDI 	$33 			; 1.000 VOLTS
		STXD
		LDI 	$00
		STR 	RD
;
		RLDI	RD,SGAIN
		LDI 	$50 			; 1.568 VOLTS
		STXD
		LDI 	$00
		STR 	RD
;
		RLDI	RD,RESET
		LDI 	$66 			; 2.00 VOLTS
		STXD
		LDI 	$00
		STR 	RD
;****************************************************************************
;       SET VERSION 1 GAINS
;****************************************************************************
	IF		((VERSION GT VER_2) AND (VERSION LT VER_5))

		RLDI	R6,GAINLD		; SET OLD GAIN FUNCTION
		SEP 	R6				; CALL FUNCTION

	MSG 	'SET VERSION 1 GAINS FOR DESCENT'
	ENDIF
;****************************************************************************
ACCOK:  LBR     TLXACC          ; RUN CONTROLLER
;****************************************************************************
;
;       PROGRAMABLE CONTROLLER MODE
;
;****************************************************************************
PRG:    RLDI    RE,$0004        ; 4 BLINKS
		LDI 	PRGLIT			; PRG LIGHT
		PLO 	RC				; STORE
;
PRGBNK: GLO     RC              ; GET LIGHT
		STR 	R2				; STACK IT
		XRI 	$FF 			; INVERT BITS
		ANI 	PRGLIT			; MASK BITS
		PLO 	RC				; RESET LIGHT
		OUT 	6				; SEND STATUS
		DEC 	R2				; RESET STACK
;
		RLDI	RF,$1FFF		; COUNTER DELAY
DELAY3: NOP                     ; NO OPERATION
		DBNZ	RF,DELAY3		; DELAY LOOP
		DBNZ	RE,PRGBNK		; BLINK
;
		GHI 	RC				; COMMAND
		SHL
		SHL
		SHL
		SHL
		STR 	R2
		OUT 	6
		DEC 	R2
;
	IF		(VERSION GT VER_2)

PRGOK:	LBR 	PRGPAT			; RUN CONTROLLER

		MSG 	'PRGPAT PATCH'

	ELSE

PRGOK:	LBR 	TLXACC			; RUN CONTROLLER

	ENDIF
;****************************************************************************
;
;       ALTERNATE CONTROLLER MODE
;
;****************************************************************************
ALT:    RLDI    RE,$0004        ; 4 BLINKS
		LDI 	ALTLIT			; ALT LIGHT
		PLO 	RC				; STORE
;
ALTBNK: GLO     RC              ; GET LIGHT
		STR 	R2				; STACK IT
		XRI 	$FF 			; INVERT BITS
		ANI 	ALTLIT			; MASK BITS
		PLO 	RC				; RESET LIGHT
		OUT 	6				; SEND STATUS
		DEC 	R2				; RESET STACK
;
		RLDI	RF,$1FFF		; COUNTER DELAY
DELAY4: NOP                     ; NO OPERATION
		DBNZ	RF,DELAY4		; DELAY LOOP
		DBNZ	RE,ALTBNK		; BLINK
;
		GHI 	RC	;COMMAND
		SHL
		SHL
		SHL
		SHL
		STR 	R2
		OUT 	6
		DEC 	R2
;
		RLDI	RD,TLXALT
		LDN 	RD
		SMI 	$C4 			; NOP
		BZ		ALTOK
;
		RLDI	R0,$0000
		SEX 	R0
		SEP 	R0
;
ALTOK:  LBR     TLXALT          ; RUN CONTROLLER
;****************************************************************************
;
;       DETERMINE ALT DEVIATION
;
;****************************************************************************
DETDEC: RLDI    RD,UTIL
;****************************************************************************
;
;       THUMB WHEEL PRESET DECODE
;
;               PRESET=( (ONES/27) + (TENS/27)*10 +
;                 (HUNDS/27)*100 )*255/100
;
;       THE GROUND STATION TRANSMITS THE
;       PRESET ALTITUDE USING THUMB WHEEL
;       SWITCHES. AS WIRED, THE DIGITS ARE
;       INVERTED (9=0VOLTS,0=5VOLTS). IF
;       THIS IS CHANGED, THE XRI INSTRUCTION
;       IN THE CODE SHOULD BE COMMENTED OUT
;       AND THE CORRESPONDING NOP SHOULD
;       BE UN-COMMENTED.
;
;****************************************************************************
		SEX 	RD				; MATH STACK
		LDI 	$1B 			; LOW BYTE OF 27
		STXD					; STACK IT
		LDI 	$00 			; HIGH BYTE OF 27
		STR 	RD				; STACK IT
		RLDI	R7,$0000
		RLDI	R8,$0000
		RLDI	R9,$0000
;
		RLDI	R4,PREONE
		RLDI	R5,PRETEN
		RLDI	R1,PREHUN
;****************************************************************************
ONES:   RLDI    RB,PRESE1
		LDN 	RB
;       NOP
;       NOP
		XRI 	$FF
		LBZ 	TENS
		PLO 	RF
		LDI 	$00
		PHI 	RF
		PLO 	RE
		PHI 	RE
;
		RLDI	R6,DIVQ
		SEP 	R6
;
		GHI 	RF
		PHI 	R7
		GLO 	RF
		PLO 	R7
;
		STR 	R4
;****************************************************************************
TENS:   RLDI    RD,UTIL-1
		RLDI	RB,PRESE2
		LDN 	RB
;		NOP
;       NOP
		XRI 	$FF
		LBZ 	HUNDS
		PLO 	RF
		LDI 	$00
		PHI 	RF
		PLO 	RE
		PHI 	RE
;
		RLDI	R6,DIVQ
		SEP 	R6
;
		GLO 	RF
		STR 	R5
;
		RLDI	RD,MATH
		LDI 	$0A
		STXD
		LDI 	$00
		STR 	RD
;
		RLDI	R6,MULT
		SEP 	R6
;
		GHI 	RF
		PHI 	R8
		GLO 	RF
		PLO 	R8
;****************************************************************************
HUNDS:  RLDI    RD,UTIL-1
		RLDI	RB,PRESE3
		LDN 	RB
;       NOP
;       NOP
		XRI 	$FF
		BZ		SUMUP
		PLO 	RF
		LDI 	$00
		PHI 	RF
		PLO 	RE
		PHI 	RE
;
		RLDI	R6,DIVQ
		SEP 	R6
;
		GLO 	RF
		STR 	R1
;
		RLDI	RD,MATH
		LDI 	$64
		STXD
		LDI 	$00
		STR 	RD
;
		RLDI	R6,MULT
		SEP 	R6
;
		GHI 	RF
		PHI 	R9
		GLO 	RF
		PLO 	R9
;****************************************************************************
;       SUM UP DIGITS
;****************************************************************************
SUMUP:  RLDI    RD,UTIL
		LDI 	$00
		STXD
		STR 	RD
		INC 	RD
;
		GLO 	R7
		ADD
		STXD
		GHI 	R7
		ADC
		STR 	RD
		INC 	RD
;
		GLO 	R8
		ADD
		STXD
		GHI 	R8
		ADC
		STR 	RD
		INC 	RD
;
		GLO 	R9
		ADD
		STXD
		GHI 	R9
		ADC
		STR 	RD
		INC 	RD
;****************************************************************************
;       MULT SUM BY 255 BITS
;****************************************************************************
		RLDI	RD,UTIL-1
		LDI 	$FF
		PLO 	RF
		LDI 	$00
		PHI 	RF
		RLDI	R6,MULT
		SEP 	R6
;****************************************************************************
;       DIVIDE BY 100 BITS
;****************************************************************************
		RLDI	RD,UTIL
		LDI 	$64
		STXD
		LDI 	$00
		STR 	RD
		RLDI	R6,DIVQ
		SEP 	R6
;****************************************************************************
;       CHECK FOR MINIMUM PRESET OF 20 FT
;****************************************************************************
		GHI 	RF				; HIGH BYTE
		BNZ 	PREOK			; IF NON-ZERO OK
		GLO 	RF				; LOW BYTE
		SMI 	$33 			; 20 FT
		BDF 	PREOK			; DF=1 OK
		RLDI	RF,$0033		; PRESET=20 FT
;****************************************************************************
;       PUT PRESET ON STACK
;****************************************************************************
PREOK:  RLDI    RD,PRESET
		GLO 	RF
		STXD
		GHI 	RF
		STR 	RD
;****************************************************************************
;       SHIFT ALT STACK DOWN ONE ENTRY
;****************************************************************************
ALTSFC: RLDI    RA,ABOT+$02     ; BOTTOM OF STACK+2
		RLDI	RB,ABOT 		; BOTTOM STACK
		RLDI	RC,ABYTE-$02	; NUMBER BYTES-2
		SEX 	RA				; RA IS SP
;
ASHFTC: LDXA                    ; GET BYTE
		STR 	RB				; MOVE DOWN
		INC 	RB				; INCREMENT POINTER
;
		DBNZ	RC,ASHFTC		; LOOP FOR ALL BYTES
		INC 	RB				; POINT TO LOW BYTE
;****************************************************************************
;
;       A/D INPUT SECTION
;
;       A/D CHANNELS
;
;       CHANNEL 0 ALT RELIBILITY
;       CHANNEL 1 100 FT RANGE ALT
;       CHANNEL 2 1000 FT RANGE ALT
;       CHANNEL 3 FEEDBACK POT
;       CHANNEL 4 ACCELOERMETER
;
;****************************************************************************
ADSETC: RLDI    R2,INPUT        ; INPUT STACK
;
		RLDI	R4,ALTREL
		RLDI	R5,ALTLOW
		RLDI	R6,ALTHIH
		RLDI	R8,FEDPOT
		RLDI	R9,GLOADS
;
ADGETC: SEX     R3              ; MAKE PC SP
		OUT 	1				; SELECT I/O
		DB		$30
		OUT 	6				; CHANNEL MODE
		DB		$01
		OUT 	5				; RELIBILITY
		DB		$00
		SEX 	R2				; RESET SP
;
		BN1 	*				; WAIT
		INP 	4				; RELIBILITY
		STR 	R4				; STASH
;
		BN1 	*				; WAIT FOR FLAG
		INP 	4				; 100FT RANGE
		PLO 	RF				; STASH INPUT
		STR 	R5				; STASH
;
		BN1 	*				; WAIT
		INP 	4				; 500FT RANGE
		PLO 	R7				; STASH
		STR 	R6				; STASH
;
		BN1 	*				; WAIT
		INP 	4				; WING POSITION
		STR 	R8				; STASH
;
		BN1 	*				; WAIT
		INP 	3				; ACCELEROMETER
		STR 	R9				; STASH
;****************************************************************************
;       AUTO-RANGE ALT INPUTS
;****************************************************************************
AUTOC:	LDI 	$00 			; ZERO BYTE
		PHI 	RF				; HIGH BYTE OF ALT
;
		GLO 	RF				; GET 100FT RANGE
		SDI 	$FF 			; MAX INPUT
		LBNZ	STACKC			; IF NON-ZERO, LOW RANGE
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
STACKC: GLO     RF              ;
		STR 	RB				;
		DEC 	RB				;
		GHI 	RF				;
		STR 	RB				;
;****************************************************************************
;
;       AVERAGE ALT INPUTS
;
;****************************************************************************
;		DON'T AVERAGE IN MANUAL MODE
;****************************************************************************
		RLDI	RC,COMWRD
		LDN		RC
		ANI 	$07
		SMI 	MANBIT
		LBZ 	CALERC
;****************************************************************************
;       AVERAGE LAST 4 ALT SAMPLES
;****************************************************************************
		RLDI	RD,ATOP 		; TOP OF ALT STACK
		RLDI	RC,$0004		; NUMBER OF SAMPLES
		RLDI	RF,$0000		; ZERO SUM
		SEX 	RD				; SET STACK
;
AVGC:   GLO     RF              ; GET LOW BYTE
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
		DBNZ	RC,AVGC 		; ADD ALL WORDS
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
CALERC: RLDI    RD,PRESET       ; POINTER TO PRESET
		SEX 	RD				; SET STACK POINTER
;
		GLO 	RF				; GET LOW BYTE OF ALT
		SD						;
		PLO 	R9				;
;
		DEC 	RD				;
;
		GHI 	RF				; GET HIGH BYTE OF ALT
		SDB 					;
		PHI 	R9				;
;****************************************************************************
;
;       DETERMINE +/- 50 FT ALT DEVIATION
;
;****************************************************************************
		GHI 	R9				; CHECK SIGN OF ERROR
		SHL 					;
		LBDF	AHIGHC			;
;****************************************************************************
;       LOW ON PRESET
;****************************************************************************
		GHI 	R9				;
		LBNZ	MAXLC			;
;
		GLO 	R9				;
		ADI 	$80 			;
		LBDF	MAXLC			;
;
		LSKP					;
;
MAXLC:  LDI     $FF             ;
		LBR 	OUTC			;
;****************************************************************************
;       HIGH ON PRESET
;****************************************************************************
AHIGHC: GHI     R9              ;
		SMI 	$FF 			;
		LBNZ			MAXHC	;
;
		GLO 	R9				;

		SMI 	$80 			;
		LBNF	MAXHC			;
;
		LSKP					;
;
MAXHC:  LDI     $00
;****************************************************************************
;       OUTPUT ALT DEVIATION TO D/A CH 2
;****************************************************************************
OUTC:   XRI     $FF             ;
		STR 	R2				;
		SEX 	R2				;
		OUT 	4				;
		DEC 	R2				;
;****************************************************************************
;
;       RETURN TO COMMAND LINK
;
;****************************************************************************
       LBR      GRABER
;****************************************************************************
;
;       STASH VERSION NUMBER
;
;****************************************************************************
	IF		(VERSION GT 3)

		ORG 	$03F0

		DB		'VERSION'
		DB		VERSION
		DD		_DATE

	ENDIF
;****************************************************************************
;
;
;       TLX ACCEL CONTOLLER SOFTWARE
;
;
;****************************************************************************
	ORG     $0400           ; EPROM NUMBER ONE
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
ABORTP: LDI     $00     ; HALT FLAG
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
;
;       COMMON CODE AREA
;
;****************************************************************************
		ORG 	$0700
;****************************************************************************
;
;       ADDRESS FOR COMMAND JUMP
;
;****************************************************************************
LINK:   LBR     TMWAIT
;****************************************************************************
;
; MATH SUBROUTINES
;
;****************************************************************************
;
;       UN-SIGNED RIGHT SHIFT OF RF
;
;       NUMBER OF SHIFTS PASSED INLINE
;       THROUGH THE PROGRAM COUNTER
;
;       CALLED BY:
;               RLDI R6,USHR
;               SEP R6;,$SHIFTS
;
;       USES REGISTER RD.0
;
;****************************************************************************
USHR:   LDA     R3              ; GET NUMBER OF SHIFTS
		PLO 	RD				; SET COUNTER
;
01$:    GHI     RF              ; GET HIGH BYTE
		SHR 					; RIGHT SHIFT
		PHI 	RF				; RESET BYTE
;
		GLO 	RF				; GET LOW BYTE
		SHRC					; RIGHT SHIFT WITH CARRY
		PLO 	RF				; RESET LOW BYTE
;
		DEC 	RD				; DECREMENT COUNTER
		GLO 	RD				; GET COUNTER
		BNZ 	01$ 			; SHIFT TILL ZERO
;
		SEP 	R3				; RESET PROGRAM COUNTER
;****************************************************************************
;
;       ARITHMETIC RIGHT SHIFT OF RF
;
;       NUMBER OF SHIFTS PASSED INLINE
;       THROUGH THE PROGRAM COUNTER
;
;       CALLED BY:
;               RLDI R6,ASHR
;               SEP R6;,$SHIFTS
;
;       USES REGISTER RD.0
;
;****************************************************************************
ASHR:   LDA     R3              ; GET NUMBER OF SHIFTS
		PLO 	RD				; SET COUNTER
;
01$:    GHI     RF              ; GET HIGH BYTE
		SHL 					; PUT SIGH INTO DF
		GHI 	RF				; HIGH BYTE AGAIN
		SHRC					; RIGHT SHIFT WITH CARRY
		PHI 	RF				; RESET HIGH BYTE
;
		GLO 	RF				; GET LOW BYTE
		SHRC					; RIGHT SHIFT WITH CARRY
		PLO 	RF				; RESET LOW BYTE
;
		DEC 	RD				; DECREMENT COUNTER
		GLO 	RD				; GET COUNTER
		BNZ 	01$ 			; SHIFT TILL ZERO
;
		SEP 	R3				; RESET PROGRAM COUNTER
;****************************************************************************
;
;       ARITHMETIC RIGHT SHIFT OF R0,R1
;
;       NUMBER OF SHIFTS PASSED IN RD.0
;
;       CALLED BY:
;               RLDI RD,NUMSFT
;               LDN RD
;               PLO RD
;               RLDI R6,SCALE
;               SEP R6
;
;       USES REGISTER RD.0
;
;****************************************************************************
SCALE:  GLO     RD              ; GET NUMBER SHIFTS
		BZ		02$ 			; EXIT IF ZERO
;****************************************************************************
01$:    GHI     R0              ; GET MSB
		SHL 					; PUT SIGN IN DF
		GHI 	R0				; GET MSB AGAIN
		SHRC					; RIGHT SHIFT WITH DF
		PHI 	R0				; RESET MSB
;
		GLO 	R0
		SHRC
		PLO 	R0
;
		GHI 	R1
		SHRC
		PHI 	R1
;
		GLO 	R1				; GET LSB
		SHRC					; RIGHT SHIFT WITH DF
		PLO 	R1				; RESET LSB
;
		DEC 	RD				; DECREMENT COUNTER
		GLO 	RD				; GET COUNTER
		BNZ 	01$ 			; SHIFT TILL ZERO
;
02$:    SEP     R3
;****************************************************************************
;
;       ADDITION ROUTINE
;       32 BIT 2'S COMPLEMENT ADDITION
;       R0,R1 = R0,R1 +RE,RF
;
;       CALLED BY :
;
;               RLDI R6,ADDER
;               SEX R2
;               SEP R6
;
;****************************************************************************
ADDER:  GLO     R1              ; GET LSB
		STR 	R2				; PUT ON STACK
		GLO 	RF				; GET LOW BYTE
		ADD 					; ADD LOW BYTES
		PLO 	R1				; RESET LSB
;
		GHI 	R1
		STR 	R2
		GHI 	RF
		ADC
		PHI 	R1
;
		GLO 	R0
		STR 	R2
		GLO 	RE
		ADC
		PLO 	R0
;
		GHI 	R0				; GET MSB
		STR 	R2				; PUT ON STACK
		GHI 	RE				; GET HIGH BYTE
		ADC 					; ADD BYTES WITH CARRY
		PHI 	R0				; RESET MSB
;
		SEP 	R3				; RESET PROGRAM COUNTER
;****************************************************************************
;
;       16 BIT SIGNED MULT ROUTINE
;
;       RF * M(RD) = RE (HIGH) AND RF (LOW)
;
;****************************************************************************
MULT:   GHI     RF
		XOR
		ANI 	$80
		PHI 	RC
		LDI 	$10
		PLO 	RC
		LDI 	$00
		PHI 	RE
		PLO 	RE
;****************************************************************************
MPL:    DEC     RC
		GHI 	RF
		SHR
		PHI 	RF
		GLO 	RF
		SHRC
		PLO 	RF
		GHI 	RE
		BNF 	MPB
		INC 	RD
		GLO 	RC
		BNZ 	MPA
		GLO 	RE
		SM
		PLO 	RE
		DEC 	RD
		GHI 	RE
		SMB
		PHI 	RE
		GHI 	RC
;****************************************************************************
MPB:    SHL
		BR		MPS+$03
MPA:    GLO     RE
		ADD
		PLO 	RE
		DEC 	RD
		GHI 	RE
		ADC
		PHI 	RE
;****************************************************************************
MPS:    LSDF
		LDX
		SHL
MSP3:   GHI     RE
		SHRC
		PHI 	RE
		GLO 	RE
		SHRC
		PLO 	RE
		BNF 	MT
		GHI 	RF
		ORI 	$80
		PHI 	RF
;****************************************************************************
MT:     GLO     RC
		BNZ 	MPL
;****************************************************************************
MPX:    GHI     RE
		SHL
		BNF 	MEXIT
		BNZ 	MEXIT
		SHR
		PHI 	RE
;****************************************************************************
MEXIT:  SEP     R3
;****************************************************************************
;
;       DIVISION ROUTINE
;
;       RF = RE,RF/M(RD)
;
;****************************************************************************
DIVQ:   GHI     RE              ;
		SHL 					;
		LDI 	$90 			;
		LSNF					;
		LDI 	$50 			;
		PLO 	RC				;
DVL:    GLO     RE              ;
		SHL 					;
		PLO 	RE				;
		GHI 	RE				;
		SHLC					;
		PHI 	RE				;
;
		GLO 	RF				;
		SHL 					;
		PLO 	RF				;
		GHI 	RF				;
		SHLC					;
		PHI 	RF				;
;
		BNF 	DSEX
;
		INC 	RE				;
;
DSEX:   SEX     RD              ;
		GLO 	RC				;
		XOR 					;
		SHL 					;
		INC 	RD				;
		GLO 	RE				;
		BNF 	DSA 			;
		SM						;
		PLO 	RE				;
		DEC 	RD				;
		GHI 	RE				;
		SMB 					;
		BR		DSM 			;
;
DSA:    ADD                     ;
		PLO 	RE				;
		DEC 	RD				;
		GHI 	RE				;
		ADC 					;
DSM:    PHI     RE              ;
		DEC 	RC				;
		GLO 	RC				;
		ANI 	$7F 			;
		BNF 	DCR
;
		INC 	RF				;
		ORI 	$80 			;
DCR:    PLO     RC              ;
		ANI 	$3F 			;
		BNZ 	DVL 			;
		BDF 	DVR 			;
		SKP 					;
;
DVC:    INC     RF              ;
		XOR 					;
		SHL 					;
		INC 	RD				;
		GLO 	RE				;
		BDF 	DVM 			;
		ADD 					;
		PLO 	RE				;
		DEC 	RD				;
		GHI 	RE				;
		ADC 					;
		BR		DVR-$01
;
DVM:    SM                      ;
		PLO 	RE				;
		DEC 	RD				;
		GHI RE					;
		SMB 					;
		PHI 	RE				;
;
DVR:    GHI     RE              ;
		BNZ 	DCR1			;
		GLO 	RE				;
		BZ		DVN 			;
DCR1:   GLO     RC              ;
		SHL 					;
		SHL 					;
		GHI 	RE				;
		LSDF					;
		XRI 	$80 			;
		ADI 	$80 			;
		BNF 	DVC 			;
;
DVN:	LDX 					;
		SHL 					;
		BNF 	DVX 			;
		PLO 	RF				;
		GHI 	RF				;
		XRI 	$FF 			;
		PHI 	RF				;
		INC 	RF				;
;
		ADI 	$00 			; CLEAR DF
;
DVX:	SEP 	R3				;RETURN
;****************************************************************************
;
;       END OF 2K EPROM $0000-$07FF
;
;****************************************************************************
;
; START OF ADDRESS SPACE IN 2ND EPROM
;
;****************************************************************************
		ORG 	$0800
;****************************************************************************
;
;
; ALTERNATE PROGRAM
;
;
;****************************************************************************
TLXALT: NOP 					; NO OPERATION

	IF		((VERSION GT VER_2) AND (VERSION LT VER_5))

		LBR 	LINK			; RETURN TO COMMAND LINK

	ELSE

		LBR 	ALTCON			; CALL ALTERNATE PROGRAM WITH PROGRAMMABLE
								; GAINS

	ENDIF
;****************************************************************************
;
;       SETUP COUNTER MODE 2
;
;       1805 DOWN COUNTER WITH DOWN COUNT ON
;       EACH TRANSITION OF EF2 (DATA VALID)
;       THE INITIAL COUNTER VALUE IS SET TO
;       UNDERFLOW EVERY 64 DEC COUNTS
;       (0.25 SECONDS) AND TO FLIP/FLOP THE
;       Q TOGGLE.
;       THE COUNTER VALUE IS COMPUTED BY:
;
;       2048 BITS/SEC FROM REMTRON DECODER
;       256 FRAMES/SEC FROM DECODER
;       0.25 SEC / 256 SEC/FRAME = 64
;
;****************************************************************************
	IF		(VERSION GT VER_2)

OKSLOW: STPC                    ; STOP COUNTER
		LDI 	OKLITE			; 64 COUNTS
		LDC 					; LOAD COUNTER
		ETQ 					; ENABLE Q
		SCM2					; COUNTER MODE 2
		SEP 	R3				; RETURN

	ENDIF
;****************************************************************************
;
;       SETUP COUNTER MODE 2
;
;       1805 DOWN COUNTER WITH DOWN COUNT ON
;       EACH TRANSITION OF EF2 (DATA VALID)
;       THE INITIAL COUNTER VALUE IS SET TO
;       UNDERFLOW EVERY 64 DEC COUNTS
;       (0.25 SECONDS) AND TO FLIP/FLOP THE
;       Q TOGGLE.
;       THE COUNTER VALUE IS COMPUTED BY:
;
;       2048 BITS/SEC FROM REMTRON DECODER
;       256 FRAMES/SEC FROM DECODER
;       0.25 SEC / 256 SEC/FRAME = 64
;
;****************************************************************************
	IF		(VERSION GT VER_2)

OKFAST: STPC                    ; STOP COUNTER
		LDI 	OKLITE			; 64 COUNTS
		SHR 					; 32 COUNTS
		SHR 					; 16 COUNTS
		LDC 					; LOAD COUNTER
		ETQ 					; ENABLE Q
		SCM2					; COUNTER MODE 2
		SEP 	R3				; RETURN

	ENDIF
;****************************************************************************
;
;       SETUP PIO PORTS FOR COMMAND/TM LINK
;
;       I/O GROUP NUMBER 8
;       CHANNEL A INPUT (DATA WORD)
;       CHANNEL B BIT PROGRAMABLE
;       LOW NIBBLE INPUT (ADDRESS)
;       HIGH NIBBLE OUTPUT (STATUS)
;
;****************************************************************************
	IF		(VERSION GT VER_2)

CMWAIT: SEX     R3              ; SET PC TO SP
		OUT 	1				; I/O GROUP 8
		DB		$08
		OUT 	2
		DB		$0F
		OUT 	2
		DB		$F7
		OUT 	2
		DB		$F0
;
		RLDI	R2,INPUT		; INPUT STACK
		SEX 	R2				; SET SP
		LDI 	RECLIT			; RECOVERY LIGHT
		PLO 	RC				; STORE WORD
;****************************************************************************
01$:    B2      01$             ; WAIT FOR DATA VALID
;****************************************************************************
		INP 	6				; INPUT ADDRESS
		IRX 					; INCREMENT SP
		INP 	4				; INPUT ADDRESS
		DEC 	R2				; RESET SP
		LDX 					; GET ADDRESS
		ANI 	$0F 			; MASK BITS
;
		SMI 	$09 			; CHANNEL NINE
		BNZ 	01$ 			; IF NOT NEXT INPUT
;****************************************************************************
;       CHECK REC/FLY BIT FOR FLY
;****************************************************************************
		IRX 					; INCREMENT TO DATA
		LDX 					; GET DATA
		PHI 	RC				; STORE WORD
		DEC 	R2				; RESET SP
;
		GLO 	RC				; GET LIGHT
		STR 	R2				; STACK IT
		XRI 	$FF 			; INVERT BITS
		ANI 	RECLIT			; ONLY REC LIGHT
		PLO 	RC				; RESET LIGHT
		OUT 	6				; SEND STATUS
		DEC 	R2				; RESET STACK
;
		RLDI	RF,$3FFF		; COUNTER DELAY
02$:    NOP                     ; NO OPERATION
		DBNZ	RF,02$			; DELAY LOOP
;
		GHI 	RC				; GET WORD
		ANI 	RECBIT			; MASK REC BIT
		XRI		$01				; INVERT REC BIT
		BZ		01$				; IF NOT REC, WAIT
;****************************************************************************
		LBR 	LINK			; RETURN TO COMMAND LINK

	ENDIF
;****************************************************************************
	IF		(VERSION GT VER_3)

		ORG $0900

	ENDIF
;****************************************************************************
;
;       SET FIXED GAINS (VERSION 2 GAINS)
;
;****************************************************************************
	IF		(VERSION GT VER_2)

GAINEW: RLDI    RD,WFLAG        ; WING POSITION
		SEX 	RD				; POINT STACK AT WING POSITION
		LDN 	RD				; GET WING POSITION
		SMI 	WNGRL			; SUB -4.5 DEGREES
		BNF 	01$ 			; DON'T ENGAGE NEW GAINS IF LESS THAN
		LDN 	RD				; GET WING POSITION
		SMI 	WNGRH			; SUB -3.5 DEGREES
		BDF 	01$ 			; DON'T ENGAGE NEW GAINS IF GREATER THAN
;****************************************************************************
		RLDI	RD,PFLAG		; PRESET FLAG
		LDI 	$FF 			; SET FLAG
		STR 	RD				; STORE FLAG

		SEX 	RD				; SET SP TO RD
;
		RLDI	RD,EGAIN
		LDI 	$66 			; 2.000 VOLTS
		STXD
		LDI 	$00
		STR 	RD
;
		RLDI	RD,RGAIN
		LDI 	$4C 			; 1.500 VOLTS
		STXD
		LDI 	$00
		STR 	RD
;
		RLDI	RD,IGAIN
		LDI 	$33 			; 1.000 VOLTS
		STXD
		LDI 	$00
		STR 	RD
;
		RLDI	RD,SGAIN
		LDI 	$50 			; 1.568 VOLTS
		STXD
		LDI 	$00
		STR 	RD
;
		RLDI	RD,RESET

	IF		(VERSION GT 3)

		LDI 	$66 			; 2.00 VOLTS

	ELSE

		LDI 	$73 			; 2.25 VOLTS

	ENDIF

		STXD
		LDI 	$00
		STR 	RD

		RLDI	RC,EGAIN2		; POINT TO GAIN 2
		RLDI	RD,EGAIN		; POINT TO GAIN
		LDN 	RC				; GET GAIN 2
		STXD					; STORE AND DECREMENT STACK
		LDI 	$00 			; ZERO HIGH BYTE
		STR 	RD				; STACK IT

01$:	SEP 	R3				; RETURN

	ENDIF
;****************************************************************************
;
;       SET FIXED GAINS  (VERSION 1 GAINS)
;
;****************************************************************************
	IF		(VERSION GT VER_2)

GAINLD: SEX     RD              ; SET SP TO RD
;
		RLDI	RD,EGAIN
		LDI 	$33 			; 1.000 VOLTS
		STXD
		LDI 	$00
		STR 	RD
;
		RLDI	RD,RGAIN
		LDI 	$4C 			; 1.500 VOLTS
		STXD
		LDI 	$00
		STR 	RD
;
		RLDI	RD,IGAIN
		LDI 	$33 			; 1.000
		STXD
		LDI 	$00
		STR 	RD
;
		RLDI	RD,SGAIN
		LDI 	$50 			; 1.568 VOLTS
		STXD
		LDI 	$00
		STR 	RD
;
		RLDI	RD,SGAIN
		LDI 	$50 			; 1.568 VOLTS
		STXD
		LDI 	$00
		STR 	RD
;
		RLDI	RD,RESET

	IF		(VERSION GT 3)

		LDI 	$66 			; 2.00 VOLTS

	ELSE

		LDI 	$73 			; 2.25 VOLTS

	ENDIF

		STXD
		LDI 	$00
		STR 	RD

		RLDI	RD,EGAIN2
		LDI 	$66 			; 2.000 VOLTS
		STXD
		LDI 	$00
		STR 	RD

		SEP 	R3				; RETURN

	ENDIF
;****************************************************************************
;       RESET TO OLD GAINS IF TOO HIGH
;****************************************************************************
	IF		(VERSION GT VER_3)

GAINRT:
		RLDI RD,PFLAG	; PRESET FLAG
		LDN 	RD		; LOAD FLAG
		BZ		01$ 	; IF FLAG RESET SKIP RESETTING SLOW OK

		LDI 	$00 	; RESET FLAG
		STR 	RD		; STORE FLAG

		RLDI R6,GAINLD	; SET OLD GAIN FUNCTION
		SEP 	R6		; CALL FUNCTION

		RLDI R6,OKSLOW	; SET STANDARD OK LITE
		SEP 	R6		; CALL FUNCTION

01$:	LBR 	OUT 	; RETURN TO MAIN CODE

	ENDIF
;****************************************************************************
;       PROGRAMABLE CONTROLLER PATCH AREA
;****************************************************************************
	IF		(VERSION GT VER_2)

PRGPAT: SEX     RD              ; SET SP TO RD

		RLDI	RC,SGAIN		; POINT TO SCALE FACTOR
		RLDI	RD,EGAIN2		; POINT TO GAIN 2
		LDN 	RC				; GET SCALE
		STXD					; STORE AND DECREMENT STACK
		LDI 	$00 			; ZERO HIGH BYTE
		STR 	RD				; STACK IT

		RLDI	RD,SGAIN
		LDI 	$50 			; 1.568 VOLTS
		STXD
		LDI 	$00
		STR 	RD

		LBR TLXACC				; RUN CONTROLLER

	ENDIF
;****************************************************************************
	IF		(VERSION GT VER_3)

	ORG 	$0A00

	ENDIF
;****************************************************************************
;       TEST FOR ACTIVE DECODER
;       IF DECODER NOT ACTIVE, OR CPU HAS LOCKED UP
;       THE OK LITE WILL NOT BLINK
;****************************************************************************
	IF      (VERSION GT VER_2)

RTD1:

	ENDIF

	IF      (VERSION GT VER_3)

		RLDI	RD,COMCHK	   ; POINT TO COMMAND WORD LAST
		LDI		RECBIT		   ; LOAD RECOVERY BIT
		STR 	RD			   ; STORE AS LAST COMMAND
		RLDI	RD,COMCNT	   ; POINT TO COMMAND COUNT
		LDI 	$00			   ; ZERO COUNT
		STR 	RD			   ; SET COUNTER

	ENDIF

	IF      (VERSION GT VER_2)

		RLDI    RC,$FF          ; NUMBER COUNTS
01$:	B2		01$				; WAIT FOR DATA VALID
		DBNZ    RC,01$          ; LOOP FOR ALL COUNTS ( APPROXIMATELY 1 SECOND )
		LBR 	OKSET			; RETURN TO MAIN CODE STREAM

	ENDIF
;****************************************************************************
;       COMMAND MODE PATCH AREA
;****************************************************************************
	IF      (VERSION GT VER_3)

CMDPAT:
		RLDI    RD,COMCHK       ; POINT TO COMMAND WORD LAST
		SEX 	RD				; MAKE RD STACK POINTER
		GHI 	RC				; GET CURRENT COMMAND
		SM						; SUBTRACK LAST COMMAND FROM CURRENT
		BZ		01$				; IF COMMANDS SAME CHECK COUNT

		GHI 	RC				; GET CURRENT COMMAND
		STR 	RD				; UPDATE LAST COMMAND
		RLDI	RD,COMCNT		; POINT TO COMMAND COUNT
		LDI 	$00				; ZERO COUNT
		STR 	RD				; SET COUNTER
		LBNZ    PULLIT          ; RECOVERY MODE STAYS IN EFFECT

01$:    RLDI    RD,COMCNT       ; GET COMMAND COUNT
		LDN 	RD				; LOAD COUNT
		ADI 	$01				; ADD 1 TO COUNT
		STR 	RD				; UPDATE COUNT
		SMI 	$03				; FOUR VALID COMMANDS
		LBNZ    PULLIT          ; RECOVERY MODE STAYS IN EFFECT

	ENDIF

	IF      (VERSION GT VER_4)

		LDI     $02                     ; SET COUNT TO ALLOW MAUNAL
		STR     RD                      ; UPDATE COUNT

	ENDIF

	IF      (VERSION GT VER_3)

		LBR             ECHO            ; EXECUTE COMMAND

	ENDIF
;****************************************************************************
;       HALT MODE PATCH AREA
;****************************************************************************
	IF      (VERSION GT VER_3)

HALTPAT:
		RLDI    RF,$0167        ; DELAY COUNTER

01$:	BN3 	02$
		BN3 	02$
		BN3 	02$
		BN3 	02$

		BN3 	02$
		BN3 	02$
		BN3 	02$
		BN3 	02$

		DBNZ	RF,01$		   ; DECREMENT DELAY COUNT
		LBR 	TLXFLY		   ; CONTINUE WITH FLY MODE
;****************************************************************************
;       JUMP TO COMMAND LINK
;****************************************************************************
02$:	LBR	   ABORTP		   ; ABORT PROCESSOR

	ENDIF
;****************************************************************************
;
;       END OF PATCH AREA
;
;****************************************************************************
	IF      (VERSION GT VER_3)
	ORG     $0B00
	ENDIF
;****************************************************************************
;
;       TEST ROUTINE
;
;       IF PRESET ALTITUDE SET LESS THAN
;       900 FEET RUN ALTERNATE CONTROLLER
;       PROGRAM. OTHERWISW USE PRESET FOR
;       TEST CODE.
;
;****************************************************************************
TEST:   RLDI    RD,PREHUN
		LDN 	RD
		SMI 	$09
		LBNZ	ALTCON
;****************************************************************************
;
; READ TENS DIGIT FOR TEST CODE
;
;****************************************************************************
		RLDI	RD,PRETEN
		LDN 	RD
		LBZ 	TEST0
;
		SMI 	$01
		LBZ 	TEST1
;
		SMI 	$01
		LBZ 	TEST2
;
		SMI 	$01
		LBZ 	TEST3
;
		SMI 	$01
		LBZ 	TEST4
;
		SMI 	$01
		LBZ 	TEST5
;
		SMI 	$01
		LBZ 	TEST6
;
		SMI 	$01
		LBZ 	TEST7
;
		SMI 	$01
		LBZ 	TEST8
;
		LBR 	TEST9
;****************************************************************************
; TEST ROUTINE JUMP TABLE
;****************************************************************************
TEST0:	NOP
		NOP
		NOP
TEST1:	NOP
		NOP
		NOP
TEST2:	NOP
		NOP
		NOP
TEST3:	NOP
		NOP
		NOP
TEST4:	NOP
		NOP
		NOP
TEST5:	NOP
		NOP
		NOP
TEST6:	NOP
		NOP
		NOP
TEST7:	NOP
		NOP
		NOP
TEST8:	NOP
		NOP
		NOP
TEST9:	NOP
		NOP
		NOP
;****************************************************************************
;
; RETURN TO MAIN COMMAND LINK LOOP
;
;****************************************************************************
TESTX:  NOP
		NOP
		NOP
;
		LBR 	GRABER
;****************************************************************************
;
;       STASH VERSION NUMBER
;
;****************************************************************************
	IF		(VERSION GT 3)

		ORG 	$0BF0

		DB		'VERSION'
		DB		VERSION
		DD		_DATE

	ENDIF
;****************************************************************************
;
; ALTERNATE CONTROLLER
;
;****************************************************************************
	ORG     $0C00
;****************************************************************************
ALTCON: NOP 			; NO OPERATION

	IF		(VERSION GT VER_5)

	MSG 	'INTERFACE TO ALTERNATE PROGRAM'

		LBR 	LINK			; RETURN TO COMMAND LINK

	ENDIF
;****************************************************************************
	END

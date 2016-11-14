	LIST
	TITLE	LOW LEVEL OPERATING SYSTEM
	SUBTTL	(C)1984,85,86,87,88,89 HAYES INTERNATION CORPORATION
	SECTION TLX,$0000
;*************************************
;
; LOW LEVEL OPERATING SYSTEM
;
; PRODUCTION VERSION 1.0/07 JUL84
;
; WRITTEN BY DENTON MARLOWE
;
; COPYRIGHT HAYES INTERNATIONAL CORPORATION
;
;*************************************
;
; REVISION DATE
;
; SEPTEMBER 14 1989
;
; CHANGED PROPORTIONAL GAIN FROM 1 VOLT TO 2 VOLTS
;
; JANUARY 31 1989
;
; PRODUCTION VERSION
;
; THIS RELEASE REPLACES THE SOFTWARE INSTALLED IN THE PROTOTYPE TARGETS
; REVISIONS ARE IN THE FIXED GAIN SETTINGS AND ARE NOTED
;
;*************************************
;
; EQUATES
;
;*************************************
	ORG $0000       ;EPROM NUMBER ONE
;
RAM	EQU	$4000       ;CPU CONFIGURATION
;*************************************
; ACCEL CONTROLLER ADDRESS
;*************************************
; TLXACC	EQU	$0400    ;(EPROM ONE)
;*************************************
; ALTERNATE CONTROLLER ADDRESS
;*************************************
; TLXALT	EQU	$0800    ;(EPROM TWO
;*************************************
;
; DATA STACK EQUATES
;
;*************************************
ATOP	EQU	RAM+$01FF  ;TOP OF ALT STACK
ABOT	EQU	RAM+$01E0  ;BOTTOM OF ALT STACK
ABYTE	EQU	$0020     ;NUMBER BYTES
;
CTOP	EQU	RAM+$02FF  ;TOP OF ACCEL STACK
CBOT	EQU	RAM+$02E0  ;BOTTOM OF ACCEL STACK
CBYTE	EQU	$0020     ;NUMBER BYTES
CBYTE2	EQU	$0010    ;NUMBER WORDS
;
COUNT	EQU	RAM+$03FF ;LOOP COUNTER
;
ALTREL	EQU	RAM+$037F        ;RELIBILITY
RELFLG	EQU	RAM+$037D        ;REL FLAG
RELCNT	EQU	RAM+$037B        ;REL COUNTER
;
ALTLOW	EQU	RAM+$036F        ;100FT ALT
ALTHIH	EQU	RAM+$035F        ;1000FT ALT
FEDPOT	EQU	RAM+$034F        ;FEEDBACK POT
GLOADS	EQU	RAM+$033F        ;ACCEL
;
MATH	EQU	RAM+$043F  ;MATH STACK
INPUT	EQU	RAM+$047F ;INPUT STACK
UTIL	EQU	RAM+$04FF  ;UTILLITY STACK
;
PRESET	EQU	RAM+$050F        ;PRESET ALT
PRESE1	EQU	RAM+$050B        ;PRESET ONE
PRESE2	EQU	RAM+$0507        ;PRESET TEN
PRESE3	EQU	RAM+$0503        ;PRESET 100
;
EGAIN	EQU	RAM+$051F ;PROPORTIONAL GAIN
RGAIN	EQU	RAM+$052F ;DERIVATIVE GAIN
IGAIN	EQU	RAM+$053F ;INTEGRAL GAIN
SGAIN	EQU	RAM+$054F ;SCALE FACTOR
RESET	EQU	RAM+$055F ;MANUAL RESET
;*************************************
; TEST REGISTERS
;*************************************
PREONE	EQU	RAM+$063F        ;ONE DIGIT
PRETEN	EQU	RAM+$063B        ;TEN DIGIT
PREHUN	EQU	RAM+$0637        ;HUN DIGIT
;*************************************
; STATUS LIGHTS AND BITS EQUATES
;*************************************
MANLIT	EQU	$0000    ;MANUAL CONTROL LIGHT
RECLIT	EQU	$0010    ;RECOVERY LIGHT
PRGLIT	EQU	$0020    ;PRG CONTROLLER LIGHT
ALTLIT	EQU	$0040    ;ALT PROGRAM LIGHT
ACCLIT	EQU	$0060    ;ACC CONTROLLER LIGHTS
;*************************************
ACCBIT	EQU	$0000    ;ACC BITS
RECBIT	EQU	$0001    ;RECOVERY BIT
PRGBIT	EQU	$0002    ;PRG BIT
ALTBIT	EQU	$0004    ;ALT BITS
MANBIT	EQU	$0006    ;MANUAL BIT
;*************************************
; OK LIGHT COUNTER
;*************************************
OKLITE	EQU	$40
;*************************************
; COMMAND WORD
;*************************************
COMWRD	EQU	RAM+$06FF        ;COMMAND WORD
TIMER	EQU	RAM+$067F         ;TIMER STACK
;*************************************
;
; POWER ON RESET ENTRY
;
;*************************************
        DIS		;DISABLE INTERRUPTS
	DB	$00
;*************************************
; SET WINGS TO RECOVERY POSITION
;*************************************
        OUT 1		;I/O GROUP 30
	DB	$30
        OUT 3		;RECOVERY 
	DB	WNGREC
;*************************************
;
; SETUP COUNTER MODE 2
;
; 1805 DOWN COUNTER WITH DOWN COUNT ON
; EACH TRANSITION OF EF2 (DATA VALID)
; THE INITIAL COUNTER VALUE IS SET TO
; UNDERFLOW EVERY 64 DEC COUNTS
; (0.25 SECONDS) AND TO FLIP/FLOP THE
; Q TOGGLE.
; THE COUNTER VALUE IS COMPUTED BY:
;
; 2048 BITS/SEC FROM REMTRON DECODER
; 256 FRAMES/SEC FROM DECODER
; 0.25 SEC / 256 SEC/FRAME = 64
;
;*************************************
        STPC    	;STOP COUNTER
        LDI OKLITE 	;64 COUNTS
        LDC     	;LOAD COUNTER
        ETQ     	;ENABLE Q
        SCM2    	;COUNTER MODE 2
;*************************************
; SETUP PROGRAM COUNTER IN R3
;*************************************
TMINIT: RLDI R3,TMWAIT
        SEP R3
;*************************************
;
; COMMAND LINK SOFTWARE
;
;*************************************
;
; WAIT FOR RECOVERY/FLY SWITCH TO REC
; BLINK RECOVERY LIGHT DURING WAIT
;
;*************************************
;*************************************
;
; SETUP PIO PORTS FOR COMMAND/TM LINK
;
; I/O GROUP NUMBER 8
; CHANNEL A INPUT (DATA WORD)
; CHANNEL B BIT PROGRAMABLE
;  LOW NIBBLE INPUT (ADDRESS)
;  HIGH NIBBLE OUTPUT (STATUS)
;
;*************************************
TMWAIT: SEX R3  	;SET PC TO SP
        OUT 1		;I/O GROUP 8
	DB	$08
        OUT 2
	DB	$0F
        OUT 2
	DB	$F7
        OUT 2
	DB	$F0
;
        RLDI R2,INPUT	;INPUT STACK
        SEX R2  	;SET SP
        LDI RECLIT	;RECOVERY LIGHT
        PLO RC		;STORE WORD
;*************************************
VAL:    B2 VAL		;WAIT FOR DATA VALID
;*************************************
        INP 6   	;INPUT ADDRESS
        IRX     	;INCREMENT SP
        INP 4   	;INPUT ADDRESS
        DEC R2  	;RESET SP
        LDX     	;GET ADDRESS
        ANI $0F 	;MASK BITS
;
        SMI $09 	;CHANNEL NINE
        BNZ VAL         ;IF NOT NEXT INPUT
;*************************************
; CHECK REC/FLY BIT FOR REC
;*************************************
        IRX     	;INCREMENT TO DATA
        LDX     	;GET DATA
        PHI RC  	;STORE WORD
        DEC R2  	;RESET SP
;
        GLO RC  	;GET LIGHT
        STR R2  	;STACK IT
        XRI $FF 	;INVERT BITS
        ANI RECLIT 	;ONLY REC LIGHT
        PLO RC  	;RESET LIGHT
        OUT 6   	;SEND STATUS
        DEC R2  	;RESET STACK
;
        RLDI RF,$3FFF   ;COUNTER DELAY
DELAY1: NOP		;NO OPERATION
        DBNZ RF,DELAY1	;DELAY LOOP
;
        GHI RC  	;GET WORD
        ANI RECBIT 	;MASK REC BIT
        BZ VAL  	;IF NOT REC, WAIT
;*************************************
;
; COMMAND LINK GRABER ROUTINE
;
;*************************************
GRABER: SEX R3  	;SET PC TO SP
        OUT 1		;I/O GROUP 8
	DB	$08
        RLDI R2,INPUT	;INPUT STACK
        SEX R2		;SET SP
;*************************************
; INITIALIZE CHANNEL NUMBER TO ONE
;*************************************
        RLDI R7,$0001
;*************************************
; WAIT ON DATA VALID
;*************************************
VALID:  B2 VALID
;*************************************
; INPUT BYTES FROM 1851 INPUT PORTS
;*************************************
        INP 6   	;INPUT ADDRESS
        IRX     	;INCREMENT SP
        INP 4   	;INPUT DATA
        DEC R2  	;RESET SP
        LDX     	;GET ADDRESS
        ANI $0F 	;MASK BITS
        STR R2  	;REPLACE ON STACK
;*************************************
; CHECK FOR CORRECT ADDRESS
;*************************************
        GLO R7  ;GET CHANNEL
        SM      ;COMPARE INPUT
        BNZ VALID
                ;IF NOT SAME, NEXT
;*************************************
; RETREIVE DATA
;*************************************
        IRX     ;INCREMENT TO DATA
        LDX     ;GET DATA
        DEC R2  ;RESET SP
        STR R2  ;REPLACE ON STACK
;*************************************
; DETERMINE DATA CHANNEL
;*************************************
WHICH:  GLO R7  ;GET CHANNEL
        INC R7  ;INC FOR NEXT
;
        SMI $01
        BZ CH1
;
        SMI $01
        BZ CH2
;
        SMI $01
        BZ CH3
;
        SMI $01
        BZ CH4
;
        SMI $01
        BZ CH5
;
        SMI $01
        BZ CH6
;
        SMI $01
        BZ CH7
;
        SMI $01
        BZ CH8
;
        BR CH9
;*************************************
;
; LOAD DATA AND STORE IN M(RD)
; AND STORE ZERO IN HIGH BYTE M(RD)-1
;
;*************************************
CH0:    LDX     ;GET DATA
        STR RD  ;STACK IT
;
        DEC RD  ;DECREMENT TO HIGH BYTE
        LDI $00 ;ZERO BYTE
        STR RD  ;STACK IT
;
        BR VALID
                ;NEXT CHANNEL
;*************************************
; CHANNEL ONE (PRESET ONE'S)
;*************************************
CH1:    RLDI RD,PRESE1
        BR CH0
;*************************************
; CHANNEL TWO (PRESET TEN'S)
;*************************************
CH2:    RLDI RD,PRESE2
        BR CH0
;*************************************
; CHANNEL THREE (PRESET 100'S)
;*************************************
CH3:    RLDI RD,PRESE3
        BR CH0
;*************************************
; CHANNEL FOUR (PROPORTIONAL GAIN)
;*************************************
CH4:    RLDI RD,EGAIN
        BR CH0
;*************************************
; CHANNEL FIVE (HEIGHT RATE GAIN)
;*************************************
CH5:    RLDI RD,RGAIN
        BR CH0
;*************************************
; CHANNEL SIX (ACCEL INTEGRAL GAIN)
;*************************************
CH6:    RLDI RD,IGAIN
        BR CH0
;*************************************
; CHANNEL SEVEN (OUTPUT SCALE FACTOR)
;*************************************
CH7:    RLDI RD,SGAIN
        BR CH0
;*************************************
; CHANNEL EIGHT (MANUAL RESET)
;*************************************
CH8:    RLDI RD,RESET
        BR CH0
;*************************************
;
; CHANNEL NINE: COMMAND
;
;*************************************
CH9:    RLDI RC,COMWRD
                ;COMMAND WORD
        LDX     ;GET DATA
        STR RC  ;STASH WORD
;
        PHI RC  ;STORE
;*************************************
; ECHO COMMAND WORD TO STATUS LIGHTS
;*************************************
ECHO:   RLDI R2,UTIL
                ;UTIL STACK
        SEX R2  ;SET SP
        GHI RC  ;GET COMMAND
;
        SHL     ;LEFT JUSTIFY
        SHL     ;
        SHL     ;
        SHL     ;
;
        STR R2  ;STACK IT
        OUT 6   ;OUTPUT
        DEC R2  ;RESET STACK
;*************************************
;
; EXECUTE COMMAND INSTRUCTIONS
; BIT 0 REC/FLY (1=REC/0=FLY)
; BIT 1 PRG1
; BIT 2 PRG2
; BIT 3 FIRE ONE
;
; WHEN BIT 0 = 1 (FLY)
;
; PRG1=0,PRG2=0 ACCEL CONTROLLER
; PRG1=1,PRG2=0 PROGRAMABLE CONTROLLER
; PRG1=0,PRG2=1 ALTERNATE CONTROL MODE
; PRG1=1,PRG2=1 MANUAL CONTROL MODE
;
;************************************
        GHI RC  ;GET COMMAND
        ANI RECBIT ;RECOVERY BIT
        LBNZ PULLIT
                ;RECOVERY MODE
;
        GHI RC  ;GET COMMAND
        ANI $07 ;MASK BIT 3
        LBZ ACC ;ACCEL CONTROLLER
;
        GHI RC  ;GET COMMAND
        ANI $07 ;MASK BIT 3
        SMI PRGBIT      ;PRG BIT
        LBZ PRG
                ;PROGRAM MODE
;
        GHI RC  ;GET COMMAND
        ANI $07 ;MASK BIT 3
        SMI ALTBIT      ;ALTERNATE BIT
        LBZ ALT
                ;ALTERNATE MODE
;
        GHI RC  ;GET COMMAND
        ANI $07 ;MASK BIT 3
        SMI MANBIT      ;MANUAL BIT
        LBZ MANUAL
                ;MANUAL MODE
;*************************************
;
; RECOVERY MODE
;
;*************************************
PULLIT: SEX R3		;SET R3 TO SP
        OUT 1		;I/O GROUP
	DB	$30
        OUT 3		;WINGS AT -1.5 DEG
	DB	WNGREC
        SEX R2		;RESET SP
;
        LBR DETDEC
;*************************************
;
; MANUAL WING CONTROL
;
;*************************************
MANUAL: SEX R3  ;SET R3 TO SP
        OUT 1		;I/O GROUP
	DB	$30
        SEX R2  ;RESET SP
;
        RLDI RD,RESET
                ;POINT TO WING
        LDN RD  ;GET WING
        PLO R1  ;STASH
;*************************************
; SOFTWARE LIMIT SWITCHES
;*************************************
        GLO R1  ;GET COMMAND
        SMI WNGNEG ;- LIMIT
        BNF STOPC
                ;IF DF=1 OK
        GLO R1  ;GET COMMAND
        SMI WNGPOS ;+ LIMIT
        BDF LIMITC
                ;IF DF=1 LIMIT
;
        GLO R1  ;GET COMMAND
;
        LSKP    ;SKIP NEXT
;
LIMITC: LDI WNGPOS ;MAXIMUM + LIMIT
;
        LSKP    ;SKIP NEXT
;
STOPC:  LDI WNGNEG ;MAXIMUM - LIMIT
;*************************************
; OUTPUT TO SERVO
;*************************************
SERVOC: STR R2  ;STACK IT
        OUT 3   ;OUT D/A CH 1
        DEC R2  ;RESET STACK
;
        LBR DETDEC
;*************************************
;
; ACCEL CONTROLLER MODE
;
;*************************************
ACC:    RLDI RE,$0004
                ;4 BLINKS
        LDI ACCLIT ;ACC LIGHT
        PLO RC  ;STORE
;
ACCBNK: GLO RC  ;GET LIGHT
        STR R2  ;STACK IT
        XRI $FF ;INVERT BITS
        ANI ACCLIT ;MASK BITS
        PLO RC  ;RESET LIGHT
        OUT 6   ;SEND STATUS
        DEC R2  ;RESET STACK
;
        RLDI RF,$1FFF
                ;COUNTER DELAY
DELAY2: NOP     ;NO OPERATION
        DBNZ RF,DELAY2
                ;DELAY LOOP
        DBNZ RE,ACCBNK
                ;BLINK
;
        GHI RC  ;COMMAND
        SHL
        SHL
        SHL
        SHL
        STR R2
        OUT 6
        DEC R2
;*************************************
;
; SET FIXED GAINS (REVISION JAN 31 1989)
;
;*************************************
GAINER: SEX RD  ;SET SP TO RD
;
        RLDI RD,EGAIN
        LDI $66 ;2.000 VOLTS (REV WAS 66 2.000 VOLTS) (REV WAS 33 1.000 VOLTS)
        STXD
        LDI $00
        STR RD
;
        RLDI RD,RGAIN
        LDI $4C ;1.500 VOLTS (REV WAS 7F 2.500 VOLTS)
        STXD
        LDI $00
        STR RD
;
        RLDI RD,IGAIN
        LDI $33 ;1.000 VOLTS (REV WAS 7F 2.500 VOLTS)
        STXD
        LDI $00
        STR RD
;
        RLDI RD,SGAIN
        LDI $50 ;1.568 VOLTS
        STXD
        LDI $00
        STR RD
;
        RLDI RD,RESET
        LDI $66 ;2.00 VOLTS
        STXD
        LDI $00
        STR RD
;*************************************
ACCOK:  LBR TLXACC
                ;RUN CONTROLLER
;*************************************
;
; PROGRAMABLE CONTROLLER MODE
;
;*************************************
PRG:    RLDI RE,$0004
                ;4 BLINKS
        LDI PRGLIT ;PRG LIGHT
        PLO RC  ;STORE
;
PRGBNK: GLO RC  ;GET LIGHT
        STR R2  ;STACK IT
        XRI $FF ;INVERT BITS
        ANI PRGLIT ;MASK BITS
        PLO RC  ;RESET LIGHT
        OUT 6   ;SEND STATUS
        DEC R2  ;RESET STACK
;
        RLDI RF,$1FFF
                ;COUNTER DELAY
DELAY3: NOP     ;NO OPERATION
        DBNZ RF,DELAY3
                ;DELAY LOOP
        DBNZ RE,PRGBNK
                ;BLINK
;
        GHI RC  ;COMMAND
        SHL
        SHL
        SHL
        SHL
        STR R2
        OUT 6
        DEC R2
;
PRGOK:  LBR TLXACC
                ;RUN CONTROLLER
;*************************************
;
; ALTERNATE CONTROLLER MODE
;
;*************************************
ALT:    RLDI RE,$0004
                ;4 BLINKS
        LDI ALTLIT ;ALT LIGHT
        PLO RC  ;STORE
;
ALTBNK: GLO RC  ;GET LIGHT
        STR R2  ;STACK IT
        XRI $FF ;INVERT BITS
        ANI ALTLIT ;MASK BITS
        PLO RC  ;RESET LIGHT
        OUT 6   ;SEND STATUS
        DEC R2  ;RESET STACK
;
        RLDI RF,$1FFF
                ;COUNTER DELAY
DELAY4: NOP     ;NO OPERATION
        DBNZ RF,DELAY4
                ;DELAY LOOP
        DBNZ RE,ALTBNK
                ;BLINK
;
        GHI RC  ;COMMAND
        SHL
        SHL
        SHL
        SHL
        STR R2
        OUT 6
        DEC R2
;
        RLDI RD,TLXALT
        LDN RD
        SMI $C4 ;NOP
        BZ ALTOK
;
        RLDI R0,$0000
        SEX R0
        SEP R0
;
ALTOK:  LBR TLXALT
                ;RUN CONTROLLER
;*************************************
;
; DETERMINE ALT DEVIATION
;
;*************************************
DETDEC: RLDI RD,UTIL
;*************************************
;
; THUMB WHEEL PRESET DECODE
;
; PRESET=( (ONES/27) + (TENS/27)*10 +
;          (HUNDS/27)*100 )*255/100
;
; THE GROUND STATION TRANSMITS THE
; PRESET ALTITUDE USING THUMB WHEEL
; SWITCHES. AS WIRED, THE DIGITS ARE
; INVERTED (9=0VOLTS,0=5VOLTS). IF
; THIS IS CHANGED, THE XRI INSTRUCTION
; IN THE CODE SHOULD BE COMMENTED OUT
; AND THE CORRESPONDING NOP SHOULD
; BE UN-COMMENTED.
;
;*************************************
        SEX RD  ;MATH STACK
        LDI $1B ;LOW BYTE OF 27
        STXD    ;STACK IT
        LDI $00 ;HIGH BYTE OF 27
        STR RD  ;STACK IT
        RLDI R7,$0000
        RLDI R8,$0000
        RLDI R9,$0000
;
        RLDI R4,PREONE
        RLDI R5,PRETEN
        RLDI R1,PREHUN
;*************************************
ONES:   RLDI RB,PRESE1
        LDN RB
;       NOP
;	NOP
        XRI $FF
        LBZ TENS
        PLO RF
        LDI $00
        PHI RF
        PLO RE
        PHI RE
;
        RLDI R6,DIVQ
        SEP R6
;
        GHI RF
        PHI R7
        GLO RF
        PLO R7
;
        STR R4
;*************************************
TENS:   RLDI RD,UTIL-1
        RLDI RB,PRESE2
        LDN RB
;       NOP
;	NOP
        XRI $FF
        LBZ HUNDS
        PLO RF
        LDI $00
        PHI RF
        PLO RE
        PHI RE
;
        RLDI R6,DIVQ
        SEP R6
;
        GLO RF
        STR R5
;
        RLDI RD,MATH
        LDI $0A
        STXD
        LDI $00
        STR RD
;
        RLDI R6,MULT
        SEP R6
;
        GHI RF
        PHI R8
        GLO RF
        PLO R8
;*************************************
HUNDS:  RLDI RD,UTIL-1
        RLDI RB,PRESE3
        LDN RB
;	NOP
;	NOP
        XRI $FF
        BZ SUMUP
        PLO RF
        LDI $00
        PHI RF
        PLO RE
        PHI RE
;
        RLDI R6,DIVQ
        SEP R6
;
        GLO RF
        STR R1
;
        RLDI RD,MATH
        LDI $64
        STXD
        LDI $00
        STR RD
;
        RLDI R6,MULT
        SEP R6
;
        GHI RF
        PHI R9
        GLO RF
        PLO R9
;*************************************
; SUM UP DIGITS
;*************************************
SUMUP:  RLDI RD,UTIL
        LDI $00
        STXD
        STR RD
        INC RD
;
        GLO R7
        ADD
        STXD
        GHI R7
        ADC
        STR RD
        INC RD
;
        GLO R8
        ADD
        STXD
        GHI R8
        ADC
        STR RD
        INC RD
;
        GLO R9
        ADD
        STXD
        GHI R9
        ADC
        STR RD
        INC RD
;*************************************
; MULT SUM BY 255 BITS
;*************************************
        RLDI RD,UTIL-1
        LDI $FF
        PLO RF
        LDI $00
        PHI RF
        RLDI R6,MULT
        SEP R6
;*************************************
; DIVIDE BY 100 BITS
;*************************************
        RLDI RD,UTIL
        LDI $64
        STXD
        LDI $00
        STR RD
        RLDI R6,DIVQ
        SEP R6
;*************************************
; CHECK FOR MINIMUM PRESET OF 20 FT
;*************************************
        GHI RF  ;HIGH BYTE
        BNZ PREOK
                ;IF NON-ZERO OK
        GLO RF  ;LOW BYTE
        SMI $33 ;20 FT
        BDF PREOK
                ;DF=1 OK
        RLDI RF,$0033
                ;PRESET=20 FT
;*************************************
; PUT PRESET ON STACK
;*************************************
PREOK:  RLDI RD,PRESET
        GLO RF
        STXD
        GHI RF
        STR RD
;*************************************
; SHIFT ALT STACK DOWN ONE ENTRY
;*************************************
ALTSFC: RLDI RA,ABOT+$02
                ;BOTTOM OF STACK+2
        RLDI RB,ABOT
                ;BOTTOM STACK
        RLDI RC,ABYTE-$02
                ;NUMBER BYTES-2
        SEX RA  ;RA IS SP
;
ASHFTC: LDXA    ;GET BYTE
        STR RB  ;MOVE DOWN
        INC RB  ;INCREMENT POINTER
;
        DBNZ RC,ASHFTC
                ;LOOP FOR ALL BYTES
        INC RB  ;POINT TO LOW BYTE
;*************************************
;
; A/D INPUT SECTION
;
; A/D CHANNELS
;
; CHANNEL 0 ALT RELIBILITY
; CHANNEL 1 100 FT RANGE ALT
; CHANNEL 2 1000 FT RANGE ALT
; CHANNEL 3 FEEDBACK POT
; CHANNEL 4 ACCELOERMETER
;
;*************************************
ADSETC: RLDI R2,INPUT
                ;INPUT STACK
;
        RLDI R4,ALTREL
        RLDI R5,ALTLOW
        RLDI R6,ALTHIH
        RLDI R8,FEDPOT
        RLDI R9,GLOADS
;
ADGETC: SEX R3  ;MAKE PC SP
        OUT 1		;SELECT I/O
	DB	$30
        OUT 6		;CHANNEL MODE
	DB	$01
        OUT 5		;RELIBILITY
	DB	$00
        SEX R2  ;RESET SP
;
        BN1 *   ;WAIT
        INP 4   ;RELIBILITY
        STR R4  ;STASH
;
        BN1 *   ;WAIT FOR FLAG
        INP 4   ;100FT RANGE
        PLO RF  ;STASH INPUT
        STR R5  ;STASH
;
        BN1 *   ;WAIT
        INP 4   ;500FT RANGE
        PLO R7  ;STASH
        STR R6  ;STASH
;
        BN1 *   ;WAIT
        INP 4   ;WING POSITION
        STR R8  ;STASH
;
        BN1 *   ;WAIT
        INP 3   ;ACCELEROMETER
        STR R9  ;STASH
;*************************************
; AUTO-RANGE ALT INPUTS
;*************************************
AUTOC:  LDI $00 ;ZERO BYTE
        PHI RF  ;HIGH BYTE OF ALT
;
        GLO RF  ;GET 100FT RANGE
        SDI $FF ;MAX INPUT
        LBNZ STACKC
                ;IF NON-ZERO, LOW RANGE
;*************************************
; FULL SCALE RANGE
;*************************************
        GLO R7  ;HIGH RANGE ALT
        PLO RF  ;SET LOW BYTE OF ALT
;
        RLDI RD,UTIL
                ;SET MATH STACK
        SEX RD  ;MAKE RD SP
        LDI $05 ;LOW BYTE OF 5
        STXD    ;PUT ON STACK
        LDI $00 ;HIGH BYTE OF 5
        STR RD  ;PUT ON STACK
;
        RLDI R6,MULT
                ;ADDRESS OF MULT
        SEP R6  ;CALL MULT
;*************************************
; PUT CURRENT ALT ON STACK
;*************************************
STACKC: GLO RF  ;
        STR RB  ;
        DEC RB  ;
        GHI RF  ;
        STR RB  ;
;*************************************
;
; AVERAGE ALT INPUTS
;
;*************************************
; DON'T AVERAGE IN MANUAL MODE
;*************************************
        RLDI RC,COMWRD
        LDN RC
        ANI $07
        SMI MANBIT
        LBZ CALERC
;*************************************
; AVERAGE LAST 4 ALT SAMPLES
;*************************************
        RLDI RD,ATOP
                ;TOP OF ALT STACK
        RLDI RC,$0004
                ;NUMBER OF SAMPLES
        RLDI RF,$0000
                ;ZERO SUM
        SEX RD  ;SET STACK
;
AVGC:   GLO RF  ;GET LOW BYTE
        ADD     ;ADD TO ALT
        PLO RF  ;RESET SUM
;
        DEC RD  ;POINT TO HIGH BYTE
;
        GHI RF  ;GET HIGH BYTE
        ADC     ;ADD WITH DF
        PHI RF  ;RESET SUM
;
        DEC RD  ;POINT TO NEXT LOW BYTE
;
        DBNZ RC,AVGC
                ;ADD ALL WORDS
;
        SEX R2  ;RESET STACK
        RLDI R6,USHR
                ;ADDRESS OF USHR
        SEP R6		;DIVIDE SUM BY 4
	DB	$02
;*************************************
; RESET CURRENT INPUT WITH AVERAGE
;*************************************
        RLDI RD,ATOP
        SEX RD  ;RESET SP
        GLO RF  ;LOW BYTE
        STXD    ;STACK IT
        GHI RF  ;HIGH BYTE
        STR RD  ;STACK IT
;*************************************
; CALCULATE ALT ERROR
;*************************************
CALERC: RLDI RD,PRESET
                ;POINTER TO PRESET
        SEX RD  ;SET STACK POINTER
;
        GLO RF  ;GET LOW BYTE OF ALT
        SD      ;
        PLO R9  ;
;
        DEC RD  ;
;
        GHI RF  ;GET HIGH BYTE OF ALT
        SDB     ;
        PHI R9  ;
;*************************************
;
; DETERMINE +/- 50 FT ALT DEVIATION
;
;*************************************
        GHI R9  ;CHECK SIGN OF ERROR
        SHL     ;
        LBDF AHIGHC      ;
;*************************************
; LOW ON PRESET
;*************************************
        GHI R9  ;
        LBNZ MAXLC       ;
;
        GLO R9  ;
        ADI $80 ;
        LBDF MAXLC       ;
;
        LSKP    ;
;
MAXLC:  LDI $FF ;
        LBR OUTC         ;
;*************************************
; HIGH ON PRESET
;*************************************
AHIGHC: GHI R9  ;
        SMI $FF ;
        LBNZ MAXHC       ;
;
        GLO R9  ;

        SMI $80 ;
        LBNF MAXHC       ;
;
        LSKP    ;
;
MAXHC:  LDI $00
;*************************************
; OUTPUT ALT DEVIATION TO D/A CH 2
;*************************************
OUTC:   XRI $FF ;
        STR R2  ;
        SEX R2  ;
        OUT 4   ;
        DEC R2  ;
;*************************************
;
; RETURN TO COMMAND LINK
;
;*************************************
       LBR GRABER
;*************************************
;
;
; TLX ACCEL CONTOLLER SOFTWARE
;
;
;*************************************
	ORG $0400       ;EPROM NUMBER ONE
;*************************************
;
; WING LIMITS
;
; HARDWARE WING POSITION
; BASED ON FOLLOWING CALIBRATION
;
; ANGLE  HEX  DEC  VOLTS
;
; +1     FF   255   2.5
;  0     E5   229   2.25
; -1     CC   204   2.0
; -1.5   BF   191   1.875
; -2     B2   178   1.75
; -3     99   153   1.5
; -4     7F   127   1.25
; -5     66   102   1.0
; -6     4C    76   0.75
; -7     33    51   0.50
; -8     19    25   0.25
; -9     00     0   0.0
;
; SOFTWARE WING POSITION
; BASED ON THE FOLLOWING CONVENTION
;
; ANGLE    HEX
;
; +8       00CC + 00E5 = 01B1
;  0       0000 + 00E5 = 00E5
; -8       FF34 + 00E5 = 0019
;
;*************************************
; WING POSITON EQUATES
;*************************************
WNGPOS	EQU	$E5      ;MAX POSITIVE (0)
WNGZER	EQU	$E5      ;ZERO ANGLE
WNGREC	EQU	$BF      ;RECOVERY
WNGNEG	EQU	$19      ;MAX NEGATIVE (-8)
;
WNGMAX	EQU	$00CC    ;ABS POS LIMIT (+8)
WNGXRI	EQU	$FF34    ;ABS NEG LIMIT (-8)
;*************************************
;
; ACCELEROMETER SCALING
;
; G'S    HEX    DEC    VOLTS
;
; +1.5   FF     255     2.5
; +1.25  BF     191     1.875
; +1     7F     127     1.25
; +0.75  3F      63     0.625
; +0.5   00       0     0.0
;
;*************************************
;
; TLX ACCEL CONTROLLER
;
;*************************************
TLXACC: NOP
;*************************************
; ZERO ACCEL STACK
;*************************************
ACCZER: RLDI RD,CTOP
                ;TOP OF ACCEL STACK
        RLDI RC,CBYTE
                ;NUMBER BYTES
        SEX RD  ;SET STACK
        LDI $00 ;ZERO G'S
;
ZEROC1: STXD    ;STORE BYTE
;
        DBNZ RC,ZEROC1
                ;ZERO ALL BYTES
;*************************************
; SET COUNTER FOR FOUR CYCLES
;*************************************
        RLDI RD,COUNT
        LDI $04
        STR RD
;*************************************
;
; DELAY LOOP SECTION
;
; DELAY LOOP FOR PROGRAM EXECUTION
; OF 0.25 SECONDS PER CYCLE.
; POLL THE HALT SWITCH WHILE
; WAITING, USING THE EF3 FLAG .
;
;*************************************
LOWALT: RLDI RF,$0167
                ;DELAY COUNTER
LOWHLT: BN3 HALT
        BN3 HALT
        BN3 HALT
        BN3 HALT
;
        BN3 HALT
        BN3 HALT
        BN3 HALT
        BN3 HALT
;
        DBNZ RF,LOWHLT
                ;DECREMENT DELAY COUNT
        BR TLXFLY
;*************************************
;
; DEBUG:CYCLE TIMER
;
; NOP OUT ABOVE BRANCH AND STOP
; CLOCK COUNTER. USES Q OUTPUT
; TO INDICATE CONTROL LOOP
;
;*************************************
        LSQ     ;SKIP NEXT 2 IF Q=1
;
        SEQ     ;SET Q=1
        LSKP     ;SKIP NEXT 2
;
        REQ     ;SET Q=0
        REQ     ;EVEN OUT LOOP
;
        BR CLTSHF
;*************************************
; JUMP TO COMMAND LINK
;*************************************
HALT:   LBR ABORTP
                ;ABORT PROCESSOR
;*************************************
; CONTROL LOOP
;*************************************
TLXFLY: NOP		; NO OPERATION
	NOP		; NO OPERATION
;*************************************
; SHIFT ALT STACK DOWN ONE ENTRY
;*************************************
CLTSHF: RLDI RA,ABOT+$02
                ;BOTTOM OF STACK+2
        RLDI RB,ABOT
                ;BOTTOM STACK
        RLDI RC,ABYTE-$02
                ;NUMBER BYTES-2
        SEX RA  ;RA IS SP
;
CAHFT:  LDXA    ;GET BYTE
        STR RB  ;MOVE DOWN
        INC RB  ;INCREMENT POINTER
;
        DBNZ RC,CAHFT
                ;LOOP FOR ALL BYTES
        INC RB  ;POINT TO LOW BYTE
;*************************************
; SHIFT ACCEL STACK DOWN ONE ENTRY
;*************************************
CELSFT: RLDI R7,CBOT+$02
                ;BOTTOM OF STACK+2
        RLDI R8,CBOT
                ;BOTTOM STACK
        RLDI R9,CBYTE-$02
                ;NUMBER BYTES-2
        SEX R7  ;R7 IS SP
;
CSHFT:  LDXA    ;GET BYTE
        STR R8  ;MOVE DOWN
        INC R8  ;INCREMENT POINTER
;
        DBNZ R9,CSHFT
                ;LOOP FOR ALL BYTES
        INC R8  ;POINT TO LOW BYTE
;*************************************
;
; A/D INPUT SECTION
;
;*************************************
ADSET:  RLDI R2,INPUT	;INPUT STACK
;
        SEX R3		;MAKE PC SP
        OUT 1		;SELECT I/O
	DB	$30
        OUT 6		;SCAN MODE
	DB	$01
        OUT 5		;START AT CH 0
	DB	$00
        SEX R2		;RESET SP
;
        BN1 *		;WAIT FOR FLAG
;
        INP 4   	;INPUT RELIBILITY
        SMI $80
        LBNF ABORTR
;*************************************
; RELIBILITY OK
;*************************************
RELOK:  BN1 *   ;WAIT FOR FLAG
;
        INP 4   ;100FT RANGE
        PLO RF  ;STASH
;
        BN1 *   ;WAIT FOR FLAG
;
        INP 4   ;1000FT RANGE
        PLO R7  ;STASH
;*************************************
        BN1 *   ;WAIT FOR FLAG
        INP 4   ;WING POSITION
;*************************************
        BN1 *   ;WAIT
        INP 3   ;ACCEL
;*************************************
;
; SUBTRACT +1 G BIAS ($007F)
;
;************************************
BIAS:   SMI $7F ;BIAS
        STR R8  ;STACK IT
        DEC R8  ;HIGH BYTE
        LDI $00 ;ZERO BYTE
        SMBI $00        ;BIAS
        STR R8  ;STACK IT
;*************************************
; AUTO-RANGE ALT INPUTS
;*************************************
AUTO:   LDI $00 ;ZERO BYTE
        PHI RF  ;HIGH BYTE OF ALT
;
        GLO RF  ;LOW RANGE ALT
        SDI $FF ;SUBTRACT MAX VALVE
        BNZ STACKA
           ;IF NOT MAX USE LOW RANGE
;*************************************
; FULL SCALE RANGE
;*************************************
        GLO R7  ;HIGH RANGE ALT
        PLO RF  ;SET LOW BYTE OF ALT
;
        RLDI RD,UTIL
                ;SET MATH STACK
        SEX RD  ;MAKE RD SP
        LDI $05 ;LOW BYTE OF 5
        STXD    ;PUT ON STACK
        LDI $00 ;HIGH BYTE OF 5
        STR RD  ;PUT ON STACK
;
        RLDI R6,MULT
                ;ADDRESS OF MULT
        SEP R6  ;CALL MULT
;*************************************
; PUT CURRENT ALT ON STACK
;*************************************
STACKA: GLO RF  ;
        STR RB  ;
        DEC RB  ;
        GHI RF  ;
        STR RB  ;
;*************************************
; CHECK FOR ALT < 8 FT
;************************************
        GLO R7  ;HIGH RANGE
        SMI $04 ;SUB 8FT
        LBNF ABORTL
        ;IF BORROW ABORT
;*************************************
; AVERAGE 4 ALT SAMPLES
;*************************************
        RLDI RD,ATOP
                ;TOP OF ALT STACK
        RLDI RC,$0004
                ;NUMBER OF SAMPLES
        RLDI RF,$0000
                ;ZERO SUM
        SEX RD  ;SET STACK
;
AVG:    GLO RF  ;GET LOW BYTE
        ADD     ;ADD TO ALT
        PLO RF  ;RESET SUM
;
        DEC RD  ;POINT TO HIGH BYTE
;
        GHI RF  ;GET HIGH BYTE
        ADC     ;ADD WITH DF
        PHI RF  ;RESET SUM
;
        DEC RD  ;POINT TO NEXT LOW BYTE
;
        DBNZ RC,AVG	;ADD ALL WORDS
;
        SEX R2		;RESET STACK
        RLDI R6,USHR	;ADDRESS OF USHR
        SEP R6		;DIVIDE SUM BY 4
	DB	$02
;*************************************
; RESET CURRENT INPUT WITH AVERAGE
;*************************************
        RLDI RD,ATOP
        SEX RD  ;RESET SP
        GLO RF  ;LOW BYTE
        STXD    ;STACK IT
        GHI RF  ;HIGH BYTE
        STR RD  ;STACK IT
;*************************************
; CALCULATE ALT ERROR
;*************************************
CALERR: RLDI RD,PRESET
                ;POINTER TO PRESET
        SEX RD  ;SET STACK POINTER
;
        GLO RF  ;GET LOW BYTE OF ALT
        SD      ;
;
        PLO RF  ;
        PLO R9  ;
;
        DEC RD  ;
;
        GHI RF  ;GET HIGH BYTE OF ALT
        SDB     ;
;
        PHI RF  ;
        PHI R9  ;
;*************************************
; DETERMINE +/- 50 FT ALT DEVIATION
;*************************************
DETDEV: GHI R9  ;CHECK SIGN OF ERROR
        SHL     ;
        LBDF AHIGH       ;
;*************************************
; LOW ON PRESET
;*************************************
        GHI R9  ;
        BNZ MAXL        ;
;
        GLO R9  ;
        ADI $80 ;
        BDF MAXL        ;
;
        LSKP    ;
;
MAXL:   LDI $FF ;
        BR OUT  ;
;*************************************
; HIGH ON PRESET
;*************************************
AHIGH:  GHI R9  ;
        SMI $FF ;
        BNZ MAXH        ;
;
        GLO R9  ;
        SMI $80 ;
        BNF MAXH        ;
;
        LSKP    ;
;
MAXH:   LDI $00
;*************************************
; OUTPUT ALT DEVIATION TO D/A CH 2
;*************************************
OUT:    XRI $FF ;
        STR R2  ;
        SEX R2  ;
        OUT 4   ;
        DEC R2  ;
;*************************************
; GAIN ALT ERROR (KP*ERROR)
;*************************************
        RLDI RD,EGAIN-1
                ;POINT TO HIGH BYTE
        RLDI R6,MULT
                ;ADDRESS OF MULT
        SEX RD  ;RD IS SP
        SEP R6  ;CALL MULT
;*************************************
; STORE RESULT (RE,RF) IN R0,R1
;*************************************
        GHI RE  ;
        PHI R0  ;
;
        GLO RE  ;
        PLO R0  ;
;
        GHI RF  ;
        PHI R1  ;
;
        GLO RF  ;
        PLO R1  ;
;*************************************
; DETERMINE ALT(N)-ALT(1)
;
; THIS IS REVERSED SO THAT AN UPWARD
; ALT CHANGE WITH YIELD A NEGATIVE
; WING ANGLE
;************************************
ARATE:  RLDI RD,ATOP
                ;ALT(1)
        RLDI RC,ABOT+$01
                ;ALT(N)
        SEX RD
;
        LDN RC
        SM      ;(N)-(1)
        PLO RF  ;STORE
;
        DEC RD  ;POINT TO HIGH BYTE
        DEC RC  ;POINT TO HIGH BYTE
;
        LDN RC  ;HIGH BYTE
        SMB     ;HIGH BYTE
        PHI RF  ;STORE
;*************************************
; DIVIDE ALT DIFFERENCE BY N*0.0625
; WITH N=15 THEN IS APPROXIMATELY
; EQUAL TO 1, THEREFORE NO OPERATION
;*************************************
        NOP
	NOP
	NOP
;*************************************
; GAIN ALT DIFF (KV* DIFF)
;*************************************
        RLDI RD,RGAIN-1
                ;POINT TO HIGH BYTE
        RLDI R6,MULT
                ;ADDRESS OF MULT
        SEX RD  ;MAKE RD STACK
        SEP R6  ;CALL MULT
;*************************************
; ADD KP*ERROR + KV*ALT DIFF
;*************************************
        RLDI R6,ADDER
                ;ADDRESS OF ADDER
        SEX R2  ;RESET STACK
        SEP R6  ;CALL ADDER
;*************************************
; AVERAGE ACCEL INPUT
;*************************************
ACCEL:  RLDI RB,CTOP
                ;ACCEL STACK
        RLDI RC,$0004
                ;4 AVERAGES
        RLDI RF,$0000
                ;ZERO SUM
        SEX RB  ;SET STACK
;
CELAVG: GLO RF  ;LOW BYTE
        ADD     ;ADD LOW BYTES
        PLO RF  ;RESET SUM
;
        DEC RB  ;POINT TO HIGH BYTE
;
        GHI RF  ;HIGH BYTE
        ADC     ;ADD HIGH BYTES
        PHI RF  ;RESET SUM
;
        DEC RB  ;POINT TO NEXT LOW
;
        DBNZ RC,CELAVG
                ;ADD UP FOUR INPUTS
;
        SEX R2  ;RESET SP
        RLDI R6,ASHR
                ;ADDRESS OF ASHR
        SEP R6
	DB	$02
;
        RLDI RB,CTOP
        GLO RF  ;LOW BYTE
        STR RB  ;STACK IT
        DEC RB  ;POINT TO HIGH
        GHI RF  ;HIGH BYTE
        STR RB  ;STACK IT
;*************************************
; DOUBLE INTEGRATE THE ACCEL
;*************************************
DOUBLE: RLDI RD,COUNT
                ;POINT TO COUNT
        LDN RD  ;LOAD COUNT
        SMI $01 ;COUNT-1
        STR RD  ;RESET COUNT
        LBNZ LOWALT
                ;LOOP TILL ZERO
        LDI $04 ;4 CYCLES
        STR RD  ;RESET COUNT
;*************************************
; INTEGRATE ACCEL
;*************************************
        RLDI RB,CTOP
                ;ACCEL STACK
        RLDI RA,CBYTE2
                ;NUMBER WORBS
        RLDI R9,$0000
                ;DOUBLE SUM
        SEX RD  ;SET MATH STACK
;
CCEL:   LDN RB  ;GET LOW BYTE
        PLO RF  ;STORE
        DEC RB  ;POINT TO HIGH BYTE
        LDN RB  ;GET HIGH BYTE
        PHI RF  ;STORE
        DEC RB  ;NEXT LOW BYTE
;
        RLDI RD,UTIL
                ;SET STACK
        GLO RA  ;LOW COUNT
        STXD    ;STACK IT
        GHI RA  ;HIGH COUNT
        STR RD  ;STACK IT
        RLDI R6,MULT
                ;ADDRESS OF MULT
        SEP R6  ;MULT
;
        GLO RF  ;LOW BYTE
        STR RD  ;STACK IT
        GLO R9  ;LOW BYTE
        ADD     ;ADD LOW BYTES
        PLO R9  ;RESET SUM
;
        GHI RF  ;HIGH BYTE
        STR RD  ;STACK IT
        GHI R9  ;HIGH BYTE
        ADC     ;ADD HIGH BYTES
        PHI R9  ;RESET SUM
;
        DBNZ RA,CCEL
                ;SUM STACK
;
        GLO R9  ;LOW BYTE
        PLO RF  ;
        GHI R9  ;HIGH BYTE
        PHI RF  ;
;*************************************
; MULT ACCEL INTEGRAL BY 0.0625**2
; AND DIVIDE BY 15*0.0625
; WHICH IS APPROXIMATELY 2**(-8)
;*************************************
        RLDI R6,ASHR
                ;ADDRESS OF ASHR
        SEP R6
	DB	$08
;*************************************
; GAIN ACCEL INTEGRAL (KV* ACCEL)
;*************************************
        RLDI RD,IGAIN-1
                ;POINT TO HIGH BYTE
        RLDI R6,MULT
                ;ADDRESS OF MULT
        SEX RD  ;MAKE RD STACK
        SEP R6  ;CALL MULT
;*************************************
; ADD KP*ERROR + KV*ACCEL INTEGRAL
;*************************************
        RLDI R6,ADDER
                ;ADDRESS OF ADDER
        SEX R2  ;RESET STACK
        SEP R6  ;CALL ADDER
;*************************************
; WING COMMAND SECTION
;*************************************
WING:   RLDI R2,UTIL
                ;MATH STACK
        SEX R2  ;SETUP STACK
;
        RLDI RD,SGAIN
                ;SCALE FACTOR
        LDN RD  ;GET SCALE
        SHR     ;DIVIDE BY 16
        SHR
        SHR
        SHR
        PLO RD  ;STORE
;*************************************
; DIVIDE BY 2**SCALE
;*************************************
        RLDI R6,SCALE
                ;ADDRESS OF SCALE
        SEP R6  ;CALL SCALE
;*************************************
; CHECK WING COMMAND FOR OVERFLOW
;*************************************
OVERFL: GHI R0  ;GET MSB
        SHL     ;PUT SIGN IN DF
        BDF NEGWNG
                ;IF NEG BRANCH
        GHI R1  ;GET HIGH BYTE
        BNZ POSOVF
                ;IF NOT ZERO OVERFLOW
        GLO R1  ;GET LOW BYTE
        SHL     ;PUT BIT 7 IN DF
        BNF _OFFSET
                ;IF ZERO OK
POSOVF: RLDI R1,WNGMAX
                ;MAXIMUM (+)
        BR _OFFSET
                ;ADD IN OFFSET
;
NEGWNG: GHI R1  ;GET HIGH BYTE
        SDI $FF ;SUBTRACK NEG BITS
        BNZ NEGOVF
                ;IF NOT ZERO OVERFLOW
        GLO R1  ;GET LOW BYTE
        SHL     ;PUT SIGN IN DF
        BDF _OFFSET
                ;IF SET OK
NEGOVF: RLDI R1,WNGXRI
                ;MAXIMUM (-)
;*************************************
; ADD IN HARDWARE OFFSET
;************************************
_OFFSET: GLO R1  ;GET WING COMMAND
        ADI LOW(WNGZER) ;ADD OFFSET
        PLO R1  ;RESET COMMAND
        GHI R1  ;GET HIGH BYET
        ADCI HIGH(WNGZER)
        PHI R1  ;RESET HIGH BYTE
;*************************************
; SUBTRACT MANUAL RESET
;************************************
SUBMAN: RLDI RD,RESET
                ;MANUAL RESET
        LDN RD  ;GET RESET
        STR R2  ;STACK IT
;
        GLO R1  ;GET WING POSITION
        SM      ;SUBTRACT MR
        PLO R1  ;RESET WING POSITION
        GHI R1  ;HIGH BYTE
        SMBI $00
                ;HIGH BYTE OF LIMIT
        PHI R1  ;RESET HIGH BYTE
;*************************************
; SOFTWARE LIMIT SWITCHES
;*************************************
SOFT:   GHI R1  ;CHECK HIGH BYTE
        BZ SOFT1
                ;IF ZERO IN LIMITS
;
        SHL     ;PUT SIGN IN DF
        BDF STOP
                ;NEG OVERFLOW
        BR LIMIT
                ;POS OVERFLOW
;
SOFT1:  GLO R1  ;GET COMMAND
        SMI LOW(WNGNEG) ;- LIMIT
        BNF STOP
                ;IF DF=0 LIMIT
;
        GLO R1  ;GET COMMAND
        SMI LOW(WNGPOS) ;+ LIMIT
        BDF LIMIT
                ;IF DF=1 LIMIT
;
        GLO R1  ;GET COMMAND
;
        LSKP    ;SKIP NEXT
;
LIMIT:  LDI LOW(WNGPOS) ;MAXIMUM + LIMIT
;
        LSKP    ;SKIP NEXT
;
STOP:   LDI LOW(WNGNEG) ;MAXIMUM - LIMIT
;*************************************
; OUTPUT TO SERVO
;*************************************
SERVO:  STR R2  ;STACK IT
        OUT 3   ;OUT D/A CH 1
        DEC R2  ;RESET STACK
;*************************************
; CYCLE CONTROLLER
;*************************************
        LBR LOWALT
;*************************************
;
; END OF CONTROLLER SECTION
;
;*************************************
;
; ABORT CONTROLLER
;
;*************************************
; ABORT ON HALT SWITCH
;*************************************
ABORTP: LDI $00 ;HALT FLAG
        PLO RC  ;SET ABORT FLAG
        BR PREC
                ;BRANCH
;*************************************
; ABORT IF ALT LESS THAN 8 FT
;*************************************
ABORTL: LDI $01 ;ABOTRL FLAG
        PLO RC  ;SET ABORT FLAG
        BR ABORT
                ;BRANCH
;*************************************
; ABORT IF ALT RELIBILITY IS LOST
;*************************************
ABORTR: LDI $02 ;ABORTR FLAG
        PLO RC  ;SET ABORT FLAG
        BR ABORT
                ;BRANCH
;*************************************
; ABORT PROCESSOR
;*************************************
ABORT:  RLDI R2,INPUT
                ;I/O STACK
        SEX R3  ;SET SP TO PC
        OUT 1		;I/O GROUP 30
	DB	$30
        OUT 6		;SCAN MODE
	DB	$01
        OUT 5		;REL
	DB	$00
        SEX R2  ;RESET SP
;
        BN1 *   ;WAIT
        INP 4   ;REL
;
        BN1 *   ;WAIT
        INP 4   ;100FT ALT
;
        BN1 *   ;WAIT
        INP 4   ;500FT ALT
;
        BN1 *   ;WAIT
        INP 3   ;WING
;
        SMI LOW(WNGREC) ;SUBTRACK RECOVERY
        BNF PREC
        ;DF=0 WING BELOW RECOVERY
;*************************************
; DO NOT PUT WINGS IN RECOVERY
;*************************************
        LDX     ;GET POSITION
;
        LSKP    ;SKIP NEXT INSTRUCTION
;*************************************
; PUT WINGS INTO RECOVERY
;*************************************
PREC:   LDI LOW(WNGREC) ;RECOVERY
;
        STR R2  ;PUT ON STACK
        OUT 3   ;OUPUT D/A CH1
        DEC R2  ;RESET STACK
;*************************************
; EXIT TO COMMAND LINK ERROR ROUTINE
;*************************************
EXIT:   LBR LINK
;*************************************
;
; END OF ABORT PROCESSOR
;
;*************************************
;
; COMMON CODE AREA
;
;*************************************
        ORG $0700
;*************************************
;
; ADDRESS FOR COMMAND JUMP
;
;*************************************
LINK:   LBR TMWAIT
;*************************************
;
; MATH SUBROUTINES
;
;*************************************
;
; UN-SIGNED RIGHT SHIFT OF RF
;
; NUMBER OF SHIFTS PASSED INLINE
; THROUGH THE PROGRAM COUNTER
;
; CALLED BY:
;      RLDI R6,USHR
;      SEP R6;,$SHIFTS
;
; USES REGISTER RD.0
;
;*************************************
USHR:   LDA R3  ;GET NUMBER OF SHIFTS
        PLO RD  ;SET COUNTER
;
USHR1:  GHI RF  ;GET HIGH BYTE
        SHR     ;RIGHT SHIFT
        PHI RF  ;RESET BYTE
;
        GLO RF  ;GET LOW BYTE
        SHRC    ;RIGHT SHIFT WITH CARRY
        PLO RF  ;RESET LOW BYTE
;
        DEC RD  ;DECREMENT COUNTER
        GLO RD  ;GET COUNTER
        BNZ USHR1
                ;SHIFT TILL ZERO
;
        SEP R3  ;RESET PROGRAM COUNTER
;*************************************
;
; ARITHMETIC RIGHT SHIFT OF RF
;
; NUMBER OF SHIFTS PASSED INLINE
; THROUGH THE PROGRAM COUNTER
;
; CALLED BY:
;      RLDI R6,ASHR
;      SEP R6;,$SHIFTS
;
; USES REGISTER RD.0
;
;*************************************
ASHR:   LDA R3  ;GET NUMBER OF SHIFTS
        PLO RD  ;SET COUNTER
;
ASHR1:  GHI RF  ;GET HIGH BYTE
        SHL     ;PUT SIGH INTO DF
        GHI RF  ;HIGH BYTE AGAIN
        SHRC    ;RIGHT SHIFT WITH CARRY
        PHI RF  ;RESET HIGH BYTE
;
        GLO RF  ;GET LOW BYTE
        SHRC    ;RIGHT SHIFT WITH CARRY
        PLO RF  ;RESET LOW BYTE
;
        DEC RD  ;DECREMENT COUNTER
        GLO RD  ;GET COUNTER
        BNZ ASHR1
                ;SHIFT TILL ZERO
;
        SEP R3  ;RESET PROGRAM COUNTER
;*************************************
;
; ARITHMETIC RIGHT SHIFT OF R0,R1
;
; NUMBER OF SHIFTS PASSED IN RD.0
;
; CALLED BY:
;      RLDI RD,NUMSFT
;      LDN RD
;      PLO RD
;      RLDI R6,SCALE
;      SEP R6
;
; USES REGISTER RD.0
;
;************************************
SCALE:  GLO RD  ;GET NUMBER SHIFTS
        BZ SEXIT
                ;EXIT IF ZERO
;*************************************
SCALE1: GHI R0  ;GET MSB
        SHL     ;PUT SIGN IN DF
        GHI R0  ;GET MSB AGAIN
        SHRC    ;RIGHT SHIFT WITH DF
        PHI R0  ;RESET MSB
;
        GLO R0
        SHRC
        PLO R0
;
        GHI R1
        SHRC
        PHI R1
;
        GLO R1  ;GET LSB
        SHRC    ;RIGHT SHIFT WITH DF
        PLO R1  ;RESET LSB
;
        DEC RD  ;DECREMENT COUNTER
        GLO RD  ;GET COUNTER
        BNZ SCALE1
                ;SHIFT TILL ZERO
;
SEXIT:  SEP R3
;*************************************
;
; ADDITION ROUTINE
; 32 BIT 2'S COMPLEMENT ADDITION
; R0,R1 = R0,R1 +RE,RF
;
; CALLED BY :
;
;      RLDI R6,ADDER
;      SEX R2
;      SEP R6
;
;*************************************
ADDER:  GLO R1  ;GET LSB
        STR R2  ;PUT ON STACK
        GLO RF  ;GET LOW BYTE
        ADD     ;ADD LOW BYTES
        PLO R1  ;RESET LSB
;
        GHI R1
        STR R2
        GHI RF
        ADC 
        PHI R1
;
        GLO R0
        STR R2
        GLO RE
        ADC
        PLO R0
;
        GHI R0  ;GET MSB
        STR R2  ;PUT ON STACK
        GHI RE  ;GET HIGH BYTE
        ADC     ;ADD BYTES WITH CARRY
        PHI R0  ;RESET MSB
;
        SEP R3  ;RESET PROGRAM COUNTER
;*************************************
;
; 16 BIT SIGNED MULT ROUTINE
;
; RF * M(RD) = RE (HIGH) AND RF (LOW)
;
;*************************************
MULT:   GHI RF
        XOR
        ANI	$80
        PHI RC
        LDI	$10
        PLO RC
        LDI	$00
        PHI RE
        PLO RE
;*************************************
MPL:    DEC RC
        GHI RF
        SHR
        PHI RF
        GLO RF
        SHRC
        PLO RF
        GHI RE
        BNF MPB
        INC RD
        GLO RC
        BNZ MPA
        GLO RE
        SM
        PLO RE
        DEC RD
        GHI RE
        SMB
        PHI RE
        GHI RC
;*************************************
MPB:    SHL
        BR MPS+$03
MPA:    GLO RE
        ADD
        PLO RE
        DEC RD
        GHI RE
        ADC
        PHI RE
;**********************************
MPS:    LSDF
        LDX
        SHL
MSP3:   GHI RE
        SHRC
        PHI RE
        GLO RE
        SHRC
        PLO RE
        BNF MT
        GHI RF
        ORI $80
        PHI RF
;*************************************
MT:     GLO RC
        BNZ MPL
;*************************************
MPX:    GHI RE
        SHL
        BNF MEXIT
        BNZ MEXIT
        SHR
        PHI RE
;*************************************
MEXIT:  SEP R3
;*************************************
;
; DIVISION ROUTINE
;
; RF = RE,RF/M(RD)
;
;*************************************
DIVQ:   GHI RE  ;
        SHL     ;
        LDI $90 ;
        LSNF    ;
        LDI $50 ;
        PLO RC  ;
DVL:    GLO RE  ;
        SHL     ;
        PLO RE  ;
        GHI RE  ;
        SHLC    ;
        PHI RE  ;
;
        GLO RF  ;
        SHL     ;
        PLO RF  ;
        GHI RF  ;
        SHLC    ;
        PHI RF  ;
;
        BNF DSEX
;
        INC RE  ;
;
DSEX:   SEX RD  ;
        GLO RC  ;
        XOR     ;
        SHL     ;
        INC RD  ;
        GLO RE  ;
        BNF DSA ;
        SM      ;
        PLO RE  ;
        DEC RD  ;
        GHI RE  ;
        SMB     ;
        BR DSM  ;
;
DSA:    ADD     ;
        PLO RE  ;
        DEC RD  ;
        GHI RE  ;
        ADC     ;
DSM:    PHI RE  ;
        DEC RC  ;
        GLO RC  ;
        ANI $7F ;
        BNF DCR
;
        INC RF  ;
        ORI $80 ;
DCR:    PLO RC  ;
        ANI $3F ;
        BNZ DVL ;
        BDF DVR ;
        SKP     ;
;
DVC:    INC RF  ;
        XOR     ;
        SHL     ;
        INC RD  ;
        GLO RE  ;
        BDF DVM ;
        ADD     ;
        PLO RE  ;
        DEC RD  ;
        GHI RE  ;
        ADC     ;
        BR DVR-$01
;
DVM:    SM      ;
        PLO RE  ;
        DEC RD  ;
        GHI RE  ;
        SMB     ;
        PHI RE  ;
;
DVR:    GHI RE  ;
        BNZ DCR1        ;
        GLO RE  ;
        BZ DVN  ;
DCR1:   GLO RC  ;
        SHL     ;
        SHL     ;
        GHI RE  ;
        LSDF    ;
        XRI $80 ;
        ADI $80 ;
        BNF DVC ;
;
DVN:    LDX     ;
        SHL     ;
        BNF DVX ;
        PLO RF  ;
        GHI RF  ;
        XRI $FF ;
        PHI RF  ;
        INC RF  ;
;
        ADI $00 ;CLEAR DF
;
DVX:    SEP R3  ;RETURN
;*************************************
;
; END OF 2K EPROM $0000-$07FF
;
;*************************************
;
; START OF ADDRESS SPACE IN 2ND EPROM
;
;*************************************
        ORG $0800
;*************************************
;
;
; ALTERNATE PROGRAM
;
;
;*************************************
TLXALT: NOP     ;NO OPERATION
;*************************************
;
; TEST ROUTINE
;
; IF PRESET ALTITUDE SET LESS THAN
; 900 FEET RUN ALTERNATE CONTROLLER
; PROGRAM. OTHERWISW USE PRESET FOR
; TEST CODE.
;
;*************************************
        RLDI RD,PREHUN
        LDN RD
        SMI $09
        LBNZ ALTCON
;*************************************
;
; READ TENS DIGIT FOR TEST CODE
;
;*************************************
        RLDI RD,PRETEN
        LDN RD
        LBZ TEST0
;
        SMI $01
        LBZ TEST1
;
        SMI $01
        LBZ TEST2
;
        SMI $01
        LBZ TEST3
;
        SMI $01
        LBZ TEST4
;
        SMI $01
        LBZ TEST5
;
        SMI $01
        LBZ TEST6
;
        SMI $01
        LBZ TEST7
;
        SMI $01
        LBZ TEST8
;
        LBR TEST9
;*************************************
; TEST ROUTINE JUMP TABLE
;*************************************
TEST0:  NOP
	NOP
	NOP
TEST1:  NOP
	NOP
	NOP
TEST2:  NOP
	NOP
	NOP
TEST3:  NOP
	NOP
	NOP
TEST4:  NOP
	NOP
	NOP
TEST5:  NOP
	NOP
	NOP
TEST6:  NOP
	NOP
	NOP
TEST7:  NOP
	NOP
	NOP
TEST8:  NOP
	NOP
	NOP
TEST9:  NOP
	NOP
	NOP
;*************************************
;
; RETURN TO MAIN COMMAND LINK LOOP
;
;*************************************
TESTX:  NOP
	NOP
	NOP
;
        LBR GRABER
;*************************************
;
; ALTERNATE CONTROLLER
;
;*************************************
        ORG $0C00
;*************************************
ALTCON: NOP     ;NO OPERATION
;*************************************
	END

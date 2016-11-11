	LIST
	TITLE	MANCHESTER DECODER DATA VIEWER
	SUBTTL	(C)1984,85,86,87,88 HAYES INTERNATIONAL CORPORATION
	SECTION TLXTEST,$0000
;******************************************************************************
;
;	TLX-1 TEST SOFTWARE
;
;
;	(C) 1988 HAYES INTERNATIONAL CORP.
;
;	BY DENTON MARLOWE
;
;******************************************************************************
ORGIN	EQU	$0000		; START ADDRESS OF THIS CODE
STACK	EQU	$F000		; TEST ROUTINE RAM
LIMIT	EQU	$2000		; END OF TABLE PLUS 1 (USE 2000 FOR SHORT)
DEBUG	EQU	$F000		; BASE ADDRESS OF TERMINAL I/O ROUTINES
;******************************************************************************
OSTRN1	EQU	DEBUG+$0800
OSTRNG	EQU	DEBUG+$0804
;**************************
TYPEH1	EQU	DEBUG+$0810
TYPEHD	EQU	DEBUG+$0814
TYPEH	EQU	DEBUG+$0816
TYPED1	EQU	DEBUG+$0827
TYPED	EQU	DEBUG+$082B
TYPER	EQU	DEBUG+$082D
;**************************
RCA1	EQU	DEBUG+$0874
DELAY1	EQU	DEBUG+$0889
;**************************
READHX	EQU	DEBUG+$0898
READH1	EQU	DEBUG+$08B8
READHD	EQU	DEBUG+$08BC
READH	EQU	DEBUG+$08BE
READ1	EQU	DEBUG+$0900
READD	EQU	DEBUG+$0904
READ	EQU	DEBUG+$0906
;**************************
TYPE31	EQU	DEBUG+$0933
TYPE3D	EQU	DEBUG+$0937
TYPE3	EQU	DEBUG+$0939
TYPE61	EQU	DEBUG+$094A
TYPE6D	EQU	DEBUG+$094E
TYPE6	EQU	DEBUG+$0950
TYPE81	EQU	DEBUG+$095F
TYPE8D	EQU	DEBUG+$0963
TYPE8	EQU	DEBUG+$0965
;**************************
WORD31	EQU	DEBUG+$096E
WORD3D	EQU	DEBUG+$0972
WORD3	EQU	DEBUG+$0974
WORD61	EQU	DEBUG+$0994
WORD6D	EQU	DEBUG+$0998
WORD6	EQU	DEBUG+$099A
WORD81	EQU	DEBUG+$09AC
WORD8D	EQU	DEBUG+$09B0
WORD	EQU	DEBUG+$09B2
;**************************
TOUT31	EQU	DEBUG+$09C8
TOUT3D	EQU	DEBUG+$09CC
TOUT3	EQU	DEBUG+$09CE
MOUT31	EQU	DEBUG+$09EE
MOUT3D	EQU	DEBUG+$09F2
MOUT3	EQU	DEBUG+$09F4
TOUT61	EQU	DEBUG+$0A4B
TOUT6D	EQU	DEBUG+$0A4F
TOUT6	EQU	DEBUG+$0A51
MOUT1	EQU	DEBUG+$0A76
MOUTD	EQU	DEBUG+$0A7A
MOUT	EQU	DEBUG+$0A7C
T8OUT1	EQU	DEBUG+$0AB5
T8OUTD	EQU	DEBUG+$0AB9
T8OUT	EQU	DEBUG+$0ABB
M8OUT1	EQU	DEBUG+$0AD7
M8OUTD	EQU	DEBUG+$0ADB
M8OUT	EQU	DEBUG+$0ADD
;**************************
CLEAR	EQU	DEBUG+$0B6F
CRLF	EQU	DEBUG+$0B1B
MLFCR	EQU	DEBUG+$0B40
RVON	EQU	DEBUG+$0B8E
RVOFF	EQU	DEBUG+$0B99
CURON	EQU	DEBUG+$0C18
CUROFF	EQU	DEBUG+$0C23
ITEST	EQU	DEBUG+$0D0C
;******************************************************************************
	ORG	ORGIN
;******************************************************************************
        DIS				; DISABLE INTERRUPTS
	DB	$00			; SET SP AND PC = R0

	OUT	1			; SELECT A/D-D/A I/O
	DB	$30
	OUT	3			; D/A CH 1
	DB	$00
	OUT	4			; D/A CH 2
	DB	$00

        OUT 	1			; SELECT I/O GROUP 8
	DB	$08			; I/O GROUP 8	
        OUT 	2
	DB	$0F			; A IS INPUT
        OUT 	2
	DB	$F7			; B IS BIT PROG
        OUT 	2
	DB	$F0			; LOW IN,HIGH OUT

	RLDI	R2,USER			; SETUP FOR NEW SP
        SEX 	R2			; NEW SP IS R2
	RLDI	R3,MENU			; SETUP FOR NEW PC
	SEP	R3			; NEW PC IS R3
;******************************************************************************
;
;	MAIN TEST AND FUNCTION MENU
;
;******************************************************************************
MENU:	NOP				;
;	SCAL	R6,CLEAR		; CLEAR SCREEN
;	SCAL	R6,CLEAR		; CLEAR SCREEN
;	SCAL	R6,RVOFF		; NORMAL VIEDO
;	SCAL	R6,CURON		; TURN CURSOR ON
;******************************************************************************
	SCAL	R6,OSTRN1		; PRINT MESSAGE
	DB	$0A,$0D			;
	DB	'AMERICAN AUTOMATION DEVELOPMENT SYSTEM DATA REDUCTION'
	DB	$0A,$0D
	DB	'VERSION 1.0/25DEC88'
	DB	$0A,$0D
	DB	'WRITTEN BY DENTON MARLOWE'
	DB	$0A,$0D
	DB	'COPYRIGHT 1988 HAYES TARGETS'
	DB	$0A,$0A,$0A,$0D
	DB	'TLX DEVELOPMENT SYSTEM MAIN TEST MENU',$0A,$0D
	DB	$0A,$0D	
	DB	'(1) D/A CALIBRATION',$0A,$0D
	DB	'(2) DECODER VIEWER',$0A,$0D
	DB	'(3) DECODER SCANNER',$0A,$0D
	DB	'(4) DECODER CAPTURE',$0A,$0D
	DB	'(5) DECODER PLAYBACK',$0A,$0D
	DB	$0A,$0D
	DB	'ENTER TEST NUMBER '
	DB	$00

	SCAL	R6,READD		; KEYBOARD INPUT

	GHI	RF			; GET KEYPRESS
	SMI	'1'			; ONE
	LBZ	DACAL			; D/A CALIBRATION

	GHI	RF			; GET KEYPRESS
	SMI	'2'			; TWO
	LBZ	VIEW			; DECODER VIEWER

	GHI	RF			; GET KEYPRESS
	SMI	'3'			; THREE
	LBZ	SCANER			;

	GHI	RF			; GET KEYPRESS
	SMI	'4'			; FOUR
	LBZ	CAPT			;

	GHI	RF			; GET KEYPRESS
	SMI	'5'			; FIVE
	LBZ	PLAYB			;

	LBR	MENU		       ; TRY AGAIN
;******************************************************************************
;
;	END OF MENU
;
;******************************************************************************




;******************************************************************************
;
;	D/A OUPUT CALIBRATION
;
;******************************************************************************
DACAL:	SCAL	R6,OSTRN1		; PRINT MESSAGE
        DB	$0A,$0D
	DB	'READY TO START D/A OUTPUT CALIBRATION'
	DB	$00

	SCAL	R6,PRESSA		; KEYBOARD INPUT
;******************************************************************************
;
;	INITIALIZE OUTPUT DATA TO ZERO
;
;******************************************************************************
	RLDI	R1,$0000		; ZERO DATA
;******************************************************************************
;
;	SETUP D/A I/O MODE
;
;******************************************************************************
DATST1: SEX	R3			; SET PROGRAM COUNTER EQUAL SP
	OUT	1			; SET I/O GROUP
	DB	$30			; I/O GROUP 30
	SEX	R2			; RESET SP TO R2
;******************************************************************************
;
;	OUTPUT DATA TO D/A CHANNELS 1 AND TWO
;
;******************************************************************************
	GLO	R1			; GET DATA
	STR	R2			; STACK IT
	OUT	3			; OUTPUT DATA TO CHANNEL 1
	DEC	R2			; RESET STACK POINTER
	OUT	4			; OUTPUT DATA TO CHANNEL 2
	DEC	R2			; RESET STACK POINTER
;******************************************************************************
;
;	DISPLAY D/A OUTPUT DATA
;
;******************************************************************************
	RLDI	R7,$0001	    	; SET INITIAL CHANNEL NUMBER
	RLDI	RB,$0002	    	; TWO CHANNELS

	SCAL	R6,OSTRN1	    	; PRINT MESSAGE
	DB	'D/A OUTPUT'
	DB	$0A,$0A,$0D
	DB	$00

01$:	SCAL	R6,OSTRN1	    	; PRINT MESSAGE
	DB	'CH ',$00	    	; CHANNEL
	GLO	R7		    	; GET CHANNEL NUMBER
	PHI	RF		    	; LOAD REGISTER
	SCAL	R6,TYPEH	    	; PRINT CHANNEL NUMBER
	SCAL	R6,OSTRN1	    	; PRINT MESSAGE
	DB	'  ',$00	    	; SPACE OVER
	GLO	R1		    	; LOAD DATA
	PHI	RD		    	; LOAD REGISTER
	SCAL	R6,WORD 	    	; DISPLAY AS BITS
	DB	$00		    	; TERMINATION
	SCAL	R6,OSTRN1           	; PRINT MESSAGE
	DB	'  ',$00            	; SPACE OVER
	GLO	R1		    	; LOAD DATA
	PHI	RF		    	; LOAD REGISTER
	SCAL	R6,TYPEH	    	; DISPLAY AS HEX

	SCAL	R6,OSTRN1	    	; PRINT MESSAGE
	DB	$0A,$0D,$00	    	; NEW LINE

	INC	R7		    	; INCREMENT CHANNEL NUMBER
	DBNZ	RB,01$		    	; LOOP FOR 2 CHANNELS

	SCAL	R6,OSTRN1	    	; PRINT MESSAGE
	DB	$0A,$0A,$0D,$00     	;
;******************************************************************************
;
;	DELAY 3.75 SECONDS TO GIVE D/A OUTPUTS TIME TO SETTLE
;
;******************************************************************************
	RLDI	RE,$04		    	; LOAD OUTER COUNTER
02$:	RLDI	RF,$AFFF	    	; LOAD INNER COUNTER
03$:	NOP			    	; NO OPERATION
	DBNZ	RF,03$		    	; DECREMENT INNER COUNTER
	DBNZ	RE,02$		    	; DECREMENT OUTER COUNTER
;******************************************************************************
;
;	INCREMENT OUTPUT DATA 1/3 FULL SCALE (55)
;	INCREMENT OUTPUT DATA 1/5 FULL SCALE (33)
;	INCREMENT OUTPUT DATA 1/17 FULL SCALE (F)
;	INCREMENT OUTPUT DATA 1/51 FULL SCALE (5)
;
;*****************************************************************************
	GLO	R1		; GET DATA
	SMI	$FF		; OUTPUT DATA MAX
	BZ	05$		; IF DATA MAX, EXIT

	GLO	R1		; GET DATA
	ADI	$33		; ADD 1/5 FULL SCALE
	PLO	R1		; RESET DATA

04$:	LBR	DATST1		; LOOP 
;******************************************************************************
;
;	RESET D/A OUTPUTS TO ZERO
;
;*****************************************************************************
05$:	RLDI	R2,USER 	; SETUP INPUT STACK
	SEX	R3		; SET PROGRAM COUNTER EQUAL SP
	OUT	1		; SET I/O GROUP
	DB	$30		; I/O GROUP 30
	OUT	3		; OUTPUT DATA TO D/C CHANNEL 1
	DB 	$00
	OUT	4		; OUTPUT DATA TO D/A CHANNEL 2
	DB	$00
	SEX	R2		; RESET SP TO R2
;******************************************************************************
;
;	REPEAT CALIBRATION OR RETURN TO MAIN MENU
;
;*****************************************************************************
        SCAL	R6,REPEAT	; CALL REPEAT QUESTION ROUTINE
	LBZ	DACAL		; REPEAT TEST IF D=0

	LBR	MENU		; RETURN MENU
;******************************************************************************
;
;	END OF D/A CALIBATION
;
;******************************************************************************
;
;	REPEAT PROCESS QUESTION ROUTINE
;
;******************************************************************************
REPEAT: SCAL R6,OSTRN1		; PRINT MESSAGE
	DB	$0A,$0A,$0D
	DB	'REPEAT TEST (Y OR N) ? ',$00

	SCAL	R6,READD	; READ AFTER DELAY
	GHI	RF		; GET USER INPUT
	SMI	'Y'		; YES
	SRET	R6		; RETURN D=0 FOR YES
;******************************************************************************
;
;	PRESS ANY KEY ROUTINE
;
;******************************************************************************
PRESSA:	SCAL	R6,OSTRN1		; OUTPUT
	DB	$0A,$0D
	DB	'PRESS ANY KEY '
	DB	$00
	SCAL	R6,READ			; GET KEYPRESS
	SCAL	R6,CRLF			; LINE FEED
	DB	$01			; ONLY ONE
	SRET	R6			; RETURN
;******************************************************************************
;
;	END OF MENU ITEM 1 - D/A CALIBRATION
;
;******************************************************************************




;******************************************************************************
;
;	DECODER MENU ITEM 2 - DECODER CHANNEL VIEWER
;
;******************************************************************************
VIEW:	SCAL	R6,OSTRN1		; PRINT MESSAGE
        DB	$0A,$0D
	DB	'DECODER VIEWER'
	DB	$0A,$0D,$00
;******************************************************************************
;
;
;
;******************************************************************************
	RLDI	R1,$0001		; INITIALIZE TO CHANNEL ONE
	RLDI	R0,$0000		; INITIALIZE LINE COUNTER
;******************************************************************************
;
;	INPUT PIO DATA
;
;******************************************************************************
PIO:	SEX	R3			; SET SP TO PC
        OUT 	1			; SELECT I/O GROUP 8
	DB	$08
        SEX 	R2			; RESET SP TO R2
WAIT:	B2 	WAIT			; WAIT FOR DATA VALID
;******************************************************************************
        INP 	6    			; ADDRESS
        IRX
        INP 	4    			; DATA
        DEC 	R2
        LDX
        ANI 	$0F
        STR 	R2
        PHI 	R7
;******************************************************************************
        GLO 	R1			; LOAD CHANNEL NUMBER
        SM				; COMPARE TO INPUT CHANNEL
        BNZ 	WAIT			; IF NOT SAME WAIT FOR NEXT INPUT
;******************************************************************************
        IRX				; INCREMENT STACK TO DATA
        LDX				; LOAD DATA
        PLO 	R7			; STASH IN REGISTER
        DEC 	R2			; RESET STACK POINTER
;******************************************************************************
        INC 	R1			; INCREMENT TO NEXT CHANNEL
        GLO 	R1			; LOAD NEXT CHANNEL
	SMI	$0A			; $0A PAST CHANNEL 9?
        BNZ 	OUT			; IF NOT OUTPUT, OTHERWISE-
        RLDI 	R1,$0001		; RESET CHANNEL TO ONE
;******************************************************************************
;
;	OUTPUT CHANNEL NUMBER AND INPUT BYTE TO SCREEN
;
;******************************************************************************
OUT:	SCAL	R6,OSTRN1
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
	DB	$02,$01

;	 BN4	 04$		 ; ABORT ON BREAK

	INC	R0		    ; INCREMENT LINE COUNTER
	GLO	R0		    ; GET COUNT
	SMI	$09		    ; 9 LINES
	LBNZ	PIO		    ; IF LESS NEXT CHANNEL, OTHERWISE-

;	SCAL	R6,MLFCR	    ; LINE UP
;	DB	$12		    ; 18 TIMES
	RLDI	R0,$0000	    ; RESET LINE COUNTER

	LBR	PIO
;****************************************************************************
;
;	RTE1/RTD1 TEST ABORTED
;
;****************************************************************************
04$:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'**** RTE1/RTD1 TEST ABORTED ****'
	DB	$0A,$0D,$00
;******************************************************************************
;
;	REPEAT PROCESS
;
;*****************************************************************************
05$:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D,$00	; 1 LINES DOWN
        SCAL	R6,REPEAT	; CALL REPEAT QUESTION ROUTINE
	LBZ	VIEW		; REPEAT TEST IF D=0
	LBR	MENU		; RETURN TO MAIN TEST MENU
;******************************************************************************
;
;	END OF MENU ITEM 2 - DECODER CHANNEL VIEWER
;
;******************************************************************************




;******************************************************************************
;
;	DECODER MENU ITEM 3 - DECODER 2 CHANNEL SCANER
;
;******************************************************************************
;
;	DECODER SCANNER
;
;******************************************************************************
SCANER:	SCAL	R6,OSTRN1		;
	DB	$0A,$0D
	DB	"ENTER FIRST CHANNEL "
	DB	$00

	SCAL	R6,READHX		; KEYBOARD INPUT

	GLO	RD			; GET KEYPRESS
	PLO	R4			; ONE

	SCAL	R6,OSTRNG		;
	DB	$0A,$0D
	DB	"ENTER SECOND CHANNEL "
	DB	$00

	SCAL	R6,READHX		; KEYBOARD INPUT

	GLO	RD			; GET KEYPRESS
	PHI	R4			; TWO

	SCAL	R6,PRESSA			;		
;******************************************************************************
;
;	SETUP DATA TABLE HEADER
;
;******************************************************************************
	RLDI 	RD,TABLE		; POINTER TO DATA TABLE
	LDI	$02			; 2 CHANNELS
	STR	RD			; STORE IN TABLE
	INC	RD			;
	GLO	R4			; FIRST CHANNEL
	STR	RD			; STORE IN TABLE
	INC	RD			;
	GHI	R4			; SECOND CHANNEL
	STR	RD			; STORE IN TABLE
	RLDI	RD,TABLE+$0020		;
	RLDI	RA,$0000		; ZERO DATA COUNTER
;******************************************************************************
;
;	INPUT PIO DATA
;
;******************************************************************************
PIOS:	SEX	R3			; SET SP TO PC
        OUT 	1			; SELECT I/O GROUP 8
	DB	$08
	SEX	R2			; RESET SP TO R2
;******************************************************************************
;
;	FIRST CHANNEL
;
;******************************************************************************
	GLO	R4
	PLO	R1
	LDI	$00
	PHI	R1
;******************************************************************************	
01$:	B2	01$			; WAIT FOR DATA VALID
;******************************************************************************
        INP 	6    			; ADDRESS
        IRX
        INP 	4    			; DATA
        DEC 	R2
        LDX
        ANI 	$0F
        STR 	R2
;******************************************************************************
        GLO 	R1			; LOAD CHANNEL NUMBER
        SM				; COMPARE TO INPUT CHANNEL
	BNZ	01$			; IF NOT SAME WAIT FOR NEXT INPUT
;******************************************************************************
	IRX				; INCREMENT STACK TO DATA
        LDX				; LOAD DATA
        PLO 	R7			; STASH IN REGISTER
        DEC 	R2			; RESET STACK POINTER
;******************************************************************************
;
;	SECOND CHANNEL
;
;******************************************************************************
	GHI	R4 ; INITIALIZE TO SECOND CHANNEL
	PLO	R1
	LDI	$00
	PHI	R1
;******************************************************************************
WAIT2:	B2	WAIT2			; WAIT FOR DATA VALID
;******************************************************************************
        INP 	6    			; ADDRESS
        IRX
        INP 	4    			; DATA
        DEC 	R2
        LDX
        ANI 	$0F
        STR 	R2
;******************************************************************************
        GLO 	R1			; LOAD CHANNEL NUMBER
        SM				; COMPARE TO INPUT CHANNEL
	LBNZ	WAIT2			; IF NOT SAME WAIT FOR NEXT INPUT
;******************************************************************************
        IRX				; INCREMENT STACK TO DATA
        LDX				; LOAD DATA
	PHI	R7			; STASH IN REGISTER
        DEC 	R2			; RESET STACK POINTER
;******************************************************************************
;
;	OUTPUT SELECTED CHANNELS TO D/A CONVERTER
;
;******************************************************************************
OUTS:	SEX	R3
	OUT	1
	DB	$30
	SEX	R2

	GLO	R7
	STR	R2
	OUT	3
	DEC	R2

	GHI	R7
	STR	R2
	OUT	4
	DEC	R2

	INC	RA			; INCREMENT DATA COUNTER

	LBR	PIOS

;******************************************************************************
;
;	STORE DATA IN TABLE
;
;******************************************************************************	

	GLO	R7			; GET FIRST CHANNEL
	STR	RD			; STORE IN TABLE
	INC	RD			;
	GHI	R7			; GET SECOND CHANNEL
	STR	RD			; STORE IN TABLE
	INC	RD			;

;******************************************************************************
;
;	CHECK FOR FULL TABLE
;
;******************************************************************************

	GHI	RD			; GET HIGH BYTE OF TABLE
	SMI	HIGH(LIMIT)		; 4 KILOBYTES OF DATA
	LBNF	PIOS			; NEXT CYCLE

	LDI	$00			; RESET D/A TO ZERO
	STR	R2
	OUT	3
	DEC	R2
	OUT	4
	DEC	R2

	SCAL	R6,OSTRN1		; OUTPUT MESSAGE
	DB	$0A,$0D
	DB	"END OF SAMPLE"
	DB	$00

	SCAL	R6,PRESSA			;
	LBR	MENU			; RETURN TO MENU
;******************************************************************************
;
;	END OF MENU ITEM 3 - DECODER 2 CHANNEL SCANER
;
;******************************************************************************




;******************************************************************************
;
;	DECODER MENU ITEM 4 - DECODER CAPTURE
;
;******************************************************************************
;
;	DECODER CAPTURE FUNCTION - READS ALL 9 CHANNELS TO TABLE
;
;******************************************************************************
CAPT:	SCAL	R6,OSTRNG		;
	DB	$0A,$0D
	DB	"ENTER HIGH BYTE OF TABLE LIMIT "
	DB	$00

	RLDI	RD,$0000		; SET INPUT TO ZERO
	SCAL	R6,READHX		; KEYBOARD INPUT

	RLDI	RA,TABTOP		; POINTER TO LIMIT OF DATA TABLE
	LDI	HIGH(LIMIT)		; GET DEFAULT
	STR	RA			; STORE AT LOCATION

	GLO	RD			; GET INPUT BYTE
	BZ	CAPT1			; NO INPUT USE DEFAULT
	STR	RA			; STORE AT LOCATION

CAPT1:	SCAL	R6,PRESSA		; KEYBOARD INPUT

	SCAL	R6,OSTRNG		;
	DB	$0A,$0D
	DB	"SCAN STARTED"
	DB	$0A,$0D
	DB	$00
;******************************************************************************
;
;	SETUP DATA TABLE HEADER
;
;******************************************************************************
	RLDI 	RD,TABLE		; POINTER TO DATA TABLE
	LDI	$09			; 9 CHANNELS
	STR	RD			; STORE IN TABLE
	INC	RD			;
	LDI	$01			; FIRST CHANNEL
	STR	RD			; STORE IN TABLE
	INC	RD			;
	LDI	$02			; SECOND CHANNEL
	STR	RD			; STORE IN TABLE
	INC	RD			;
	LDI	$03			; THIRD CHANNEL
	STR	RD			; STORE IN TABLE
	INC	RD			;
	LDI	$04			; CHANNEL 4
	STR	RD			; STORE IN TABLE
	INC	RD			;
	LDI	$05			; CHANNEL 5
	STR	RD			; STORE IN TABLE
	INC	RD			;
	LDI	$06			; CHANNEL 6
	STR	RD			; STORE IN TABLE
	INC	RD			;
	LDI	$07			; CHANNEL 7
	STR	RD			; STORE IN TABLE
	INC	RD			;
	LDI	$08			; CHANNEL 8
	STR	RD			; STORE IN TABLE
	INC	RD			;
	LDI	$09			; CHANNEL 9
	STR	RD			; STORE IN TABLE
	INC	RD			;

	RLDI	RD,TABLE+$0020		;
;******************************************************************************
;
;	INPUT PIO DATA
;
;******************************************************************************
	SEX	R3			; SET SP TO PC
        OUT 	1			; SELECT I/O GROUP 8
	DB	$08
	SEX	R2			; RESET SP TO R2
;******************************************************************************
;
;	FIRST CHANNEL
;
;******************************************************************************
CH1:	RLDI	R1,$0001
;******************************************************************************
VALID:	B2	VALID			; WAIT FOR DATA VALID
;******************************************************************************
        INP 	6    			; ADDRESS
        IRX
        INP 	4    			; DATA
        DEC 	R2
        LDX
        ANI 	$0F
        STR 	R2
;******************************************************************************
        GLO 	R1			; LOAD CHANNEL NUMBER
        SM				; COMPARE TO INPUT CHANNEL
	BNZ	VALID			; IF NOT SAME WAIT FOR NEXT INPUT
;******************************************************************************
	IRX				; INCREMENT STACK TO DATA
        LDX				; LOAD DATA
        PLO 	R7			; STASH IN REGISTER
	DEC	R2			; RESET STACK POINTER
;******************************************************************************
;
;	STORE DATA IN TABLE
;
;******************************************************************************	
	GLO	R7			; GET FIRST CHANNEL
	STR	RD			; STORE IN TABLE
	INC	RD			;
;******************************************************************************
;
;	CHECK FOR FULL TABLE
;
;******************************************************************************
	SEX	RA			;
	RLDI	RA,TABTOP		;
	GHI	RD			;
	SM				;
	SEX	R2			;
	BDF	EXIT			;
;******************************************************************************
;
;	INCREMENT TO NEXT CHANNEL
;
;******************************************************************************	
	INC	R1			; INCREMENT TO NEXT CHANNEL
	GLO	R1			; GET CHANNEL
	SMI	$0A			; PAST CHANNEL 9
	LBNZ	VALID			; NEXT CHANNEL
	LBR	CH1			; RESET TO CHANNEL 1
;******************************************************************************
;
;	TABLE FULL QUIT
;
;******************************************************************************
EXIT:	SCAL	R6,OSTRN1		; OUTPUT MESSAGE
	DB	$0A,$0D
	DB	"END OF SAMPLE"
	DB	$00

	SCAL	R6,PRESSA 		;

	LBR	MENU			; STOP
;******************************************************************************
;
;	END OF MENU ITEM 4 - DECODER CAPTURE
;
;******************************************************************************



;******************************************************************************
;
;	DECODER MENU ITEM 5 - DECODER PLAYBACK
;
;******************************************************************************
;
;	TIME CYCLE
;
;******************************************************************************
PLAYB:	SCAL	R6,OSTRN1		;
	DB	$0A,$0D
	DB	'ENTER HEX WORD FOR DELAY FACTOR '
	DB	$00

	RLDI	RD,$0000		; SET HEX INPUT TO ZERO
	SCAL	R6,READHX		;
	RLDI	R9,FACTOR		;
	LDI	$0C			;
	STR	R9			;
	INC	R9			;
	LDI	$6A			;
	STR	R9			;

	GLO	RD			;
	BZ	PLAYC			; HIGH BYTE MUST BE NON-ZERO
	STR	R9			;
	DEC	R9			;
	GHI	RD			;
	STR	R9			;
;*****************************************************************************	
PLAYC:	SCAL	R6,OSTRN1		;
	DB	$0A,$0D
	DB	"ENTER FIRST CHANNEL "
	DB	$00

	SCAL	R6,READHX		; KEYBOARD INPUT

	DEC	RD			;
	GLO	RD			; GET KEYPRESS
	PLO	R4			; ONE

	SCAL	R6,OSTRNG		;
	DB	$0A,$0D
	DB	"ENTER SECOND CHANNEL "
	DB	$00

	SCAL	R6,READHX		; KEYBOARD INPUT

	DEC	RD			;
	GLO	RD			; GET KEYPRESS
	PHI	R4			; TWO

	SCAL	R6,OSTRN1		;
	DB	$0A,$0D
	DB	"READY TO START PLAYBACK"
	DB	$00

	SCAL	R6,PRESSA			;
;******************************************************************************
;
;	DETERMINE ADDRESS OF FIRST TWO DATA POINTS
;
;******************************************************************************
	RLDI	RA,TABLE+$0020		; RA POINTS TO CHANNEL 1
	RLDI	RB,TABLE+$0020		; RB POINTS TO CHANNEL 2

	GLO	R4			; GET FIRST CHANNEL OFFSET
	STR	R2			; STACK IT
	GLO	RA			; GET POINTER TO FIRST CHANNEL
	ADD				; ADD IN CHANNEL OFFSET
	PLO	RA			; RESET POINTER TO FIRST CHANNEL

	GHI	R4			; GET SECOND CHANNEL OFFSET
	STR	R2			; STACK IT
	GLO	RB			; GET POINTER TO SECOND CHANNEL
	ADD				; ADD IN CHANNEL OFFSET
	PLO	RB			; RESET POINTER TO SECOND CHANNEL
;******************************************************************************
;
;	OUTPUT DATA TO D/A
;
;******************************************************************************
	SEX	R3			;
	OUT	1			;
	DB	$30			;
	SEX	R2			;
;*****************************************************************************
PLAY:	LDN	RA			; GET FIRST DATA POINT
	STR	R2			; STACK IT
	OUT	3			; OUTPUT BYTE FROM D/A CHANNEL 1
	DEC	R2			; RESET SP

	LDN	RB			; GET SECOND DATA POINT
	STR	R2			; STACK IT
	OUT	4			; OUTPUT BYTE FROM D/A CHANNEL 2
	DEC	R2			; RESET SP
;******************************************************************************
;
;	DELAY LOOP
;
;******************************************************************************
	RLDI	R1,FACTOR		; POINTER TO FACTOR
	LDA	R1			;
	PHI	R9			;
	LDN	R1			;
	PLO	R9			;

;	RLDI	R9,$0C6A		; EXPERIMENTAL VALUE AT 5MhZ

DELAY:	NOP				; NO OPERATION
	DBNZ	R9,DELAY		; DELAY LOOP
;******************************************************************************
;
;	INCREMENT TO NEXT DATA POINT
;
;******************************************************************************
	RLDI	RD,$0009		; 9 TIMES
NEXT:	INC	RA			; INCREMENT POINTER TO FIRST CHANNEL
	INC	RB			; INCREMENT POINTER TO SECOND CHANNEL
	DBNZ	RD,NEXT			;
;******************************************************************************
;
;	CHECK FOR END OF TABLE
;
;******************************************************************************
	SEX	RD			; SET SP
	RLDI	RD,TABTOP		; POINTER TO END OF TABLE
	GHI	RA			; GET HIGH BYTE OF TABLE POINTER
	SM				;
	SEX	R2			; RESET SP
	LBNF	PLAY			; OUTPUT NEXT DATA POINTS

	SEX	R3		; SET PROGRAM COUNTER EQUAL SP
	OUT	1		; SET I/O GROUP
	DB	$30		; I/O GROUP 30
	OUT	3		; OUTPUT DATA TO D/C CHANNEL 1
	DB 	$00
	OUT	4		; OUTPUT DATA TO D/A CHANNEL 2
	DB	$00
	SEX	R2		; RESET SP TO R2

        SCAL	R6,REPEAT	; CALL REPEAT QUESTION ROUTINE
	LBZ	PLAYB		; REPEAT TEST IF D=0

	LBR	MENU			; RESTART
;******************************************************************************
;
;	END OF MENU ITEM 5 - DECODER PLAYBACK
;
;******************************************************************************


;******************************************************************************
;
;	END OF MANCHESTER DECODER SOFTWARE
;
;******************************************************************************
TABTOP:	DB	$00			;
FACTOR:	DW	$0000			;
	DS	$00FF			;
;******************************************************************************
	CORG	$0100
;******************************************************************************
USER:	NOP
;******************************************************************************
	ORG	$1000
;******************************************************************************
TABLE:	NOP				; START OF TABLE
;******************************************************************************
;
;	END OF DATA REDUCTION SOFTWARE
;
;******************************************************************************
	END

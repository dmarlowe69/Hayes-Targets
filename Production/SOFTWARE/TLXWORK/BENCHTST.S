	LIST
	TITLE	1805 CPU / A/D-D/A TEST
	SUBTTL	(C)1984,85,86,87,88 HAYES INTERNATIONAL CORPORATION
	SECTION TLXTEST,$B000
;******************************************************************************
;
;	TLX-1 TEST SOFTWARE
;
;       MODIFIED NOV 13,1990 TO FIX ERROR IN EVENT FLAG TEST
;
;	(C) 1988 HAYES INTERNATIONAL CORP.
;
;	BY DENTON MARLOWE
;
;******************************************************************************
ORGIN	EQU	$B000		; START ADDRESS OF THIS CODE
RAM	EQU	$4000		; BASE ADDRESS OF CPU RAM
STACK	EQU	$3000		; TEST ROUTINE RAM
USER	EQU	$2000		; USER STACK SPACE
TAPERAM	EQU	$8C00		; BASE ADDRESS OF RAM ON TAPE I/O BOARD
TAPETOP	EQU	$8FFF		; TOP OF TAPE I/O BOARD RAM
MASTER	EQU	$C800		; BASE ADDRESS OF MASTER TLX EPROM
DEBUG	EQU	$D000		; BASE ADDRESS OF TERMINAL I/O ROUTINES
EPROM	EQU	$D000		; BASE ADDRESS OF EPROM PROGRAMMING CODE
;******************************************************************************
OSTRN1	EQU	DEBUG+$0800
OSTRNG	EQU	DEBUG+$1804
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
        RLDI 	R2,RAM+$0FFF		; SETUP FOR NEW SP
        SEX 	R2			; NEW SP IS R2
	RLDI	R3,MENU			; SETUP FOR NEW PC
	SEP	R3			; NEW PC IS R3
;******************************************************************************
;
;	MAIN TEST AND FUNCTION MENU
;
;******************************************************************************
MENU:	SCAL	R6,CLEAR		; CLEAR SCREEN
	SCAL	R6,CLEAR		; CLEAR SCREEN
	SCAL	R6,RVOFF		; NORMAL VIEDO
	SCAL	R6,CURON		; TURN CURSOR ON
;******************************************************************************
	SCAL	R6,OSTRN1		; PRINT MESSAGE
	DB	$0A,$0D			;
	DB	'TLX COMPONENT BENCH TEST SOFTWARE'
	DB	$0A,$0D
	DB	'VERSION 1.0/26AUG88'
	DB	$0A,$0D
	DB	'WRITTEN BY DENTON MARLOWE'
	DB	$0A,$0D
	DB	'COPYRIGHT 1988 HAYES TARGETS'
	DB	$0A,$0A,$0A,$0D
	DB	'TLX DEVELOPMENT SYSTEM MAIN TEST MENU',$0A,$0D
	DB	$0A,$0D	
	DB	'(1) MB606 1805 CPU TEST',$0A,$0D
	DB	'(2) MB654A A/D-D/A TEST',$0A,$0D
	DB	'(3) EPROM PROGRAMING FUNCTIONS',$0A,$0D
	DB	'(4) RTE1/RDT1 FUNCTION TEST',$0A,$0D
	DB	'(5) DEBUGGER TEST',$0A,$0D
	DB	$0A,$0D
	DB	'ENTER TEST NUMBER '
	DB	$00

	SCAL	R6,READD		; KEYBOARD INPUT

	GHI	RF			; GET KEYPRESS
	SMI	'1'			; ONE
	LBZ	CPUTST			; CPU TEST

	GHI	RF			; GET KEYPRESS
	SMI	'2'			; TWO
	LBZ	AMENU			; A/D-D/A TEST

	GHI	RF			; GET KEYPRESS
	SMI	'3'			; THREE
	LBZ	IEPROM			; EPROM FUNCTIONS

	GHI	RF			; GET KEYPRESS
	SMI	'4'			; FOUR
	LBZ	RTERTD			; ENCODER/DECODER TEST

	GHI	RF			; GET KEYPRESS
	SMI	'5'			; FIVE
	LBZ	ITEST			; DEBUGGER TEST

	LBR	MENU		       ; TRY AGAIN
;******************************************************************************
;
;	MB654A A/D-D/A TEST MENU
;
;******************************************************************************
AMENU:	SCAL	R6,CLEAR		; CLEAR SCREEN
	SCAL	R6,OSTRN1		; PRINT MESSAGE
	DB	$0A,$0D			;
	DB	'MB654A A/D-D/A CONVERTER TEST MENU',$0A,$0D
	DB	$0A,$0D	
	DB	'(1) A/D-D/A TEST',$0A,$0D
	DB	'(2) A/D CALIBRATION',$0A,$0D
	DB	'(3) D/A CALIBRATION',$0A,$0D
	DB	'(4) EXIT TO MAIN MENU',$0A,$0D
	DB	$0A,$0D
	DB	'ENTER TEST NUMBER '
	DB	$00

	SCAL	R6,READD		; KEYBOARD INPUT

	GHI	RF			; GET KEYPRESS
	SMI	'1'			; ONE
	LBZ	ADTST			; A/D TEST

	GHI	RF			; GET KEYPRESS
	SMI	'2'			; THREE
	LBZ	ADCAL			; A/D CALIBRATION

	GHI	RF			; GET KEYPRESS
	SMI	'3'			; FOUR
	LBZ	DACAL			; D/A CALIBRATION

	GHI	RF			; GET KEYPRESS
	SMI	'4'			; FIVE
	LBZ	MENU			; RETURN TO MAIN MENU

	LBR	AMENU			; TRY AGAIN
;******************************************************************************

		CORG	$0100		; SET NEXT PAGE

;******************************************************************************
;
;	A/D-D/A TEST ROUTINE
;
;******************************************************************************
ADTST:	NOP				; NO OPERATION
	SCAL	R6,CLEAR		; CLEAR SCREEN
	SCAL	R6,OSTRN1		; PRINT MESSAGE
        DB	$0A,$0D
	DB	'MB654A A/D-D/A TEST'
	DB	$0A,$0A,$0D,$00
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
DATST1: RLDI	R2,RAM+$0800		; SETUP INPUT STACK
	SEX	R3			; SET PROGRAM COUNTER EQUAL SP
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
	RLDI	R2,RAM+$0FFF	    	; RESET STACK POINTER FOR DEBUG ROUTINES
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
;	DELAY 3 SECONDS TO GIVE D/A OUTPUTS TIME TO SETTLE
;
;******************************************************************************
	RLDI	RE,$02		    	; LOAD OUTER COUNTER
02$:	RLDI	RF,$FFFF	    	; LOAD INNER COUNTER
03$:	NOP			    	; NO OPERATION
	DBNZ	RF,03$		    	; DECREMENT INNER COUNTER
	DBNZ	RE,02$		    	; DECREMENT OUTER COUNTER
;******************************************************************************
;
;	SETUP A/D FOR SCAN MODE
;
;******************************************************************************
	SEX	R3			; SET PROGRAM COUNTER EQUAL SP
	OUT	1			; SET I/O GROUP
	DB	$30			; I/O GROUP 30
	OUT	6			; SET CONVERSION MODE
	DB	$01			; SCAN MODE LSB=1
	OUT	5			; SELECT FIRST CHANNEL
	DB	$00			; START AT CHANNEL ZERO
	SEX	R2			; RESET SP TO R2
;******************************************************************************
;
;	SETUP INPUT STACK AND CHANNEL COUNTER
;
;******************************************************************************
        RLDI 	R2,RAM+$0800		; SETUP INPUT STACK
	RLDI	R4,$0008		; EIGHT CYCLES
;******************************************************************************
;
;	INPUT AND STACK DATA FROM A/D CONVERTER
;
;******************************************************************************
ADTST2: BN1 	*			; WAIT FOR CONVERSION
        INP 	4			; INPUT AND START NEXT CONVERSION
        DEC 	R2			; DECREMENT SP FOR NEXT INPUT
        DBNZ 	R4,ADTST2		; LOOP FOR 8 CHANNELS
;******************************************************************************
;
;	PRINT OUT A/D INPUT STACK
;
;******************************************************************************
	RLDI	R2,RAM+$0FFF	    ; RESET STACK POINTER FOR DEBUG ROUTINES
	RLDI	RA,RAM+$0800	    ; LOAD STACK OF DATA STACK
	RLDI	RB,$0008	    ; LOAD COUNTER FOR NUMBER OF BYTES
	RLDI	R7,$0001	    ; SET INITIAL CHANNEL NUMBER

	SCAL	R6,OSTRN1	    ; PRINT MESSAGE
	DB	'A/D INPUT'
	DB	$0A,$0A,$0D
	DB	$00

OUTIT:	SCAL	R6,OSTRN1	    ; PRINT MESSAGE
	DB	'CH ',$00	    ; CHANNEL
	GLO	R7		    ; GET CHANNEL NUMBER
	PHI	RF		    ; LOAD REGISTER
	SCAL	R6,TYPEH	    ; PRINT CHANNEL NUMBER
	SCAL	R6,OSTRN1	    ; PRINT MESSAGE
	DB	'  ',$00	    ; SPACE OVER
	LDN	RA		    ; LOAD DATA
	PHI	RD		    ; LOAD REGISTER
	SCAL	R6,WORD 	    ; DISPLAY AS BITS
	DB	$00		    ; TERMINATION
	SCAL	R6,OSTRN1           ; PRINT MESSAGE
	DB	'  ',$00            ; SPACE OVER
	LDN	RA		    ; LOAD DATA
	PHI	RF		    ; LOAD REGISTER
	SCAL	R6,TYPEH	    ; DISPLAY AS HEX

	SCAL	R6,OSTRN1	    ; PRINT MESSAGE
	DB	$0A,$0D,$00	    ; NEW LINE

	DEC	RA	            ; DECREMENT POINTER TO DATA
	INC	R7		    ; INCREMENT CHANNEL NUMBER
	DBNZ	RB,OUTIT	    ; LOOP FOR 8 CHANNELS
;******************************************************************************
;
;	DELAY 15 SECONDS TO ALLOW FOR CHECK OR ABORT
;
;******************************************************************************
	RLDI	R4,$0C		    ; LOAD OUTER COUNTER
02$:	RLDI	R5,$FFFF	    ; LOAD INNER COUNTER
03$:	BN4	06$		    ; ABORT ON BREAK
	DBNZ	R5,03$		    ; DECREMENT INNER COUNTER
	DBNZ	R4,02$		    ; DECREMENT OUTER COUNTER

	SCAL	R6,MLFCR	    ; LINE UP
	DB	$10	            ; 16 TIMES
;******************************************************************************
;
;	INCREMENT OUTPUT DATA 1/4 FULL SCALE
;
;*****************************************************************************
	GLO	R1		; GET DATA
	SMI	$FF		; OUTPUT DATA MAX
	BZ	05$		; IF DATA MAX, EXIT

	GLO	R1		; GET DATA
	ADI	$40		; ADD 1/4 FULL SCALE
	PLO	R1		; RESET DATA

	BNZ	04$		; IF NOT ZERO OK, OTHERWISE ROLLED OVER

	RLDI	R1,$00FF	; RESET DATA TO MAX 

04$:	LBR	DATST1		; LOOP 
;******************************************************************************
;
;	REPEAT PROCESS
;
;*****************************************************************************
05$:	SCAL	R6,RCA1		; SETUP TERMINAL I/O
	SCAL	R6,CRLF		; LINE DOWN
	DB	$0E		; 15 LINES

06$:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D,$00
        SCAL	R6,REPEAT	; CALL REPEAT QUESTION ROUTINE
	LBZ	ADTST		; REPEAT TEST IF D=0
;******************************************************************************
;
;	RESET D/A OUTPUTS TO ZERO AND EXIT TO MAIN MENU
;
;*****************************************************************************
	RLDI	R1,$0000	; ZERO DATA
	RLDI	R2,RAM+$0800	; SETUP INPUT STACK
	SEX	R3		; SET PROGRAM COUNTER EQUAL SP
	OUT	1		; SET I/O GROUP
	DB	$30		; I/O GROUP 30
	SEX	R2		; RESET SP TO R2
	GLO	R1		; GET DATA
	STR	R2		; STACK IT
	OUT	3		; OUTPUT DATA TO CHANNEL 1
	DEC	R2		; RESET STACK POINTER
	OUT	4		; OUTPUT DATA TO CHANNEL 2
	DEC	R2		; RESET STACK POINTER
	LBR	AMENU		; RETURN TO A/D-D/A TEST MENU
;******************************************************************************
;
;	END OF A/D-D/A TEST
;
;******************************************************************************

		CORG	$0100		; SET NEXT PAGE

;*****************************************************************************
;
; A/D CALIBRATION
;
;******************************************************************************
ADCAL:	NOP				; NO OPERATION
	SCAL	R6,CLEAR		; CLEAR SCREEN
	SCAL	R6,OSTRN1		; PRINT MESSAGE
	DB	$0A,$0D
	DB	'MB654A A/D CALIBRATION'
	DB	$0A,$0A,$0A,$0A,$0D,$00
;******************************************************************************
;
;	SETUP CYCLE COUNTER
;
;******************************************************************************
	SCAL	R6,CUROFF		; TURN OFF CURSOR
	SCAL	R6,RVON			; REVERSE VIEDO
	RLDI	R1,$0200		; 512 TIMES
;******************************************************************************
ADCAL1:	SEX	R3			; SET PC EQUAL SP
	OUT	1			; SELECT I/O GROUP
	DB	$30			; I/O GROUP 30 IS A/D BOARD
	OUT	6			; SELECT A/D MODE
	DB	$00			; ZERO MEANS FIXED CHANNEL
	OUT	5			; SELECT CHANNEL
	DB	$00			; CHANNEL ZERO
	SEX	R2			; RESET SP TO R2
	BN1	*			; WAIT FOR CONVERSION
	INP	3			; INPUT CHANNEL ZERO
	PHI	RD			; LOAD REGISTER
	SCAL	R6,WORD81		; DISPLAY AS BITS
	DB	$01			; TERMINATE WITH A CR
;******************************************************************************
;
;	LOOP FOR NEXT SCAN
;
;******************************************************************************
	BN4	ADCAL2			; EXIT ON BREAK
	DBNZ	R1,ADCAL1		; LOOP R1 TIMES
;*****************************************************************************
;
;	REPEAT PROCESS
;
;*****************************************************************************
ADCAL2:	SCAL	R6,RVOFF	; NORMAL VIDEO
	SCAL	R6,CURON	; TURN CURSOR BACK ON
	SCAL	R6,REPEAT	; CALL REPEAT QUESTION ROUTINE
	LBZ	ADCAL		; REPEAT CALIBRATION IF D=0
	LBR	AMENU		; RETURN TO A/D-D/A TEST MENU
;******************************************************************************

	CORG	$0100		; SET TO NEXT PAGE

;******************************************************************************
;
;	D/A CALIBRATION
;
;******************************************************************************
DACAL:	NOP			; NO OPERATION
	SCAL	R6,CLEAR	; CLEAR SCREEN
	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'MB654A D/A CALIBRATION'
	DB	$0A,$0A,$0D
	DB	$0A,$0D,$00
;******************************************************************************
;
;	SETUP OUTPUT DATA
;
;******************************************************************************
DACAL1:  RLDI	 R2,RAM+$0FFF  ; RESET STACK POINTER FOR DEBUG ROUTINES
	 SCAL	 R6,OSTRN1	 ; PRINT MESSAGE
	 DB	 'ENTER HEX DATA FOR OUTPUT '
	 DB	 $00

	 SCAL	 R6,CREAD	 ; READ HEX DATA
	 BNZ	 DACAL1		 ; RETRY ON NO HEX CHARACTER

	 GLO	 RD		 ; GET BYTE
	 PLO	 R1		 ; SET INPUT BYTE
;******************************************************************************
;
;	SETUP D/A I/O MODE
;
;******************************************************************************
	RLDI	R2,RAM+$0800	; SETUP INPUT STACK
	SEX	R3		; SET PROGRAM COUNTER EQUAL SP
	OUT	1		; SET I/O GROUP
	DB	$30		; I/O GROUP 30
	SEX	R2		; RESET SP TO R2
;******************************************************************************
;
;	OUTPUT DATA TO D/A CHANNELS 1 AND TWO
;
;******************************************************************************
	GLO	R1		; GET DATA
	STR	R2		; STACK IT
	OUT	3		; OUTPUT DATA TO CHANNEL 1
	DEC	R2		; RESET STACK POINTER
	OUT	4		; OUTPUT DATA TO CHANNEL 2
	DEC	R2		; RESET STACK POINTER
;******************************************************************************
;
;	DISPLAY OUTPUT DATA
;
;******************************************************************************
	RLDI	R2,RAM+$0FFF	    ; RESET STACK POINTER FOR DEBUG ROUTINES
	RLDI	R7,$0001	    ; SET INITIAL CHANNEL NUMBER
	RLDI	RB,$0002	    ; TWO CHANNELS
	
	SCAL	R6,OSTRN1	    ; PRINT MESSAGE
	DB	$0A,$0A,$0D
	DB	$00	

01$:	SCAL	R6,OSTRN1	    ; PRINT MESSAGE
	DB	'CH ',$00	    ; CHANNEL
	GLO	R7		    ; GET CHANNEL NUMBER
	PHI	RF		    ; LOAD REGISTER
	SCAL	R6,TYPEH	    ; PRINT CHANNEL NUMBER
	SCAL	R6,OSTRN1	    ; PRINT MESSAGE
	DB	'  ',$00	    ; SPACE OVER
	GLO	R1		    ; LOAD DATA
	PHI	RD		    ; LOAD REGISTER
	SCAL	R6,WORD 	    ; DISPLAY AS BITS
	DB	$00		    ; TERMINATION
	SCAL	R6,OSTRN1           ; PRINT MESSAGE
	DB	'  ',$00            ; SPACE OVER
	GLO	R1		    ; LOAD DATA
	PHI	RF		    ; LOAD REGISTER
	SCAL	R6,TYPEH	    ; DISPLAY AS HEX

	SCAL	R6,OSTRN1	    ; PRINT MESSAGE
	DB	$0A,$0D,$00	    ; NEW LINE

	INC	R7		    ; INCREMENT CHANNEL NUMBER
	DBNZ	RB,01$		    ; LOOP FOR 2 CHANNELS
;******************************************************************************
;
;	DELAY 15 SECONDS FOR CHECK OR ABORT
;
;******************************************************************************
	RLDI	RE,$0C		    ; LOAD OUTER COUNTER
02$:	RLDI	RF,$FFFF	    ; LOAD INNER COUNTER
03$:	BN4	05$		    ; ABORT ON BREAK
	DBNZ	RF,03$		    ; DECREMENT INNER COUNTER
	DBNZ	RE,02$		    ; DECREMENT OUTER COUNTER

	SCAL	R6,RCA1		    ; SETUP FOR TERMINAL I/O
	SCAL	R6,MLFCR	    ; LINE UP
	DB	$04		    ; 4 TIMES
;******************************************************************************
;
;	REPEAT CYCLE
;
;*****************************************************************************
	LBR	DACAL1		; LOOP
;******************************************************************************
;
;	REPEAT PROCESS
;
;*****************************************************************************
05$:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0A,$0D,$00	; 2 LINES DOWN
        SCAL	R6,REPEAT	; CALL REPEAT QUESTION ROUTINE
	LBZ	DACAL		; REPEAT TEST IF D=0
	LBR	AMENU		; RETURN TO A/D-D/A TEST MENU
;******************************************************************************

	CORG	$0100		; SET NEXT PAGE

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
;	CUSTOM HEX READER ROUTINE
;
;******************************************************************************
CREAD:	SCAL	R6,READH1	; CALL READER
	BDF	CREAD		; WAIT FOR INPUT
	GHI	RF		; CHECK ASCII
	XRI	$0D		; IS IT A CR?
	BNZ	01$		; IF NON HEX ERROR
	SRET	R6		; RETURN WITH RESULT IN RD

01$:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0D,$00		; RESET TO START OF LINE
	LDI	$FF		; SET D WITH SOME NON-ZERO VAULE
	SRET	R6		; RETURN
;******************************************************************************

	CORG	$0100		; SET NEXT PAGE

;******************************************************************************
;
;	CPU TEST 
;
;******************************************************************************
CPUTST:	NOP			; NO OPERATION
	RLDI	R1,$0000	; ZERO OUT GLOBAL ERROR COUNTER
;******************************************************************************
;
;	TEST RAM AT $4000
;
;******************************************************************************
	SCAL	R6,CLEAR	; CLEAR SCREEN
	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'TESTING RAM ON CPU BOARD'
	DB	$0A,$0A,$0D
	DB	'PLEASE WAIT...'
	DB	$0A,$0A,$0D,$00

	RLDI	R0,$0000	; ZERO OUT ERROR COUNT

	RLDI	R8,RAM		; BASE ADDRESSS FOR CPU RAM
	RLDI	R9,$0FFF	; NUMBER OF BYTES
RAMTST:	LDI	$00		; ZERO BYTE
	STR	R8		; STORE IN RAM
	NOP			;
	NOP			;
	
	BN4	04$		; ABORT ON BREAK

	NOP			;
	NOP			;
	LDN	R8		; GET BYTE FROM RAM
	BZ	01$		; CHECK OK?

	SCAL	R6,ERRRAM	; REPORT RAM ERROR

01$:	LDI	$FF		; ALL ONES
	STR	R8		; STORE IN RAM
	NOP			;
	NOP			;

	BN4	04$		; ABORT ON BREAK
	
	NOP			;
	NOP			;
	LDN	R8		; GET BYTE FROM RAM
	SMI	$FF		;
	BZ	02$		; CHECK OK

	SCAL	R6,ERRRAM	; REPORT RAM ERROR
	
02$:	INC	R8		; INCREMENT RAM ADDRESS
	DBNZ	R9,RAMTST	; CHECK 4K BYTE

	GHI	R0		; HIGH BYTE OF ERROR COUNT
	BNZ	03$		; IF ERROR BRANCH
	GLO	R0		; LOW BYTE OF ERROR COUNT
	BNZ	03$		; JUMP TO EPROM TEST IF NO ERRORS

	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'RAM TEST COMPLETED'
	DB	$00
	LBR	EPTST		; JUMP TO EPROM TEST IF NO ERRORS

03$:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'**** RAM TEST ERRORS ****'
	DB	$0A,$0D,$00
	BR	05$		; WAIT FOR KEY
		
04$:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'**** RAM TEST ABORTED ****'
	DB	$0A,$0D,$00

05$:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	'      PRESS ANY KEY'
	DB	$0A,$0D,$00
	SCAL	R6,READD	; SCAN KEYBOARD
;******************************************************************************
;
;	COMPARE EPROM AT $0000 TO MASTER AT $C000
;
;******************************************************************************
EPTST:	SCAL	R6,CLEAR	; CLEAR SCREEN
	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'CHECK EPROM AGAINST MASTER'
	DB	$0A,$0A,$0D
	DB	'PLEASE WAIT...'
	DB	$0A,$0A,$0D,$00

	RLDI	R0,$0000	; ZERO OUT ERROR COUNT

	RLDI	R7,MASTER	; STORAGE ADDREES FOR EPROM CODE
	RLDI	R8,$0000	; BASE ADDRESS FOR EPROM CODE
	RLDI	R9,$07FF	; NUMBER OF BYTES

01$:	SEX	R8		; POINTER TO EPROM IS SP
	LDN	R7		; GET BYTE FORM MASTER
	SM			; SUBTRACK FORM EPROM
	SEX	R2		; RESET SP TO R2
	BZ	02$		; CHECK OK?

	SCAL	R6,ERRCOM	; IF NOT EQUAL ERROR

02$:	BN4	04$		; ABORT ON BREAK
	INC	R7		; INCREMENT ADDRESS OF EPROM
	INC	R8		; INCREMENT ADDRESS OF MASTER
	DBNZ	R9,01$		; COMPARE 2K BYTES

	GHI	R0		; HIGH BYTE OF ERROR COUNT
	BNZ	03$		; IF ERROR BRANCH
	GLO	R0		; LOW BYTE OF ERROR COUNT
	BNZ	03$		; IF NOT ZERO ERRORS

	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'EPROM TEST COMPLETED'
	DB	$00
	LBR	PIOTST		; JUMP TO PIO TEST IF NO ERRORS

03$:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'**** EPROM VERIFY ERRORS ****'
	DB	$0A,$0D,$00
	BR	05$		; WAIT FOR KEY

04$:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'**** EPROM TEST ABORTED ****'
	DB	$0A,$0D,$00

05$:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	'      PRESS ANY KEY'
	DB	$0A,$0D,$00
	SCAL	R6,READD	; SCAN KEYBOARD
;******************************************************************************
;
;	PIO TEST
;
;******************************************************************************
PIOTST: SCAL	R6,CLEAR	; CLEAR SCREEN
	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'TESTING PROGRAMMABLE I/O CHIP'
	DB	$0A,$0A,$0D
	DB	'PLEASE WAIT...'
	DB	$0A,$0A,$0D,$00

	RLDI	R0,$0000	; ZERO OUT ERROR COUNT

;******************************************************************************
;
;	SETUP PIO PORTS
;
;******************************************************************************
	SEX	R3		; SET SP TO PC
	OUT	1		; SELECT I/O GROUP
	DB	$08		; PIO CHIP
	OUT	2		; SETUP PORT A FOR INPUT
	DB	$0F
	OUT	2		; SETUP PORT B FOR OUTPUT
	DB	$77
	SEX	R2		; RESET SP
;******************************************************************************
;
;	TEST PIO PORTS
;
;******************************************************************************
	RLDI	R7,$0100	; COUNTER
	RLDI	R8,$00FF	; DATA

01$:	SEX	R3		; SET SP TO PC
	OUT	1		; SET I/O GROUP
	DB	$08		; PIO CHIP
	SEX	R2		; RESET SP

	GLO	R8		; GET DATA
	STR	R2		; STACK IT
	OUT	6		; OUTPUT DATA FROM PORT B
	DEC	R2		; RESET SP

	NOP			; NO OPERATION
	NOP			;
	BN4	04$		; ABORT ON BREAK KEY
	NOP			;
	NOP			;

	INP	4		; READ PORT A
	GLO	R8		; GET DATA
	SM			; DATA OUT - DATA IN
	BZ	02$		; IF ZERO OK

	SCAL	R6,ERRIO	; REPORT I/O ERRORS

02$:	DEC	R8		; DECREMENT DATA
	DBNZ	R7,01$		; LOOP TILL ALL DATA TESTED

	GHI	R0		; HIGH BYTE OF ERROR COUNT
	BNZ	03$		; IF ERROR BRANCH
	GLO	R0		; LOW BYTE OF ERROR COUNT
	BNZ	03$		; IF NOT ZERO ERRORS

	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'I/O TEST COMPLETED'
	DB	$00
	LBR	EFNTST		; JUMP TO EVENT FLAG TEST IF NO ERRORS

03$:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'**** I/O TEST ERRORS ****'
	DB	$0A,$0D,$00
	BR	05$		; WAIT FOR KEY

04$:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'**** I/O TEST ABORTED ****'
	DB	$0A,$0D,$00

05$:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	'      PRESS ANY KEY'
	DB	$0A,$0D,$00
	SCAL	R6,READD	; SCAN KEYBOARD
;******************************************************************************
;
;      EVENT FLAG TEST
;
;******************************************************************************
EFNTST: SCAL	R6,CLEAR	; CLEAR SCREEN
	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'TESTING EVENT FLAGS'
	DB	$0A,$0A,$0D
	DB	'PLEASE WAIT...'
	DB	$0A,$0A,$0D,$00

	RLDI	R0,$0000	; ZERO OUT ERROR COUNT

;******************************************************************************
	SEX	R3		; SET SP TO PC
	OUT	1		; SELECT I/O GROUP
	DB	$08		; PIO CHIP
	SEX	R2		; RESET SP

	RLDI	R8,$0001	; EVENT FLAG COUNTER

	LDI	$00		; ZERO BYTE
	STR	R2		; STACK IT
	OUT	6		; OUTPUT DATA
	DEC	R2		; RESET SP
	
	B1	10$		; EF1 SHALL BE ZERO - REV
	SCAL	R6,ERREF	; DISPLAY ERROR
	BN4	04$		; ABORT ON BREAK
	
10$:	INC	R8		; INCREMENT EVENT FLAG COUNTER
	B2	20$		; EF2 SHALL BE ZERO - REV
	SCAL	R6,ERREF	; DISPLAY ERROR
	BN4	04$		; ABORT ON BREAK

20$:	INC	R8		; INCREMENT EVENT FLAG COUNTER
	B3	30$		; EF3 SHALL BE ZERO - REV
	SCAL	R6,ERREF	; DISPLAY ERROR
	BN4	04$		; ABORT ON BREAK

30$:	SEX	R3		; SET SP TO PC
	OUT	1		; SELCT I/O GROUP
	DB	$08		; PIO CHIP
	SEX	R2		; RESET SP

	RLDI	R8,$FF01	; EVENT FLAG COUNTER

	LDI	$FF		; DATA BYTE
	STR	R2		; STACK IT
	OUT	6		; OUTPUT DATA
	DEC	R2		; RESET SP

	BN1	11$		; EF1 SHALL BE HIGH - REV
	SCAL	R6,ERREF	; DISPLAY ERROR
	BN4	04$		; ABORT ON BREAK

11$:	INC	R8		; INCREMENT EVENT FLAG COUNTER
	BN2	12$		; EF2 SHALL BE HIGH - REV
	SCAL	R6,ERREF	; DISPLAY ERROR
	BN4	04$		; ABORT ON BREAK

12$:	INC	R8		; INCREMENT EVENT FLAG COUNTER
	BN3	13$		; EF3 SHALL BE HIGH - REV
	SCAL	R6,ERREF	; DISPLAY ERROR
	BN4	04$		; ABORT ON BREAK

13$:	GHI	R0		; HIGH BYTE OF ERROR COUNT
	BNZ	03$		; IF ERROR BRANCH
	GLO	R0		; LOW BYTE OF ERROR COUNT
	BNZ	03$		; IF NOT ZERO ERRORS

	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'EVENT FLAG TEST COMPLETED'
	DB	$00
	LBR	CLKTST		; JUMP TO CLOCK TEST IF NO ERRORS

03$:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'**** EVENT FLAG TEST ERRORS ****'
	DB	$0A,$0D,$00
	BR	05$		; WAIT FOR KEY

04$:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'**** EVENT FLAG TEST ABORTED ****'
	DB	$0A,$0D,$00

05$:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	'      PRESS ANY KEY'
	DB	$0A,$0D,$00
	SCAL	R6,READD	; SCAN KEYBOARD
;******************************************************************************
;
;	CLOCK - COUNTER TEST
;
;******************************************************************************
CLKTST: SCAL	R6,CLEAR	; CLEAR SCREEN
	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'TESTING CLOCK-COUNTER'
	DB	$0A,$0A,$0D
	DB	'PLEASE WAIT...'
	DB	$0A,$0A,$0D,$00

	RLDI	R0,$0000	; ZERO OUT ERROR COUNT

;******************************************************************************
;
;	SET UP CLOCK COUNTER
;
;******************************************************************************
	STPC			; STOP COUNTER
	LDI	$40		; 64 COUNTS
	LDC			; LOAD COUNTER
	ETQ			; ENABLE Q FLIP/FLOP
	SCM2			; START COUNTER MODE 2
;******************************************************************************
	SEQ			; SET Q=1
	RLDI	R7,$0040+$10	; 80 COUNTS
	SEX	R3		; SET SP TO PC
	OUT	1		; SELECT I/O GROUP
	DB	$08		; PIO CHIP
	SEX	R2		; RESET SP
;******************************************************************************
01$:	LDI	$FF		; HIGH DATA
	STR	R2		; STACK IT
	OUT	6		; OUTPUT TO PORT B
	DEC	R2		; RESET SP

	NOP			; NO OPERATION
	NOP			;
	BN4	04$		; ABORT ON BREAK
	NOP			;
	NOP			;

	LDI	$00		; ZERO DATA
	STR	R2		; STACK IT
	OUT	6		; OUTPUT TO PORT B
	DEC	R2		; RESET SP

	NOP			; NO OPERATION
	NOP			;
	BN4	04$		; ABORT ON BREAK
	NOP			;
	NOP			;

	DBNZ	R7,01$		; LOOP FOR 42 CYCLES
;******************************************************************************
;
;	ON EXIT FROM LOOP Q SHALL BE LOW
;
;******************************************************************************
	BNQ	02$		; IF Q NOT LOW ERROR

	SCAL	R6,ERRCLK	; DISPLAY ERROR

02$:	GHI	R0		; HIGH BYTE OF ERROR COUNT
	BNZ	03$		; IF ERROR BRANCH
	GLO	R0		; LOW BYTE OF ERROR COUNT
	BNZ	03$		; IF NOT ZERO ERRORS

	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'CLOCK-COUNTER TEST COMPLETED'
	DB	$00
	LBR	CPUEND		; JUMP TO END OF TEST IF NO ERRORS

03$:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'**** CLOCK-COUNTER TEST ERRORS ****'
	DB	$0A,$0D,$00
	BR	05$		; WAIT FOR KEY

04$:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'**** CLOCK-COUNTER TEST ABORTED ****'
	DB	$0A,$0D,$00

05$:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	'      PRESS ANY KEY'
	DB	$0A,$0D,$00
	SCAL	R6,READD	; SCAN KEYBOARD
;******************************************************************************
;
;	END OF CPU TEST
;
;******************************************************************************
CPUEND:	SEX	R2		; RESET SP TO R2
	SCAL	R6,CLEAR	; CLEAR SCREEN

	GHI	R1		; HIGH BYTE OF ERROR COUNT
	BNZ	03$		; IF ERROR BRANCH
	GLO	R1		; LOW BYTE OF ERROR COUNT
	BNZ	03$		; IF NOT ZERO ERRORS

	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'MB606 1805 CPU TEST COMPLETED'
	DB	$00
	LBR	MENU		; RETURN TO MAIN TEST MENU IF NO ERRORS

03$:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'**** MB606 1805 CPU TEST ERRORS ****'
	DB	$0A,$0A,$0D
	DB	'**** REJECT AND/OR RE-INSPECT *****'
	DB	$0A,$0A,$0D,$00
	BR	05$		; WAIT FOR KEY

04$:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'**** MB606 1805 CPU TEST ABORTED ****'
	DB	$0A,$0D,$00

05$:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	'      PRESS ANY KEY'
	DB	$0A,$0D,$00
	SCAL	R6,READD	; SCAN KEYBOARD
	LBR	MENU		; RETURN TO MAIN MENU
;******************************************************************************
;
;	CPU TEST ERROR ROURINES
;
;******************************************************************************
;
;	RAM VERIFY ERROR
;
;******************************************************************************
ERRRAM:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	'RAM FAILURE AT ADDRESS '
	DB	$00
	
	GHI	R8		; GET HIGH BYTE OF RAM ADDRESS
	PHI	RD		; PASS FOR PRINT
	GLO	R8		; GET LOW BYTE OF RAM ADDRESS
	PLO	RD		; PASS FOR PRINT
	SCAL	R6,TYPE6	; PRINT HEX ADDRESS
	DB	$02,$01		; FOLLOW WITH ONE CRLF
	INC	R0		; INCREMENT LOCAL ERROR COUNTER
	INC	R1		; INCREMENT GOBAL ERROR COUNTER
	SRET	R6		; RETURN
;******************************************************************************
;
;	EPROM VERIFY ERROR
;
;******************************************************************************
ERRCOM:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	'EPROM FAILURE AT ADDRESS '
	DB	$00

	GHI	R8		; GET HIGH BYTE OF EPROM ADDRESS
	PHI	RD		; PASS FOR PRINT
	GLO	R8		; GET LOW BYTE OF EPROM ADDRESS
	PLO	RD		; PASS FOR PRINT
	SCAL	R6,TYPE6	; PRINT HEX ADDRESS
	DB	$02,$01		; FOLLOW WITH ONE CRLF
	INC	R0		; INCREMENT LOCAL ERROR COUNTER
	INC	R1		; INCREMENT GOBAL ERROR COUNTER
	SRET	R6		; RETURN
;******************************************************************************
;
;	I/O TEST ERROR
;
;******************************************************************************
ERRIO:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	'I/O FAILURE TO READ DATA '
	DB	$00

	GLO	R8		; GET LOW BYTE OF EPROM ADDRESS
	PHI	RD		; PASS FOR PRINT
	SCAL	R6,TYPE8	; PRINT HEX DATA
	DB	$02,$01		; FOLLOW WITH ONE CRLF
	INC	R0		; INCREMENT LOCAL ERROR COUNTER
	INC	R1		; INCREMENT GOBAL ERROR COUNTER
	SRET	R6		; RETURN
;******************************************************************************
;
;	EVENT FLAG TEST ERROR
;
;******************************************************************************
ERREF:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	'EVENT FLAG ERROR ON EF'
	DB	$00

	GLO	R8		; GET LOW BYTE OF EPROM ADDRESS
	PHI	RD		; PASS FOR PRINT
	SCAL	R6,TYPE8	; PRINT HEX DATA
	DB	$02,$01		; FOLLOW WITH ONE CRLF
	INC	R0		; INCREMENT LOCAL ERROR COUNTER
	INC	R1		; INCREMENT GOBAL ERROR COUNTER
	SRET	R6		; RETURN
;******************************************************************************
;
;	CLOCK-COUNTER TEST ERROR
;
;******************************************************************************
ERRCLK: SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0D,$0D,$0D,$0D
	DB	'CLOCK-COUNTER ERROR'
	DB	$0A,$0A,$00

	INC	R0		; INCREMENT LOCAL ERROR COUNTER
	INC	R1		; INCREMENT GOBAL ERROR COUNTER
	SRET	R6		; RETURN
;******************************************************************************

	CORG	$0100		; SET NEXT PAGE

;******************************************************************************
;
;	EPROM PROGRAMING ROUTINE
;
;	TRANSFER EPROM FROM ROM AT EPROM ADDRESS TO ADDRESS 0000
;	AND THEN RUN
;
;******************************************************************************
IEPROM:	SCAL	R6,CLEAR	; CLEAR SCREEN
	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'LOADING EPROM SOFTWARE INTO RAM'
	DB	$0A,$0A,$0D
	DB	'PLEASE WAIT...'
	DB	$0A,$0A,$0D,$00

	RLDI	R7,EPROM	; STORAGE ADDREES FOR EPROM CODE
	RLDI	R8,$0000	; BASE ADDRESS FOR EPROM CODE
	RLDI	R9,$07FF	; NUMBER OF BYTES

01$:	LDN	R7		; GET BYTE FROM ROM
	STR	R8		; STORE IN RAM
	
	SEX	R7		; MAKE ROM POINTER SP
	LDN	R8		; GET BYTE FROM RAM
	SM			; SUBTRCK BYTES
	SEX	R2		; RESET SP TO R2
	BZ	02$		; BYTE OK?

	SCAL	R6,ERRCOM	; IF NOT EQUAL ERROR
	BR	03$		; QUIT ON ERROR

02$:	INC	R7		; INCREMENT ROM ADDRESS
	INC	R8		; INCREMENT RAM ADDRESS
	DBNZ	R9,01$		; TRANSFER 2K BYTES

	SCAL	R6,CLEAR	; CLEAR SCREEN

	RLDI	R0,$0000	; INITIAL R0 
	SEX	R0		; R0 IS SP
	SEP	R0		; R0 IS PC
;******************************************************************************
;
;	EPROM PROGRAMMING ABORTED
;
;******************************************************************************
03$:	SCAL	R6,OSTRN1	; PRINT MESSAGE
	DB	$0A,$0D
	DB	'**** EPROM PROGRAMING  ABORTED ****'
	DB	$0A,$0D
	DB	'       PRESS ANY KEY'
	DB	$0A,$0D,$00
	SCAL	R6,READD	; SCAN KEYBOARD
	LBR	MENU
;******************************************************************************

		  CORG	  $0100 	  ; SET NEXT PAGE

;******************************************************************************
;
;	RTE1/RTD1 TEST ROUTINE
;
;******************************************************************************
RTERTD: SCAL	R6,CLEAR		; CLEAR SCREEN
	SCAL	R6,OSTRN1		; PRINT MESSAGE
        DB	$0A,$0D
	DB	'RTE1/RTD1 TEST'
	DB	$0A,$0D,$00
;******************************************************************************
;
;	SET-UP FOR PIO PORTS
;
;	PORT A FOR DATA INPUT
;	PORT B LOW NIBBLE ADDRESS INPUT
;	PORT B HIGH NIBBLE OUTPUT ECHO
;
;******************************************************************************
	RLDI 	R2,RAM+$0800		; USER STACK
        SEX 	R3			; SET SP TO PC
        OUT 	1			; SELECT I/O GROUP 8
	DB	$08 ; I/O GROUP 8	
        OUT 	2
	DB	$0F ; A IS INPUT
        OUT 	2
	DB	$F7 ; B IS BIT PROG
        OUT 	2
	DB	$F0 ; LOW IN,HIGH OUT
        SEX 	R2

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

	BN4	04$		; ABORT ON BREAK

	INC	R0		    ; INCREMENT LINE COUNTER
	GLO	R0		    ; GET COUNT
	SMI	$09		    ; 9 LINES
	LBNZ	PIO		    ; IF LESS NEXT CHANNEL, OTHERWISE-

	SCAL	R6,MLFCR	    ; LINE UP
	DB	$12		    ; 18 TIMES
	RLDI	R0,$0000	    ; RESET LINE COUNTER

	LBR	PIO
;****************************************************************************
;
;	RTE1/RTD1 TEST ABORTED
;
;****************************************************************************
04$:	SCAL	R6,CLEAR	; CLEAR SCREEN
	SCAL	R6,OSTRN1	; PRINT MESSAGE
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
	LBZ	RTERTD		; REPEAT TEST IF D=0
	LBR	MENU		; RETURN TO MAIN TEST MENU
;******************************************************************************
;
;	END OF TLX TEST SOFTWARE
;
;******************************************************************************
	END

REM *************************************************************************
REM
REM     TLX PROFILE PROGRAM
REM     WRITTEN BY DENTON MARLOWE
REM     (C)1988,1989 HAYES TARGETS
REM     REVISION V1.1
REM     written in microsoft QuickBasic 4.0
REM
REM *************************************************************************
        CLS : SCREEN 0: COLOR 3
        PRINT "TLX FLIGHT PROFILE PLANNING PROGRAM VERSION 1.1"
        PRINT "COPYRIGHT 1988,1989 HAYES TARGETS"
        PRINT "WRITTEN BY DENTON MARLOWE"
        PRINT
REM *************************************************************************
enterlevel:    
        INPUT "(F)irst or (S)subsequent Presentation"; TITLE$: TITLE$ = UCASE$(TITLE$)
REM     INPUT "ENTER PROFILE TITLE"; TITLE$: TITLE$ = UCASE$(TITLE$)
        IF TITLE$ = "TEST" THEN GOTO TESTDATA
        IF TITLE$ = "FILE" THEN GOTO READFILE
        IF TITLE$ <> "S" AND TITLE$ <> "F" THEN GOTO enterlevel
        IF TITLE$ = "F" THEN TITLE$ = "FIRST PRESENTATION": LEVEL = 1
        IF TITLE$ = "S" THEN TITLE$ = "SUBSEQUENT PRESENTATION": LEVEL = 2
REM *************************************************************************
REM     INPUT "ENTER STAGE 1 ALTITUDE (FEET)"; STAGE1
        STAGE1 = 750
        INPUT "ENTER CABLE LENGTH (FEET)"; L
        INPUT "ENTER GROUND SPEED (KNOTS)"; V1
        INPUT "ENTER TURN BANK ANGLE (DEGREES)"; BANK
REM *************************************************************************
120     INPUT "ENTER RANGE IN YARDS FOR PRESENTATION START"; PRANGE
        IF PRANGE <= 10000 THEN GOTO 120
REM *************************************************************************
140     INPUT "ENTER TLX POP-UP RANGE (YARDS)"; POPUP
        INPUT "ENTER TLX PRESET ALTITUDE (FT)"; SETALT
        IF POPUP = 0 THEN INPUT "ENTER TLX SHIP CLEARENCE ALT (FT)"; a1:                ELSE GOTO extratime
        IF (a1 = 0) AND (POPUP = 0) THEN GOTO 140
REM *************************************************************************
extratime:
REM     INPUT "ADD EXTRA TIME (Y/N)"; R$
REM *************************************************************************
        INPUT " final output device (S)creen,(P)rinter or (F)ile"; DEV$
        DEV$ = UCASE$(DEV$)
        IF DEV$ = "F" THEN INPUT "enter output file name"; FILE$
REM *************************************************************************
        FORM$ = "SHORT"
REM *************************************************************************
        GOSUB PROFILE
        END
REM *************************************************************************
REM
REM     TLX PROFILE
REM
REM *************************************************************************
PROFILE:
        RANGEFLAG$ = "LEAR": POPPED$ = "NO"
        IF DEV$ = "S" THEN OPEN "SCRN:" FOR OUTPUT AS #3
        IF DEV$ = "P" THEN OPEN "LPT1:" FOR OUTPUT AS #3
        IF DEV$ = "F" THEN OPEN FILE$ FOR OUTPUT AS #3
REM *************************************************************************
REM
REM     PROCESS TIME AND SPACE
REM
REM *************************************************************************
        GOSUB CABLEDELAY
        GOSUB TIMES
        GOSUB TITLE
        GOSUB CRANGE
        GOSUB PTIMES
        GOSUB POPCLEAR
        GOSUB POPUPTIME
        GOSUB START
        GOSUB FINISH
        RETURN
REM *************************************************************************
REM
REM     COMPUTE CABLE DELAY
REM
REM *************************************************************************
CABLEDELAY:
        V = V1 * 1.689
        DELAY = L / V
        RETURN
REM *************************************************************************
REM
REM     CALCULATE TIME AND DISTANCE FOR FIRST PART OF PROFILE
REM
REM *************************************************************************
TIMES:
        T0 = 0
        GOSUB TURN
        timeTurn = T
        GOSUB STAGE1
        T0 = T0 + T: time0 = T
        GOSUB STAGE2
        T0 = T0 + T: time1 = T
        GOSUB STAGE2D
        T0 = T0 + T: time2 = T
        GOSUB STAGE3
        T0 = T0 + T: time3 = T
        GOSUB STAGE3D
        T0 = T0 + T: time4 = T
        GOSUB STAGE4
        T0 = T0 + T: time5 = T
        GOSUB STAGE4D
        T0 = T0 + T: time6 = T
        GOSUB ADJUST
        T0 = T0 + T: time7 = T
        RETURN
REM *************************************************************************
REM
REM     CALCULATE TOTAL RANGE DISTANCE
REM
REM *************************************************************************
CRANGE:
        RANGE1 = RADIUS / 6076
        RANGE2 = T0 * V / 6076
        RANGE3 = PRANGE * 3 / 6076
        RANGE = RANGE1 + RANGE2 + RANGE3
        RETURN
REM *************************************************************************
REM
REM     PRINT TIME AND DISTANCE FOR FIRST PART OF PROFILE
REM
REM *************************************************************************
PTIMES:
        T0 = 0: M0 = RANGE
        GOSUB PTURN
        T = time0: M0 = RANGE - RANGE1
        GOSUB PSTAGE1
        T = time1: M0 = M0 - T * V / 6076
        GOSUB PSTAGE2
        T = time2: M0 = M0 - T * V / 6076
        GOSUB PSTAGE2D
        T = time3: M0 = M0 - T * V / 6076
        GOSUB PSTAGE3
        T = time4: M0 = M0 - T * V / 6076
        GOSUB PSTAGE3D
        T = time5: M0 = M0 - T * V / 6076
        GOSUB PSTAGE4
        T = time6: M0 = M0 - T * V / 6076
        GOSUB PSTAGE4D
        T = time7: M0 = M0 - T * V / 6076
        GOSUB PADJUST
        RETURN
REM *************************************************************************
REM
REM     CALCULATE TURN RADIUS
REM
REM *************************************************************************
TURN:
        RADIUS = (V ^ 2) / (TAN(BANK / 57.29577951#) * 32.174)
        SEMICIRCLE = RADIUS * 3.141592654#
        T = SEMICIRCLE / V
        RETURN
REM *************************************************************************
REM
REM     CALCULATE TURN RADIUS
REM
REM *************************************************************************
PTURN:
        N = 1
        N1$ = "TURN IN FOR PRESENTATION"
        N2$ = " "
        N3$ = " "
        N5$ = " "
        N6$ = "^"
        PRINT #3, "          ";
        PRINT #3, "       ";
        GOSUB RANGEONE
        GOSUB TEXTLPRINT
        RETURN
REM *************************************************************************
REM
REM     START PROFILE
REM
REM *************************************************************************
STAGE1:
        T = 0
        RETURN
REM *************************************************************************
REM
REM     START PROFILE
REM
REM *************************************************************************
PSTAGE1:
        N = 3
        N1$ = "TURN COMPLETE"
        N2$ = "STAGE 1:START DESCENT"
        N3$ = "AT 500FPM TO STAGE 2"
        N5$ = " "
        GOSUB TIMEINC
        PRINT #3, "       ";
        GOSUB RANGEONE
        GOSUB TEXTLPRINT
        RETURN
REM *************************************************************************
REM
REM  STAGE 1 ALTITUDE: DESCEND AT 500 FPM TO STAGE 2
REM
REM *************************************************************************
STAGE2:
        T = ((STAGE1 - 200 - SETALT) / 500) * 60
        RETURN
REM *************************************************************************
REM
REM  STAGE 1 ALTITUDE: DESCEND AT 500 FPM TO STAGE 2
REM
REM *************************************************************************
PSTAGE2:
        IF LEVEL = 2 GOTO PSTAGE2A
        N = 1
        N1$ = "STAGE 2 ALT: A/C STRAIGHT AND LEVEL"
        GOSUB TIMEINC
        PRINT #3, "       ";
        GOSUB RANGEONE
        GOSUB TEXTLPRINT
        RETURN
PSTAGE2A:
        N = 2
        N1$ = "STAGE 2 ALT"
        N2$ = "DESCEND AT 200FPM TO STAGE 3"
        GOSUB TIMEINC
        PRINT #3, "       ";
        GOSUB RANGEONE
        GOSUB TEXTLPRINT
        RETURN
REM *************************************************************************
REM
REM     STAGE 2:TLX STABLE AT 200 FT ABOVE PRESET
REM
REM *************************************************************************
STAGE2D:
        IF LEVEL = 1 THEN T = DELAY ELSE T = 0
        RETURN
REM *************************************************************************
REM
REM     STAGE 2:TLX STABLE AT 200 FT ABOVE PRESET
REM
REM *************************************************************************
PSTAGE2D:
        IF LEVEL = 2 THEN RETURN
        N = 2
        N1$ = "TLX STABLE AT 200 FT ABOVE PRESET"
        N2$ = "DESCEND AT 200FPM TO STAGE 3"
        GOSUB TIMEINC
        PRINT #3, "       ";
        GOSUB RANGEONE
        GOSUB TEXTLPRINT
        RETURN
REM *************************************************************************
REM
REM     DESCEND TO STAGE 3 ALTITUDE
REM
REM *************************************************************************
STAGE3:
        T = (100 / 200) * 60
        RETURN
REM *************************************************************************
REM
REM     DESCEND TO STAGE 3 ALTITUDE
REM
REM *************************************************************************
PSTAGE3:
        IF LEVEL = 2 GOTO PSTAGE3A
        N = 2
        N1$ = "STAGE 3 ALT:"
        N2$ = "A/C STRAIGHT AND LEVEL"
        GOSUB TIMEINC
        PRINT #3, "       ";
        GOSUB RANGEONE
        GOSUB TEXTLPRINT
        RETURN
PSTAGE3A:
        N = 2
        N1$ = "STAGE 3 ALT:"
        N2$ = "DESCEND TO STAGE 4 AT 100FPM"
        GOSUB TIMEINC
        PRINT #3, "       ";
        GOSUB RANGEONE
        GOSUB TEXTLPRINT
        RETURN
REM *************************************************************************
REM
REM     TLX STABLE AT 100 FT ABOVE PRESET
REM
REM *************************************************************************
STAGE3D:
        IF LEVEL = 1 THEN T = DELAY ELSE T = 0
        RETURN
REM *************************************************************************
REM
REM     TLX STABLE AT 100 FT ABOVE PRESET
REM
REM *************************************************************************
PSTAGE3D:
        IF LEVEL = 2 THEN RETURN
        N = 2
        N1$ = "TLX STABLE AT 100 FT ABOVE PRESET"
        N2$ = "DESCEND AT 100FPM TO STAGE 4"
        GOSUB TIMEINC
        PRINT #3, "       ";
        GOSUB RANGEONE
        GOSUB TEXTLPRINT
        RETURN
REM *************************************************************************
REM
REM     STAGE 4 ALT: A/C STRAIGHT AND LEVEL
REM
REM *************************************************************************
STAGE4:
        T = ((100 - SETALT) / 100) * 60
        RETURN
REM *************************************************************************
REM
REM     STAGE 4 ALT: A/C STRAIGHT AND LEVEL
REM
REM *************************************************************************
PSTAGE4:
        N = 2
        N1$ = "STAGE 4 ALT:"
        N2$ = "A/C STRAIGHT AND LEVEL"
        GOSUB TIMEINC
        PRINT #3, "       ";
        GOSUB RANGEONE
        GOSUB TEXTLPRINT
        RETURN
REM *************************************************************************
REM
REM     TLX STABLE AT PRESET ALT
REM
REM *************************************************************************
STAGE4D:
        T = DELAY
        RETURN
REM *************************************************************************
REM
REM     TLX STABLE AT PRESET ALT
REM
REM *************************************************************************
PSTAGE4D:
        N = 2
        N1$ = "TLX STABLE AT PRESET ALT"
        N2$ = "MAKE FINAL ALTITUDE ADJUST"
        GOSUB TIMEINC
        PRINT #3, "       ";
        GOSUB RANGEONE
        GOSUB TEXTLPRINT
        RETURN
REM *************************************************************************
REM
REM     MAKE FINAL ALT CORRECTION
REM
REM *************************************************************************
ADJUST:
        T = DELAY
        RETURN
REM *************************************************************************
REM
REM     MAKE FINAL ALT CORRECTION
REM
REM *************************************************************************
PADJUST:
        N = 1
        N1$ = "TLX STABLE AT PRESENTATION ALT"
        GOSUB TIMEINC
        PRINT #3, PRANGE; TAB(18);
        GOSUB RANGEONE
        GOSUB TEXTLPRINT
        RETURN
REM *************************************************************************
REM
REM     START PRESENTATION
REM
REM *************************************************************************
START:
        LASTCALL = PRANGE
REM *************************************************************************
REM
REM     SHIP CALL 15,000 YARDS
REM
REM *************************************************************************
        T = ((LASTCALL - 15000) * 3) / V
        N = 1
        N1$ = "SHIP CALL RANGE"
        GOSUB TIMEINC
        PRINT #3, " 15000 ";
        M0 = 15000 / 6076 * 3: GOSUB RANGEONE
        GOSUB TEXTLPRINT
        LASTCALL = 15000
REM *************************************************************************
REM
REM     SHIP CALL 10,000 YARDS START CLOCK
REM
REM *************************************************************************
        T = ((LASTCALL - 10000) * 3) / V
        T1 = 0
        N = 1
        N1$ = "SHIP CALL RANGE:START A/C CLOCK"
        GOSUB TIMEINC
        PRINT #3, " 10000 ";
        M0 = 10000 * 3 / 6076: GOSUB RANGEONE
        T = 0
        GOSUB STOPWATCH
        GOSUB TEXTLPRINT
        LASTCALL = 10000
REM *************************************************************************
REM
REM     CHECK FOR POP UP
REM
REM *************************************************************************
        IF ACPOPUP > 8000 THEN GOSUB ACPOP
REM *************************************************************************
REM
REM     SHIP CALL RANGE
REM
REM *************************************************************************
        T = ((LASTCALL - 8000) * 3) / V
        N = 1
        N1$ = "SHIP CALL RANGE"
        GOSUB TIMEINC
        PRINT #3, "  8000 ";
        M0 = 8000 * 3 / 6076: GOSUB RANGEONE
        GOSUB STOPWATCH
        GOSUB TEXTLPRINT
        LASTCALL = 8000
REM *************************************************************************
REM
REM     CHECK FOR POP UP
REM
REM *************************************************************************
        IF ACPOPUP > 6000 THEN GOSUB ACPOP
REM *************************************************************************
REM
REM     SHIP CALL 6,000 YARDS
REM
REM *************************************************************************
        T = ((LASTCALL - 6000) * 3) / V
        N = 1
        N1$ = "SHIP CALL RANGE"
        GOSUB TIMEINC
        PRINT #3, "  6000 ";
        M0 = 6000 * 3 / 6076: GOSUB RANGEONE
        GOSUB STOPWATCH
        GOSUB TEXTLPRINT
        LASTCALL = 6000
REM *************************************************************************
REM
REM     POPUP MUST BE INSIDE 6000 YARDS
REM
REM *************************************************************************
        IF ACPOPUP < 6000 THEN GOSUB ACPOP
        GOTO XXXX
REM *************************************************************************
REM
REM     DETERMINE CABLE DELAY MAGNITUDE TO TIME TO CLIMB
REM
REM *************************************************************************
        IF T < DELAY THEN GOTO 2810
        T = DELAY
        GOTO 2980
REM *************************************************************************
REM
REM     A/C STRAIGHT AND LEVEL AT STAGE 1 ALT
REM
REM *************************************************************************
2810    T = (STAGE1 / (2500 / 60))
        N = 2
        N1$ = "A/C STRAIGHT AND LEVEL AT"
        N2$ = "STAGE 1 ALTITUDE"
        GOSUB TIMEINC
        PRINT #3, " ";
        PRINT #3, USING "#####"; ACPOPUP - (T * V) / 3;
        PRINT #3, " ";
        M0 = (ACPOPUP * 3 - (T * V)) / 6076: GOSUB RANGEONE
        GOSUB STOPWATCH
        GOSUB TEXTLPRINT
REM *************************************************************************
REM
REM     SHIP CALL RANGE: COMMAND TLX INTO RECOVERY MODE
REM
REM *************************************************************************
        T = DELAY - T
XXXX:
        T = ((LASTCALL - POPUP) * 3) / V
2980    N = 2
        N1$ = "SHIP CALL RANGE: COMMAND TLX"
        N2$ = "INTO RECOVERY MODE"
        GOSUB TIMEINC
        PRINT #3, " ";
        PRINT #3, USING "#####"; POPUP;
        PRINT #3, " ";
        M0 = POPUP * 3 / 6076: GOSUB RANGEONE
        GOSUB STOPWATCH
        GOSUB TEXTLPRINT
        LASTCALL = POPUP
REM *************************************************************************
REM
REM     TLX OVER SHIP
REM
REM *************************************************************************
        T = (POPUP * 3) / V
        a1 = (T * 2500 / 60) + SETALT
        a2 = a1 + (4! * 2000 / 60)
        N = 1
        N1$ = "TLX OVER SHIP"
        GOSUB TIMEINC
        PRINT #3, "     0 ";
        M0 = 0: GOSUB RANGEONE
        GOSUB STOPWATCH
        N6$ = " ": GOSUB TEXTLPRINT: N6$ = "^"
        PRINT #3, TAB(45); "AT ";
        PRINT #3, USING "####"; a1;
        PRINT #3, " FT  without target popup"
        PRINT #3, TAB(45); "AT ";
        PRINT #3, USING "####"; a2;
        PRINT #3, " FT  with target popup"
        PRINT #3,
        RETURN
REM *************************************************************************
REM
REM     AIRCRAFT POPUP SUBROUTINE
REM
REM *************************************************************************
ACPOP:  IF POPPED$ = "YES" THEN RETURN
        T = ((LASTCALL - ACPOPUP) * 3) / V
        N = 3
        N1$ = "SHIP CALL RANGE: A/C CLIMB TO"
        N2$ = "STAGE 1 ALT AT 2500 FPM AND"
        N3$ = "PRESENTATION AIRSPEED"
        GOSUB TIMEINC
        PRINT #3, " ";
        PRINT #3, USING "#####"; ACPOPUP;
        PRINT #3, " ";
        M0 = ACPOPUP * 3 / 6076: GOSUB RANGEONE
        GOSUB STOPWATCH
        GOSUB TEXTLPRINT
        POPPED$ = "YES"
        LASTCALL = ACPOPUP
        RETURN
REM *************************************************************************
REM
REM     CALCULATE POPUP RNAGE IF CLEARENCE IS GIVEN
REM
REM *************************************************************************
POPCLEAR:
        IF POPUP <> 0 THEN RETURN
        POPUP = ((clearence - SETALT) * 60 / 2500) * V1 * 1.689 / 3
        RETURN
REM *************************************************************************
REM
REM     CALCULATE RANGE AT WHICH A/C SHOULD PULL-UP
REM
REM *************************************************************************
POPUPTIME:
REM     TIME TO PULL UP
        C = STAGE1 / (2500 / 60)
REM     COMPUTE RANGE TO TARGET PULL UP IN YARDS
        R = (DELAY * V) / 3
REM     ADD POPUP REQUIRED RANGE
        ACPOPUP = R + POPUP
REM     ROUND UP TO NEAREST 100 YARDS
        ACPOPUP = (INT(ACPOPUP / 100) + 1) * 100
REM *************************************************************************
REM
REM     TLX OVER SHIP
REM
REM *************************************************************************
        T = (POPUP * 3) / V
        a1 = (T * 2500 / 60) + SETALT
        RETURN
REM *************************************************************************
REM
REM     FINISH OF LOOP
REM
REM *************************************************************************
FINISH:
        IF FORM$ = "SHORT" THEN GOTO SHORTFINISH
        IF DEV$ = "P" THEN PRINT #3, CHR$(12);
        PRINT #3,
        PRINT #3,
        PRINT #3, "--------------------------------------------------------";
        PRINT #3, "-----------------"
        PRINT #3, " PROFILE   TLX RANGE TO SHIP  PASS TIME          REMARKS"
        PRINT #3, "  TIME      YARDS     NM"
        PRINT #3, "--------------------------------------------------------";
        PRINT #3, "-----------------"
REM *************************************************************************
REM
REM     TLX AT STAGE 1 ALTITUDE
REM
REM *************************************************************************
        T = 7.5 - T + ((STAGE1 - 7.5 * 3500 / 60) / 2500) * 60
        PASTSHIP = (T * V) / 6076
        M0 = -PASTSHIP
        N = 3
        N1$ = "TLX AT STAGE 1: START TURN FOR"
        N2$ = "NEXT PASS. BANK ANGLE 25 DEG"
        N3$ = "AT STEADY PROFILE AIRSPEED"
        GOSUB TIMEINC
        PRINT #3, "       ";
        GOSUB RANGEONE
        GOSUB STOPWATCH
        GOSUB TEXTLPRINT
REM *************************************************************************
REM
REM     OUT BOUND TURN COMPLETE
REM
REM *************************************************************************
        RADIUS = (V ^ 2) / (TAN(BANK / 57.29577951#) * 32.174)
        SEMICIRCLE = 3.141592654# * RADIUS
        T = SEMICIRCLE / V
        M0 = -PASTSHIP
        N = 1
        N1$ = "TURN COMPLETE START OUT BOUND"
        GOSUB TIMEINC
        PRINT #3, "       ";
        GOSUB RANGEONE
        GOSUB TEXTLPRINT
REM *************************************************************************
REM
REM     START TURN BACK IN PROFILE
REM
REM *************************************************************************
        T = (RANGE2 + RANGE3 + PASTSHIP) * 6076 / V
        M0 = RANGE2 + RANGE3
        N = 3
        N1$ = "START TURN BACK TOWARD SHIP"
        N2$ = "BANK ANGLE 25 DEG AT PROFILE"
        N3$ = "AIRSPEED"
        GOSUB TIMEINC
        PRINT #3, "       ";
        GOSUB RANGEONE
        GOSUB TEXTLPRINT
REM *************************************************************************
REM
REM     TURN COMPLETE
REM
REM *************************************************************************
        RADIUS = (V ^ 2) / (TAN(BANK / 57.29577951#) * 32.174)
        SEMICIRCLE = 3.141592654# * RADIUS
        T = SEMICIRCLE / V
        N = 3
        N1$ = "TURN COMPLETE AT STAGE 1 ALT"
        N2$ = "STRAIGHT AND LEVEL, START DESCENT"
        N3$ = "AT 500 FPM"
        GOSUB TIMEINC
        PRINT #3, "       ";
        GOSUB RANGEONE
        N6$ = " ": GOSUB TEXTLPRINT: N6$ = "^"
        PRINT #3,
        PRINT #3, "--------------------------------------------------------";
        PRINT #3, "-----------------"
        IF DEV$ = "P" THEN PRINT #3, CHR$(12);
        CLOSE #3
        RETURN
REM *************************************************************************
REM
REM     SHORT FINISH
REM
REM *************************************************************************
SHORTFINISH:
REM *************************************************************************
REM
REM     TLX PAST SHIP
REM
REM *************************************************************************
        T = 7.5 - T + ((STAGE1 - 7.5 * 3500 / 60) / 2500) * 60
        PASTSHIP = (T * V) / 6076
        GOSUB OTIMEINC
REM *************************************************************************
REM
REM     START TURN BACK
REM
REM *************************************************************************
        RADIUS = (V ^ 2) / (TAN(BANK / 57.29577951#) * 32.174)
        SEMICIRCLE = 3.141592654# * RADIUS
        T = SEMICIRCLE / V
        GOSUB OTIMEINC
REM *************************************************************************
REM
REM     OUT BOUND LEG
REM
REM *************************************************************************
        T = (RANGE2 + RANGE3 + PASTSHIP) * 6076 / V
        M0 = RANGE2 + RANGE3
        GOSUB OTIMEINC
REM *************************************************************************
REM
REM     START TURN BACK
REM
REM *************************************************************************
        T = SEMICIRCLE / V
        GOSUB OTIMEINC
REM *************************************************************************
        PRINT #3,
        PRINT #3, "--------------------------------------------------------";
        PRINT #3, "-----------------"
        PRINT #3, "TOTAL TIME FOR PRESENTATION TO START OF NEXT PASS ";
        T = 0
        GOSUB TIMEINC
        IF DEV$ = "P" THEN PRINT #3, CHR$(12);
        CLOSE #3
        RETURN
REM *************************************************************************
REM
REM     UTILITIES
REM
REM *************************************************************************
REM
REM     print profile title
REM
REM *************************************************************************
TITLE:
        PRINT #3,
        PRINT #3,
        PRINT #3, TAB(15); "TLX MISSION PROFILE"; " "; TITLE$;
        PRINT #3, TAB(60); DATE$
        PRINT #3,
        PRINT #3, TAB(2); "CABLE LENGTH:";
        PRINT #3, USING "#####"; L;
        PRINT #3, " FT";
        PRINT #3, TAB(30); "GROUND SPEED:";
        PRINT #3, USING "###"; V1;
        PRINT #3, " KNOTS";
        PRINT #3, TAB(60); "PRESET:";
        PRINT #3, USING "###"; SETALT;
        PRINT #3, " FT"
        PRINT #3, TAB(2); "CABLE DELAY:";
        PRINT #3, USING "###.#"; DELAY;
        PRINT #3, " SECONDS";
        PRINT #3, TAB(50); "TURN BANK ANGLE:";
        PRINT #3, USING "###"; BANK;
        PRINT #3, " DEGREES"
        PRINT #3, TAB(2); "PRESENTATION START:";
        PRINT #3, USING "#####"; PRANGE;
        PRINT #3, " YARDS";
        PRINT #3, TAB(50); "TARGET POPUP RANGE:";
        PRINT #3, USING "####"; POPUP;
        PRINT #3, " YARDS"
        PRINT #3,
        PRINT #3, "--------------------------------------------------------";
        PRINT #3, "-----------------"
        PRINT #3, " PROFILE  TLX RANGE  A/C RANGE  PASS TIME        REMARKS"
        PRINT #3, "  TIME      YARDS     NM"
        PRINT #3, "--------------------------------------------------------";
        PRINT #3, "-----------------"
        RETURN
REM *************************************************************************
REM
REM TIME INCREMENT SUBROUTINE : STOP WATCH
REM
REM ************************************************************************
STOPWATCH:
        H = INT(T / 3600)
        M = INT(T / 60) - (H * 60)
        S = T - (H * 3600) - (M * 60)
        REM PRINT#3, T,H,M,S

        T1 = T1 + T

        H = INT(T1 / 3600)
        M = INT(T1 / 60) - (H * 60)
        S = T1 - (H * 3600) - (M * 60)
        REM PRINT#3, T1,H,M,S
        PRINT #3, TAB(30);
        PRINT #3, USING "##"; H;
        PRINT #3, ":";
        PRINT #3, USING "##"; M;
        PRINT #3, ":";
        PRINT #3, USING "##"; S;
        RETURN
REM *************************************************************************
REM
REM     ROUND UP TO NEAREST 30 SECONDS
REM
REM *************************************************************************
ROUND30:
        IF R$ <> "Y" THEN GOTO 4710
        IF S < 30 THEN GOTO 4690
        T = 60 - S
        RETURN
4690    T = 30 - S
        RETURN
4710    T = 0
        RETURN
REM *************************************************************************
REM
REM     LPRINT TEXT
REM
REM *************************************************************************
TEXTLPRINT:
        IF N = 0 THEN RETURN
        PRINT #3, TAB(45); N1$
        N = N - 1
        IF N = 0 THEN GOTO 4940
        PRINT #3, TAB(45); N2$
        N = N - 1
        IF N = 0 THEN GOTO 4940
        PRINT #3, TAB(45); N3$
        N = N - 1
        IF N = 0 THEN GOTO 4940
        PRINT #3, TAB(45); N4$
        N = N - 1
        IF N = 0 THEN GOTO 4940
        PRINT #3, TAB(45); N5$
        N = N - 1
        IF N = 0 THEN GOTO 4940
4940    IF N6$ = "^" THEN PRINT #3,
        RETURN
REM *************************************************************************
REM
REM     TIME INCREMENT SUBROUTINE
REM
REM *************************************************************************
TIMEINC:
        H = INT(T / 3600)
        M = INT(T / 60) - (H * 60)
        S = T - (H * 3600) - (M * 60)
        REM PRINT#3, T,H,M,S

        T2 = T2 + T
        T0 = T0 + T

        H = INT(T0 / 3600)
        M = INT(T0 / 60) - (H * 60)
        S = T0 - (H * 3600) - (M * 60)
        REM PRINT#3, T0,H,M,S
        PRINT #3, USING "##"; H;
        PRINT #3, ":";
        PRINT #3, USING "##"; M;
        PRINT #3, ":";
        PRINT #3, USING "##"; S;
        PRINT #3, "  ";

        IF DEV$ = "S" THEN FOR QQQ = 0 TO 10000: NEXT

        RETURN
REM ************************************************************************
REM
REM     COMPUTE RANGE OF AIRCRAFT
REM
REM *************************************************************************
RANGEONE:
        IF RANGEFLAG$ = "TLX" THEN GOTO RANGEONEA
        PRINT #3, "  ";
        PRINT #3, USING "###.#"; M0 - L / 6076;
        PRINT #3, "  ";
        RETURN
RANGEONEA:   
        PRINT #3, "  ";
        PRINT #3, USING "###.#"; M0 - L / 6076;
        PRINT #3, "  ";
        RETURN
REM *************************************************************************
REM
REM     TIME INCREMENT SUBROUTINE WITHOUT PRINT
REM
REM *************************************************************************
OTIMEINC:
        H = INT(T / 3600)
        M = INT(T / 60) - (H * 60)
        S = T - (H * 3600) - (M * 60)
        REM PRINT#3, T,H,M,S

        T2 = T2 + T
        T0 = T0 + T

        H = INT(T0 / 3600)
        M = INT(T0 / 60) - (H * 60)
        S = T0 - (H * 3600) - (M * 60)

        RETURN
REM *************************************************************************
REM
REM     TEST DATA FOR HANDBOOK DEMO
REM
REM *************************************************************************
TESTDATA:
        TITLE$ = "FIRST PRESENTATION"
        LEVEL = 2
        STAGE1 = 750
        L = 20000
        V1 = 250
        BANK = 25
        PRANGE = 20000
        POPUP = 1000
        SETALT = 20
        R$ = "Y"
        DEV$ = "S"
        FORM$ = "SHORT"
        GOSUB PROFILE
        END
REM *************************************************************************
REM
REM     READ PROFILE DATA FROM FILE
REM
REM *************************************************************************
READFILE:
        OPEN "PROFILE.DAT" FOR INPUT AS #1
READFILE1:
        INPUT #1, TITLE$: IF TITLE$ = "*" THEN CLOSE #1: END
        INPUT #1, LEVEL
        INPUT #1, STAGE1
        INPUT #1, L
        INPUT #1, V1
        INPUT #1, BANK
        INPUT #1, PRANGE
        INPUT #1, POPUP
        INPUT #1, SETALT
        INPUT #1, R$
        INPUT #1, DEV$
        INPUT #1, FORM$
        IF DEV$ = "F" THEN INPUT #1, FILE$
REM     PRINT TITLE$, LEVEL, STAGE1, L, V1, BANK, PRANGE,POPUP, SETALT, R$, DEV$, FORM$
        GOSUB PROFILE
        GOTO READFILE1
REM *************************************************************************


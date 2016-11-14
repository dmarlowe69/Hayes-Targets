REM *************************************************************************
REM
REM     program name: track
REM
REM     target free fall trajectory program
REM
REM *************************************************************************
        debug = 0
        CLS : SCREEN 0: COLOR 3
        DIM ALT(100), KIAS(100), SLANT(100, 100), RANGE(100, 100)
        FOR H = 0 TO 5000 STEP 1000
        R0 = EXP(-.00003 * H - 3.04E-15 * H ^ 3)
        R1 = .002378 * R0
        IF debug THEN PRINT R0
        NEXT H
REM *************************************************************************
        PRINT "TARGET TRAJECTORY CALCULATION PROGRAM"
        PRINT "written by Denton Marlowe"
        PRINT "(C)1987 HAYES TARGETS DIVISION"
        PRINT "REFERENCE AIRFORCE PARACHUTE MANUAL"
        PRINT
        INPUT "ENTER TARGET WEIGHT (LBS) AND DRAG COEFFICIENT (9INCH):"; W, C0
        INPUT "ENTER STARTING ALT, ENDING ALT AND ALT INCREMENT"; ALTS, ALTE, ALTI
        INPUT "ENTER STARTING KIAS,ENDING KIAS AND KIAS INCREMENT"; VELS, VELE, VELI
REM INPUT"ENTER INITIAL FLIGHT ANGLE (0 FOR LEVEL TRACK) :";O
REM INPUT "PRINT OPTIONS (0)RESULT (1)COMPLETE";P$
        P$ = "0"
REM *************************************************************************
REM
REM     ALTITUDE AND VELOCITY LOOPS
REM
REM *************************************************************************
        J = 0
262 FOR V0 = VELS TO VELE STEP VELI
        J = J + 1
        I = 0

263 FOR H0 = ALTS TO ALTE STEP ALTI
        I = I + 1
265 H = H0
270 M = W / 32.174
280 V = (V0 * 1.689) * SQR(EXP(.00003 * H + 3.04E-15 * H ^ 3))
290 V1 = SQR(W / (C0 * .5 * .002378 * .4417))
300 REM PRINT
305 REM PRINT "KIAS :"; V0; ""
310 REM PRINT "TRUE AIRSPEED (FPS) :"; V; ""
320 REM PRINT "VTERM AT SEALEVEL (FPS) "; V1; ""
330 REM PRINT
340 T = 0
350 R = 0
351 O = 0
360 IF P$ = "0" GOTO 410
370 PRINT "   TIME           RATE           ANGLE"; ""
380 PRINT "         HEIGHT        SLANT RANGE"; ""
390 PRINT "   SEC            FPS             DEG"; ""
400 PRINT "           FT              FT  "; ""
410 REM PRINT
420 REM****************
430 R0 = EXP(-.00003 * H - 3.04E-15 * H ^ 3)
440 R1 = .002378 * R0
450 V1 = SQR(W / (C0 * .5 * R1 * .4417))
460 F1 = 32.174 * SIN(O) - (R1 * V * V * C0 * .4417 / (2 * M))
470 F2 = 32.174 * COS(O)
480 F3 = -V * SIN(O)
490 F4 = V * COS(O)
500 REM  PRINT F1,F2,F3,F4
510 REM****************
520 V = V + F1 * .05
530 IF V > V1 THEN V = V1
540 O = O + F2 * .05 / 57.3
550 H = H + F3 * .05
560 R = R + F4 * .05
570 T = T + .05
580 REM****************
590 IF P$ = "0" GOTO 600 ELSE PRINT T, V, O * 57.3, H0 - H, SQR(R * R + H0 * H0)
600 IF H < 0 THEN GOTO 630
610 REM****************
620 GOTO 430
630 REM NEXT V0
631 REM STOP
640 REM PRINT "IMPACT"; ""
650 REM PRINT
660 REM PRINT "TIME TO IMPACT (SEC): "; T; ""
670 REM PRINT "RATE OF DESCENT AT IMPACT (FPS): "; V; ""
680 REM PRINT "RANGE DISTANCE AT IMPACT (FT): "; R; ""
681 REM PRINT "SLANT RANGE DISTANCE AT IMPACT (FT): "; SQR(R * R + H0 * H0); ""
690 REM PRINT
REM *************************************************************************
REM
REM     LOAD ARRAYS WITH RESULTS
REM
REM *************************************************************************
       
        ALT(I) = H0
        SPEED(J) = V0
        RANGE(I, J) = R
        SLANT(I, J) = SQR(R * R + H0 * H0)

691 NEXT H0
692 NEXT V0

REM *************************************************************************
REM
REM     PRINT ARRAYS
REM
REM *************************************************************************
        II = I: JJ = J:
        PRINT TAB(25); "AIRSPEED KIAS"
        PRINT "ALT(ft)    ";
        FOR J = 1 TO JJ STEP 1
        PRINT SPEED(J); "     ";
        NEXT J
        PRINT

        FOR I = 1 TO II STEP 1
        PRINT USING "#####"; ALT(I);
        PRINT "       ";
        FOR J = 1 TO JJ STEP 1
REM        PRINT USING "#####.#"; SLANT(I, J);
        PRINT USING "#####.#"; RANGE(I, J);
        PRINT "    ";
        NEXT J: PRINT
        NEXT I
REM *************************************************************************
REM
REM     WRITE DATA TO FILE
REM
REM *************************************************************************
        INPUT "Write data to file (y/n)"; a$: a$ = UCASE$(a$)
        IF a$ <> "Y" THEN END
        OPEN "c:\qbasic\software\track\TRACK.DAT" FOR OUTPUT AS #1
        PRINT #1, " "
        FOR I = 1 TO II STEP 1
        PRINT #1, USING "#####.##"; ALT(I)
        NEXT I
        PRINT #1, "*"
        FOR J = 1 TO JJ STEP 1
        PRINT #1, USING "#####.##"; SPEED(J);
        PRINT #1, " KIAS"
        FOR I = 1 TO II STEP 1
REM        PRINT #1, USING "#####.##"; SLANT(I, J)
        PRINT #1, USING "#####.##"; RANGE(I, J)
        NEXT I
        PRINT #1, "*"
        NEXT J
        PRINT #1, "$"
        CLOSE 1
        PRINT "Run macro TRACK.xlm under excel to plot data"
REM *************************************************************************
        END


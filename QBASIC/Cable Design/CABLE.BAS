50      CLS : SCREEN 0: COLOR 3
100 PRINT "HAYES CABLE DESIGN PROGRAM V19AUG88 (C)HAYES TARGETS DIVISION"
110 DIM D(50, 4), L(50), T(50), P(50), M(50, 3), S(50, 3), F(50), C(50), W(50)
REM***************************************************************
REM
REM          LOAD ARRAYS WITH CABLE SPECIFICATION ON FILE
REM
REM***************************************************************
REM                     MONIFILAMENT CABLE DATA
REM                    DIAMETER(INCHES),RTS (LBS)
REM***************************************************************
loadmono:
        FOR I1 = 1 TO 100
                READ D(I1, 2), S(I1, 2), M(I1, 2)
                IF D(I1, 2) = 999 THEN I1 = 999
        NEXT I1
REM   DATA .019,102,1.067
DATA .024,154,1.536
DATA .028,206,2.091
DATA .032,263,2.731
DATA .035,314,3.27
DATA .038,362,3.85
DATA .041,418,4.48
DATA .044,480,5.16
DATA .047,540,5.89
DATA .050,605,6.67
DATA .053,680,7.49
DATA .056,745,8.36
DATA .059,816,9.2797
DATA .062,900,10.2474
REM  DATA .064,940,10.92
DATA .066,1005,11.62
REM  DATA .069,1090,12.696
DATA .072,1150,13.824
REM   DATA .075,1270,15.000
REM   DATA .078,1325,16.224
DATA 999,999,999
REM***************************************************************
REM                    STRANDED CABLE DATA
REM            DIAMETER(INCHES),RTS (LBS),WEIGHT FACTOR
REM***************************************************************
loadstrand:
        FOR I1 = 1 TO 100
                READ D(I1, 1), S(I1, 1), M(I1, 1)
                IF D(I1, 1) = 999 THEN I1 = 999
        NEXT I1
DATA .028,200,422.07
DATA .032,260,422.07
DATA .035,290,422.07
DATA .040,380,441.18
DATA .044,440,428.06
DATA .050,560,422.43
DATA .055,680,424.88
DATA .059,750,416.62
DATA .064,880,420.32
DATA .070,1040,430.30
DATA .076,1210,425.35
DATA .082,1390,419.92
DATA .087,1570,428.75
DATA .092,1770,424.57
DATA .097,1980,444.29
DATA .100,1500,444.29
DATA .104,2200,427.18
DATA .108,2390,443.28
DATA .114,2660,435.93
DATA 999,999,999
REM***************************************************************
REM                 CABLE DESIGN INPUT DATA
REM***************************************************************
INPUTDATA:
        D0$ = "0"
        INPUT "PROJECT NAME "; T2$: T2$ = UCASE$(T2$)
                IF T2$ = "DATA" THEN GOTO READDATA
                IF T2$ = "TEST" THEN GOTO TESTDATA
                IF T2$ = "FILE" THEN GOTO READFILE
        INPUT "DESIGN NAME ( TARGET , LTC-6 ) "; T1$: T1$ = UCASE$(T1$)
                IF T1$ = "LTC-6" THEN GOTO GETNAME
                        T0$ = T1$
                        GOTO INPUTPARA
GETNAME:
        INPUT "TARGET NAME "; T0$
INPUTPARA:
        INPUT "WEIGHT (LBS), CD (DRAG COEFFICIENT) ,G LOADING "; M, C0, G
        INPUT "TARGET SPOOL VOLUME (CUBIC INCHES) OR DIAMETER (IN) "; S0
                IF S0 = 9 THEN S0 = 268
                IF S0 = 7 OR S0 = 7.125 THEN S0 = 171
        INPUT "REQUIRED CABLE LENGHT (FEET) , AIRSPEED (KIAS) , SAFETY FACTOR "; L, K, F0
        INPUT "CABLE ANALYSIS STEP LENGTH ( 1 - 25 FT)"; L8
                IF L8 <= 0 OR L8 > 25 THEN L8 = 10
STARTSTRAND:
        INPUT "STRANDED LEADER CABLE DIAMETER (INCHES) (ZERO IF NONE) "; D1
                IF D1 = 0 THEN GOTO STARTMONO
        D = D1
        GOSUB 4890
        IF S = 9999 THEN GOTO STARTSTRAND
        INPUT "CUT OFF ANGLE (DEGREES) OR LEADER LENGTH (FEET) "; P1
        IF P1 > 100 OR P1 = 0 THEN Y1 = 0 ELSE Y1 = P1 / 57.29577951#
STARTMONO:
        INPUT "STARTING MONOFILAMENT DIAMETER (INCHES) (ZERO IF NONE) "; D2
        IF D2 = 0 GOTO DESIGN
        D = D2
        GOSUB 4840
        IF S = 9999 THEN GOTO STARTMONO
REM***************************************************************
REM
REM              ROUTINE TO DESIGN A CABLE
REM
REM***************************************************************
DESIGN:
        I = 0
        J = 0
        L0 = 0: H = 0: X = 0: W = 0: V = 0
        D6 = .5: N6 = 0: D7 = .625: N7 = 0
        L0$ = "0"
REM***************************************************************
        GOSUB TARGETLOADS
        T2 = T(0)
REM***************************************************************
PRINT
PRINT "                    TOW CABLE STEADY STATE DESIGN"
PRINT "                            HAYES METHOD"
PRINT
PRINT "   DIA        LENGTH    TENSION   ANGLE";
PRINT "    SF     FC    SEG WT      REVS"
PRINT "  (IN)         (FT)      (LBS)    (DEG)";
PRINT "          (IN)   (LBS)     SEG  ACCUM"
PRINT
PRINT " TARGET"; TAB(24); " ";
PRINT USING "####"; T(0);
PRINT TAB(33); " ";
PRINT USING "##.##"; P(0) * 57.3
REM***************************************************************
1330 D = D1
1340 IF D1 = 0 THEN GOSUB 4510 ELSE GOSUB 4890
1350 IF D = 999 THEN GOTO DESIGNEND
1360 GOTO DESIGNLOOP
REM***************************************************************
1380 IF D2 <> 0 THEN GOSUB 4510 ELSE GOSUB 4570
1390 IF D = 999 GOTO DESIGNEND
REM***************************************************************
DESIGNLOOP:
1410 I = I + 1
1420 M0 = M1 * D * D * L8 / 144
1430 V0 = D * D * L8 * 12
1440 L0 = L0 + L8
1450 P0 = ATN(T1 / T0)
1460 IF P(I - 1) - P0 < .0005 THEN P0 = P(I - 1)
1470 W = W + M0
1480 V = V + V0
1490 H = H + L8 * SIN(P0)
1500 X = X + L8 * COS(P0)
1510 IF L0$ = "0" THEN GOTO 1540 ELSE GOTO 1520
1520 IF L0 > L OR V > (S0 + 513 - 6.28) THEN GOTO 2310
1530 GOTO 1550
1540 IF L0 > L OR V > S0 THEN GOTO 2310
1550 L1 = D * L8 * Q * 2! * SIN(P0) ^ 2 * COS(P0) / 12
1560 D3 = D * L8 * Q * (2! * SIN(P0) ^ 3 + 3.1416 * C) / 12
1570 T0 = T0 + D3
1580 T1 = T1 + M0 - L1
1590 T2 = SQR(T0 * T0 + T1 * T1)
1600 IF I = 1 AND P0 < Y1 AND Y1 > 0 THEN GOTO 1630
1610 IF I = 1 AND L0 > P1 AND Y1 = 0 AND D1 <> 0 AND P1 <> 0 THEN GOTO 1630
1620 IF T2 < (S / F0) THEN GOTO 1440
1630 L0 = L0 - L8
1640 T0 = T0 - D3
1650 T1 = T1 - M0 + L1
1660 W = W - M0
1670 M(I, 3) = W
1680 V = V - V0
1690 H = H - L8 * SIN(P0)
1700 X = X - L8 * COS(P0)
1710 IF L0 > 0 THEN GOTO 1740
1720 I = I - 1
1730 GOTO 1380
1740 T(I) = SQR(T0 * T0 + T1 * T1)
1750 P(I) = P0
1760 L(I) = L0
1770 D(I, 3) = D
1780 D(I, 4) = C
1790 F(I) = S / T(I)
1800 S(I, 3) = S
1810 GOSUB 2790
1820 C(I) = C1
1830 W(I) = N6
1840 GOSUB 1980
1850 GOTO 1380
1860 PRINT
1870 PRINT "   DIA        LENGTH    TENSION   ANGLE";
1880 PRINT "    SF     FC    SEG WT      REVS"
1890 PRINT "  (IN)         (FT)      (LBS)    (DEG)";
1900 PRINT "          (IN)   (LBS)     SEG  ACCUM"
1910 PRINT
1920 RETURN
1930 PRINT " TARGET"; TAB(24); " ";
1940 PRINT USING "####"; T(0);
1950 PRINT TAB(33); " ";
1960 PRINT USING "##.##"; P(0) * 57.3
1970 RETURN
1980 PRINT ; " ";
1990 PRINT USING "#.###"; D;
2000 IF D(I, 4) = .007 THEN PRINT " 1 X 7";
2010 PRINT TAB(13); " ";
2020 PRINT USING "#####"; L(I) - L(I - 1);
2030 PRINT TAB(24); " ";
2040 PRINT USING "####"; T(I);
2050 PRINT TAB(33); " ";
2060 PRINT USING "##.##"; P(I) * 57.3;
2070 PRINT TAB(40); " ";
2080 PRINT USING "##.##"; F(I);
2090 PRINT TAB(48); " ";
2100 PRINT USING "#.###"; C(I);
2110 PRINT TAB(55); " ";
2120 PRINT USING "###.##"; M(I, 3) - M(I - 1, 3);
2130 PRINT TAB(64); " ";
2140 PRINT USING "#####"; W(I) - W(I - 1);
2150 PRINT TAB(71); " ";
2160 IF J <> 0 THEN GOTO 2190
2170 PRINT USING "#####"; W(I)
2180 RETURN
2190 PRINT USING "#####"; W(I) - W(J)
2200 RETURN
2210 PRINT
2220 PRINT
2230 PRINT
2240 PRINT
2250 RETURN
2260 REM***************************************************************
2270 REM
2280 REM           OUTPUT FINAL CABLE STEP PARAMETERS
2290 REM
2300 REM***************************************************************
2310 L(I) = L0 - L8
2320 T(I) = T2
2330 P(I) = P0
2340 W = W - M0
2350 M(I, 3) = W
2360 V = V - V0
2370 H = H - L8 * SIN(P0)
2380 X = X - L8 * COS(P0)
2390 D(I, 3) = D
2400 D(I, 4) = C
2410 S = S - 15
2420 S(I, 3) = S
2430 F(I) = S / T(I)
2440 GOSUB 2790
2450 C(I) = C1
2460 W(I) = N6
PRINT ; " ";
PRINT USING "#.###"; D;
IF D(I, 4) = .007 THEN PRINT " 1 X 7";
PRINT TAB(13); " ";
PRINT USING "#####"; L(I) - L(I - 1);
PRINT TAB(24); " ";
PRINT USING "####"; T(I);
PRINT TAB(33); " ";
PRINT USING "##.##"; P(I) * 57.3;
PRINT TAB(40); " ";
PRINT USING "##.##"; F(I);
PRINT TAB(48); " ";
PRINT USING "#.###"; C(I);
PRINT TAB(55); " ";
PRINT USING "###.##"; M(I, 3) - M(I - 1, 3);
PRINT TAB(64); " ";
PRINT USING "#####"; W(I) - W(I - 1);
PRINT TAB(71); " ";
IF J <> 0 THEN GOTO LTCTGTEND
PRINT USING "#####"; W(I): GOTO DESIGNEND
LTCTGTEND: PRINT USING "#####"; W(I) - W(J)
REM******************************************************
DESIGNEND:
2481 PRINT
2482 PRINT "CABLE WEIGHT (LBS):";
2483 PRINT USING "####.#"; W;
2484 PRINT TAB(35); "VOLUME (CUBIC INCHES):";
2485 PRINT USING "####.#"; V
2486 PRINT "CABLE DROOP  (FT) :";
2487 PRINT USING "######"; H * 1.25;
2488 PRINT TAB(35); "DESIGN LENGTH (FT)   :";
2489 PRINT USING "######"; L(I)
2490 PRINT "CABLE TRAIL  (FT) :";
2491 PRINT USING "######"; X;
2492 PRINT TAB(35); "CABLE REVS :";
2493 PRINT USING "######"; W(I)
2494 REM***************************************************************
2500 REM
2510 REM                    LTC-6 REEL DESIGN
2520 REM
2530 REM***************************************************************
2540 IF L0 > L OR D = 999 THEN GOTO DESIGNOPT
2550 IF L0$ = "0" AND T1$ = "LTC-6" GOTO 2560 ELSE GOTO DESIGNOPT
2560 L0$ = "1"
2570 J = I
2580 W2 = W
2590 V2 = V
2600 PRINT
2610 PRINT "                   LTC-6 CABLE STEADY STATE DESIGN"
2620 PRINT "                           HAYES METHOD"
2630 GOSUB 1860
2640 GOTO 1380
REM***************************************************************
REM
REM            SUBROUTINE TO CALCULATE TARGET LOADS
REM
REM***************************************************************
TARGETLOADS:
        V1 = K * 1.69
        Q = .5 * .002378 * V1 * V1
        A = 3.1416 * (9 / 2) ^ 2
        T0 = C0 * Q * A / 144
        T1 = M * G
        T(0) = SQR(T0 * T0 + T1 * T1)
        P(0) = ATN(T1 / T0)
        RETURN
2760 REM***************************************************************
2770 REM            REEL FLANGE CLEARENCE FOR TARGET
2780 REM***************************************************************
2790 C1 = V + .8835729
2800 C1 = SQR(C1 / 3.1416 / 4.5)
2810 IF S0 > 171 THEN C1 = (9 / 2) - C1 - .07 ELSE C1 = (7.125 / 2) - C1 - .0725
2820 IF T1$ = "LTC-6" AND C1 < 0 THEN GOTO 2830 ELSE GOTO 2900
2830 C1 = V + 1.84078125# - V2
2840 C1 = SQR(C1 / 3.1416 / 6)
2850 C1 = (11 / 2) - C1
2860 GOTO 3040
2870 REM**************************************************************
2880 REM         CALCULATE NUMBER OF REVS
2890 REM**************************************************************
2900 IF I = 1 THEN L6 = L(I) ELSE L6 = L(I) - L(I - 1)
2910 W6 = 4.5 / D
2920 D6 = D6 + 2 * D - (D / 6.0955)
2930 C6 = D6 * 3.1416 / 12
2940 N6 = N6 + W6
2950 L6 = L6 - C6 * W6
2960 IF L6 > 0 THEN GOTO 2920
2970 N8 = L6 / C6
2980 N6 = N6 + N8
2990 REM PRINT "DIR";D6;" FC ";4.5-D6/2
3000 RETURN
3010 REM***************************************************************
3020 REM               REVS ON LTC-6 REEL
3030 REM***************************************************************
3040 L7 = L(I) - L(I - 1)
3050 W7 = 6 / D
3060 D7 = D7 + 2 * D - (D / 6.0955)
3070 C7 = D7 * 3.1416 / 12
3080 N6 = N6 + W7
3090 L7 = L7 - C7 * W7
3100 IF L7 > 0 THEN GOTO 3060
3110 N8 = L7 / C7
3120 N6 = N6 + N8
3130 RETURN
REM***************************************************************
REM
REM                   SELECT DESIGN OPTIONS
REM
REM***************************************************************
DESIGNOPT:
PRINT "YOUR OPTIONS ARE:"
PRINT " (P) PRINT OUT (M) SELECT MONOFILAMENT (V) NEW AIRSPEED "
PRINT " (N) NEW SAFETY FACTOR (R) RE-DESIGN (S) INIT (T) ABORT RUN";

INPUT I$: I$ = UCASE$(I$)
        IF I$ = "F" THEN GOSUB WRITEFILE: GOTO DESIGNOPT
        IF I$ = "T" THEN END
        IF I$ = "P" THEN GOTO designprt
        IF I$ = "M" THEN GOTO STARTMONO
        IF I$ = "R" THEN GOTO STARTSTRAND
        IF I$ = "S" THEN GOTO INPUTPARA
        IF I$ = "L" THEN GOTO NEWLENGTH
        IF I$ = "V" THEN GOTO NEWSPEED
        IF I$ = "N" THEN GOTO NEWSAFETY ELSE GOTO DESIGNOPT

NEWSPEED: INPUT "ENTER NEW AIRSPEED (KIAS)"; K: GOTO DESIGN
NEWSAFETY: INPUT "ENTER NEW SAFETY FACTOR"; F0: GOTO DESIGN
NEWLENGTH: INPUT "ENTER NEW LENGTH:"; L: GOTO DESIGN
3340 REM***************************************************************
designprt:
        INPUT "(S)creen or (P)rinter"; dev$: dev$ = UCASE$(dev$)
        IF dev$ <> "S" AND dev$ <> "P" THEN GOTO designprt
        IF dev$ = "S" THEN OPEN "SCRN:" FOR OUTPUT AS #3
        IF dev$ = "P" THEN OPEN "LPT1:" FOR OUTPUT AS #3
        J1 = J
3360 GOSUB 3500
        PRINT #3,
        PRINT #3,
        PRINT #3,
        PRINT #3,
        CLOSE #3
3370 REM***************************************************************
3380 REM           SET OUTPUT FLAG AND RUN ANALYSIS
3390 REM***************************************************************
3410 A0$ = "1"
3420 GOSUB 5170
3430 IF O1 <> 1 THEN END
        PRINT #3,
        PRINT #3,
        PRINT #3,
        PRINT #3,
        CLOSE #3
3440 END
3450 REM***************************************************************
3460 REM
3470 REM          SUBROUTINE TO PRINT OUT CABLE DESIGN
3480 REM
3490 REM***************************************************************
3500 REM**************************************************** GOSUB 2210
3501 PRINT #3,
3502 PRINT #3,
3503 PRINT #3,
3510 PRINT #3, "                   TOW CABLE STEADY STATE ANALYSIS"
3520 PRINT #3, "                            HAYES METHOD"
3530 PRINT #3,
3540 PRINT #3, "DATE "; DAT; "  "; T2$
3550 IF D1 = 0 THEN GOTO 3590
3560 PRINT #3, "FIRST SEGMENT IS 1 X 7 STRANDED CABLE : CUT-OFF ANGLE (DEG):";
3570 PRINT #3, USING "##.##"; P(1) * 57.3
3580 PRINT #3, "STRANDED CF: 0.007"; "  ";
3590 PRINT #3, "CD0:  2"; "  ";
3600 IF D2 <> 0 THEN PRINT #3, "MONOFILMENT CF: 0.0057" ELSE PRINT #3,
3610 PRINT #3, "REEL VOLUME AVAILABLE (CUBIC INCHES): ";
3620 PRINT #3, USING "####.#"; S0;
3630 IF S0 > 180 THEN PRINT #3, " (9 INCH REEL)" ELSE PRINT #3, " (7 1/8 INCH REEL)";
3640 PRINT #3, "ANALYSIS DESIGN STEP LENGTH: "; L8
3650 PRINT #3,
3660 PRINT #3, "TARGET:"; T0$; TAB(30); "WEIGHT (LBS):"; TAB(60); M
3670 PRINT #3, "AIRSPEED (KIAS):"; TAB(20); K, "DRAG COEFFICIENT "; TAB(60);
3680 PRINT #3, USING "#.###"; C0
3690 F0 = 999
3700 FOR I1 = 1 TO I
3710 IF F(I1) < F0 THEN F0 = F(I1)
3720 NEXT I1
3730 PRINT #3, "SAFETY FACTOR:"; TAB(20);
3740 PRINT #3, USING "##.##"; F0;
3750 PRINT #3, TAB(30); "G FACTOR:"; TAB(60); G
3760 REM*************************************************** GOSUB 1860
3761 PRINT #3,
3762 PRINT #3, "   DIA        LENGTH    TENSION   ANGLE";
3763 PRINT #3, "    SF     FC    SEG WT      REVS"
3764 PRINT #3, "  (IN)         (FT)      (LBS)    (DEG)";
3765 PRINT #3, "          (IN)   (LBS)     SEG  ACCUM"
3766 PRINT #3,
3770 REM*************************************************** GOSUB 1930
3371 PRINT #3, " TARGET"; TAB(24); " ";
3772 PRINT #3, USING "####"; T(0);
3773 PRINT #3, TAB(33); " ";
3774 PRINT #3, USING "##.##"; P(0) * 57.3
3780 REM***************************************************************
3790 I1 = 1
3800 PRINT #3, ; " ";
3810 PRINT #3, USING "#.###"; D(I1, 3);
3820 IF D(I1, 4) = .007 THEN PRINT #3, " 1 X 7";
3830 PRINT #3, TAB(13); " ";
3840 PRINT #3, USING "#####"; L(I1) - L(I1 - 1);
3850 PRINT #3, TAB(24); " ";
3860 PRINT #3, USING "####"; T(I1);
3870 PRINT #3, TAB(33); " ";
3880 PRINT #3, USING "##.##"; P(I1) * 57.3;
3890 PRINT #3, TAB(40); " ";
3900 PRINT #3, USING "##.##"; F(I1);
3910 PRINT #3, TAB(48); " ";
3920 PRINT #3, USING "##.###"; C(I1);
3930 PRINT #3, TAB(56); " ";
3940 PRINT #3, USING "###.##"; M(I1, 3) - M(I1 - 1, 3);
3950 PRINT #3, TAB(65); " ";
3960 PRINT #3, USING "#####"; W(I1) - W(I1 - 1);
3970 PRINT #3, "  ";
3980 IF J1 <> 0 AND I1 > J1 THEN GOTO 3990 ELSE GOTO 4010
3990 PRINT #3, USING "#####"; W(I1) - W(J1)
4000 GOTO 4020
4010 PRINT #3, USING "#####"; W(I1)
4020 IF J1 = I1 THEN GOTO 4030 ELSE GOTO 4150
4030 PRINT #3,
4040 PRINT #3, TAB(10); "CABLE "; D(I1, 3); " RTS REDUCED 15 LBS FOR SWAGE"
4050 PRINT #3,
4060 PRINT #3, "CABLE WEIGHT (LBS):";
4070 PRINT #3, USING "####.#"; W2;
4080 PRINT #3, TAB(35); "VOLUME (CUBIC INCHES):";
4090 PRINT #3, USING "####.#"; V2
4100 PRINT #3, TAB(35); "DESIGN LENGTH (FT)   :";
4110 PRINT #3, USING "######"; L(I1)
4120 PRINT #3,
4130 PRINT #3, "                    LTC-6 CABLE DESIGN"
4140 REM**************************************************** GOSUB 1860
4141 PRINT #3,
4142 PRINT #3, "   DIA        LENGTH    TENSION   ANGLE";
4143 PRINT #3, "    SF     FC    SEG WT      REVS"
4144 PRINT #3, "  (IN)         (FT)      (LBS)    (DEG)";
4145 PRINT #3, "          (IN)   (LBS)     SEG  ACCUM"
4146 PRINT #3,
4150 I1 = I1 + 1
4160 IF I1 <= I THEN GOTO 3800
4170 REM***************************************************************
4180 PRINT #3,
4190 PRINT #3, TAB(10); "CABLE "; D(I, 3); " RTS REDUCED 15 LBS FOR SWAGE"
4200 PRINT #3,
4210 IF T1$ = "LTC-6" AND J1 <> 0 THEN GOTO 4220 ELSE GOTO 4310
4220 PRINT #3, "CABLE WEIGHT (LBS):";
4230 PRINT #3, USING "####.#"; W - W2;
4240 PRINT #3, TAB(35); "VOLUME (CUBIC INCHES):";
4250 PRINT #3, USING "####.#"; V - V2
4260 PRINT #3, TAB(35); "DESIGN LENGTH (FT)   :";
4270 PRINT #3, USING "######"; L(I) - L(J1)
4280 PRINT #3,
4290 PRINT #3, "                    TOTAL CABLE DESIGN"
4300 PRINT #3,
4310 PRINT #3, "CABLE WEIGHT (LBS):";
4320 PRINT #3, USING "####.#"; W;
4330 PRINT #3, TAB(35); "VOLUME (CUBIC INCHES):";
4340 PRINT #3, USING "####.#"; V
4350 PRINT #3, "CABLE DROOP  (FT) :";
4360 PRINT #3, USING "######"; H * 1.25;
4370 PRINT #3, TAB(35); "DESIGN LENGTH (FT)   :";
4380 PRINT #3, USING "######"; L(I)
4390 PRINT #3, "CABLE TRAIL  (FT) :";
4400 PRINT #3, USING "######"; X;
4410 PRINT #3, TAB(35); "CABLE REVS :";
4420 PRINT #3, USING "######"; W(I)
4421 IF dev$ = "P" THEN PRINT #3, CHR$(12);
4430 RETURN
4440 REM***************************************************************
4450 REM
4460 REM SUBROUTINE TO PICK A MONFILAMENT CABLE BASED ON STRENGTH
4470 REM
4480 REM***************************************************************
4490 REM            MONOFILMENT CABLE
4500 REM***************************************************************
4510 I4 = 2
4520 D4 = D2
4530 GOTO 4600
4540 REM***************************************************************
4550 REM              STRANDED CABLE
4560 REM***************************************************************
4570 I4 = 1
4580 D4 = D1
4590 REM***************************************************************
4600 I3 = 0
4610 FOR I1 = 1 TO 100
4620 IF T2 * F0 < S(I1, I4) AND D(I1, I4) >= D4 THEN GOTO 4650
4630 IF S(I1, I4) = 999 THEN I1 = 999
4640 GOTO 4670
4650 I3 = I1
4660 I1 = 999
4670 NEXT I1
4680 IF I3 <> 0 THEN 4720
4690 PRINT "CABLE CAPABILITY EXCEEDED"
4700 D = 999
4710 RETURN
4720 D = D(I3, I4)
4730 S = S(I3, I4)
4740 IF I4 = 1 THEN M1 = .7854 * M(I3, 1) ELSE M1 = 384
4750 IF I4 = 1 THEN C = .007 ELSE C = .0057
4760 RETURN
4770 REM***************************************************************
4780 REM
4790 REM     SUBROUTINE TO RETURN MONIFILAMENT CABLE PARAMETERS
4800 REM
4810 REM***************************************************************
4820 REM              MONOFILMENT CABLE
4830 REM***************************************************************
4840 I4 = 2
4850 GOTO 4910
4860 REM***************************************************************
4870 REM                STRANDED CABLE
4880 REM***************************************************************
4890 I4 = 1
4900 REM***************************************************************
4910 I3 = 0
4920 FOR I2 = 1 TO 100
4930 IF D(I2, I4) = D THEN GOTO 4960
4940 IF D(I2, I4) = 999 THEN I2 = 999
4950 GOTO 4980
4960 I3 = I2
4970 I2 = 999
4980 NEXT I2
4990 IF I3 <> 0 THEN 5080
5000 S = 9999
5010 IF D0$ = "0" THEN GOTO 5050
5020 IF I4 = 1 THEN M1 = 380 ELSE M1 = 384
5030 IF I4 = 1 THEN C = .007 ELSE C = .0057
5040 RETURN
5050 IF I4 = 1 THEN PRINT "STRANDED ";  ELSE PRINT "MONOFILAMENT ";
5060 PRINT "CABLE DIAMETER "; D; " NOT ON FILE"
5070 RETURN
5080 S = S(I3, I4)
5090 IF I4 = 1 THEN M1 = .7854 * M(I3, 1) ELSE M1 = 384
5100 IF I4 = 1 THEN C = .007 ELSE C = .0057
5110 RETURN
5120 REM***************************************************************
5130 REM
5140 REM        ENTER AIRSPEED PARAMETERS FOR ANALYSIS
5150 REM
5160 REM***************************************************************
5170 PRINT
5180 INPUT " ENTER START AIRSPEED,FINAL AIRSPEED AND INCREMENT (KIAS):"; K, K1, K2
5200 IF K = 0 THEN RETURN
5210 INPUT " ENTER PRINTOUT OPTION 0=STANDARD 1=BREIF"; O1
5230 IF O1 <> 0 AND O1 <> 1 THEN GOTO 5210
        INPUT "(S)creen or (P)rinter"; dev$: dev$ = UCASE$(dev$)
        IF dev$ <> "S" AND dev$ <> "P" THEN GOTO designprt
        IF dev$ = "S" THEN OPEN "SCRN:" FOR OUTPUT AS #3
        IF dev$ = "P" THEN OPEN "LPT1:" FOR OUTPUT AS #3
5240 GOSUB 5360
5241 IF A0$ = "0" THEN GOTO 5250 ELSE GOSUB 2210
5250 IF A0$ = "0" THEN GOSUB 3500 ELSE GOTO 5290
5251 GOSUB 2210
5260 K = K + K2
5270 IF K > K1 THEN RETURN
5280 GOSUB 5360
5290 GOSUB 5910
5300 GOTO 5260
5310 REM***************************************************************
5320 REM
5330 REM                     CABLE ANALYSIS
5340 REM
5350 REM***************************************************************
5360 GOSUB 6370
5370 W2 = 0
5380 V2 = 0
5390 D0$ = "1"
5400 I1 = 1
5410 GOSUB TARGETLOADS
5420 L0 = L0 + L8
5430 P0 = ATN(T1 / T0)
5440 IF P(I1 - 1) - P0 < .0005 THEN P0 = P(I1 - 1)
5450 IF L0 > L(I1) THEN GOTO 5670
5460 D = D(I1, 3)
5470 REM***************************************************************
5480 IF D1 <> 0 AND I1 = 1 THEN GOSUB 4890 ELSE GOTO 5500
5490 GOTO 5580
5500 IF D2 = 0 AND I1 > 1 THEN GOSUB 4890 ELSE GOTO 5520
5510 GOTO 5580
5520 IF D1 <> 0 AND I1 > 1 THEN GOSUB 4840 ELSE GOTO 5540
5530 GOTO 5580
5540 IF D1 = 0 AND I1 > 1 THEN GOSUB 4840 ELSE GOTO 5560
5550 GOTO 5580
5560 GOSUB 4840
5570 REM***************************************************************
5580 L1 = D * L8 * Q * 2! * SIN(P0) ^ 2 * COS(P0) / 12
5590 D3 = D * L8 * Q * (2! * SIN(P0) ^ 3 + 3.1417 * C) / 12
5600 T0 = T0 + D3
5610 T1 = T1 + (M1 * D * D * L8 / 144) - L1
5620 H = H + L8 * SIN(P0)
5630 X = X + L8 * COS(P0)
5640 W = W + M1 * D * D * L8 / 144
5650 V = V + D * D * L8 * 12
5660 GOTO 5420
5670 L0 = L0 - L8
5680 M(I1, 3) = W
5690 T(I1) = SQR(T0 * T0 + T1 * T1)
5700 P(I1) = P0
5710 IF I1 = I THEN S = S - 15
5720 IF I1 = J1 THEN GOTO 5730 ELSE GOTO 5750
5730 W2 = W
5740 V2 = V
5750 S(I1, 3) = S
5760 F(I1) = S / T(I1)
5770 X1 = I
5780 I = I1
5790 GOSUB 2790
5800 C(I1) = C1
5810 W(I1) = N6
5820 I = X1
5830 I1 = I1 + 1
5840 IF I1 > I GOTO 5850 ELSE GOTO 5420
5850 RETURN
5860 REM***************************************************************
5870 REM
5880 REM               OUTPUT CABLE ANALYSIS
5890 REM
5900 REM***************************************************************
5910 IF O1 = 1 THEN GOSUB 2220
5920 PRINT #3, "AIRSPEED (KIAS):"; K;
5930 PRINT #3, TAB(40); "TENSION (LBS):";
5931 PRINT #3, USING "#####"; T(I)
5940 PRINT #3, "CABLE DROOP (FT):";
5950 PRINT #3, USING "######"; H * 1.25;
5960 PRINT #3, TAB(40); "CABLE TRAIL (FT):";
5970 PRINT #3, USING "######"; X
5980 IF O1 = 1 THEN RETURN
5990 PRINT #3,
6000 PRINT #3, "DIA (IN)";
6010 PRINT #3, TAB(12); "LENGHT (FT)";
6020 PRINT #3, TAB(28); "TENSION (LBS)";
6030 PRINT #3, TAB(45); "CABLE SFTY FACTOR";
6040 PRINT #3, TAB(65); "ANGLE (DEG)"
6050 PRINT #3,
6060 PRINT #3, TAB(7); "TARGET";
6070 PRINT #3, TAB(25); " ";
6080 PRINT #3, USING "####"; T(0);
6090 PRINT #3, TAB(65); " ";
6100 PRINT #3, USING "##.#"; P(0) * 57.3
6110 FOR I1 = 1 TO I
6120 PRINT #3, TAB(45); " ";
6130 PRINT #3, USING "##.##"; S(I1, 3) / T(I1 - 1);
6140 PRINT #3, TAB(55); " ";
6150 PRINT #3, USING "###.#"; (1 / (S(I1, 3) / T(I1 - 1))) * 100
6160 PRINT #3, ; " ";
6170 PRINT #3, USING "#.###"; D(I1, 3);
6180 IF D(I1, 4) = .007 THEN PRINT #3, " 1 X 7";
6190 PRINT #3, TAB(10); " ";
6200 PRINT #3, USING "#####"; L(I1) - L(I1 - 1);
6210 PRINT #3, TAB(25); " ";
6220 PRINT #3, USING "####"; T(I1);
6230 PRINT #3, TAB(32); " ";
6240 PRINT #3, USING "####"; S(I1, 3);
6250 PRINT #3, " "; "RTS";
6260 PRINT #3, TAB(45); " ";
6270 PRINT #3, USING "##.##"; S(I1, 3) / T(I1);
6280 PRINT #3, TAB(55); " ";
6290 PRINT #3, USING "###.#"; (1 / (S(I1, 3) / T(I1))) * 100;
6300 PRINT #3, "%";
6310 PRINT #3, TAB(65); " ";
6320 PRINT #3, USING "##.#"; P(I1) * 57.3
6330 NEXT I1
6340 PRINT #3, TAB(15); "CABLE "; D(I, 3); " RTS REDUCED 15 LBS FOR SWAGE"
6342 IF dev$ = "P" THEN PRINT #3, CHR$(12);
6350 RETURN
6360 REM************************************************************
6370 L0 = 0
6380 H = 0
6390 X = 0
6400 W = 0
6410 V = 0
6420 D6 = .5
6430 N6 = 0
6440 D7 = .625
6450 N7 = 0
6460 RETURN
REM***************************************************************
REM
REM   READ A DESIGN FROM DATA STATEMENTS  FOR ANAYLSIS
REM
REM***************************************************************
READDATA:
A0$ = "0"
J1 = 0
READ T2$, T1$, T0$, M, C0, G, S0, L, J1, L8, D1, D2
I1 = 1
READ D(I1, 3), L(I1)
6550 I1 = I1 + 1
        READ D(I1, 3), L(I1)
        IF D(I1, 3) = 999 THEN GOTO 6600
        L(I1) = L(I1) + L(I1 - 1)
        GOTO 6550
6600 I = I1 - 1
GOSUB 5170
IF O1 = 1 THEN GOSUB 2210
END
REM*************************************************************************
REM
REM     TEST INPUT DATA FOR DESIGN
REM
REM*************************************************************************
TESTDATA:
T1$ = "LTC-6"
T0$ = "TPT-3"
M = 88.5: C0 = .34: G = 1
S0 = 276
L = 30000: K = 300: F0 = 1.7
L8 = 1
D2 = .024
D1 = .035
P1 = 390
IF P1 > 100 OR P1 = 0 THEN Y1 = 0 ELSE Y1 = P1 / 57.29577951#
GOTO DESIGN
REM*************************************************************************
REM
REM     TEST INPUT DATA FOR DESIGN
REM
REM*************************************************************************
READFILE:
        INPUT "ENTER INPUT DATA FILE NAME"; DATA$
        DATA$ = DATA$ + ".CAB"
        OPEN DATA$ FOR INPUT AS #1
        A0$ = "0"
        J1 = 0
        INPUT #1, T2$, T1$, T0$, M, C0, G, S0, L, J1, L8, D1, D2
        I1 = 1
        INPUT #1, D(I1, 3), L(I1)
READFILE1:
        I1 = I1 + 1
        INPUT #1, D(I1, 3), L(I1)
        IF D(I1, 3) = 999 THEN GOTO READFILE2
        L(I1) = L(I1) + L(I1 - 1)
        GOTO READFILE1
READFILE2:
        I = I1 - 1
        GOSUB 5170
        IF O1 = 1 THEN GOSUB 2210
        END
REM*************************************************************************
REM
REM     WRITE OUT DESIGN DATA TO A FILE
REM
REM*************************************************************************
WRITEFILE:
        INPUT "ENTER OUTPUT DATA FILE NAME"; DATA$
        DATA$ = DATA$ + ".CAB"
        OPEN DATA$ FOR OUTPUT AS #1
        PRINT #1, T2$; ","; T1$; ","; T0$; ","; M; ","; C0; ","; G; ","; S0;
        PRINT #1, L; ","; J1; ","; L8; ","; D1; ","; D2
        FOR I1 = 1 TO I
        PRINT #1, D(I1, 3); ","; L(I1)
        NEXT I1
        PRINT #1, "999,999"
        RETURN
REM***************************************************************
REM
REM USER INPUT DATA FOR ANALYSIS PROGRAM
REM
REM****************************************************************
USERDATA:
DATA "GUNNERY"
DATA "TGX-3","TGX-3",45,.20,1,513,15000,0,10,.064,.050
DATA .064,500,.050,7000,.053,1000,.056,1000,.059,1000
DATA .062,1000,.066,1000,.072,1000,999,999
DATA .050,800,.053,1750,.056,1450,.059,1550,.062,1700
DATA .066,2050,.072,2000,999,999
REM***************************************************************
END


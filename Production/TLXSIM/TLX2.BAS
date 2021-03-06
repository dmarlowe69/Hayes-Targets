1 CLS
3 PRINT TAB(17) CHR$(14) "LOW ALTITUDE TARGET MODEL "
4 PRINT TAB(30) "(1/2 INTIAL CABLE STEP)"
20 PRINT "LOW ALTITUDE TARGET MODEL"
25 PRINT "-------------------------"
30 PRINT " ":PRINT " ":PRINT " "
35 INPUT "TARGET NAME";TI$
40 PRINT "     TARGET CHARACTERISTICS"
50 INPUT "DIAMETER (IN.)";D
60 INPUT "LENGTH (IN.)";L:S=.83*D*L:A=D*D/S
70 INPUT "FLY WEIGHT (LB.)";M
80 INPUT "DRAG COEFFICIENT";CD:CD=CD*D*D*3.14/4/S:S=S/144
90 INPUT "CENTER OF PRESS. TO C.G. (IN.)";CP:CP=CP/12
100 CLS
110 PRINT "     CABLE CHARACTERISTICS"
120 INPUT "LENGTH (FT.)";LO
160 INPUT "NUMBER OF DIAMETER STEPS";NS: DIM DI(NS-1)
163 PRINT " ENTER DIAMETERS (IN.) STARTING AT AIRCRAFT"
165 FOR K=0 TO NS-1:INPUT DI(K):DI(K)=DI(K)/12:NEXT K
167 CLS:PRINT "     WING CHARACTERISTICS"
168 INPUT "WING AREA (SQ. IN.)";SW:SW=SW/144
169 INPUT "LIFT CURVE SLOPE (1/DEG.)";CL
170 INPUT "SERVO RESPONSE (DEG./SEC.)";RE
172 CLS:PRINT "     FLIGHT CONDITIONS"
173 INPUT "INDICATED AIRSPEED (KNOTS)";VA:VA=VA*1.69
175 INPUT "AIRCRAFT ALTITUDE (FT.)";AE:RH=.00238*EXP(-AE/32500):VA=VA*SQR(.00238/RH)
176 CLS:PRINT "     AIRCRAFT OSCILLATIONS"
177 INPUT "AMPLITUDE (FT.)";AA:INPUT "MAX. CLIMB RATE (FPS)";MM:MM=MM/AA
178 PRINT "     WAVE CHARACTERISTICS"
179 INPUT "HEIGHT (FT.)";WA:INPUT "LENGTH (FT.)";WL
180 CLS:PRINT "     MATHEMATICAL CONSIDERATIONS"
183 INPUT "CABLE INTERVAL LENGTH (FT. -APPROX. 50)";DL
185 INPUT "TIME INTERVAL (SEC.)";DM
186 INPUT "ALTIMETER MEMORY (SEC.)";MT:NT=MT/DM:DIM RS(NT)
187 CLS
190 GOSUB 5000
200 'INITIALIZE VARIABLES
210 CF=.0057:CC=2!
220 K=LO/DL:I=K
230 DIM T(1),P(1),VX(1),VZ(1),G(1),H(1)
235 DIM X(K,1),Z(K,1)
240 D=RH*VA*VA/2*CD*S
245 T(1)=SQR(D*D+M*M)
250 P(1)=ATN(M/D)
260 TN=T(1):PH=P(1)
270 FG=-10:FH=-5
285 GOTO 590
290 JJ=JJ+1
305 VX(0)=VX(1):VZ(0)=VZ(1)
320 N=0:J=0
325 FOR K=0 TO I:X(K,0)=X(K,1):Z(K,0)=Z(K,1):NEXT K
330 'FIND AIRCRAFT POSITION
340 AB=250:MN=25:TD=10
350 IF TM+DM<TD THEN ZA=0:GOTO 380
360 IF TM+DM<TD+3.1416*AB/MN THEN ZA=-AB*COS(MN/AB*(TM+DM-TD))+AB:GOTO 380
370 ZA=2*AB+AA*COS(MM*(TM+DM-TD-(3.1416*AB/MN)))-AA
380 GOSUB 6000
382 'FIND WAVE HEIGHT AND RATE
385 WH=WA*SIN((TM+DM)*VA/WL*6.28)
387 WR=WA*VA/WL*6.28*COS((TM+DM)*VA/WL*6.28)
420 'FIND TARGET POSITION
430 TN=T(1):PH=P(1)
435 PRINT N
440 PRINT T(1),P(1)*57.3
460 V=SQR((VA-VX(1))^2+VZ(1)*VZ(1))
470 Q=RH/2*V*V
480 AP=AW
490 LT=SW*CL*Q*AP
500 D=S*Q*CD
510 AX=32.2*(D*(VA-VX(1))/V-LT*VZ(1)/V-TN*COS(PH))/M
520 AZ=32.2*(1-(LT*(VA-VX(1))/V+D*VZ(1)/V+TN*SIN(PH))/M)
530 VX(1)=AX*DM+VX(0)
540 VZ(1)=AZ*DM+VZ(0)
550 X(I,1)=(VX(1)+VX(0))/2*DM+X(I,O)
560 Z(I,1)=(VZ(1)+VZ(0))/2*DM+Z(I,0)
570 '
580 'FIND CABLE SHAPE
590 FOR K=I TO 1 STEP -1
610 XV=(X(K,1)-X(K,0))/DM:ZV=(Z(K,1)-Z(K,0))/DM
615 IF JJ<.5 THEN XV=0:ZV=0
640 V=SQR((VA-XV)^2+ZV*ZV)
650 AL=PH+ATN(ZV/(VA-XV))
655 IF Y=1 THEN V=VA:AL=PH
660 Q=RH/2*V*V
665 DC=DI(FIX(NS*(K-1)*DL/LO))
670 MD=380*DC*DC*DL
677 IF K=I THEN DL=DL/2
680 DN=MD*SIN(PH)+DC*DL*Q*3.1416*CF*COS(AL)
690 DP=ATN((DC*DL*Q*(CC*ABS(SIN(AL))*SIN(AL)+3.1416*CF*SIN(AL))-MD*COS(PH))/TN)
691 TN=TN+DN:PH=PH-DP
692 X(K-1,1)=X(K,1)-DL*COS(PH+DP/2)
693 Z(K-1,1)=Z(K,1)-DL*SIN(PH+DP/2)
694 IF K<I THEN GOTO 725
695 AL=PH+ATN(ZV/(VA-XV))
697 DN=MD*SIN(PH)+DC*DL*Q*3.1416*CF*COS(AL)
698 DP=ATN((DC*DL*Q*(CC*(SIN(AL))^2+3.1416*CF*SIN(AL))-MD*COS(PH))/TN)
700 TN=TN+DN:PH=PH-DP
710 X(K-1,1)=Z(K-1,1)-DL*COS(PH+DP/2)
720 Z(K-1,1)=Z(K-1,1)-DL*SIN(PH+DP/2)
722 DL=DL*2
725 PRINT X(K-1,1);Z(K-1,1)
727 NEXT K
730 IF JJ>.5 THEN 765
740 FOR K=0 TO NT:RS(K)=AE+Z(0,1):NEXT K
745 WR=WA*VA/WL*6.28:WH=0
750 FOR K=I TO 0 STEP -1:X(K,1)=X(K,1)-X(0,1):Z(K,1)=Z(K,1)-Z(0,1):NEXT K
765 'TEST FOR CABLE POSITION AT AIRCRAFT
770 IF (X(0,1)*X(0,1)+(Z(0,1)-ZA) ^2)/DL/DL<.0001 THEN GOTO 860
790 H(1)=SQR(X(I,1)^2+(Z(I,1)-ZA)^2)-SQR((X(I,1)-X(0,1))^2+(Z(I,1)-Z(0,1))^2)
800 G(1)=ATN((Z(I,1)-ZA)/X(I,1))-ATN((Z(I,1)-Z(0,1))/(X(I,1)-X(0,1)))
805 IF J=1 AND H(1)<>H(0) THEN FH=(H(1)-H(0))/(T(1)-T(0))
810 IF J=3 AND G(1)<>G(0) THEN FG=(G(1)-G(0))/(P(1)-P(0))
820 T(0)=T(1):P(0)=P(1):G(0)=G(1):H(0)=H(1)
825 IF J<2 THEN T(1)=T(0)-H(1)/FH
830 IF J>1 THEN P(1)=P(0)-G(1)/FG
835 N=N+1:J=J+1:IF J=4 THEN J=0
840 GOTO 430
860 TM=DM*JJ
870 FG=2*FG:FH=2*FH
910 GOSUB 1000
930 GOTO 290
1000 'GRAPHICS SUBROUTINE
1020 CLS
1025 GOTO 1075
1063 GOSUB 4000
1065 RETURN
1075 PRINT "ELAPSED TIME:";TM;"SEC"
1090 PRINT "ALTITUDE DIFF.:";Z(I,1);"FT";"    TARGET ANGLE:";-ATN(VZ(1)/(VA-VX(1)))*57.3;"DEG"
1095 PRINT "CABLE TENSION AND ANGLE AT TARGET:";T(1);"LB, ";P(1)*57.3;"DEG"
1100 PRINT "TUG ALTITUDE:";AE-ZA;"FT";"   TARGET ALTITUDE:";AE-Z(I,1);"FT"
1105 PRINT "WING LIFT AND ANGLE OF ATTACK:";LT;"LB, ";AW;"DEG"
1200 GOTO 1063
3010 RESUME 1070
4000 'PRINT SUBROUTINE
4005 IF JJ>.5 THEN 4045
4010 PRINT " ":PRINT " "
4015 PRINT TAB(23)"TARGET";TAB(47)"CABLE"
4020 PRINT TAB(14)"-----------------------";TAB(41)"-------------------"
4025 PRINT "TIME   ALFA   ALT.  CLIMB  VERT.  ANGLE  TENS.  ANGLE  ALT.   WAVE   WAVE"
4030 PRINT "(SEC)   (DEG)  (FT)   RATE  ACCEL  (DEG)  @TRGT  @TRGT  @TUG  HGHT.   CLEAR."
4035 PRINT "                     (FPS)  (FPSSQ)        (LB)  (DEG)  (FT)   (FT)   (FT)"
4037 PRINT " "
4040 S1$="###.#":S2$="+##.#":S3$="+###"
4045 PRINT USING S1$;TM;:PRINT " ";
4047 PRINT USING S2$;AP;:PRINT " ";
4050 PRINT USING S3$;AE-Z(I,1);:PRINT " ";:PRINT USING S3$;-VZ(1);:PRINT " ";
4055 PRINT USING S2$;-AZ;:PRINT " ";
4060 PRINT USING S2$;-ATN(VZ(1)/(VA-VX(1)))*57.3;:PRINT " ";
4065 PRINT USING S1$;T(1);:PRINT " ";:PRINT USING S1$;P(1)*57.3;:PRINT " ";:PRINT USING S1$;AE-ZA;:PRINT " ";
4070 PRINT USING S2$;WH;:PRINT "  ";:PRINT USING S3$;AE-Z(I,1)-WH
4090 IF INKEY$="L" THEN GOTO 4110
4100 RETURN
4110 PRINT "CABLE COORDINATES (X,Z):";
4120 FOR K=0 TO I: PRINT "(";X(K,1);",";Z(K,1);")";NEXT K
4130 PRINT " "
4190 PRINT " "
4200 RETURN
5000 PRINT CHR$(27)"G"
5002 PRINT " "
5003 PRINT " "
5010 PRINT "        TARGET SYSTEM:";TI$
5012 PRINT " "
5013 PRINT " "
5014 PRINT "   FLY WEIGNT:";M;"LB","DRAG CODFF.:";CD*4*144*S/D/D/3.14,"XP:";CP*12;"IN"
5020 PRINT "   CABLE LENGTH:";LO;"FT"
5030 PRINT "   CABLE DIAMETERS (IN):";
5040 FOR K=0 TO NS-1:PRINT DI(K)*12;:NEXT K
5041 PRINT " "
5050 PRINT "   WING AREA AND LIFT CURVE SLOPE:";SW*144;"SQ IN",CL;"1/DEG"
5060 PRINT "   TRUE AIRCRAFT AIRSPEED:";VA/1.69;"KNOTS","AIRCRAFT ALTITUDE:";AE;"FT"
5070 PRINT "   INTERVAL:";DL;"FT","MEMORY:";MT;"SEC","RESPONSE:";RE;"DEG/S"
5190 PRINT CHR$(27)"H"
5200 RETURN
6000 'CONTROL ALGORITHM
6005 K1=1:K2=2:RP=20
6110 TA=AE-Z(I,1):CR=-VZ(1):WC=TA-WH
6115 GOSUB 7000
6117 'SERVO RESPONSE
6118 IF AT>AW THEN AW=AW+RE*DM ELSE 6124
6120 IF AW>AT THEN AW=AT
6122 GOTO 6130
6124 AW=AW-RE*DM
6126 IF AW<AT THEN AW=AT
6130 IF AW>9.2 THEN AW=9.2
6140 IF AW<-4.6 THEN AW=-4.6
6200 'TA=TRGT ALT (FT), CR=CLIMB RATE (FPS), AW=EFFECTIVE WING ANGLE (DEG)
6900 RETURN
7000 'CURVE FIT
7010 FOR K=1 TO NT:RS(K-1)=RS(K):NEXT K
7020 RS(NT)=WC
7030 ST=0:SR=0:SS=0:SP=0:EM=0
7040 FOR K=NT TO 0 STEP -1
7050 ST=ST+(K-NT)*DM
7060 SR=SR+RS(K)
7070 SP=SP+RS(K)*(K-NT)*DM
7080 SS=SS+((K-NT)*DM)^2
7090 NEXT K
7095 IF (NT+1)*SS-ST*ST=0 THEN RETURN
7100 RM=(SR*SS-SP*ST)/((NT+1)*SS-ST*ST)
7110 RD=((NT+1)*SP-ST*SR)/((NT+1)*SS-ST*ST)
7120 FOR K=0 TO NT
7130 E=RM+RD*(K-NT)*DM-RS(K)
7140 IF E>EM THEN EM=E
7150 NEXT K
7170 RM=RM-EM
7200 AT=K1*(RP-RM)-K2*RD
7900 RETURN

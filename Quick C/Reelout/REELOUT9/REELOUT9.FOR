C*****************************************************************
C
C 9 INCH TOW REEL OUT SIMULATION
C
C*****************************************************************
      DIMENSION DIAT(25),LENT(25),LENGHT(25)
      REAL KIAS
      REAL MUT,LENT,LENGHT,LENONT,LENOFT,LENOUT
      REAL LENOTT
      DATA PI/3.1415926536/
      DATA KIAS/275.0/
C****************************************************************
      OPEN(UNIT=5,FILE='CON')
      OPEN(UNIT=6,FILE='CON')
      OPEN(UNIT=7,FILE='d:\reelout9\REEL9.DAT')
      OPEN(UNIT=10,FILE='d:\reelout9\REEL10.OUT')
      OPEN(UNIT=11,FILE='d:\reelout9\REEL11.OUT')
      MUT=0.1100
      WRITE(6,500)
500   FORMAT(1X,'ENTER AVERAGE STARTING RPM OF REEL')
      READ(5,*)RPM
      WRITE(6,555)
555   FORMAT(1X,'ENTER AVERAGE ENDING RPM OF REEL')
      READ(5,*)RPMEND
      READ(7,*)MUT
      READ(7,*)WTART
      READ(7,*)CD
456   FORMAT( )
      READ(7,*)NOSEGT
      DO 459 I=1,NOSEGT
      READ(7,*)DIAT(I)
459   CONTINUE
      DO 460 I=1,NOSEGT
      READ(7,*)LENT(I)
460   CONTINUE
      READ(7,*)KIAS
C****************************************************************
      WRITE(6,462)KIAS
      WRITE(10,462)KIAS
462   FORMAT(1X,/,1X,'LAUNCH AIRSPEED ',F5.0,1X,'KIAS',//)
C****************************************************************
      VOLUMT=0.0
      TEIGHT=0.0
      WRITE(6,11)
      WRITE(11,11)
11    FORMAT(1X,/,1X,'TARGET CABLE DESIGN',/)
      LENGHT(1)=0.0
      DO 102 I=2,NOSEGT,1
      VOLUMT=VOLUMT+DIAT(I)*DIAT(I)*LENT(I)*12.0
      TEIGHT=TEIGHT+384.0*DIAT(I)*DIAT(I)*LENT(I)/144.0
      LENGHT(I)=LENT(I)
      LENGHT(I)=LENGHT(I)+LENGHT(I-1)
      WRITE(6,101)DIAT(I),LENT(I),LENGHT(I)
      WRITE(11,101)DIAT(I),LENT(I),LENGHT(I)
101   FORMAT(1X,F5.3,5X,F6.0,5X,F6.0,5X,F6.0)
102   CONTINUE
      WRITE(6,12)VOLUMT
      WRITE(11,12)VOLUMT
12    FORMAT(1X,'VOLUME (IN**3):',F6.2,/)
C****************************************************************
      WRITE(6,501)
      WRITE(11,501)
501   FORMAT(1X,'TIME      RPM    RADIUS    C      RATE   INC OFF  ',
     &'LEN OFF  LEN TO GO  VOLUME',/,'    TENSION    TROQUE    KIAS',//)
      DELTA=.05
      TIME=DELTA
      COUNT=0
      KOUNT=INT(1.0/DELTA)
C****************************************************************
      DT=DIAT(NOSEGT)
      LENONT=LENGHT(NOSEGT)
      LENTOT=LENGHT(NOSEGT)
      LENOUT=0.0
      LENOTT=0.0
      INDEXT=NOSEGT-1
C****************************************************************
      WEIGHT=WTART+TEIGHT
      CEIGHT=WEIGHT
      TNUM=4.0
      TLIFT=SQRT(32.174*60.0**2/(4.0*PI*PI*2.43751))
C****************************************************************
C INITIAL TENSIONAL LOADS OF REEL
C****************************************************************
      Q=0.5*.002378*(1.689*KIAS)**2
      TDRAG=CD*Q*PI*(9.0/2.0)**2/144.0
      DRAG=TDRAG
      TENSOT=SQRT( TDRAG*TDRAG + WEIGHT*WEIGHT )
C***************************************************************
400   LENOFT=0.0
C****************************************************************
C TARGET REELOUT
C****************************************************************
401   IF(LENONT.LE.0.0)GOTO 402
      RADIUT=SQRT( VOLUMT/(4.5*PI))+.25
      CIRCUT=2.0*PI*RADIUT/12.0
      TROQUT=TENSOT*RADIUT
      FRICIT=TROQUT/3.1875
      NORMAT=FRICIT/MUT
      RPMT=NORMAT/((8*276.*.002205/32.174)*((2.*PI/60.)**2)*2.4375)
      RPMT=SQRT(RPMT)
C************************
      RPMT=RPM
      IF(RADIUT.LE.1.46)RPM=RPMEND
C************************
      IF(RPMT.GT.TLIFT)TNUM=8.0
      RATET=RPMT*CIRCUT
      LENOFT=RATET*DELTA
      LENOUT=LENOUT+LENOFT
      LENONT=LENONT-LENOFT
      LENOTT=LENOTT+LENOFT
C***************************************************************
C TARGET CABLE ANALYSIS
C***************************************************************
402   VRATE=(KIAS*1.689)-RATET/60.0
      VKIAS=VRATE/1.689
      Q=0.5*.002378*(VRATE)**2
      TDRAG=CD*Q*PI*(9.0/2.0)**2/144.0
      WEIGHT=WEIGHT-384.0*DT*DT*LENOFT/144.0
      TENSOT=SQRT( WEIGHT*WEIGHT + TDRAG*TDRAG )
C****************************************************************
C DECREASE VOLUME ON REEL AND CHECK FOR SPLICE
C***************************************************************
404   IF(LENOFT.LE.0.0)GOTO 406
      VOLUMT=VOLUMT-DT*DT*LENOFT*12.
      IF(VOLUMT.LT.0.0)VOLUMT=0.0
      IF(LENONT.GT.LENGHT(INDEXT))GOTO 406
      DT=DIAT(INDEXT)
      INDEXT=INDEXT-1
      LENOTT=0.0
      WRITE(6,405)TIME,DT,LENONT
      WRITE(11,405)TIME,DT,LENONT
405   FORMAT(1X,/,1X,F5.2,5X,' CABLE SPLICE ',5X,F5.3,5X,F6.0,/)
      CALL PRINT(TIME,RPMT,RADIUT,CIRCUT,RATET,LENOFT,LENOUT,
     &LENONT,VOLUMT,TENSOT,TROQUT,VKIAS)
C***************************************************************
C PRINT OUT LINE RESULTS
C***************************************************************
406   IF(TIME.NE.DELTA)GOTO 407
      CALL PRINT(TIME,RPMT,RADIUT,CIRCUT,RATET,LENOFT,LENOUT,
     &LENONT,VOLUMT,TENSOT,TROQUT,VKIAS)
407   COUNT=COUNT+1
      IF(COUNT.NE.KOUNT)GOTO 408
      COUNT=0
      CALL PRINT(TIME,RPMT,RADIUT,CIRCUT,RATET,LENOFT,LENOUT,
     &LENONT,VOLUMT,TENSOT,TROQUT,VKIAS)
408   TIME=TIME+DELTA
411   IF(LENONT.GT.0.0)GOTO 400
C***************************************************************
      CALL PRINT(TIME,RPMT,RADIUT,CIRCUT,RATET,LENOFT,LENOUT,
     &LENONT,VOLUMT,TENSOT,TROQUT,VKIAS)
C****************************************************************
9999  STOP
      END
C***************************************************************
      SUBROUTINE PRINT(TIME,RPMT,RADIUT,CIRCUT,RATET,LENOFT,
     &LENOUT,LENONT,VOLUMT,TENSOT,TROQUT,VKIAS)
      REAL LENONT,LENOFT,LENOUT
      WRITE(6,407)TIME,RPMT,RADIUT,CIRCUT,RATET,LENOFT,LENOUT,
     &LENONT,VOLUMT,TENSOT,TROQUT,VKIAS
      WRITE(11,407)TIME,RPMT,RADIUT,CIRCUT,RATET,LENOFT,LENOUT,
     &LENONT,VOLUMT,TENSOT,TROQUT,VKIAS
407   FORMAT(1X,F5.2,3X,F6.1,3X,F5.2,3X,F5.2,3X,F6.1,3X,F6.1,
     &3X,F6.0,3X,F6.0,3X,F6.2,/,1X,F8.2,3X,F6.2,3X,F6.2)
      RETURN
      END

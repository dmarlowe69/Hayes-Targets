C*****************************************************************
C
C LTC-6/TPT-D REEL OUT SIMULATION
C
C*****************************************************************
      DIMENSION DIAR(25),LENR(25),LENGHR(25),LENRR(25)
      DIMENSION DIAT(25),LENT(25),LENGHT(25)
      REAL KIAS,LENR,LENGHR,LENRR,LENONR,LENOFR,LENOUR
      REAL MUR,MUT,LENT,LENGHT,LENONT,LENOFT,LENOUT
      REAL LINC,LENOTT,LCOUNT
      DATA PI/3.1415926536/
      DATA KIAS/275.0/
C****************************************************************
      OPEN(UNIT=5,FILE='TPTLTC.DAT')
      OPEN(UNIT=6,FILE='CON')
      OPEN(UNIT=10,FILE='LTC10.OUT')
      OPEN(UNIT=11,FILE='LTC11.OUT')
C****************************************************************
C
C     WTART    TARGET WEIGHT (LBS)
C     CD       TARGET DRAG COEFFICIENT
C
C****************************************************************
      READ(5,*)WTART
      READ(5,*)CD
C****************************************************************
C
C     NOSEGR   NUMBER CABLE SEGMENTS IN LTC-6 REEL
C     DIAR     SEGEMENT DIAMETER
C     LENR     SEGEMENT LENGTH
C
C****************************************************************
      READ(5,*)NOSEGR
      DO 457 I=1,NOSEGR
      READ(5,*)DIAR(I)
456   FORMAT( )
457   CONTINUE
      DO 458 I=1,NOSEGR
      READ(5,*)LENR(I)
458   CONTINUE
C****************************************************************
C
C     NOSEGT   NUMBER CABLE SEGMENTS IN TARGET REEL
C     DIAT     SEGEMENT DIAMETER
C     LENT     SEGEMENT LENGTH
C
C****************************************************************
      READ(5,*)NOSEGT
      DO 459 I=1,NOSEGT
      READ(5,*)DIAT(I)
459   CONTINUE
      DO 460 I=1,NOSEGT
      READ(5,*)LENT(I)
460   CONTINUE
C****************************************************************
      READ(5,*)SKIAS
      READ(5,*)STIME
      READ(5,*)FTIME
      READ(5,*)KIAS
C****************************************************************
      WRITE(6,462)KIAS
      WRITE(10,462)KIAS
462   FORMAT(1X,/,1X,'LAUNCH AIRSPEED ',F5.0,1X,'KIAS',//)
C****************************************************************
      VOLUMR=0.0
      VOLUMT=0.0
      TEIGHT=0.0
      WRITE(6,10)
      WRITE(10,10)
10    FORMAT(1X,/,1X,'LTC-6 CABLE DESIGN',/)
      LENGHR(1)=0.0
      LENRR(NOSEGR+1)=0.0
      DO 99 I=NOSEGR,1,-1
      LENRR(I)=LENR(I)
      LENRR(I)=LENRR(I)+LENRR(I+1)
99    CONTINUE
      DO 100 I=2,NOSEGR,1
      VOLUMR=VOLUMR+DIAR(I)*DIAR(I)*LENR(I)*12.0
      LENGHR(I)=LENR(I)
      LENGHR(I)=LENGHR(I)+LENGHR(I-1)
      WRITE(6,101)DIAR(I),LENR(I),LENGHR(I),LENRR(I)
      WRITE(10,101)DIAR(I),LENR(I),LENGHR(I),LENRR(I)
101   FORMAT(1X,F5.3,5X,F6.0,5X,F6.0,5X,F6.0)
100   CONTINUE
      WRITE(6,12)VOLUMR
      WRITE(10,12)VOLUMR
      WRITE(6,11)
      WRITE(11,11)
11    FORMAT(1X,/,1X,'TPT-D CABLE DESIGN',/)
      LENGHT(1)=0.0
      DO 102 I=2,NOSEGT,1
      VOLUMT=VOLUMT+DIAT(I)*DIAT(I)*LENT(I)*12.0
      TEIGHT=TEIGHT+384.0*DIAT(I)*DIAT(I)*LENT(I)/144.0
      LENGHT(I)=LENT(I)
      LENGHT(I)=LENGHT(I)+LENGHT(I-1)
      WRITE(6,101)DIAT(I),LENT(I),LENGHT(I)
      WRITE(11,101)DIAT(I),LENT(I),LENGHT(I)
102   CONTINUE
      WRITE(6,12)VOLUMT
      WRITE(11,12)VOLUMT
12    FORMAT(1X,'VOLUME (IN**3):',F6.2,/)
C****************************************************************
      DELTA=.05
      TIME=DELTA
      COUNT=0
      KOUNT=INT(1.0/DELTA)
      MUR=.15
      MUT=.2082
C****************************************************************
      DR=DIAR(NOSEGR)
      LENONR=LENGHR(NOSEGR)
      LENTOR=LENGHR(NOSEGR)
      LENOUR=0.0
      INDEXR=NOSEGR-1
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
      BNUM=4.0
      BLIFT=SQRT(32.174*60.0**2/(4.0*PI*PI*2.875))
      TNUM=4.0
      TLIFT=SQRT(32.174*60.0**2/(4.0*PI*PI*2.43751))
C****************************************************************
C INITIAL TENSIONAL LOADS OF REEL
C****************************************************************
      Q=0.5*.002378*(1.689*KIAS)**2
      TDRAG=CD*Q*PI*(9.0/2.0)**2/144.0
      DRAG=TDRAG
      TENSOR=SQRT( DRAG*DRAG + CEIGHT*CEIGHT )
      TENSOT=SQRT( TDRAG*TDRAG + WEIGHT*WEIGHT )
C***************************************************************
400   LENOFR=0.0
      LENOFT=0.0
C****************************************************************
C LTC-6 REELOUT
C****************************************************************
      IF(LENONR.LE.0.0)GOTO 401
      RADIUR=SQRT( VOLUMR/(6.0*PI))+.3125
      CIRCUR=2.0*PI*RADIUR/12.0
      TROQUR=TENSOR*RADIUR
      FRICIR=TROQUR/4.25
      NORMAR=FRICIR/MUR
      RPMR=NORMAR/(BNUM*845.*.002205/32.174*(2.*PI/60.)**2*2.875)
      RPMR=SQRT(RPMR)
      IF(RPMR.GT.BLIFT)BNUM=8
      IF(RPMR.LT.438.0)MUR=.40*(RPMR/60.0*2.0*4.25*PI/12.0)**(-0.4771)
      IF(RPMR.GE.438.0)MUR=.30*(RPMR/60.0*2.0*4.25*PI/12.0)**(-0.4771)
      IF(RPMR.GE.550.0)MUR=.275*(RPMR/60.0*2.0*4.25*PI/12.0)**(-0.4771)
      IF(RPMR.GE.575.0)MUR=.250*(RPMR/60.0*2.0*4.25*PI/12.0)**(-0.4771)
      IF(RPMR.GE.600.0)MUR=.225*(RPMR/60.0*2.0*4.25*PI/12.0)**(-0.4771)
C     IF(RPMR.GE.625.0)MUR=.200*(RPMR/60.0*2.0*4.25*PI/12.0)**(-0.4771)
      RATER=RPMR*CIRCUR
      LENOFR=RATER*DELTA
      LENOUR=LENOUR+LENOFR
      LENONR=LENONR-LENOFR
C****************************************************************
C TPT-D REELOUT
C****************************************************************
401   IF(LENONT.LE.0.0)GOTO 402
      RADIUT=SQRT( VOLUMT/(4.5*PI))+.25
      CIRCUT=2.0*PI*RADIUT/12.0
      TROQUT=TENSOT*RADIUT
      FRICIT=TROQUT/3.1875
      NORMAT=FRICIT/MUT
      RPMT=NORMAT/(TNUM*276.*.002205/32.174*(2.*PI/60.)**2*2.4375)
      RPMT=SQRT(RPMT)
      IF(RPMT.GT.TLIFT)TNUM=8.0
      RATET=RPMT*CIRCUT
      LENOFT=RATET*DELTA
      LENOUT=LENOUT+LENOFT
      LENONT=LENONT-LENOFT
      LENOTT=LENOTT+LENOFT
C***************************************************************
C TPT-D CABLE ANALYSIS
C***************************************************************
402   Q=0.5*.002378*(1.689*KIAS)**2
      TDRAG=CD*Q*PI*(9.0/2.0)**2/144.0
      WEIGHT=WEIGHT-384.0*DT*DT*LENOFT/144.0
      TENSOT=SQRT( WEIGHT*WEIGHT + TDRAG*TDRAG )
C****************************************************************
      IF(NOSEGR.EQ.1)GOTO 404
      T0=TDRAG
      T1=WEIGHT
      LCOUNT=0.0
      LINC=10.0
C***************************************************************
      IF(LENONT.LE.0.0)GOTO 502
      I=INDEXT+1
500   LCOUNT=LCOUNT+LINC
      IF(LCOUNT.GT.LENOUT)GOTO 502
      IF(I.EQ.(INDEXT+1).AND.LCOUNT.GT.LENOTT)GOTO 501
      IF(LCOUNT.GT.LENGHT(I))GOTO 501
C***************************************************************
      AUG=T1/T0
      ANGLE=ATAN(AUG)
      T1=T1+384.0*DIAT(I)*DIAT(I)*LINC/144.0
      T1=T1-DIAT(I)*LINC*Q*2.*SIN(ANGLE)**2*COS(ANGLE)/12.0
      T0=T0+DIAT(I)*LINC*Q*(2.*SIN(ANGLE)**3+PI*.0057)/12.0
      GOTO 500
501   LCOUNT=LCOUNT-LINC
      IF(I.GT.NOSEGT)GOTO 502
      I=I+1
      GOTO 500
502   TENSOR=SQRT(T0*T0 + T1*T1)
C****************************************************************
C CALCULATE CABLE TENSION AT LTC-6 REEL
C****************************************************************
      IF(LENONR.LE.0.0)GOTO 504
      LCOUNT=0.0
      I=NOSEGR
600   LCOUNT=LCOUNT+LINC
      IF(LCOUNT.GT.LENOUR)GOTO 602
      IF(LCOUNT.GT.LENRR(I))GOTO 601
      AUG=T1/T0
      ANGLE=ATAN(AUG)
      DNGLE=ANGLE*180.0/PI
      T1=T1+384.0*DIAR(I)*DIAR(I)*LINC/144.0
      T1=T1-DIAR(I)*LINC*Q*2.*SIN(ANGLE)**2*COS(ANGLE)/12.0
      T0=T0+DIAR(I)*LINC*Q*(2.*SIN(ANGLE)**3+PI*.0057)/12.0
      GOTO 600
601   LCOUNT=LCOUNT-LINC
      I=I-1
      IF(I.EQ.0)GOTO 602
      GOTO 600
602   TENSOR=SQRT( T0*T0 + T1*T1 )
C***************************************************************
C DECREASE VOLUME ON REEL AND CHECK FOR SPLICE
C***************************************************************
504   IF(LENOFR.LE.0.0)GOTO 404
      VOLUMR=VOLUMR-DR*DR*LENOFR*12.
      IF(VOLUMR.LT.0.0)VOLUMR=0.0
      IF(LENONR.GT.LENGHR(INDEXR))GOTO 404
      DR=DIAR(INDEXR)
      INDEXR=INDEXR-1
      WRITE(6,403)TIME,DR,LENONR
      WRITE(10,403)TIME,DR,LENONR
403   FORMAT(1X,/,1X,F5.2,5X,' LTC SPLICE ',5X,F5.3,5X,F6.0,/)
      CALL PRINR(TIME,RPMR,RADIUR,CIRCUR,RATER,LENOFR,LENOUR,
     &LENONR,VOLUMR,TENSOR,TROQUR,DNGLE)
C***************************************************************
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
405   FORMAT(1X,/,1X,F5.2,5X,' TPT SPLICE ',5X,F5.3,5X,F6.0,/)
      CALL PRINT(TIME,RPMT,RADIUT,CIRCUT,RATET,LENOFT,LENOUT,
     &LENONT,VOLUMT,TENSOT,TROQUT)
C***************************************************************
C PRINT OUT LINE RESULTS
C***************************************************************
406   IF(TIME.NE.DELTA)GOTO 407
      CALL PRINR(TIME,RPMR,RADIUR,CIRCUR,RATER,LENOFR,LENOUR,
     &LENONR,VOLUMR,TENSOR,TROQUR,DNGLE)
      CALL PRINT(TIME,RPMT,RADIUT,CIRCUT,RATET,LENOFT,LENOUT,
     &LENONT,VOLUMT,TENSOT,TROQUT)
407   COUNT=COUNT+1
      IF(COUNT.NE.KOUNT)GOTO 408
      COUNT=0
      WRITE(6,410)KIAS
      WRITE(10,410)KIAS
      WRITE(11,410)KIAS
      CALL PRINR(TIME,RPMR,RADIUR,CIRCUR,RATER,LENOFR,LENOUR,
     &LENONR,VOLUMR,TENSOR,TROQUR,DNGLE)
      CALL PRINT(TIME,RPMT,RADIUT,CIRCUT,RATET,LENOFT,LENOUT,
     &LENONT,VOLUMT,TENSOT,TROQUT)
408   TIME=TIME+DELTA
      IF(TIME.GE.STIME.AND.TIME.LE.(STIME+2.0))KIAS=KIAS-(50.0*DELTA)
      IF(KIAS.LE.SKIAS)KIAS=SKIAS
      IF(TIME.GE.FTIME)KIAS=KIAS+(25.0*DELTA)
      IF(KIAS.GE.275.0)KIAS=275.0
411   IF(LENONR.GT.0.0.OR.LENONT.GT.0.0)GOTO 400
C***************************************************************
      WRITE(6,410)KIAS
      WRITE(10,410)KIAS
      WRITE(11,410)KIAS
410   FORMAT(1X,'AIRSPEED ',F5.0,' KIAS')
      CALL PRINR(TIME,RPMR,RADIUR,CIRCUR,RATER,LENOFR,LENOUR,
     &LENONR,VOLUMR,TENSOR,TROQUR,DNGLE)
      CALL PRINT(TIME,RPMT,RADIUT,CIRCUT,RATET,LENOFT,LENOUT,
     &LENONT,VOLUMT,TENSOT,TROQUT)
C****************************************************************
9999  STOP
      END
      SUBROUTINE PRINR(TIME,RPMR,RADIUR,CIRCUR,RATER,LENOFR,
     &LENOUR,LENONR,VOLUMR,TENSOR,TROQUR,DNGLE)
C***************************************************************
      REAL LENONR,LENOFR,LENOUR
C***************************************************************
      WRITE(6,407)TIME,RPMR,RADIUR,CIRCUR,RATER,LENOFR,LENOUR,
     &LENONR,VOLUMR,TENSOR,TROQUR,DNGLE
C***************************************************************
      WRITE(10,407)TIME,RPMR,RADIUR,CIRCUR,RATER,LENOFR,LENOUR,
     &LENONR,VOLUMR,TENSOR,TROQUR,DNGLE
C***************************************************************
407   FORMAT(1X,F5.2,3X,F6.1,3X,F5.2,3X,F5.2,3X,F6.1,3X,F5.2,
     &3X,F6.0,3X,F6.0,3X,F6.2,/,3X,F8.2,3X,F6.1,3X,F5.2)
C***************************************************************
      RETURN
      END
      SUBROUTINE PRINT(TIME,RPMT,RADIUT,CIRCUT,RATET,LENOFT,
     &LENOUT,LENONT,VOLUMT,TENSOT,TROQUT)
C***************************************************************
      REAL LENONT,LENOFT,LENOUT
C***************************************************************
      WRITE(6,407)TIME,RPMT,RADIUT,CIRCUT,RATET,LENOFT,LENOUT,
     &LENONT,VOLUMT,TENSOT,TROQUT
C***************************************************************
      WRITE(11,407)TIME,RPMT,RADIUT,CIRCUT,RATET,LENOFT,LENOUT,
     &LENONT,VOLUMT,TENSOT,TROQUT
C***************************************************************
407   FORMAT(1X,F5.2,3X,F6.1,3X,F5.2,3X,F5.2,3X,F6.1,3X,F5.2,
     &3X,F6.0,3X,F6.0,/,3X,F6.2,3X,F8.2,3X,F6.2)
C***************************************************************
      RETURN
      END

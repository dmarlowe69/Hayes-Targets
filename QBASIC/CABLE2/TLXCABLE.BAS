REM *************************************************************************
REM
REM     TLX cable design
REM
REM *************************************************************************
        NEWCDC0 = 1.5
        NEWCF = 1.5
        DEBUG = 0
REM        directory$ = "c:\qbasic\software\cable\"
        directory$ = "c:\qbasic\software\cable2\"
REM     directory$ = ""
        DIM D(50, 4), L(50), T(50), P(50), M(50, 3), S(50, 3), F(50), TYPE$(50)
        DIM W(50), C(50)
REM *************************************************************************
REM
REM     READ CABLE PARAMETERIS INTO ARRAYS
REM
REM *************************************************************************
        GOSUB READMONO
        GOSUB READSTRAND
REM *************************************************************************
REM
REM     CABLE PROGRAM MAIN MENU
REM
REM *************************************************************************
menu:
        CLS : SCREEN 0: COLOR 3
        PRINT "NEWCDC0"; NEWCDC0
        PRINT "NEWCF"; NEWCF
        PRINT "LILL CABLE DESIGN PROGRAM FOR TLX  (C)HAYES TARGETS DIVISION"
        PRINT "WRITTEN BY DENTON MARLOWE AUGUST 14 1995"
        PRINT
        PRINT " OPTIONS"
        PRINT
        PRINT " (1) ONE WAY TARGET CABLE DESIGN OR ANALYSIS"
        PRINT
        PRINT " (2) TWO WAY TAGERT CABLE DESIGN OR ANALYSIS"
        PRINT
        PRINT " (3) TLX TARGET CABLE DESIGN OR ANALYSIS"
        PRINT
        PRINT " (4) QUIT"
        PRINT
        PRINT "note:skin friction coefficient reduce by a factor of 2"
        INPUT " ENTER OPTION"; A$
REM *************************************************************************
        IF A$ = "1" THEN GOTO OPTONE
        IF A$ = "2" THEN GOTO OPTTWO
        IF A$ = "3" THEN GOTO OPTTHREE
        IF A$ = "4" THEN GOTO OPTFOUR
        GOTO menu
REM *************************************************************************
REM
REM     ONEWAY TARGET CABLE DESIGN AND ANALYSIS
REM
REM *************************************************************************
OPTONE:
        opt$ = "ONEWAY"
inloop1:
        GOSUB INPUTDATA
        IF USER$ = "QUIT" THEN GOTO menu
        IF opt$ <> "ONEWAY" GOTO menu
        IF function$ = "DESIGN" THEN GOTO OPTONEA
        INPUT "ENTER AIRSPEED (KIAS) :"; K
        GOSUB analysi6
        GOSUB print6
        GOTO inloop1
OPTONEA:
        IF DESIGN$ = "LTC-6" THEN GOSUB design6 ELSE GOSUB DESIGN
        GOSUB designopt
        GOTO inloop1
REM *************************************************************************
REM
REM     TWOWAY TARGET CABLE DESIGN AND ANALYSIS
REM
REM *************************************************************************
OPTTWO:
        opt$ = "TWOWAY"
inloop2:
        GOSUB INPUTDATA
        IF USER$ = "QUIT" THEN GOTO menu
        IF opt$ <> "TWOWAY" GOTO menu
        IF function$ = "DESIGN" THEN GOTO OPTTWOA
        INPUT "ENTER AIRSPEED (KIAS) :"; K
        GOSUB SCOPEIT
        GOSUB TARGETLOADS
        GOSUB CABLELOADS
        GOSUB designprt
        GOTO inloop2
OPTTWOA:
        GOSUB DESIGN
        GOSUB designopt
        GOTO inloop2
REM *************************************************************************
REM
REM     TLX TARGET CABLE DESIGN AND ANALYSIS
REM
REM *************************************************************************
OPTTHREE:
        DESIGN$ = "TLX"
        CLS
        PRINT "TLX CABLE DESIGN OPTIONS"
        PRINT
        PRINT " (1) TLX-1"
        PRINT " (2) TLX-1IR"
        PRINT
        INPUT " ENTER OPTION:"; A$
        IF A$ <> "1" AND A$ <> "2" THEN GOTO OPTTHREE
        IF A$ = "1" THEN TARGET$ = "TLX-1"
        IF A$ = "2" THEN TARGET$ = "TLX-IR"
REM *************************************************************************
        opt$ = "TWOWAY"
inloop3:
        GOSUB INPUTDATA
        IF USER$ = "QUIT" THEN GOTO menu
        IF opt$ <> "TWOWAY" THEN GOTO menu
REM        IF DESIGN$ <> "TLX" THEN GOTO menu
        IF function$ = "DESIGN" THEN GOTO OPTTHREEA
        INPUT "ENTER AIRSPEED (KIAS) :"; K
        GOSUB SCOPEIT
        GOSUB TARGETLOADS
        GOSUB CABLELOADS
        GOSUB designprt
        GOTO inloop3
OPTTHREEA:
        GOSUB DESIGN
        GOSUB designopt
        GOTO inloop3
REM *************************************************************************
OPTFOUR:
        END
REM *************************************************************************
REM
REM     inculde file directives
REM
REM *************************************************************************
'       $INCLUDE: 'c:\qbasic\software\CABLE2\tlxloads.in'
REM *************************************************************************
'       $INCLUDE: 'c:\qbasic\software\CABLE2\cabdata.in'
'       $INCLUDE: 'c:\qbasic\software\CABLE2\desinput.in'
'       $INCLUDE: 'c:\qbasic\software\CABLE2\desopt.in'
REM *************************************************************************
'       $INCLUDE: 'c:\qbasic\software\CABLE2\lookup.in'
REM *************************************************************************
'       $INCLUDE: 'c:\qbasic\software\CABLE2\cabprint.in'
'       $INCLUDE: 'c:\qbasic\software\CABLE2\print6.in'
REM *************************************************************************
'       $INCLUDE: 'c:\qbasic\software\CABLE2\design.in'
'       $INCLUDE: 'c:\qbasic\software\CABLE2\design6.in'
'       $INCLUDE: 'c:\qbasic\software\CABLE2\revs.in'
REM *************************************************************************
'       $INCLUDE: 'c:\qbasic\software\CABLE2\analysis.in'
'       $INCLUDE: 'c:\qbasic\software\CABLE2\analysi6.in'
REM *************************************************************************


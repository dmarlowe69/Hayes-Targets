REM *************************************************************************
REM
REM    program name:HEXLIST
REM
REM     written by Denton Marlowe
REM     (c)1988 HAYES TARGETS
REM
REM *************************************************************************
hexlist:     
        CLS : SCREEN 0: COLOR 3
        PRINT "American Automation hex file to RCA hex dump format"
        PRINT "written by Denton Marlowe"
        PRINT "(c)1988 hayes targets"
        IF COMMAND$ = "" THEN GOTO options
        file$ = COMMAND$
        GOTO getout
REM *************************************************************************
REM
REM     get format options
REM
REM *************************************************************************
options:
        PRINT
        PRINT " Options"
        PRINT
        PRINT " (1) american automation hex file formated listing"
        PRINT " (2) american automation hex file conversion to RCA format"
        PRINT " (3) standard paged ascii listing"
        PRINT " (4) QUIT"
        PRINT
        PRINT " note: option 1 can be used to print any ASCII file"
        PRINT
        INPUT " enter option"; type$: IF type$ = "4" THEN END
        IF type$ <> "1" AND type$ <> "2" AND type$ <> "3" THEN GOTO options
device:
        INPUT "(S)creen or (P)rinter or (F)ile"; dev$: dev$ = UCASE$(dev$)
        IF dev$ <> "S" AND dev$ <> "P" AND dev$ <> "F" THEN GOTO device
REM *************************************************************************
        IF dev$ = "S" THEN OPEN "SCRN:" FOR OUTPUT AS #3
        IF dev$ = "P" THEN OPEN "LPT1:" FOR OUTPUT AS #3
        IF dev$ <> "F" THEN GOTO getname
REM *************************************************************************
        PRINT
        INPUT " enter output file name"; file$
        IF file$ = "" THEN GOTO device
        OPEN file$ FOR OUTPUT AS #3
REM *************************************************************************
REM
REM     get input file name
REM
REM *************************************************************************
getname:
        PRINT
        INPUT " enter American Automation hex file name"; file$
        IF file$ = "" THEN file$ = "d:\american\tlxrca\master.h"
REM *************************************************************************
REM
REM     get output file name
REM
REM *************************************************************************
getout:
        IF type$ = "1" THEN GOTO american
        IF type$ = "3" THEN GOTO lister
REM *************************************************************************
start:
        pagenumber = 1
        counter = 0
        ADDRESS = 0
        OPEN file$ FOR INPUT AS #1
        GOSUB header
        INPUT #1, a$
REM *************************************************************************
nextline:
        INPUT #1, a$
        end$ = MID$(a$, 1, 2)
        IF end$ = "S9" THEN GOTO quit
        IF end$ = "SD" THEN GOTO quit
        length = LEN(a$) - 4 - 2
REM *************************************************************************
        PRINT #3, MID$(a$, 5, 4); " ";
        FOR a = 9 TO 40 STEP 4
        PRINT #3, " ";
        PRINT #3, MID$(a$, a, 4);
        ADDRESS = ADDRESS + 2
        NEXT a
        PRINT #3, ";": counter = counter + 1
REM *************************************************************************
        H$ = HEX$(ADDRESS)
        HLENGTH = LEN(H$)
        IF HLENGHT = 1 THEN H$ = "000" + H$
        IF HLENGTH = 2 THEN H$ = "00" + H$
        IF HLENGTH = 3 THEN H$ = "0" + H$
        PRINT #3, H$; " ";
REM *************************************************************************
        FOR a = 41 TO 72 STEP 4
        PRINT #3, " ";
        PRINT #3, MID$(a$, a, 4);
        ADDRESS = ADDRESS + 2
        NEXT a
        PRINT #3, ";": counter = counter + 1
REM *************************************************************************
        IF counter > 59 THEN GOTO nextpage
        IF EOF(1) THEN GOTO quit
REM        IF dev$ = "S" THEN FOR i = 0 TO 10000: NEXT
        GOTO nextline
nextpage:
        pagenumber = pagenumber + 1
        counter = 0
        IF dev$ = "P" OR dev$ = "F" THEN PRINT #3, CHR$(12);
        GOSUB header
        IF EOF(1) THEN GOTO quit
        GOTO nextline
REM *************************************************************************
REM
REM     american automation formated listing
REM
REM *************************************************************************
american:
        pagenumber = 1
        counter = 0
        OPEN file$ FOR INPUT AS #1
        GOSUB header
newline:
        INPUT #1, a$
        end$ = MID$(a$, 1, 2)
        IF end$ = "S9" THEN GOTO quit
        IF end$ = "SD" THEN GOTO quit
        PRINT #3, a$: counter = counter + 1
        IF counter > 59 THEN GOTO newpage
        IF EOF(1) THEN GOTO quit
REM        IF dev$ = "S" THEN FOR i = 0 TO 10000: NEXT
        GOTO newline
newpage:
        pagenumber = pagenumber + 1
        counter = 0
        IF dev$ = "P" OR dev$ = "F" THEN PRINT #3, CHR$(12);
        GOSUB header
        IF EOF(1) THEN GOTO quit
        GOTO newline
REM *************************************************************************
REM
REM     ascii listing
REM
REM *************************************************************************
lister:
        pagenumber = 1
        counter = 0
        OPEN file$ FOR INPUT AS #1
        GOSUB header
newerline:
        INPUT #1, a$
        PRINT #3, a$: counter = counter + 1
        IF counter > 59 THEN GOTO newerpage
        IF EOF(1) THEN GOTO quit
        GOTO newerline
newerpage:
        pagenumber = pagenumber + 1
        counter = 0
        IF dev$ = "P" OR dev$ = "F" THEN PRINT #3, CHR$(12);
        GOSUB header
        IF EOF(1) THEN GOTO quit
        GOTO newerline
REM *************************************************************************
REM
REM     close out file
REM
REM *************************************************************************
quit:
        IF dev$ = "P" OR dev$ = "F" THEN PRINT #3, CHR$(12);
        CLOSE #1
        CLOSE #3
        GOTO hexlist
        END
REM *************************************************************************
REM
REM     page header
REM
REM *************************************************************************
header:     
        PRINT #3, : counter = counter + 1
        PRINT #3, : counter = counter + 1

        PRINT #3, file$;
        PRINT #3, TAB(68); "Page ";
        PRINT #3, USING "##"; pagenumber
        counter = counter + 1

        PRINT #3, : counter = counter + 1
        PRINT #3, : counter = counter + 1
        PRINT #3, : counter = counter + 1
        RETURN
REM *************************************************************************

